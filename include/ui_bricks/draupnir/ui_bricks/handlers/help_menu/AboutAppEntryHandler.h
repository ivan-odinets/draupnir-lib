/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h"

#include <QMessageBox>
#include <QPointer>

#include "draupnir/ui_bricks/concepts/HelpContextConcept.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"

namespace Draupnir::Handlers
{

template<class HelpContext,class MenuEntry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericHelpMenuEntryHandler<HelpContext, Draupnir::Ui::AboutAppMenuTrait>
 *  @headerfile draupnir/handlers/help_menu/AboutAppEntryHandler.h
 *  @ingroup UiBricks
 *  @brief Specialization of the menu entry handler for the "About Application" menu entry.
 *  @tparam HelpContext A class providing static information for the help/about menu (must define static QString aboutAppText()).
 *
 *  @details This template specialization handles the "About App" action in a help menu. It inherits the `QAction` connection
 *           logic from ActionHandler, and provides a concrete slot (`onTriggered()`) that displays an about dialog with
 *           information taken from the given HelpContext.
 *
 *           The `HelpContext` template parameter is expected to define a static method `aboutAppText()` returning a QString
 *           containing a rich-text description of the application.
 *
 *           The handler sets the dialog’s parent to the active window, applies the current application icon, and expands
 *           the dialog for better readability. */

template<class Context>
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::AboutAppMenuTrait> :
    public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::AboutAppMenuTrait>>
{
public:
    /*! @brief Constructs the handler.
     * @note Performs a static_assert to ensure that HelpContext provides the required static method. */
    GenericMenuEntryHandlerTemplate() {
        static_assert(Draupnir::Ui::HelpContext::HasAboutAppText<Context>,
            "Provided HelpContext template argument must have QString HelpContext::aboutAppText() static method.");
    }

    /*! @brief Slot called when the "About" menu entry is triggered. Shows a QMessageBox with application info. The message text
     *         is taken from HelpContext::aboutAppText(). The dialog uses rich text formatting and adapts its size for readability. */
    void onTriggered() {
        if (p_dialog) {
            p_dialog->raise();
            p_dialog->activateWindow();
            p_dialog->showNormal();
            return;
        }

        QMessageBox* msgBox = new QMessageBox;
        msgBox->setWindowModality(Qt::NonModal);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);

        msgBox->setWindowTitle(    QObject::tr("About %1").arg(qApp->applicationName()));
        msgBox->setIconPixmap(     qApp->windowIcon().pixmap(128,128));
        msgBox->setWindowIcon(     qApp->windowIcon());
        msgBox->setSizePolicy(     QSizePolicy::Expanding,QSizePolicy::Minimum);
        msgBox->setTextFormat(     Qt::RichText);
        msgBox->setText(           Context::aboutAppText());

        p_dialog = msgBox;
        p_dialog->show();
    }

private:
    QPointer<QDialog> p_dialog;
};

};

#endif // ABOUTAPPENTRYHANDLER_H
