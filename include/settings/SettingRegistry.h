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

#ifndef SETTINGREGISTRY_H
#define SETTINGREGISTRY_H

#include "AppSettings.h"
#include "SettingTraitsConcat.h"
#include "SettingTraitForEntry.h"
#include "SettingTraitValidator.h"
#include "SettingTraitSerializer.h"

#include "../utils/common.h"

namespace Draupnir::Settings
{

/*! @class SettingsRegistry draupnir-lib/incldue/settings/SettingsRegistry.h
 *  @brief Strongly-typed, compile-time registry of application settings.
 *  @details This class aggregates all specified setting traits (either individually or via SettingsBundle), flattens them
 *           into a single tuple of AbstractSetting<Trait>, and provides:
 *           - Bulk loading of all settings from an AppSettings backend;
 *           - Type-safe accessors and mutators for individual setting values;
 *           - Construction of partial bundles for selected traits;
 *           - Compile-time membership checks.
 *
 *           Each SettingTrait must define:
 *           - `using Value` — the C++ value type (e.g. bool, QString, enum, ...);
 *           - `static QString key()` — persistent key in the AppSettings backend;
 *           - `static Value defaultValue()` — default value when no stored value exists.
 *
 *  @tparam Traits A variadic list of SettingTraits or SettingsBundle<Ts...>.
 *
 * @note Internally, all traits and bundles are flattened to a single tuple of AbstractSetting<T>.
 * @see SettingTraitForEntry, AbstractSetting, SettingsSerializer, SettingsBundle
 *
 * @todo Add static_assert's for the SettingTraits...
 * @todo Integrate MessageSystem settings within the SettingsRegistry.
 * @todo Introduce settings UI categories.
 * @todo Create templated settings widget / dialog. */

template<class... Traits>
class SettingsRegistry
{
    using AbstractSettingsTuple = typename TupleConcat<typename Flatten<Traits>::type...>::type;

public:
    /*! @brief Default constructor. Initializes internal AppSettings pointer to nullptr. */
    SettingsRegistry() :
        p_settings{nullptr}
    {}

    /*! @brief Loads all known settings from the given AppSettings backend.
     *  @param settings Non-null pointer to an AppSettings instance.
     *  @post Enables access and modification via `set()` and `get()`. */
    void loadSettings(AppSettings* settings) {
        Q_ASSERT_X(settings, "SettingsRegistry<SettingTraits...>::loadSettings",
                   "Provided AppSettings* pointer is nullptr.");
        Q_ASSERT_X(p_settings == nullptr, "SettingsRegistry<SettingTraits...>::loadSettings",
                   "This method must be called only once.");

        p_settings = settings;

        _loadSettingsImpl<0>();
    }

    /*! @brief Returns the backend AppSettings pointer. */
    AppSettings* settings() { return p_settings; }

    /*! @brief Prints all known settings to debug log. Calls key() and value for each trait if available.
     * @todo Improve formatting and handle custom types. */
    void printSettings() {
        _printSettingsImpl<0>();
    };

    /*! @brief Retrieves a SettingsBundle pre-filled with settings from this registry.
     *  @tparam Bundle A concrete instantiation of SettingsBundle<Ts...>
     *  @return An initialized SettingsBundle with pointers to internal AbstractSetting<T> instances.
     * @todo Add static_assert for the type checking. */
    template<class Bundle>
    Bundle getSettingBundle() {
        Q_ASSERT_X(p_settings, "SettingsRegistry<SettingTraits...>::getSettingsBundle<Bundle>()",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");

        Bundle result{p_settings};
        _populateSettingBundle<Bundle,0>(result);
        return result;
    };

    /*! @brief Shortcut to get a SettingsBundle for a specific subset of traits. Equivalent to
     *         getSettingBundle<SettingsBundle<SubsetOfTraits...>>().
     *  @tparam SubsetOfTraits One or more traits that exist within this registry. */
    template<class... SubsetOfTraits>
    SettingsBundle<SubsetOfTraits...> getSettingBundleForTraits() {
        Q_ASSERT_X(p_settings, "SettingsRegistry<SettingTraits...>::getSettingBundleForTraits<SubsetOfTraits...>()",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");

        return getSettingBundle<SettingsBundle<SubsetOfTraits...>>();
    };

    /*! @brief Checks at compile time whether a setting is present for the given MenuEntry. Resolves
     *         `SettingTraitForEntry<MenuEntry>::type` and checks presence in registry.
     *  @tparam MenuEntry Menu entry type mapped to a SettingTrait.
     *  @return true if trait is registered; false otherwise. */
    template<class MenuEntry>
    static constexpr bool containsSettingForMenuEntry() {
        return containsSetting<typename SettingTraitForEntry<MenuEntry>::type>();
    }

    /*! @brief Checks at compile time whether a specific SettingTrait is part of this registry.
     *  @tparam SettingTrait A trait to check for. */
    template<class SettingTrait>
    static constexpr bool containsSetting() {
        return is_type_in_tuple_v<SettingTemplate<SettingTrait>,AbstractSettingsTuple>;
    }

    /*! @brief Gets the value of a setting associated with a given MenuEntry.
     *  @tparam MenuEntry Type mapped to a SettingTrait via SettingTraitForEntry<>.
     *  @return const reference to the setting's stored value. */
    template<class MenuEntry>
    const typename SettingTraitForEntry<MenuEntry>::type::Value& getSettingForMenuEntry() const {
        return get<typename SettingTraitForEntry<MenuEntry>::type>();
    }

    /*! @brief Gets the value of a specific setting.
     *  @tparam SettingTrait Trait present in the registry.
     *  @return const reference to the setting's stored value. */
    template<class SettingTrait>
    const typename SettingTrait::Value& get() const {
        return std::get<SettingTemplate<SettingTrait>>(m_settings).value;
    }

    /*! @brief Sets and persists a new value for a specific setting.
     *  @tparam SettingTrait Trait present in the registry.
     *  @param value New value to store and persist. */
    template<class SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        Q_ASSERT_X(p_settings, "SettingsRegistry<SettingTraits...>::setSetting<SettingTrait>",
                   "SettingsRegistry<SettingTraits...>::loadSettings method must have been called before.");
        std::get<SettingTemplate<SettingTrait>>(m_settings).value = value;

        SettingTraitSerializer<AppSettings,SettingTrait>::set(
            p_settings, value
        );
    }

private:
    AppSettings* p_settings;           ///< Backend used for storage.
    AbstractSettingsTuple m_settings;  ///< Tuple of AbstractSetting<Trait> instances.

    /*! @brief Helper to extract the Trait type for a given tuple index. */
    template<std::size_t Index>
    struct _TraitForIndex {
        using type = typename std::tuple_element_t<Index,AbstractSettingsTuple>::Trait;
    };

    /*! @brief Recursively loads all trait values from the AppSettings backend. */
    template<std::size_t Index = 0>
    inline void _loadSettingsImpl() {
        std::get<Index>(m_settings).value = SettingTraitSerializer<AppSettings,typename _TraitForIndex<Index>::type>::get(p_settings);

        if constexpr (Index + 1 < std::tuple_size_v<AbstractSettingsTuple>)
            _loadSettingsImpl<Index+1>();
    }
    /*! @brief Recursively prints trait keys and values to qDebug. */
    template<std::size_t Index = 0>
    inline void _printSettingsImpl() {
        using Trait = typename _TraitForIndex<Index>::type;

        if constexpr (SettingTraitValidator::has_key<Trait>()) {
            qDebug() << "["<<Index<<"] key = "<<Trait::key()<<" value = "<<std::get<Index>(m_settings).value;
        } else {
            qDebug() << "["<<Index<<"] custom thing...";
        }

        if constexpr (Index + 1 < std::tuple_size<AbstractSettingsTuple>::value)
            _printSettingsImpl<Index+1>();
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

#endif // SETTINGREGISTRY_H
