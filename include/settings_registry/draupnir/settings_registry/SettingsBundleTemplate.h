/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
 *
 * This file is part of draupnir-lib
 *
 * draupnir-lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * draupnir-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with draupnir-lib; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef SETTINGSBUNDLETEMPLATE_H
#define SETTINGSBUNDLETEMPLATE_H

#include <tuple>

#include <QDebug>

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/settings_registry/core/SettingTemplate.h"
#include "draupnir/settings_registry/utils/SettingTraitSerializer.h"

#include "draupnir/utils/type_presense.h"
#include "draupnir/utils/tuple_like_merge.h"

#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    #include <QSettings>
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    #include "draupnir/settings_registru/core/AppSettings.h"
#elif defined(DRAUPNIR_SETTINGS_USE_CUSTOM)
    #include "draupnir/settings_registry/core/SettingsBackendInterface.h"
#endif // DRAUPNIR_SETTINGS_USE_QSETTINGS || DRAUPNIR_SETTINGS_USE_APPSETTINGS

namespace Draupnir::Settings
{

template<SettingTraitConcept... Traits>
class SettingsBundleTemplate;

template<class C>
concept SettingsBundleConcept =
    requires { draupnir::utils::is_instantiation_of_v<C,Draupnir::Settings::SettingsBundleTemplate>; };

template<class C>
concept HasNestedSettingsBundle =
    requires { typename C::SettingsBundle; } &&
    draupnir::utils::is_instantiation_of_v<typename C::SettingsBundle,Draupnir::Settings::SettingsBundleTemplate>;

/*! @class SettingsBundleTemplate draupnir/SettingsBundleTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Lightweight non-owning view over a subset of settings managed by a SettingsRegistryTemplate.
 *  @tparam SettingTraits... Variadic list of `SettingTrait` types included in this bundle.
 *
 *  @details A SettingsBundleTemplate represents a scoped subset of SettingTraits collected from a SettingsRegistryTemplate.
 *           It provides:
 *           - Type-safe access to settings values (get/set);
 *           - Persistence into the backend via SettingTraitSerializer;
 *           - Validation utilities (isLoaded/isValid);
 *           - Debug printing of all registered keys and values.
 *
 *           Each instance holds:
 *           - A pointer to the backend (QSettings, AppSettings, or other depending on build-time macro);
 *           - A tuple of non-owning pointers to SettingTemplate<Trait> objects managed by the SettingsRegistry.
 *
 *           The bundle itself does not own settings; it only references them. Bundles should be constructed by
 *           SettingsRegistryTemplate (using SettingsRegistryTemplate::getSettingBundle() or
 *           SettingsRegistryTemplate::getSettingBundleForTraits() methods)
 *
 *  @note Bundles can represent arbitrary subsets of traits. Compile-time utilities (contains, canBePopulatedFrom)
 *        allow verifying whether a bundle matches a given registry.
 *
 * @todo Add interface for partial updating of the settings. E.g. when the setting has sth like QStringList type - not replace
 *       the variable, but use append method and than write to the backend. */

template<SettingTraitConcept... SettingTraits>
class SettingsBundleTemplate
{
    using SettingTemplatePtrTuple = std::tuple<SettingTemplate<SettingTraits>*...>;

#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    using Backend = QSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    using Backend = AppSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_CUSTOM)
    using Backend = SettingsBackendInterface;
#else
    #error("Settings backend not specified. Either DRAUPNIR_SETTINGS_USE_QSETTINGS or "\
           "DRAUPNIR_SETTINGS_USE_APPSETTINGS macro must be defined.")
#endif // DRAUPNIR_SETTINGS_USE_APPSETTINGS || DRAUPNIR_SETTINGS_USE_QSETTINGS

public:
    /*! @brief Returns amount of individual traits within this SettingsBundleTemplate. */
    static constexpr std::size_t traitCount() {
        return std::tuple_size_v<SettingTemplatePtrTuple>;
    }

    /*! @brief Contains amount of individual traits within this SettingsBundleTemplate. */
    static inline constexpr std::size_t traitCount_v = traitCount();

    /*! @brief Checks at compile time whether the bundle contains the given trait.
     *  @tparam Trait A trait to check.
     *  @return true If Trait is in SettingTraits... and false Otherwise */
    template<SettingTraitConcept Trait>
    static constexpr bool contains() {
        return draupnir::utils::is_type_in_tuple_v<SettingTemplate<Trait>*,SettingTemplatePtrTuple>;
    }

    /*! @brief Static template constexpr variable containing `true` if this instantiation of the @ref Draupnir::Settings::SettingsBundle
     *         template contains the specified Trait. */
    template<SettingTraitConcept Trait>
    static constexpr bool contains_v = contains<Trait>();

    /*! @brief Returns whether the bundle is empty.
     *  @return Always false for this general template. Specialization SettingsBundle<> returns true. */
    static constexpr bool isEmpty() { return false; }

    /*! @brief Static template constexpr variable containing `true` if this instantiation of the @ref Draupnir::Settings::SettingsBundle
     *         template is empty. Always `false` for general case and always `true` for SettingsBundle<> specialization. */
    static constexpr bool isEmpty_v = isEmpty();

    /*! @brief Compile-time check: can this bundle be fully populated from the given SettingsSource?
     *  @tparam SettingsSource A registry type to check against.
     *  @return true if all SettingTraits in this bundle are present in the registry, false otherwise. */
    template<class SettingsSource>
    static constexpr bool canBeFullyPopulatedFrom() {
        return _canBePopulatedFromImpl<SettingsSource,SettingTraits...>();
    }

    /*! @brief Static template constexpr variable containing `true` if this instantiation of the @ref Draupnir::Settings::SettingsBundle
     *         template can be fully populated from the given SettingsSource. */
    template<class SettingsSource>
    static constexpr bool canBeFullyPopulatedFrom_v = canBeFullyPopulatedFrom<SettingsSource>();

    /*! @brief Default constructor. Creates an uninitialized (invalid) bundle. Internally, all pointers are set to nullptr.
     * @note Backend pointer is nullptr and working with such bundle will result in Q_ASSERT in Debug or UB in release. */
    SettingsBundleTemplate() :
        p_backend{nullptr},
        m_settingTemplatePtrTuple{ (static_cast<SettingTemplate<SettingTraits>*>(nullptr))... }
    {}

    /*! @brief Checks whether the bundle has been bound to backend.
     *  @return true if backend pointer is not nullptr, false otherwise. */
    bool isLoaded() const { return p_backend != nullptr; }

    /*! @brief Checks whether the bundle is fully usable.
     *  @return true if backend pointer is non-null and all registered SettingTemplate pointers are non-null. */
    bool isValid() const {
        return (p_backend != nullptr) &&
               (!std::apply([](auto*... ptrs) { return (... || (ptrs == nullptr)); }, m_settingTemplatePtrTuple));
    }

    /*! @brief Returns the pointer to the enabled Backend. */
    Backend* settings() { return p_backend; }

    /*! @brief Prints all keys and values in the bundle using qDebug(). Intended for quick inspection and debugging. */
    void printAllToDebug() const {
        Q_ASSERT_X(isValid(), "SettingsBundle<SettingsTraits...>::printAllToDebug",
                   "This method must be called only for valid SettingsBundle objects.");

        qDebug() << "SettingsBundle<SettingsTraits...>::printAllToDebug()";
        _debugPrintAllImpl<SettingTraits...>();
    }

    template<class Bundle>
    Bundle getSettingsBundle() {
        static_assert(Bundle::template canBeFullyPopulatedFrom<SettingsBundleTemplate<SettingTraits...>>(),
                "Requested Bundle can not be fully populated by this SettingsBundle<SettingTraits...> instance.");
        Q_ASSERT_X(p_backend, "SettingsBundle<SettingTraits...>::getSettingsBundle<Bundle>()",
                   "This bundle must have been initialized from corresponding SettingsRegistry.");

        Bundle result{p_backend};
        _populateSettingBundle<Bundle,SettingTraits...>(result);
        return result;
    }

    /*! @brief Shortcut to get a SettingsBundle for a specific subset of traits. Equivalent to
     *         getSettingsBundle<SettingsBundle<SubsetOfTraits...>>().
     *  @tparam SubsetOfTraits One or more traits that exist within this registry. */
    template<SettingTraitConcept... SubsetOfTraits>
    SettingsBundleTemplate<SubsetOfTraits...> getSettingBundleForTraits() {
        static_assert(SettingsBundleTemplate<SubsetOfTraits...>::template canBeFullyPopulatedFrom<SettingsBundleTemplate<SettingTraits...>>(),
                "Requested Bundle can not be fully populated by this SettingsBundle<SettingTraits...> instance.");
        Q_ASSERT_X(p_backend, "SettingsBundle<SettingTraits...>::getSettingBundleForTraits<Bundle>()",
                   "This bundle must have been initialized from corresponding SettingsRegistry.");

        return getSettingsBundle<SettingsBundleTemplate<SubsetOfTraits...>>();
    };

    /*! @brief Returns a const reference to the value associated with a specific SettingTrait.
     *  @tparam Trait Must be one of the traits in the bundle.
     *  @return Const reference to the setting's value. */
    template<SettingTraitConcept Trait>
    const typename Trait::Value& get() const {
        static_assert(draupnir::utils::is_one_of_v<Trait,SettingTraits...>,
                "Specified Trait is not a member of SettingTraits... pack.");
        Q_ASSERT_X(p_backend, "SettingBundle<SettingTraits...>::get<Trait>",
                   "Backend pointer was not set.");

        return std::get<SettingTemplate<Trait>*>(m_settingTemplatePtrTuple)->value;
    }

    /*! @brief Sets and persists the value of a specific SettingTrait.
     *  @tparam Trait Must be one of the traits in the bundle.
     *  @param value New value to store. */
    template<SettingTraitConcept Trait>
    void set(const typename Trait::Value& value) {
        static_assert(draupnir::utils::is_one_of_v<Trait,SettingTraits...>,
                "Specified Trait is not a member of SettingTraits... pack.");
        Q_ASSERT_X(p_backend, "SettingBundle<SettingTraits...>::set<Trait>",
                   "Backend pointer was not set.");

        std::get<SettingTemplate<Trait>*>(m_settingTemplatePtrTuple)->value = value;
        SettingTraitSerializer<Backend,Trait>::set(p_backend, value);
    }

protected:
    template<SettingTraitConcept...>
    friend class SettingsBundleTemplate;

    template<SettingTraitConcept...>
    friend class SettingsRegistryTemplate;

    /*! @brief Internal constructor. Called by SettingsRegistry when initializing the bundle.
     *  @param settings Pointer to the shared backend. */
    SettingsBundleTemplate(Backend* backend) :
        p_backend{backend},
        m_settingTemplatePtrTuple{ (static_cast<SettingTemplate<SettingTraits>*>(nullptr))... }
    {
        Q_ASSERT_X(backend,"SettingsBundle::SettingsBundle",
                   "Provided backend pointer is nullptr.");
    }

    /*! @brief Registers a setting by pointer (called by SettingsRegistry).
     *  @tparam Trait Must be a trait declared in this bundle.
     *  @param setting Pointer to SettingTemplate<Trait> owned by SettingsRegistry. */
    template<SettingTraitConcept Trait>
    void registerSetting(SettingTemplate<Trait>* setting) {
        static_assert(contains<Trait>(),
                      "Specified Trait is not contained within this SettingBundle.");

        std::get<SettingTemplate<Trait>*>(m_settingTemplatePtrTuple) = setting;
    }

private:
    friend class SettingsBundleTemplateTest;

    /*! @brief Pointer to the backend settings store. */
    Backend* p_backend;

    /*! @brief Tuple of non-owning pointers to AbstractSetting<Trait> for each registered trait. */
    SettingTemplatePtrTuple m_settingTemplatePtrTuple;

    /*! @brief Implementation detail: recursive debug print for each trait in the bundle. */
    template<class First, class... Rest>
    inline void _debugPrintAllImpl() {
        qDebug() << "    "<< First::key() << " = " << std::get<SettingTemplate<First>*>(m_settingTemplatePtrTuple)->value;

        if constexpr (sizeof...(Rest) > 0)
            _debugPrintAllImpl<Rest...>();
    }

    /*! @brief Populates a SettingsBundle by assigning internal trait pointers.
     *  @tparam Bundle Target bundle type. */
    template<class Bundle,class First, class... Rest>
    inline void _populateSettingBundle(Bundle& bundle) {
        if constexpr (Bundle::template contains<First>()) {
            bundle.registerSetting(std::get<SettingTemplate<First>*>(m_settingTemplatePtrTuple));
        }

        if constexpr (sizeof...(Rest) > 0)
            _populateSettingBundle<Bundle,Rest...>(bundle);
    }

    template<class T>
    struct always_false : std::false_type {};

    template<class T> struct print;

    template<class Source, class First,class... Rest>
    static constexpr bool _canBePopulatedFromImpl() {
        if constexpr (!Source::template contains<First>()) {
            return false;
        } else if constexpr (sizeof...(Rest) > 0) {
            return _canBePopulatedFromImpl<Source,Rest...>();
        } else {
            return true;
        }
    }
};

/*! @class SettingsBundleTemplate<> draupnir-lib/include/settings/SettingsBundleTemplate.h
 *  @brief Specialization for an empty bundle.
 *  @details Represents a bundle with no traits. Used as a placeholder or when a module does not require any settings.
 *
 *  @note All queries resolve as follows:
 *        - contains<Trait>() -> false;
 *        - isEmpty() -> true;
 *        - canBePopulatedFrom<Registry>() -> false. */

template<>
class SettingsBundleTemplate<>
{
#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    using Backend = QSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    using Backend = AppSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_CUSTOM)
    using Backend = SettingsBackendInterface;
#else
    #error("Settings backend not specified. Either DRAUPNIR_SETTINGS_USE_QSETTINGS or "\
           "DRAUPNIR_SETTINGS_USE_APPSETTINGS macro must be defined.")
#endif // DRAUPNIR_SETTINGS_USE_APPSETTINGS || DRAUPNIR_SETTINGS_USE_QSETTINGS

public:
    /*! @brief Checks at compile time whether the bundle contains the given trait.
     *  @tparam Trait A trait to check.
     *  @return always returns false for the empty specialization. */
    template<class Trait>
    static constexpr bool contains() { return false; }

    /*! @brief Static constexpr variable containing `true` if this instantiation of @ref Draupnir::Settings::SettingsBundleRegistry
     *         contains the specified trait. For empty specialization - contains `false` always. */
    template<class Trait>
    static constexpr bool contains_v = false;

    /*! @brief Returns whether the bundle is empty.
     *  @return Always false for general template. Specialization SettingsBundle<> returns true. */
    static constexpr bool isEmpty() { return true; }

    static constexpr bool isEmpty_v = true;

    SettingsBundleTemplate() = default;

    /*! @brief Compile-time check: can this bundle be populated from the given SettingsRegistry?
     *  @tparam ignored. Empty bundles can not be populated.
     *  @return false always. */
    template<class SettingsSource>
    static constexpr bool canBeFullyPopulatedFrom() { return false; }

    template<class SettingsSource>
    static constexpr bool canBeFullyPopulatedFrom_v = false;

protected:
    friend class SettingsBundleTemplateTest;

    template<SettingTraitConcept...>
    friend class SettingsRegistryTemplate;

    SettingsBundleTemplate(Backend*) {};
};

}; // namespace Draupnir::Settings

#endif // SETTINGSBUNDLETEMPLATE_H
