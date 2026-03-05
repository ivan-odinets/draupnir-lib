/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MENUENTRYTOTRAITMAPPER_H
#define MENUENTRYTOTRAITMAPPER_H

#include "draupnir/ui_bricks/traits/settings/main_window/MinimizeOnCloseSetting.h"
#include "draupnir/ui_bricks/traits/settings/main_window/MinimizeToTraySetting.h"
#include "draupnir/ui_bricks/traits/settings/main_window/StartHiddenSetting.h"

#include "draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h"

/*! @file draupnir/ui_bricks/utils/MenuEntryToTraitMapper.h
 *  @brief This is a file.
 * @todo Refractor mechanism of mapping Menu entries to setting traits. */

#define DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(MenuEntry,SettingTrait) \
    template<> \
    struct MapMenuEntry<MenuEntry> { \
        using MappedEntry = MenuEntry; \
        using ToTrait = SettingTrait; \
    };

template<class Entry>
struct MapMenuEntry
{
    using MappedEntry = Entry;
    using ToTrait = void;
};

DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(Draupnir::Ui::StartHiddenMenuEntry,  Draupnir::Settings::MainWindow::StartHiddenSetting);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(Draupnir::Ui::MinimizeToTrayEntry,   Draupnir::Settings::MainWindow::MinimizeToTraySetting);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(Draupnir::Ui::MinimizeOnCloseEntry,  Draupnir::Settings::MainWindow::MinimizeOnCloseSetting);

#endif // MENUENTRYTOTRAITMAPPER_H
