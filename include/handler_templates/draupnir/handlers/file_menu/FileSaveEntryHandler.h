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

#ifndef FILESAVEENTRYHANDLER_H
#define FILESAVEENTRYHANDLER_H

#include "draupnir/handlers/templates/ActionHandler.h"

#include "draupnir/traits/entries/FileMenuEntries.h"
#include "draupnir/utils/FileManagerValidator.h"

namespace Draupnir::Handlers
{

template<class Context, class Entry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileSaveEntry>
 *  @headerfile draupnir/handlers/file_menu/FileSaveEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Specialization of the menu entry handler for "Save File" actions.
 *
 *  @details This template specialization handles the "Save" menu action in file-related menus. It inherits QAction connection
 *           logic from ActionHandler, and provides a concrete slot (`onTriggered()`) that saves the current file using the
 *           file manager supplied by FileContext.
 *
 *           On triggering, the handler checks whether the current file has a name (i.e., was previously saved or opened).
 *           - If yes, it calls `FileManager::saveCurrentFile()`.
 *           - If no, it delegates to the file context's "Save As" handler (`onSaveFileAs()`), prompting the user for a
 *             filename.
 *
 *           At construction, a static assertion enforces that the FileManager type provided by FileContext implements the
 *           required `saveCurrentFile()` method.
 *
 *  @tparam FileContext Context class providing file management interface.
 *
 * @note This handler expects FileContext to provide:
 *   - `fileManager()`: Returns a pointer to the FileManager.
 *   - `onSaveFileAs()`: Initiates the "Save As" flow.
 *   - FileManager must implement `saveCurrentFile()` and `currentFileHasName()`.
 *
 * @todo Write a test for this class. */

template<class FileContext>
class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileSaveEntry> :
        public ActionHandler<GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileSaveEntry>>
{
public:
    /*! @brief Constructs the handler, statically asserting FileManager interface compliance.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandler(FileContext& context) :
        m_context(context)
    {
        static_assert(FileManagerValidator::has_saveCurrentFile<typename FileContext::FileManager>::value,
                "FileManager must have saveCurrentFile method");
    };
    /*! @brief Slot called when the "Save File" menu entry is triggered. If the current file has a name, saves directly;
     *         otherwise, initiates "Save As". */
    void onTriggered() {
        Q_ASSERT_X(m_context.fileManager() != nullptr,"FileMenuEntriesHandler::onSaveFile",
                   "FileManager must be specified before.");

        if (m_context.fileManager()->currentFileHasName()) {
            m_context.fileManager()->saveCurrentFile();
        } else {
            m_context.onSaveFileAs();
        }
    }

private:
    FileContext& m_context;
};

}; // namespace Draupnir::Menus

#endif // FILESAVEENTRYHANDLER_H
