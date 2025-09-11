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

#ifndef MINIMIZEONCLOSE_H
#define MINIMIZEONCLOSE_H

#include "SettingTraitTemplate.h"
#include "../SettingTraitForEntry.h"
#include "../../templateMenus/entries/SettingsMenuEntry.h"

namespace Draupnir::Settings
{

/*! @var minimizeOnClose_settingsKey
 *  @brief String key used to identify the "Minimize on Close" setting in persistent storage.
 *  @details This compile-time constant is passed into the generic SettingTraitTemplate an used by SettingsSerializer/
 *           SettingsRegistry to read and write the value. */

inline constexpr const char minimizeOnClose_settingsKey[] = "minimizeOnClose";

/*! @typedef MinimizeOnClose
 *  @brief Concrete setting trait describing the "Minimize on Close" option.
 *  @details This alias instantiates SettingTraitTemplate with:
 *           - Draupnir::Menus::MinimizeOnCloseEntry — the associated menu entry trait;
 *           - bool — the underlying C++ value type;
 *           - minimizeOnClose_settingsKey — the persistent key;
 *           - false — the default value.
 *
 *           The resulting trait provides:
 *           - using Value = bool;
 *           - static QString key();
 *           - static Value defaultValue();
 *
 *           It can be used in SettingsRegistry, SettingsEntriesHandlerContext, or any handler expecting a SettingTrait. */

using MinimizeOnClose = SettingTraitTemplate<
    Draupnir::Menus::MinimizeOnCloseEntry,
    bool,
    minimizeOnClose_settingsKey,
    false
>;

/*! @struct SettingTraitForEntry<Draupnir::Menus::MinimizeOnCloseEntry>
 *  @brief Specialization mapping a menu entry to its setting trait.
 *  @details Provides a compile-time association between the menu entry trait Draupnir::Menus::MinimizeOnCloseEntry and
 *           the corresponding MinimizeOnClose setting trait. This mapping is required by SettingsEntriesHandlerContext
 *           and SettingsRegistry to resolve the correct value type and persistence key. */

template<> struct SettingTraitForEntry<Draupnir::Menus::MinimizeOnCloseEntry> {
    using type = MinimizeOnClose;
};

};

#endif // MINIMIZEONCLOSE_H
