/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef SETTINGSBUNDLE_H
#define SETTINGSBUNDLE_H

#include <tuple>

#include <QDebug>

#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    #include <QSettings>
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    #include "AppSettings.h"
#endif // DRAUPNIR_SETTINGS_USE_QSETTINGS || DRAUPNIR_SETTINGS_USE_APPSETTINGS

#include "SettingTemplate.h"
#include "SettingTraitSerializer.h"
#include "SettingTraitForEntry.h"

#include "../utils/common.h"

namespace Draupnir::Settings
{

/*! @class SettingsBundle draupnir-lib/include/settings/SettingsBundle.h
 *  @brief Lightweight non-owning view over a subset of settings managed by a SettingsRegistry.
 *  @details A SettingsBundle represents a scoped subset of SettingTraits collected from a SettingsRegistry. It provides:
 *           - Type-safe access to settings values (get/set);
 *           - Persistence into the backend via SettingTraitSerializer;
 *           - Validation utilities (isLoaded/isValid);
 *           - Debug printing of all registered keys and values.
 *
 *           Each instance holds:
 *           - A pointer to the backend (QSettings, AppSettings, or other depending on build-time macro);
 *           - A tuple of non-owning pointers to SettingTemplate<Trait> objects managed by the SettingsRegistry.
 *
 *           The bundle itself does not own settings; it only references them. Bundles should be constructed by SettingsRegistry
 *           (using SettingsRegistry::getSettingBundle() or SettingsRegistry::getSettingBundleForTraits() methods)
 *
 *  @tparam SettingTraits Variadic list of SettingTrait types included in this bundle.
 *
 *  @note Bundles can represent arbitrary subsets of traits. Compile-time utilities (contains, canBePopulatedFrom)
 *        allow verifying whether a bundle matches a given registry.
 * @see SettingsRegistry, SettingTraitSerializer, SettingTraitForEntry, SettingTemplate
 *
 * @todo Add interface for partial updating of the settings. E.g. when the setting has sth like QStringList type - not replace
 *       the variable, but use append method and than write to the backend. */

template<class... SettingTraits>
class SettingsBundle
{
#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    using Backend = QSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    using Backend = AppSettings;
#else
    #error("Settings backend not specified. Either DRAUPNIR_SETTINGS_USE_QSETTINGS or "\
           "DRAUPNIR_SETTINGS_USE_APPSETTINGS macro must be defined.")
#endif // DRAUPNIR_SETTINGS_USE_APPSETTINGS || DRAUPNIR_SETTINGS_USE_QSETTINGS

public:
    /*! @brief Checks at compile time whether the bundle contains the given trait.
     *  @tparam Trait A trait to check.
     *  @return true If Trait is in SettingTraits... and false Otherwise */
    template<class Trait>
    static constexpr bool contains() { return is_one_of_v<Trait, SettingTraits...>; }

    /*! @brief Returns whether the bundle is empty.
     *  @return Always false for this general template. Specialization SettingsBundle<> returns true. */
    static constexpr bool isEmpty() { return false; }

    /*! @brief Default constructor. Creates an uninitialized (invalid) bundle. Internally, all pointers are set to nullptr.
     * @note Backend pointer is nullptr and working with such bundle will result in Q_ASSERT in Debug or UB in release. */
    SettingsBundle() :
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

    /*! @brief Returns a const reference to the value associated with a specific SettingTrait.
     *  @tparam Trait Must be one of the traits in the bundle.
     *  @return Const reference to the setting's value. */
    template<class Trait>
    const typename Trait::Value& get() const {
        static_assert(is_one_of_v<Trait,SettingTraits...>,
                "Specified Trait is not a member of SettingTraits... pack.");
        Q_ASSERT_X(p_backend, "SettingBundle<SettingTraits...>::get<Trait>",
                   "Backend pointer was not set.");

        return std::get<SettingTemplate<Trait>*>(m_settingTemplatePtrTuple)->value;
    }

    /*! @brief Returns a const reference to the value associated with a MenuEntry.
     *  @tparam MenuEntry Type mapped to a SettingTrait via SettingTraitForEntry<MenuEntry>.
     *  @return Const reference to the setting's value. */
    template<class MenuEntry>
    const typename SettingTraitForEntry<MenuEntry>::type::Value& getByMenuEntry() const {
        static_assert(is_one_of_v<typename SettingTraitForEntry<MenuEntry>::type,SettingTraits...>,
                "Trait for the specified MenuEntry is not a member of SettingTraits... pack.");
        Q_ASSERT_X(p_backend, "SettingBundle<SettingTraits...>::get<Trait>",
                   "Backend pointer was not set.");

        return get<SettingTraitForEntry<MenuEntry>::type>();
    }

    /*! @brief Sets and persists the value of a specific SettingTrait.
     *  @tparam Trait Must be one of the traits in the bundle.
     *  @param value New value to store. */
    template<class Trait>
    void set(const typename Trait::Value& value) {
        static_assert(is_one_of_v<Trait,SettingTraits...>,
                "Specified Trait is not a member of SettingTraits... pack.");
        Q_ASSERT_X(p_backend, "SettingBundle<SettingTraits...>::set<Trait>",
                   "Backend pointer was not set.");

        std::get<SettingTemplate<Trait>*>(m_settingTemplatePtrTuple)->value = value;
        SettingTraitSerializer<Backend,Trait>::set(p_backend, value);
    }

    /*! @brief Prints all keys and values in the bundle using qDebug(). Intended for quick inspection and debugging. */
    void printAllToDebug() const {
        Q_ASSERT_X(isValid(), "SettingsBundle<SettingsTraits...>::printAllToDebug",
                   "This method must be called only for valid SettingsBundle objects.");

        qDebug() << "SettingsBundle<SettingsTraits...>::printAllToDebug()";
        _debugPrintAllImpl<SettingTraits...>();
    }

protected:
    template<class...>
    friend class SettingsRegistry;

    /*! @brief Compile-time check: can this bundle be fully populated from the given SettingsRegistry?
     *  @tparam SettingsRegistry A registry type to check against.
     *  @return true if all SettingTraits in this bundle are present in the registry, false otherwise. */
    template<class SettingsRegistry>
    static constexpr bool canBeFullyPopulatedFrom() {
        return _canBePopulatedFromImpl<SettingsRegistry,SettingTraits...>();
    }

    /*! @brief Internal constructor. Called by SettingsRegistry when initializing the bundle.
     *  @param settings Pointer to the shared backend. */
    SettingsBundle(Backend* backend) :
        p_backend{backend},
        m_settingTemplatePtrTuple{ (static_cast<SettingTemplate<SettingTraits>*>(nullptr))... }
    {
        Q_ASSERT_X(backend,"SettingsBundle::SettingsBundle",
                   "Provided backend pointer is nullptr.");
    }

    /*! @brief Registers a setting by pointer (called by SettingsRegistry).
     *  @tparam Trait Must be a trait declared in this bundle.
     *  @param setting Pointer to SettingTemplate<Trait> owned by SettingsRegistry. */
    template<class Trait>
    void registerSetting(SettingTemplate<Trait>* setting) {
        static_assert(contains<Trait>(),
                      "Specified Trait is not contained within this SettingBundle.");

        std::get<SettingTemplate<Trait>*>(m_settingTemplatePtrTuple) = setting;
    }

private:
    Backend* p_backend;             ///< Pointer to the backend settings store.

    std::tuple<
        SettingTemplate<SettingTraits>*...
    > m_settingTemplatePtrTuple;    /// Tuple of non-owning pointers to AbstractSetting<Trait> for each registered trait.

    /*! @brief Implementation detail: recursive debug print for each trait in the bundle. */
    template<class First, class... Rest>
    inline void _debugPrintAllImpl() {
        qDebug() << "    "<< First::key() << " = " << std::get<SettingTemplate<First>*>(m_settingTemplatePtrTuple)->value;

        if constexpr (sizeof...(Rest) > 0)
            _debugPrintAllImpl<Rest...>();
    }

    template<class SettingsRegistry, class First,class... Rest>
    static constexpr bool _canBePopulatedFromImpl() {
        if constexpr (!SettingsRegistry::template containsSetting<First>)
            return false;
        else if constexpr (sizeof...(Rest) > 0)
            return _canBePopulatedFromImpl<SettingsRegistry,Rest...>();
        else
            return true;
    }
};

/*! @class SettingsBundle<> draupnir-lib/include/settings/SettingsBundle.h
 *  @brief Specialization for an empty bundle.
 *  @details Represents a bundle with no traits. Used as a placeholder or when a module does not require any settings.
 *
 *  @note All queries resolve as follows:
 *        - contains<Trait>() -> false;
 *        - isEmpty() -> true;
 *        - canBePopulatedFrom<Registry>() -> false. */

template<>
class SettingsBundle<>
{
#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    using Backend = QSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    using Backend = AppSettings;
#else
    #error("Settings backend not specified. Either DRAUPNIR_SETTINGS_USE_QSETTINGS or "\
           "DRAUPNIR_SETTINGS_USE_APPSETTINGS macro must be defined.")
#endif // DRAUPNIR_SETTINGS_USE_APPSETTINGS || DRAUPNIR_SETTINGS_USE_QSETTINGS

public:
    /*! @brief Checks at compile time whether the bundle contains the given trait.
     *  @tparam Trait A trait to check.
     *  @return always returns false for the empty specialization. */
    template<class>
    static constexpr bool contains() { return false; }

    /*! @brief Returns whether the bundle is empty.
     *  @return Always false for this general template. Specialization SettingsBundle<> returns true. */
    static constexpr bool isEmpty() { return true; }

    SettingsBundle() = default;

protected:
    template<class...>
    friend class SettingsRegistry;

    SettingsBundle(Backend*) {};

    /*! @brief Compile-time check: can this bundle be populated from the given SettingsRegistry?
     *  @tparam ignored. Empty bundles can not be populated.
     *  @return false always. */
    template<class>
    static constexpr bool canBeFullyPopulatedFrom() {
        return false;
    }
};

}; // namespace Draupnir::Settings

#endif // SETTINGSBUNDLE_H
