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

#ifndef HELPENTRYHANDLER_H
#define HELPENTRYHANDLER_H

#include "draupnir/handlers/templates/ActionHandler.h"

#include <QDialog>
#include <QPointer>

#include "draupnir/traits/entries/HelpMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context,class HandledEntry>
class GenericMenuEntryHandler;

/*! @class GenericHelpMenuEntryHandler<HelpContext, Draupnir::Menus::HelpEntryMenuTrait>
 *  @headerfile draupnir/handlers/help_menu/HelpEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Specialization of the menu entry handler for HelpEntryMenuTrait
 *  @tparam HelpContext A class providing a static method `createHelpDialog()` that returns either a QDialog* or a type
 *          derived from QDialog.
 *
 *  @details This specialization handles help menu entry that should open a custom dialog. It inherits QAction connection
 *           logic from ActionHandler, and defines the slot (`onTriggered()`) which creates and shows the dialog using a
 *           static factory method on the provided HelpContext.
 *
 *           Only one instance of the dialog is created and reused on subsequent triggers. The handler uses QPointer to
 *           safely manage the dialog lifetime, even if the dialog is closed externally.
 *
 * @todo Make GenericMenuEntryHandler for the Help Menu: CRTP with user override. static_assert if sth is wrong.
 * @todo Is this should be contextless? */

template<class HelpContext>
class GenericMenuEntryHandler<HelpContext,Draupnir::Menus::HelpEntryMenuTrait> :
        public ActionHandler<GenericMenuEntryHandler<HelpContext,Draupnir::Menus::HelpEntryMenuTrait>>
{
private:
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
    /*! @brief Constructs the handler. Performs a static_assert to ensure that HelpContext provides a valid createHelpDialog method.
     *  @param Unused Reference to the help context (required for interface compatibility). */
    GenericMenuEntryHandler(HelpContext&) :
        p_helpDialog{nullptr}
    {
        static_assert(has_createHelpDialog<HelpContext>::value,
                "Provided HelpContext template argument must have HelpContext::createHelpDialog() static method "
                "returning either pointer to the QDialog or class derived from it.");
    };

    /*! @brief Slot called when the help menu entry is triggered. Shows the dialog provided by HelpContext::createHelpDialog().
     *         If the dialog has already been created, it will be shown again. Uses QPointer to ensure safe dialog management.
     * @todo Who is responsible for dialog configuration? (Title, icon, etc). This method or the HelpContext? */
    void onTriggered() {
        if (p_helpDialog) {
            p_helpDialog->show();
            return;
        }

        p_helpDialog = HelpContext::createHelpDialog();
        p_helpDialog->setParent(qApp->activeWindow());
        p_helpDialog->setAttribute(Qt::WA_DeleteOnClose);
        p_helpDialog->show();
    }

private:
    QPointer<QDialog> p_helpDialog;
};

};

#endif // HELPENTRYHANDLER_H
