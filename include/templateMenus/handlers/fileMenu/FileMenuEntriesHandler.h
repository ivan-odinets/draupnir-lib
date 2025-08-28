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

#include "../GenericMenuEntriesHandler.h"

#include "FileEntriesHandlersContext.h"
#include "FileNewEntryHandler.h" // IWYU pragma: keep
#include "FileOpenEntryHandler.h" // IWYU pragma: keep
#include "FileSaveEntryHandler.h" // IWYU pragma: keep
#include "FileSaveAsEntryHandler.h" // IWYU pragma: keep
#include "FileCloseEntryHandler.h" // IWYU pragma: keep
#include "FileRecentEntryHandler.h" // IWYU pragma: keep
#include "FileExitEntryHandler.h" // IWYU pragma: keep

namespace Draupnir::Menus {

/*! @class FileMenuEntriesHandler draupnir-lib/include/templateMenus/handlers/fileMenu/FileMenuEntriesHandler.h
 *  @brief Handler for File menu entries, managing their interaction with a specific FileManager.
 *  @details This template aggregates handlers for all specified file-related menu entries (such as "New", "Open", "Save",
 *           "Close", etc.) and delegates context and state management to FileEntriesHandlersContext. It inherits from
 *           GenericMenuEntriesHandler, instantiating one handler per HandledEntry type, and manages their lifecycle.
 *
 *           This class:
 *           - Is designed to be used as a ready-made handler for all file menu actions in the application.
 *           - Stores its context as a FileEntriesHandlersContext<FileManager>.
 *           - Provides a convenient setter to assign the FileManager instance at runtime.
 *           - All actual file menu logic and state is handled by the base context class and its handlers.
 *
 * @tparam FileManager The concrete file manager type used in this menu handler. Must provide the expected interface
 *         (see FileManagerValidator for requirements).
 * @tparam HandledEntries Variadic parameter pack of all file menu entry trait types that this handler should manage.
 *
 * @note Before using this handler, you **must** call setFileManager() to assign a valid FileManager instance.
 *
 * @see FileManagerValidator, FileEntriesHandlersContext, GenericMenuEntriesHandler */

template<class FileManager, class... HandledEntries>
class FileMenuEntriesHandler :
        public GenericMenuEntriesHandler<FileEntriesHandlersContext<FileManager>,HandledEntries...>
{
public:
    using Base = GenericMenuEntriesHandler<FileEntriesHandlersContext<FileManager>,HandledEntries...>;

    FileMenuEntriesHandler() :
        GenericMenuEntriesHandler<FileEntriesHandlersContext<FileManager>,HandledEntries...>()
    {
        Base::context().registerSaveMethod([this](){
            Base::template getHandler<FileSaveEntry>().onTriggered();
        });
        Base::context().registerSaveAsMethod([this](){
            Base::template getHandler<FileSaveAsEntry>().onTriggered();
        });

    }

    /*! @brief Assigns the FileManager instance to be used by this handler and its context. Delegates to the underlying context
     *         for actual storage.
     *  @param fileManager Pointer to the FileManager object. */
    void setFileManager(FileManager* fileManager) {
        Q_ASSERT_X(fileManager, "FileMenuEntriesHandler::setFileManager",
                   "Provided FileManager pointer is nullptr.");

        GenericMenuEntriesHandler<
                FileEntriesHandlersContext<FileManager>,HandledEntries...
        >::context().setFileManager(fileManager);
    }
};

} // namespace Draupnir::Menus

#endif // FILEMENUENTRIESHANDLER_H
