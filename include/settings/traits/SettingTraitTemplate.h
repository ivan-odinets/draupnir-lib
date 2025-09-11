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

#ifndef SETTINGTRAITTEMPLATE_H
#define SETTINGTRAITTEMPLATE_H

#include <QString>

namespace Draupnir::Settings
{

/*! @struct SettingTraitTemplate
 *  @brief Generic template for defining a setting trait.
 *  @details This struct provides a concise way to declare new setting traits that connect a menu entry with its underlying
 *           value type, persistent key, and default value.
 *
 *           A setting trait defined via this template supplies:
 *           - using Entry — the associated menu entry trait type;
 *           - using Value — the underlying C++ value type (e.g. bool, int, QString);
 *           - static QString key() — the storage key as a QString, built from @p settingsKey;
 *           - static Value defaultValue() — the default value.
 *
 *           Example:
 *           @code
 *           inline constexpr const char darkModeKey[] = "darkMode";
 *
 *           using DarkMode = SettingTraitTemplate<
 *              MyApp::Menus::DarkModeEntry, // associated menu entry
 *              bool,                        // value type
 *              darkModeKey,                 // persistent key
 *              false                        // default value
 *           >;
 *           @endcode
 *
 *  @tparam MenuEntryClass  The menu entry trait class this setting corresponds to.
 *  @tparam ValueClass      The underlying C++ type of the setting value.
 *  @tparam settingsKey     Pointer to a global constexpr string key.
 *  @tparam value           Default value of type @c ValueClass.
 *
 *  @see SettingTraitForEntry, SettingsRegistry, SettingsEntriesHandlerContext */

template<class MenuEntryClass,class ValueClass,const char* const settingsKey,auto value>
struct SettingTraitTemplate
{
    using Entry = MenuEntryClass;   //!< Associated menu entry trait type.
    using Value = ValueClass;       //!< Underlying value type.

    /*! @brief Return the persistent key as a QString. */
    static QString key() { return QString{settingsKey}; }

    /*! @brief Return the compile-time default value. */
    static Value defaultValue() { return value; }
};

};

#endif // SETTINGTRAITTEMPLATE_H
