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

#ifndef MINIMIZETOTRAY_H
#define MINIMIZETOTRAY_H

#include "SettingTraitTemplate.h"
#include "../SettingTraitForEntry.h"
#include "../../templateMenus/entries/SettingsMenuEntry.h"

namespace Draupnir::Settings
{

/*! @var minimizeToTray_settingsKey
 *  @brief String key for the "Minimize to Tray" setting in persistent storage.
 *  @details This compile-time constant is passed into SettingTraitTemplate and used as the unique key when persisting the
 *           value via SettingsSerializer / SettingsRegistry. */

inline constexpr const char minimizeToTray_settingsKey[] = "minimizeToTray";

/*! @typedef MinimizeToTray
 *  @brief Concrete setting trait describing the "Minimize to Tray" option.
 *  @details This alias instantiates SettingTraitTemplate with:
 *           - Draupnir::Menus::MinimizeToTrayEntry — the associated menu entry trait;
 *           - bool — the underlying C++ value type;
 *           - minimizeToTray_settingsKey — the persistent storage key;
 *           - false — the default value.
 *
 *           The resulting trait provides:
 *           - using Value = bool;
 *           - static QString key();
 *           - static Value defaultValue();
 *
 *           It can be registered in SettingsRegistry or used by any handler/context that expects a SettingTrait. */

using MinimizeToTray = SettingTraitTemplate<
    Draupnir::Menus::MinimizeToTrayEntry,
    bool,
    minimizeToTray_settingsKey,
    false
>;

/*! @struct SettingTraitForEntry<Draupnir::Menus::MinimizeToTrayEntry>
 *  @brief Specialization mapping the "Minimize to Tray" menu entry to its setting trait.
 *  @details Associates the menu entry trait @c Draupnir::Menus::MinimizeToTrayEntry with the corresponding MinimizeToTray
 *           setting trait. This mapping allows SettingsEntriesHandlerContext and SettingsRegistry to resolve the correct
 *           value type and key at compile time. */

template<> struct SettingTraitForEntry<Draupnir::Menus::MinimizeToTrayEntry> {
    using type = MinimizeToTray;
};

};

#endif // MINIMIZETOTRAY_H
