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

#ifndef SETTINGBUNDLE_H
#define SETTINGBUNDLE_H

#include <tuple>

#include "SettingTemplate.h"
#include "AppSettings.h"
#include "SettingTraitSerializer.h"
#include "SettingTraitForEntry.h"

#include "../utils/common.h"

namespace Draupnir::Settings
{

/*! @class SettingsBundle draupnir-lib/incldue/settings/SettingsBundle.h
 *  @brief Lightweight view over a subset of settings from a SettingsRegistry.
 *  @details This bundle represents a selected set of SettingTraits (a "bundle") that allows scoped access to settings. Each
 *           instance holds:
 *           - A pointer to an AppSettings backend;
 *           - A tuple of pointers to AbstractSetting<Trait> values managed by the SettingsRegistry.
 *
 *           The bundle does not own the settings — it simply references them.
 *
 *  @tparam SettingTraits A variadic list of SettingTrait types (not bundles).
 *
 * @note This class is typically returned from SettingsRegistry via getSettingBundle() / getSettingBundleForTraits
 * @see SettingsRegistry, AbstractSetting, SettingTraitForEntry */

template<class... SettingTraits>
class SettingsBundle
{
public:
    /*! @brief Checks at compile time whether the bundle contains the given trait.
     *  @tparam Trait A trait to check.
     *  @return true If Trait is in SettingTraits... and false Otherwise */
    template<class Trait>
    static constexpr bool contains() { return is_one_of_v<Trait, SettingTraits...>; }

    /*! @brief Default constructor. Creates an uninitialized bundle. Internally, all pointers are set to nullptr.
     *  @warning AppSettings pointer is nullptr and must be initialized manually. */
    SettingsBundle() :
        p_settings{nullptr},
        p_abstractSettingsTuple{std::make_tuple<SettingTraits...>(nullptr)}
    {}

    /*! @brief Returns a const reference to the value associated with a specific SettingTrait.
     *  @tparam Trait Must be one of the traits in the bundle.
     *  @return Const reference to the setting's value. */
    template<class Trait>
    const typename Trait::Value& get() const {
        Q_ASSERT_X(p_settings, "SettingBundle<SettingTraits...>::get<Trait>",
                   "AppSettings pointer was not set.");
        return std::get<SettingTemplate<Trait>*>(p_abstractSettingsTuple)->value;
    }

    /*! @brief Returns a const reference to the value associated with a MenuEntry.
     *  @tparam MenuEntry Type mapped to a SettingTrait via SettingTraitForEntry<MenuEntry>.
     *  @return Const reference to the setting's value. */
    template<class MenuEntry>
    const typename SettingTraitForEntry<MenuEntry>::type::Value& getByMenuEntry() const {
        return get<SettingTraitForEntry<MenuEntry>::type>();
    }

    /*! @brief Sets and persists the value of a specific SettingTrait.
     *  @tparam Trait Must be one of the traits in the bundle.
     *  @param value New value to store. */
    template<class Trait>
    void set(const typename Trait::Value& value) {
        Q_ASSERT_X(p_settings, "SettingBundle<SettingTraits...>::set<Trait>",
                   "AppSettings pointer was not set.");
        std::get<SettingTemplate<Trait>*>(p_abstractSettingsTuple)->value = value;

        SettingTraitSerializer<AppSettings,Trait>::set(
            p_settings, value
        );
    }

    /*! @brief Prints all keys and values in the bundle using qDebug(). Intended for quick inspection and debugging. */
    void debugPrintAll() {
        _debugPrintAllImpl<SettingTraits...>();
    }

protected:
    template<class...>
    friend class SettingsRegistry;

    /*! @brief Internal constructor. Called by SettingsRegistry when initializing the bundle.
     *  @param settings Pointer to the shared AppSettings backend. */
    SettingsBundle(AppSettings* settings) :
        p_settings{settings}
    {}

    /*! @brief Registers a setting by pointer (called by SettingsRegistry).
     *  @tparam Trait Must be a trait declared in this bundle.
     *  @param setting Pointer to AbstractSetting<Trait> owned by SettingsRegistry. */
    template<class Trait>
    void registerSetting(SettingTemplate<Trait>* setting) {
        static_assert(contains<Trait>(),
                      "Specified Trait is not contained within this SettingBundle.");

        std::get<SettingTemplate<Trait>*>(p_abstractSettingsTuple) = setting;
    }

private:
    AppSettings* p_settings;      ///< Pointer to the backend settings store.

    std::tuple<
        SettingTemplate<SettingTraits>*...
    > p_abstractSettingsTuple;    /// Tuple of non-owning pointers to AbstractSetting<Trait> for each registered trait.

    /*! @brief Implementation detail: recursive debug print for each trait in the bundle. */
    template<class First, class... Rest>
    inline void _debugPrintAllImpl() {
        qDebug() << First::key() << " = " << std::get<SettingTemplate<First>*>(p_abstractSettingsTuple)->value;

        if constexpr (sizeof...(Rest) > 0)
            _debugPrintAllImpl<Rest...>();
    }
};

}; // namespace Draupnir::Settings

#endif // SETTINGBUNDLE_H
