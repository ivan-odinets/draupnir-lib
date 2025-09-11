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

#ifndef SETTINGSENTRIESHANDLERCONTEXT_H
#define SETTINGSENTRIESHANDLERCONTEXT_H

#include "../settings/SettingTraitForEntry.h"

namespace Draupnir::Menus
{

/*! @class SettingsEntriesHandlerContext draupnir-lib/include/templateMenus/handlers/settingsMenu/SettingsEntriesHandlerContext.h
 *  @brief Thin context that bridges menu entry handlers to a settings registry.
 *  @details SettingsEntriesHandlerContext<SettingsRegistry> stores a pointer to a compile-time typed SettingsRegistry and
 *           exposes minimal, strongly-typed helpers to read/write a setting associated with a given menu entry trait.
 *
 *          Typical usage:
 *          - Call setRegistry(&registry) once at startup.
 *          - From a menu entry handler, call get<MinimizeOnCloseEntry>() to obtain the current value, or set<MinimizeOnCloseEntry>(value)
 *            to persist a change.
 *  @tparam SettingsRegistry Typed settings registry that owns values and persistence. */

template<class SettingsRegistry>
class SettingsEntriesHandlerContext
{
public:
    /*! @brief Construct an empty context (no registry yet). */
    SettingsEntriesHandlerContext() :
        p_registry{nullptr}
    {}

    /*! @brief Attach a settings registry instance.
     *  @param registry Non-null pointer to a live SettingsRegistry.
     * @note Must be called before get()/set(). */
    void setRegistry(SettingsRegistry* registry) {
        Q_ASSERT_X(registry, "SettingsEntriesHandlerContext<SettingsRegistry>::setRegistry",
                   "Provided Registry* pointer is nullptr.");
        p_registry = registry;
    }

    /*! @brief Read a setting value mapped from a menu entry trait.
     *  @details Resolves the corresponding SettingTrait as Settings::SettingTraitForEntry<SettingMenuEntry>::type and forwards
     *           to SettingsRegistry::getSetting<SettingTrait>().
     *  @tparam SettingMenuEntry Menu entry trait type.
     *  @return The current value of the mapped setting (by value). */
    template<class SettingMenuEntry>
    typename Draupnir::Settings::SettingTraitForEntry<SettingMenuEntry>::type::Value get() const {
        static_assert(SettingsRegistry::template containsSettingForMenuEntry<SettingMenuEntry>(),
                "SettingsRegistry specified does not have trait for the provided SettingMenuEntry.");
        return p_registry->template get<typename Draupnir::Settings::SettingTraitForEntry<SettingMenuEntry>::type>();
    }

    /*! @brief Write a setting value mapped from a menu entry trait and persist it.
     *  @details Resolves the corresponding SettingTrait as Settings::SettingTraitForEntry<SettingMenuEntry>::type and forwards
     *           to SettingsRegistry::setSetting<SettingTrait>(value).
     *  @tparam SettingMenuEntry Menu entry trait type.
     *  @param value New value to store. */
    template<class SettingMenuEntry>
    void set(const typename Draupnir::Settings::SettingTraitForEntry<SettingMenuEntry>::type::Value& value) {
        static_assert(SettingsRegistry::template containsSettingForMenuEntry<SettingMenuEntry>(),
                "SettingsRegistry specified does not have trait for the provided SettingMenuEntry.");
        p_registry->template set<typename Draupnir::Settings::SettingTraitForEntry<SettingMenuEntry>::type>(value);
    }

private:
    SettingsRegistry* p_registry;
};

};

#endif // SETTINGSENTRIESHANDLERCONTEXT_H
