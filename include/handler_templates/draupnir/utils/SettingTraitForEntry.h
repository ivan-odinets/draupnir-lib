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

#ifndef SETTINGTRAITFORENTRY_H
#define SETTINGTRAITFORENTRY_H

namespace Draupnir::Handlers
{

/*! @struct SettingTraitForEntry draupnir/utils/SettingTraitForEntry.h
 *  @brief This is a base case of SettingTraitForEntry structure which is mapping a menu entry to its setting trait.
 *  @details Used to provides a compile-time association between the menu entry trait and the corresponding setting
 *           trait. This mapping is required by SettingsMenuEntriesHandler to resolve the correct value type and
 *           persistence key. */

template<class Entry>
struct SettingTraitForEntry
{
    using type = void;
};

};

#endif // SETTINGTRAITFORENTRY_H
