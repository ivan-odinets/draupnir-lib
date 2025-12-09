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

#ifndef STARTHIDDENENTRYHANDLER_H
#define STARTHIDDENENTRYHANDLER_H

#include "SettingsCheckableEntryHandler.h"

#include "draupnir/settings_registry/traits/settings/main_window/StartHiddenSetting.h"
#include "draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context,class MenuEntry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<SettingsContext,Draupnir::Ui::StartHiddenMenuEntry>
 *  @ingroup HandlerTemplates
 *  @brief This is a class.
 *
 * @todo Change naming and location of this class after adjusting Draupnir::Ui::StartHiddenEntry
 * @todo Write reasonable documentation for this class. */

template<class SettingsContext>
class GenericMenuEntryHandler<SettingsContext,Draupnir::Ui::StartHiddenMenuEntry> :
    public SettingsCheckableEntryHandler<SettingsContext,Draupnir::Ui::StartHiddenMenuEntry>
{
public:
    GenericMenuEntryHandler(SettingsContext& context) :
        SettingsCheckableEntryHandler<SettingsContext,Draupnir::Ui::StartHiddenMenuEntry>{context}
    {}
};

/*! @struct SettingTraitForEntry<Draupnir::Ui::StartHiddenMenuEntry>
 *  @ingroup HandlerTemplates
 *  @brief Specialization mapping the "Start Hidden" menu entry to its setting trait.
 *  @details Associates the menu entry trait Draupnir::Menus::StartHidden with the corresponding StartHiddenSetting
 *           setting trait. This mapping allows SettingsMenuEntriesHandler and to resolve the correct value type and key
 *           at compile time.
 *
 * @todo Maybe this class needs to be placed in other file?
 * @todo Maybe include this mapping into the menu entry trait class? */

template<> struct SettingTraitForEntry<Draupnir::Ui::StartHiddenMenuEntry> {
    using type = Settings::MainWindow::StartHiddenSetting;
};

}; // namespace Draupnir::Menus

#endif // STARTHIDDENENTRYHANDLER_H
