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

#include "draupnir/ui_bricks/handlers/templates/GenericMenuEntriesHandler.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutDraupnirLibEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutQtEntryHandler.h"

namespace Draupnir::Handlers {

/*! @class HelpMenuEntriesHandler
 *  @headerfile draupnir/handlers/help_menu/HelpMenuEntriesHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Composite handler for help-related menu entries.
 *  @tparam HelpSource - The context type providing data and methods needed by help menu entry handlers.
 *  @tparam HandledEntries - Variadic parameter pack listing all handled help menu entry trait types (e.g., AboutAppMenuTrait).
 *
 *  @details Inherits from GenericMenuEntriesHandler and aggregates handlers for all specified help menu entries. This class
 *           acts as a convenience alias to group together various help-related actions (e.g., "About App", "About Qt", "Help
 *           Entry") under a common context.
 *
 *           Inherits all behavior from GenericMenuEntriesHandler (instantiates a handler for each HandledEntry). You are
 *           expected to specify as HandledEntries the traits corresponding to all help menu entries you want to handle.
 *
 * @todo Add static_asserts to verify the Help source.
 * @todo Add at least compilation test. */

template<class HelpSource, class... HandledEntries>
class HelpMenuEntriesHandler :
        public GenericMenuEntriesHandler<HelpMenuEntriesHandler<HelpSource,HandledEntries...>,HandledEntries...>
{
    /*! @struct has_createHelpDialog
     *  @brief Trait to check whether HelpContext defines a static method createHelpDialog() returning QDialog* or derived. */
    template<class, class = std::void_t<>>
    struct has_createHelpDialog : std::false_type {};

    template<class Class>
    struct has_createHelpDialog<
        Class,
        std::void_t<decltype(
            std::is_base_of_v<QDialog,std::remove_pointer_t<decltype(Class::createHelpDialog())>> ||
            std::is_same_v<QDialog,std::remove_pointer_t<decltype(Class::createHelpDialog())>>
        )>
    > : std::true_type {};

public:
    HelpMenuEntriesHandler() :
        GenericMenuEntriesHandler<HelpMenuEntriesHandler<HelpSource,HandledEntries...>,HandledEntries...>()
    {}

    template<bool cond = draupnir::utils::is_one_of_v<Draupnir::Ui::AboutAppMenuTrait,HandledEntries...>>
    static std::enable_if_t<cond,QString>
    aboutAppText() {
        return HelpSource::aboutAppText();
    }

    template<bool cond = draupnir::utils::is_one_of_v<Draupnir::Ui::HelpEntryMenuTrait,HandledEntries...>>
    static std::enable_if_t<cond,QDialog*>
    createHelpDialog() {
        return HelpSource::createHelpDialog();
    }
};

} // namespace Draupnir::Menus

#endif // HELPMENUENTRIESHANDLER_H
