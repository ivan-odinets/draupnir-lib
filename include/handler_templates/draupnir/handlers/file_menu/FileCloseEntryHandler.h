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

#ifndef FILECLOSEENTRYHANDLER_H
#define FILECLOSEENTRYHANDLER_H

#include "draupnir/handlers/templates/ActionHandler.h"

#include <QApplication>
#include <QMessageBox>

#include "draupnir/traits/entries/FileMenuEntries.h"
#include "draupnir/utils/FileManagerValidator.h"

namespace Draupnir::Handlers
{

template<class Context, class Entry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileCloseEntry>
 *  @headerfile draupnir/handlers/file_menu/FileCloseEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Specialization of the menu entry handler for the "File -> Close" menu entry.
 *
 *  @details This specialization provides the logic for handling file close operations in the file menu. It uses ActionHandler
 *           to connect the QAction, and implements the logic for user interaction when closing a file with possible unsaved
 *           changes./include/templateHandlers
 *
 *           This works in a following way:
 *           - The handler checks if the current file is saved by calling FileManager::isCurrentFileSaved().
 *           - If unsaved changes are detected, it prompts the user to save, discard, or cancel.
 *           - The handler uses compile-time traits to verify that FileManager implements both isCurrentFileSaved() (returns
 *             bool) and closeCurrentFile() (returns void).
 *           - Logic ensures that fileManager() is not null before proceeding.
 *
 *  @tparam FileContext - A class providing access to the file manager and related UI interactions. Must define a nested type
 *          FileManager, and provide methods:
 *          - FileManager* fileManager();
 *          - int askUser(const QString& title, const QString& message, QMessageBox::StandardButtons buttons);
 *          - void onSaveFile(); (Check FileEntriesHandlersContext)
 *
 * @todo Write a test for this class. */

template<class FileContext>
class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileCloseEntry> :
    public ActionHandler<GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileCloseEntry>>
{
public:
    /*! @brief Constructs the handler, enforcing interface compliance for FileManager. Static assertions ensure the FileManager
     *         implements required methods.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandler(FileContext& context) :
        m_context(context)
    {
        // If static_assert happens here - the FileManager provided to the FileMenuEntriesHandler must be updated.
        static_assert(FileManagerValidator::has_isCurrentFileSaved<typename FileContext::FileManager>::value,
                "FileContext::FileManager must have bool isCurrentFileSaved() method.");
        static_assert(FileManagerValidator::has_closeCurrentFile<typename FileContext::FileManager>::value,
                "FileContext::FileManager must have void closeCurrentFile() method.");
    };

    /*! @brief Slot called when the "Close File" menu entry is triggered. Prompts the user to save unsaved changes before
     *         closing the file. If the file is already saved, it is closed directly. Otherwise, the user is presented
     *         with options to Save, Discard, or Cancel. */
    void onTriggered() {
        Q_ASSERT_X(m_context.fileManager() != nullptr,"GenericMenuEntryHandler<FileContext,FileCloseEntry>::onTriggered",
                   "FileManager must be specified before by using FileMenuEntriesHandler::setFileManager method");

        if (m_context.fileManager()->hasNothingOpened())
            return;

        if (!m_context.fileManager()->isCurrentFileSaved()) {
            const int userSelection = FileContext::askUser(
                QObject::tr("Close current file?"),
                QObject::tr("Current file was modified. Do you want to save your changes or discard them?"),
                QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel
            );

            if (userSelection == QMessageBox::Save) {
                m_context.onSaveFile();
            } else if (userSelection == QMessageBox::Cancel) {
                return;
            }
        }

        m_context.fileManager()->closeCurrentFile();
    }

private:
    FileContext& m_context;
};

};

#endif // FILECLOSEENTRYHANDLER_H
