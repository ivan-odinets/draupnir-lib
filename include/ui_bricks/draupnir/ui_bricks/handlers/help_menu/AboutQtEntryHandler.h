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

#ifndef ABOUTQTENTRYHANDLER_H
#define ABOUTQTENTRYHANDLER_H

#include <QApplication>

#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context,class HandledEntry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericHelpMenuEntryHandler<HelpContext, Draupnir::Ui::AboutQtMenuTrait>
 *  @headerfile draupnir/handlers/help_menu/AboutQtEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @tparam HelpContext Any type representing the help/“About” handling context (unused in this specialization).
 *  @brief Partial specialization that wires a QAction to the standard QApplication::aboutQt dialog.
 *
 *  @details This specialization handles the AboutQtMenuTrait entry by connecting its QAction::triggered() signal directly to
 *           QApplication::aboutQt. It ignores the provided - HelpContext because no contextual data is required to show
 *           the standard Qt “About” dialog. */

template<class HelpContext>
class GenericMenuEntryHandlerTemplate<HelpContext,Draupnir::Ui::AboutQtMenuTrait>
{
public:
    /*! @brief Constructs the handler. */
    GenericMenuEntryHandlerTemplate() {}

    /*! @brief Connects the provided `QAction` to `QApplication::aboutQt`.
     *  @param entry Pointer to the action representing the "About Qt" menu item. */
    void connect(QAction* entry) {
        QObject::connect(entry,   &QAction::triggered,
                         qApp,    &QApplication::aboutQt);
    }
};

} // namespace Draupnir::Menus

#endif // ABOUTQTENTRYHANDLER_H
