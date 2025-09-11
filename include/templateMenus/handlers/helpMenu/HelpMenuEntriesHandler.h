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

#include "../GenericMenuEntriesHandler.h"

namespace Draupnir::Menus {

/*! @class HelpMenuEntriesHandler draupnir-lib/include/templateMenus/handlers/helpMenu/HelpMenuEntriesHandler.h
 *  @brief Composite handler for help-related menu entries.
 *  @details Inherits from GenericMenuEntriesHandler and aggregates handlers for all specified help menu entries. This class
 *           acts as a convenience alias to group together various help-related actions (e.g., "About App", "About Qt", "Help
 *           Entry") under a common context.
 *
 *           Inherits all behavior from GenericMenuEntriesHandler (instantiates a handler for each HandledEntry). You are
 *           expected to specify as HandledEntries the traits corresponding to all help menu entries you want to handle.
 *
 *  @tparam HelpContext - The context type providing data and methods needed by help menu entry handlers.
 *  @tparam HandledEntries - Variadic parameter pack listing all handled help menu entry trait types (e.g., AboutAppMenuTrait).
 *
 * @see GenericMenuEntriesHandler, AboutAppEntryHandler, AboutQtEntryHandler, HelpEntryHandler */

template<class HelpContext, class... HandledEntries>
class HelpMenuEntriesHandler : public GenericMenuEntriesHandler<HelpContext,HandledEntries...>
{};

} // namespace Draupnir::Menus

#endif // HELPMENUENTRIESHANDLER_H
