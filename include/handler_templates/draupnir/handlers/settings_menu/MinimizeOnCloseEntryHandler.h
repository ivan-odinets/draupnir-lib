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

#ifndef MINIMIZEONCLOSEENTRYHANDLER_H
#define MINIMIZEONCLOSEENTRYHANDLER_H

#include "SettingsCheckableEntryHandler.h"

#include "draupnir/traits/settings/main_window/MinimizeOnCloseSetting.h"
#include "draupnir/traits/entries/SettingsMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context, class Entry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<SettingsContext,Draupnir::Menus::MinimizeOnCloseEntry>
 *  @brief This is a class.
 *
 * @todo Maybe this class needs to be placed in other file?
 * @todo Write reasonable documentation for this class. */

template<class SettingsContext>
class GenericMenuEntryHandler<SettingsContext,Draupnir::Menus::MinimizeOnCloseEntry> :
    public SettingsCheckableEntryHandler<SettingsContext,Draupnir::Menus::MinimizeOnCloseEntry>
{
public:
    GenericMenuEntryHandler(SettingsContext& context) :
        SettingsCheckableEntryHandler<SettingsContext,Draupnir::Menus::MinimizeOnCloseEntry>{context}
    {}
};

/*! @struct SettingTraitForEntry<Draupnir::Menus::MinimizeOnCloseEntry>
 *  @brief Specialization mapping a menu entry to its setting trait.
 *
 *  @details Provides a compile-time association between the menu entry trait Draupnir::Menus::MinimizeOnCloseEntry and
 *           the corresponding MinimizeOnClose setting trait. This mapping is required by SettingsMenuEntriesHandler to
 *           resolve the correct value type and persistence key.
 *
 * @todo Maybe this class needs to be placed in other file?
 * @todo Maybe include this mapping into the menu entry trait class? */

template<> struct SettingTraitForEntry<Draupnir::Menus::MinimizeOnCloseEntry> {
    using type = Settings::MainWindow::MinimizeOnCloseSetting;
};

}; // namespace Draupnir::Menus

#endif // MINIMIZEONCLOSEENTRYHANDLER_H
