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

#ifndef MINIMIZETOTRAYENTRYHANDLER_H
#define MINIMIZETOTRAYENTRYHANDLER_H

#include "SettingsCheckableEntryHandler.h"

#include "draupnir/traits/entries/SettingsMenuEntries.h"
#include "draupnir/traits/settings/main_window/MinimizeToTraySetting.h"

namespace Draupnir::Handlers
{

template<class Context, class MenuEntry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<SettingsContext,Draupnir::Menus::MinimizeToTrayEntry>
 *  @ingroup HandlerTemplates
 *  @brief This is a class.
 *
 * @todo Change naming and location of this class after adjusting Draupnir::Menus::MinimizeToTrayEntry
 * @todo Write reasonable documentation for this class. */

template<class SettingsContext>
class GenericMenuEntryHandler<SettingsContext,Draupnir::Menus::MinimizeToTrayEntry> :
    public SettingsCheckableEntryHandler<SettingsContext,Draupnir::Menus::MinimizeToTrayEntry>
{
public:
    GenericMenuEntryHandler(SettingsContext& context) :
        SettingsCheckableEntryHandler<SettingsContext,Draupnir::Menus::MinimizeToTrayEntry>{context}
    {}
};

/*! @struct SettingTraitForEntry<Draupnir::Menus::MinimizeToTrayEntry>
 *  @ingroup HandlerTemplates
 *  @brief Specialization mapping the "Minimize to Tray" menu entry to its setting trait.
 *
 *  @details Associates the menu entry trait Draupnir::Menus::MinimizeToTrayEntry with the corresponding MinimizeToTray
 *           setting trait. This mapping allows SettingsMenuEntriesHandler to resolve the correct value type and key at
 *           compile time.
 *
 * @todo Change naming and location of this class after adjusting Draupnir::Menus::MinimizeToTrayEntry
 * @todo Maybe include this mapping into the menu entry trait class? */

template<> struct SettingTraitForEntry<Draupnir::Menus::MinimizeToTrayEntry> {
    using type = Settings::MainWindow::MinimizeToTraySetting;
};

};

#endif // MINIMIZETOTRAYENTRYHANDLER_H
