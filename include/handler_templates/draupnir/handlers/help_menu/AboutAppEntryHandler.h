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

#ifndef ABOUTAPPENTRYHANDLER_H
#define ABOUTAPPENTRYHANDLER_H

#include "../AbstractHandlers.h"

#include <QMessageBox>

#include "draupnir/traits/entries/HelpMenuEntries.h"

namespace Draupnir::Handlers
{

/*! @class GenericHelpMenuEntryHandler<HelpContext, Draupnir::MenusAboutAppMenuTrait>
 *  @headerfile draupnir/handlers/help_menu/AboutAppEntryHandler.h
 *
 *  @brief Specialization of the menu entry handler for the "About Application" menu entry.
 *  @details This template specialization handles the "About App" action in a help menu. It inherits the QAction connection
 *           logic from ActionHandler, and provides a concrete slot (`onTriggered()`) that displays an about dialog with
 *           information taken from the given HelpContext.
 *
 *           The HelpContext template parameter is expected to define a static method `aboutAppText()` returning a QString
 *           containing a rich-text description of the application.
 *
 *           The handler sets the dialog’s parent to the active window, applies the current application icon, and expands
 *           the dialog for better readability.
 * @tparam HelpContext A class providing static information for the help/about menu (must define static QString aboutAppText()).
 * @see ActionHandler */

template<class HelpContext>
class GenericMenuEntryHandler<HelpContext,Draupnir::Menus::AboutAppMenuTrait> :
        public ActionHandler<GenericMenuEntryHandler<HelpContext,Draupnir::Menus::AboutAppMenuTrait>,Draupnir::Menus::AboutAppMenuTrait>
{
private:
    /*! @struct has_aboutAppText
     *  @brief Trait to check whether HelpContext defines a static method aboutAppText() returning QString. Used at compile time
     *         to enforce interface compliance of HelpContext. */
    template<class, class = std::void_t<>>
    struct has_aboutAppText : std::false_type {};

    template<class Class>
    struct has_aboutAppText<
        Class,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(Class::aboutAppText())>
        )>
    > : std::true_type {};

public:
    /*! @brief Constructs the handler. Context argument is not used directly, but required by interface.
     *  @param Unused Reference to the help context.
     * @note Performs a static_assert to ensure that HelpContext provides the required static method. */
    GenericMenuEntryHandler(HelpContext&) {
        static_assert(has_aboutAppText<HelpContext>::value,
                "Provided HelpContext template argument must have QString HelpContext::aboutAppText() static method.");
    };

    /*! @brief Slot called when the "About" menu entry is triggered. Shows a QMessageBox with application info. The message text
     *         is taken from HelpContext::aboutAppText(). The dialog uses rich text formatting and adapts its size for readability. */
    void onTriggered() {
        QMessageBox msgBox;
        msgBox.setWindowTitle(    QObject::tr("About %1").arg(qApp->applicationName()));
        msgBox.setIconPixmap(     qApp->windowIcon().pixmap(128,128));
        msgBox.setWindowIcon(     qApp->windowIcon());
        msgBox.setSizePolicy(     QSizePolicy::Expanding,QSizePolicy::Minimum);
        msgBox.setTextFormat(     Qt::RichText);
        msgBox.setText(           HelpContext::aboutAppText());
        msgBox.exec();
    }
};

};

#endif // ABOUTAPPENTRYHANDLER_H
