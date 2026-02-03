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

#ifndef FILENEWENTRYHANDLER_H
#define FILENEWENTRYHANDLER_H

#include "draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h"

#include <QMessageBox>

#include "draupnir/ui_bricks/concepts/FileManagerConcept.h"
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context,class MenuEntry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericMenuEntryHandlerTemplate<FileContext, Draupnir::Ui::FileNewEntry>
 *  @headerfile draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h
 *  @ingroup UiBricks
 *  @brief Handles the **File → New** menu action.
 *  @tparam FileContext Context type that provides access to a FileManager instance and (optionally) UI helpers used in
 *          single-file workflow.
 *
 *  @details This handler supports two workflows depending on the compile-time capability flag: `FileContext::FileManager::canHaveMultipleFilesOpened()`.
 *
 *           ### Multi-file workflow
 *           If `FileManager::canHaveMultipleFilesOpened()` exists and returns `true` at compile time, the handler always creates a
 *           new file by calling `FileManager::newFile()`. No additional checks or prompts are performed.
 *
 *           ### Single-file workflow
 *           If `FileManager::canHaveMultipleFilesOpened()` is not available or returns `false`, the handler assumes that only
 *           one file can be open at a time and performs user-friendly replacement logic:
 *           - If nothing is opened (`FileManager::hasNothingOpened()` returns `true`) → creates a new file immediately.
 *           - If a file is opened and already saved (`FileManager::isCurrentFileSaved()` returns `true`) → asks the user for
 *             confirmation via `FileContext::askUser(...)`.
 *           - If a file is opened and has unsaved changes → asks the user whether to **Save**, **Discard**, or **Cancel**.
 *           - **Save** triggers `FileContext::onSaveFile()` and then creates the new file.
 *           - **Discard** creates the new file immediately.
 *           - **Cancel** aborts the operation.
 *
 *           Runtime safety: `fileManager()` is expected to return a non-null pointer. The handler checks this via `Q_ASSERT_X`
 *           before performing any logic.
 *
 *           Required API (always):
 *           `FileContext` must provide:
 *           - `using FileManager = ...;`
 *           - `FileManager* fileManager();`
 *           `FileContext::FileManager` must provide:
 *           - `void newFile();`
 *
 *           Required API (only for single-file workflow):
 *           - If `FileManager::canHaveMultipleFilesOpened()` is missing or yields `false`, the following additional API is required.
 *             `FileManager` must provide:
 *             - `bool hasNothingOpened();`
 *             - `bool isCurrentFileSaved();`
 *             `FileContext` must provide:
 *             - `static int askUser(QString title, QString message, QMessageBox::StandardButtons buttons);`
 *             - `void onSaveFile();` */

template<class Context>
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry> :
    public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry>>
{
public:
    /*! @brief Constructs the handler.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandlerTemplate(Context& context) :
        m_context{context}
    {}

    /*! @brief Slot called when the "File → New" action is triggered. Implements logic for creating a new file, prompting the
     *         user if needed. */
    void onTriggered() {
        Q_ASSERT_X(m_context.fileManager() != nullptr,
            "GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::FileNewEntry>::onTriggered",
            "FileManager must be specified before.");
        if constexpr (FileManagerConcept::CanHaveMultipleFilesOpened<typename Context::FileManager>) {
            m_context.fileManager()->newFile();
        } else {
            const bool hasNothingOpened = m_context.fileManager()->hasNothingOpened();
            // If we have nothing opened - open the new file.
            if (hasNothingOpened) {
                m_context.fileManager()->newFile();
                return;
            }

            const bool isCurrentFileSaved = m_context.fileManager()->isCurrentFileSaved();
            // If we have sth opened, and saved
            if (isCurrentFileSaved) {
                const int userSelection = Context::askUser(
                    QObject::tr("Replace current file?"),
                    QObject::tr("This action will close current file and create a new one. Continue?"),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );

                if (userSelection == QMessageBox::Yes)
                    m_context.fileManager()->newFile();
                else
                    return;
            } else {
                // If we have sth opened, BUT not saved
                const int userSelection = Context::askUser(
                    QObject::tr("Replace current file?"),
                    QObject::tr("Current file was modified. Do you want to save your changes or discard them?"),
                    QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel
                    );

                if (userSelection == QMessageBox::Discard) {
                    m_context.fileManager()->newFile();
                } else if (userSelection == QMessageBox::Save) {
                    m_context.onSaveFile();
                    m_context.fileManager()->newFile();
                } else {
                    return;
                }
            }
        }
    }

private:
    Context& m_context;

};

}; // namespace Draupnir::Menus

#endif // FILENEWENTRYHANDLER_H
