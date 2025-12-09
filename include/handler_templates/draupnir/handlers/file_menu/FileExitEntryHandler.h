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

#ifndef FILEEXITENTRYHANDLER_H
#define FILEEXITENTRYHANDLER_H

#include "draupnir/handlers/templates/ActionHandler.h"

#include <QApplication>

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers
{

template<class FileContext,class MenuEntry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<FileContext, Draupnir::Ui::ExitApplicationEntry>
 *  @headerfile draupnir/handlers/file_menu/FileExitEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Specialization of the menu entry handler for "Exit Application" actions.
 *
 *  @details This template specialization handles the "Exit Application" action in a file menu or similar context. It inherits
 *           QAction connection logic from ActionHandler, and provides a concrete slot (`onTriggered()`) that, when called,
 *           simply terminates the application by invoking `qApp->quit()`.
 *
 *  @tparam FileContext A context class that provides access to file management and application state. */

template<class FileContext>
class GenericMenuEntryHandler<FileContext,Draupnir::Ui::ExitApplicationEntry> :
        public ActionHandler<GenericMenuEntryHandler<FileContext,Draupnir::Ui::ExitApplicationEntry>>
{
public:
    /*! @brief Constructs the handler. Context is required by interface but not used directly.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandler(FileContext&) {};

    /*! @brief Slot called when the "Exit Application" menu entry is triggered. Simply requests the application to quit. */
    void onTriggered() {
        qApp->quit();
    }
};

};

#endif // FILEEXITENTRYHANDLER_H
