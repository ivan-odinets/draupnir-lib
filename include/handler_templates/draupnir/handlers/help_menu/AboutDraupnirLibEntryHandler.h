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

#ifndef ABOUTDRAUPNIRLIBENTRYHANDLER_H
#define ABOUTDRAUPNIRLIBENTRYHANDLER_H

#include "draupnir/handlers/templates/ActionHandler.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"

#include <QMessageBox>
#include <QPointer>

#include "draupnir/utils/ResourceHelper.h"

namespace Draupnir::Handlers
{

template<class Context,class HandledEntry>
class GenericMenuEntryHandler;

/*! @class GenericHelpMenuEntryHandler<HelpContext, Draupnir::Ui::AboutDraupnirLibMenuTrait>
 *  @ingroup HandlerTemplates
 *  @headerfile draupnir/handlers/help_menu/AboutDraupnirLibEntryHandler.h
 *  @brief Partial specialization that wires a QAction to the standard dialog About Draupnir Lib.
 *  @tparam HelpContext Any type representing the help/“About” handling context (unused in this specialization).
 *
 * @note The connection is made without an explicit QObject context; lifetime of QAction must be managed externally. */

template<class HelpContext>
class GenericMenuEntryHandler<HelpContext,Draupnir::Ui::AboutDraupnirLibMenuTrait> :
        public ActionHandler<GenericMenuEntryHandler<HelpContext,Draupnir::Ui::AboutDraupnirLibMenuTrait>>
{
public:
    /*! @brief Constructs the handler.
     *  @param HelpContext Reference to a help context. Not used by this specialization. */
    GenericMenuEntryHandler(HelpContext&) :
        p_msgBox{nullptr}
    {}

    /*! @brief This method will be executed when user clicks on the About Draupnir Lib QAction.
     * @todo Write a bit more extended text. */
    void onTriggered() {
        if (!p_msgBox.isNull()) {
            p_msgBox->raise();
            p_msgBox->activateWindow();
            p_msgBox->showNormal();
            return;
        }

        p_msgBox = new QMessageBox;

        p_msgBox->setWindowModality(   Qt::NonModal);
        p_msgBox->setAttribute(        Qt::WA_DeleteOnClose);

        p_msgBox->setWindowTitle( QObject::tr("About draupnir-lib - ").append(qApp->applicationName()));
        p_msgBox->setIconPixmap(  Resources::ResourceHelper::get().draupnirIcon().pixmap(128,128));
        p_msgBox->setWindowIcon(  qApp->windowIcon());
        p_msgBox->setSizePolicy(  QSizePolicy::Expanding,QSizePolicy::Minimum);
        p_msgBox->setTextFormat(  Qt::RichText);
        p_msgBox->setText(        Resources::ResourceHelper::get().aboutDraupnirLibText());

        p_msgBox->show();
    }

private:
    QPointer<QMessageBox> p_msgBox;
};

} // namespace Draupnir::Handlers

#endif // ABOUTDRAUPNIRLIBENTRYHANDLER_H
