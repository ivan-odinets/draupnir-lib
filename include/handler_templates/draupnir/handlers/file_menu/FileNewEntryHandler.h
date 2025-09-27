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

#ifndef FILENEWENTRYHANDLER_H
#define FILENEWENTRYHANDLER_H

#include "draupnir/handlers/AbstractHandlers.h"

#include <QDebug>
#include <QMessageBox>

#include "draupnir/traits/entries/FileMenuEntries.h"
#include "draupnir/utils/FileManagerValidator.h"

namespace Draupnir::Handlers
{

/*! @class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileNewEntry>
 *  @headerfile draupnir/handlers/file_menu/FileNewEntryHandler.h
 *  @brief Specialization for handling the "File → New" menu action.
 *  @details This handler implements the logic for creating a new file based on the capabilities of the associated FileManager.
 *           It respects both single-file and multi-file workflows by querying FileManager::canHaveMultipleFilesOpened() at
 *           compile time.
 *
 *           The behavior is the following:
 *           - If FileManager allows multiple files to be opened (canHaveMultipleFilesOpened() returns true), a new file is
 *             always created (FileManager::newFile()).
 *           - If only a single file may be opened at a time:
 *             - If no file is open: just create a new file.
 *             - If an opened file is present and saved: prompts user for confirmation before replacing the file.
 *             - If the opened file has unsaved changes: prompts user to save, discard, or cancel, and acts accordingly.
 *           - All user interactions are handled via FileContext::askUser.
 *
 *  @tparam FileContext The context class providing access to the FileManager and UI helpers (such as askUser).
 * @note FileManager must provide the following API:
 *       - static constexpr bool canHaveMultipleFilesOpened();
 *       - void newFile();
 *       - bool hasNothingOpened();
 *       - bool isCurrentFileSaved();
 *
 * @see ActionHandler, FileManagerValidator, FileEntriesHandlersContext */

template<class FileContext>
class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileNewEntry> :
        public ActionHandler<GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileNewEntry>,Draupnir::Menus::FileNewEntry>
{
public:
    /*! @brief Constructs the handler.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandler(FileContext& context) :
        m_context(context)
    {
        static_assert(FileManagerValidator::has_newFile<typename FileContext::FileManager>::value,
                "FileManager must have newFile() method.");
        static_assert(FileManagerValidator::has_hasNothingOpened<typename FileContext::FileManager>::value,
                "FileManager must have hasNothingOpened method.");
        static_assert(FileManagerValidator::has_isCurrentFileSaved<typename FileContext::FileManager>::value,
                "FileManager must have isCurrentFileSaved method.");
    };

    /*! @brief Slot called when the "File → New" action is triggered. Implements logic for creating a new file, prompting the
     *         user if needed. */
    void onTriggered() {
        Q_ASSERT_X(m_context.fileManager() != nullptr,"FileMenuEntriesHandler::onNewFile",
                   "FileManager must be specified before.");

        if constexpr (FileContext::FileManager::canHaveMultipleFilesOpened()) {
            m_context.fileManager()->newFile();
        } else {
            // If we have nothing opened - open the new file.
            if (m_context.fileManager()->hasNothingOpened()) {
                m_context.fileManager()->newFile();
                return;
            }

            // If we have sth opened, and saved
            if ( (!m_context.fileManager()->hasNothingOpened()) && m_context.fileManager()->isCurrentFileSaved()) {
                const int userSelection = FileContext::askUser(
                    QObject::tr("Replace current file?"),
                    QObject::tr("This action will close current file and create a new one. Continue?"),
                    QMessageBox::Yes | QMessageBox::Cancel
                );

                if (userSelection == QMessageBox::Yes)
                    m_context.fileManager()->newFile();
            }

            // If we have sth opened, BUT not saved
            if ( (!m_context.fileManager()->hasNothingOpened()) && (!m_context.fileManager()->isCurrentFileSaved()) ) {
                const int userSelection = FileContext::askUser(
                    QObject::tr("Replace current file?"),
                    QObject::tr("Current file was modified. Do you want to save your changes or discard them?"),
                    QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel
                );

                if (userSelection == QMessageBox::Discard) {
                    m_context.fileManager()->newFile();
                } else if (userSelection == QMessageBox::Save) {
                    m_context.onSaveFile();
                    m_context.fileManager()->newFile();
                }
            }
        }
    }

private:
    FileContext& m_context;
};

}; // namespace Draupnir::Menus

#endif // FILENEWENTRYHANDLER_H
