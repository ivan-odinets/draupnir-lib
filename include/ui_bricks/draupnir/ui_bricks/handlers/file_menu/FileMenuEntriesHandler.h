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

#ifndef FILEMENUENTRIESHANDLER_H
#define FILEMENUENTRIESHANDLER_H

#include "draupnir/ui_bricks/handlers/templates/MenuHandlerTemplate.h"

#include <QFileDialog>
#include <QMessageBox>

#include "draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h"     // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileOpenEntryHandler.h"    // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileSaveEntryHandler.h"    // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileSaveAsEntryHandler.h"  // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileCloseEntryHandler.h"   // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileRecentEntryHandler.h"  // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileExitEntryHandler.h"    // IWYU pragma: keep

#include "draupnir/ui_bricks/handlers/file_menu/FileMenuContext.h"

namespace Draupnir::Handlers {

/*! @class FileMenuEntriesHandler
 *  @headerfile draupnir/ui_bricks/handlers/file_menu/FileMenuEntriesHandler.h
 *  @ingroup UiBricks
 *  @brief Handler for File menu entries, managing their interaction with a specific `FileManager`.
 *  @tparam FileManager The concrete file manager type used in this menu handler. Must provide the expected interface.
 *  @tparam HandledEntries... Variadic parameter pack of all file menu entry trait types that this handler should manage.
 *
 *  @details This template aggregates handlers for all specified file-related menu entries (such as "New", "Open", "Save",
 *           "Close", etc.).
 *
 * @note Before using this handler, you **must** call setFileManager() to assign a valid FileManager instance. */

template<class FileManagerClass, class... Entries>
using FileMenuHandler = MenuHandlerTemplate<
    GenericFileMenuContext<FileManagerClass,Entries...>,
    GenericMenuEntryHandlerTemplate,
    Entries...
>;

} // namespace Draupnir::Menus

#endif // FILEMENUENTRIESHANDLER_H
