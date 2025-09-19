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

#ifndef SETTINGSREGISTRY_H
#define SETTINGSREGISTRY_H

#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    #include <QSettings>
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    #include "core/AppSettings.h"
#endif // DRAUPNIR_SETTINGS_USE_QSETTINGS || DRAUPNIR_SETTINGS_USE_APPSETTINGS

#include "utils/SettingTraitsConcat.h"
//#include "utils/SettingTraitForEntry.h"
#include "utils/SettingTraitValidator.h"
#include "utils/SettingTraitSerializer.h"

#include "../utils/common.h"

namespace Draupnir::Settings
{

template<class T, class... Ts>
struct is_in_pack : std::disjunction<std::is_same<T, Ts>...> {};

template<class T, class... BundleTraits, class... Rest>
struct is_in_pack<T, SettingsBundle<BundleTraits...>, Rest...>
    : std::disjunction<is_in_pack<T, BundleTraits...>, is_in_pack<T, Rest...>> {};

/*! @class SettingsRegistry draupnir-lib/incldue/settings/SettingsRegistry.h
 *  @brief Strongly-typed, compile-time registry of application settings.
 *  @details This class aggregates all specified setting traits (either individually or via SettingsBundle), flattens them
 *           into a single tuple of SettingTemplate<Trait>, and provides:
 *           - Bulk loading of all settings from a backend (either QSettings or AppSettings). Backend selection is done by
 *             macro (DRAUPNIR_SETTINGS_USE_QSETTINGS or DRAUPNIR_SETTINGS_USE_APPSETTINGS);
 *           - Type-safe accessors and mutators for individual setting values;
 *           - Construction of partial bundles for selected traits;
 *           - Compile-time membership checks.
 *
 *           Each SettingTrait must define:
 *           - `using Value` — the C++ value type (e.g. bool, QString, enum, ...);
 *           - `static QString key()` — persistent key in the backend;
 *           - `static Value defaultValue()` — default value when no stored value exists.
 *
 *  @tparam Traits A variadic list of SettingTraits or SettingsBundle<Ts...>.
 *
 * @note Internally, all traits and bundles are flattened to a single tuple of SettingTemplate<T>.
 * @see SettingTraitForEntry, SettingTemplate, SettingsSerializer, SettingsBundle
 *
 * @todo Add static_assert's for the SettingTraits...
 * @todo Integrate MessageSystem settings within the SettingsRegistry.
 * @todo Introduce settings UI categories.
 * @todo Create templated settings widget / dialog.
 * @todo Add interface for partial updating of the settings. E.g. when the setting has sth like QStringList type - not replace
 *       the variable, but use append method and than write to the backend. */

template<class... Traits>
class SettingsRegistry
{
#if defined(DRAUPNIR_SETTINGS_USE_QSETTINGS)
    using Backend = QSettings;
#elif defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    using Backend = AppSettings;
#else
    #error("Settings backend not specified. Either DRAUPNIR_SETTINGS_USE_QSETTINGS or "\
           "DRAUPNIR_SETTINGS_USE_APPSETTINGS macro must be defined.")
#endif // DRAUPNIR_SETTINGS_USE_APPSETTINGS || DRAUPNIR_SETTINGS_USE_QSETTINGS

    using AbstractSettingsTuple = typename TupleConcat<typename Flatten<Traits>::type...>::type;

public:
    /*! @brief Checks at compile time whether a specific SettingTrait is part of this registry.
     *  @tparam SettingTrait A trait to check for. */
    template<class SettingTrait>
    static constexpr bool contains() { return is_type_in_tuple_v<SettingTemplate<SettingTrait>,AbstractSettingsTuple>; }

    /*! @brief Checks at compile time whether a specific SettingTrait is part of this registry.
     *  @tparam SettingTrait A trait to check for. */
    template<class SettingTrait>
    [[deprecated]] static constexpr bool containsSetting() {
        return is_type_in_tuple_v<SettingTemplate<SettingTrait>,AbstractSettingsTuple>;
    }

    /*! @brief Returns whether the SettingsRegistry is empty.
     *  @return False if sizeof...(Traits> != 0. */
    static constexpr bool isEmpty() { return sizeof...(Traits) == 0; }

    /*! @brief Default constructor. Initializes internal Backend pointer to nullptr. */
    SettingsRegistry() :
        p_backend{nullptr}
    {}

    ~SettingsRegistry() {
        delete p_backend;
    }

    /*! @brief Creates Backend object and loads all registered settings from the Backend. */
    void loadSettings() {
        QSettings::setDefaultFormat(QSettings::NativeFormat);
        p_backend = new Backend;

        _loadSettingsImpl<0>();
    }

    /*! @brief Checks whether the SettingsRegistry has been bound to backend.
     *  @return true if backend pointer is not nullptr, false otherwise. */
    bool isLoaded() const { return p_backend != nullptr; }

#if defined(DRAUPNIR_SETTINGS_USE_APPSETTINGS)
    /*! @brief Enable or disable preservation mode (no writing to config file).
     *  @param arg - if true, changes will not be persisted to disk.
     * @note This is available only when using AppSettings as backend. (DRAUPNIR_SETTINGS_USE_APPSETTINGS macro defined). */
    void setPreserveConfig(bool state) { p_backend->setPreserveConfig(state); }

    /*! @brief Returns true if preservation mode is enabled.
     * @note This is available only when using AppSettings as backend. (DRAUPNIR_SETTINGS_USE_APPSETTINGS macro defined). */
    bool preserveConfig() const { return p_backend->preserveConfig(); }

#endif // DRAUPNIR_SETTINGS_USE_APPSETTINGS

    /*! @brief Returns the pointer to the enabled Backend. */
    Backend* settings() { return p_backend; }

    /*! @brief Prints all known settings to debug log. Calls key() and value for each trait if available.
     * @todo Improve formatting and handle custom types. */
    void printAllToDebug() {
        Q_ASSERT_X(p_backend, "SettingsRegistry<SettingTraits...>::printAllToDebug",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");
        qDebug() << "SettingsRegistry<Traits...>::printAllToDebug()";

        _printAllToDebugImpl<0>();
    };

    /*! @brief Retrieves a SettingsBundle pre-filled with settings from this registry.
     *  @tparam Bundle A concrete instantiation of SettingsBundle<Ts...>
     *  @return An initialized SettingsBundle with pointers to internal AbstractSetting<T> instances. */
    template<class Bundle>
    Bundle getSettingsBundle() {
        static_assert(Bundle::template canBeFullyPopulatedFrom<SettingsRegistry<Traits...>>(),
                "Requested Bundle can not be fully populated by this SettingsRegistry<Traits...> instance.");
        Q_ASSERT_X(p_backend, "SettingsRegistry<SettingTraits...>::getSettingsBundle<Bundle>()",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");

        Bundle result{p_backend};
        _populateSettingBundle<Bundle,0>(result);
        return result;
    };

    /*! @brief Shortcut to get a SettingsBundle for a specific subset of traits. Equivalent to
     *         getSettingBundle<SettingsBundle<SubsetOfTraits...>>().
     *  @tparam SubsetOfTraits One or more traits that exist within this registry. */
    template<class... SubsetOfTraits>
    SettingsBundle<SubsetOfTraits...> getSettingBundleForTraits() {
        static_assert(SettingsBundle<SubsetOfTraits...>::template canBeFullyPopulatedFrom<SettingsRegistry<Traits...>>(),
                "Requested Bundle can not be fully populated by this SettingsRegistry<Traits...> instance.");
        Q_ASSERT_X(p_backend, "SettingsRegistry<SettingTraits...>::getSettingBundleForTraits<SubsetOfTraits...>()",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");

        return getSettingsBundle<SettingsBundle<SubsetOfTraits...>>();
    };

//    /*! @brief Gets the value of a setting associated with a given MenuEntry.
//     *  @tparam MenuEntry Type mapped to a SettingTrait via SettingTraitForEntry<>.
//     *  @return const reference to the setting's stored value. */
//    template<class MenuEntry>
//    const typename SettingTraitForEntry<MenuEntry>::type::Value& getSettingForMenuEntry() const {
//        static_assert(containsSetting<typename SettingTraitForEntry<MenuEntry>::type>,
//                "Setting trait for MenuEntry specified is not registered within this SettingsRegistry.");
//        Q_ASSERT_X(p_backend, "SettingsRegistry<SettingTraits...>::getSettingForMenuEntry<MenuEntry>",
//                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");
//        return get<typename SettingTraitForEntry<MenuEntry>::type>();
//    }

    /*! @brief Gets the value of a specific setting.
     *  @tparam SettingTrait Trait present in the registry.
     *  @return const reference to the setting's stored value. */
    template<class SettingTrait>
    const typename SettingTrait::Value& get() const {
        static_assert(contains<SettingTrait>(),
                "SettingTrait specified is not registered within this SettingsRegistry.");
        Q_ASSERT_X(p_backend, "SettingsRegistry<SettingTraits...>::get<SettingTrait>",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");
        return std::get<SettingTemplate<SettingTrait>>(m_settings).value;
    }

    /*! @brief Sets and persists a new value for a specific setting.
     *  @tparam SettingTrait Trait present in the registry.
     *  @param value New value to store and persist. */
    template<class SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        static_assert(contains<SettingTrait>(),
                "SettingTrait specified is not registered within this SettingsRegistry.");
        Q_ASSERT_X(p_backend, "SettingsRegistry<SettingTraits...>::set<SettingTrait>",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");
        std::get<SettingTemplate<SettingTrait>>(m_settings).value = value;
        SettingTraitSerializer<Backend,SettingTrait>::set(p_backend, value);
    }

private:
    Backend* p_backend;                ///< Backend used for storage.
    AbstractSettingsTuple m_settings;  ///< Tuple of AbstractSetting<Trait> instances.

    /*! @brief Helper to extract the Trait type for a given tuple index. */
    template<std::size_t Index>
    struct _TraitForIndex {
        using type = typename std::tuple_element_t<Index,AbstractSettingsTuple>::Trait;
    };

    /*! @brief Recursively loads all trait values from the AppSettings backend. */
    template<std::size_t Index = 0>
    inline void _loadSettingsImpl() {
        std::get<Index>(m_settings).value = SettingTraitSerializer<Backend,typename _TraitForIndex<Index>::type>::get(p_backend);

        if constexpr (Index + 1 < std::tuple_size_v<AbstractSettingsTuple>)
            _loadSettingsImpl<Index+1>();
    }
    /*! @brief Recursively prints trait keys and values to qDebug. */
    template<std::size_t Index = 0>
    inline void _printAllToDebugImpl() {
        using Trait = typename _TraitForIndex<Index>::type;

        if constexpr (SettingTraitValidator::has_key<Trait>()) {
            qDebug() << "["<<Index<<"] key = "<<Trait::key()<<" value = "<<std::get<Index>(m_settings).value;
        } else {
            qDebug() << "["<<Index<<"] custom thing...";
        }

        if constexpr (Index + 1 < std::tuple_size<AbstractSettingsTuple>::value)
            _printAllToDebugImpl<Index+1>();
    }

    /*! @brief Populates a SettingsBundle by assigning internal trait pointers.
     *  @tparam Bundle Target bundle type. */
    template<class Bundle,std::size_t Index = 0>
    inline void _populateSettingBundle(Bundle& bundle) {
        using Trait = typename _TraitForIndex<Index>::type;

        if constexpr (Bundle::template contains<Trait>()) {
            bundle.registerSetting(&std::get<SettingTemplate<Trait>>(m_settings));
        }

        if constexpr (Index + 1 < std::tuple_size_v<AbstractSettingsTuple>)
            _populateSettingBundle<Bundle,Index+1>(bundle);
    }
};

};

#endif // SETTINGSREGISTRY_H
