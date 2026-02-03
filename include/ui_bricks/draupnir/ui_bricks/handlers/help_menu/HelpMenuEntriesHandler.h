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

#ifndef HELPMENUENTRIESHANDLER_H
#define HELPMENUENTRIESHANDLER_H

#include "draupnir/ui_bricks/handlers/templates/MenuHandlerTemplate.h"

#include "draupnir/ui_bricks/handlers/help_menu/AboutAppEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutDraupnirLibEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutQtEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/HelpEntryHandler.h"


namespace Draupnir::Handlers {

/*! @headerfile draupnir/handlers/help_menu/HelpMenuEntriesHandler.h
 *  @ingroup UiBricks
 *  @brief Composite handler for help-related menu entries.
 *  @tparam HelpSource - The context type providing data and methods needed by help menu entry handlers.
 *  @tparam HandledEntries - Variadic parameter pack listing all handled help menu entry trait types (e.g., AboutAppMenuTrait). */

template<class HelpContext, class... HandledEntries>
using HelpMenuEntriesHandler = MenuHandlerTemplate<HelpContext,GenericMenuEntryHandlerTemplate,HandledEntries...>;

} // namespace Draupnir::Menus

#endif // HELPMENUENTRIESHANDLER_H
