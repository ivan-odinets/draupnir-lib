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
 @details This handler has two compile-time specializations selected by
 *           `FileManagerConcept::CanHaveMultipleFilesOpened<typename Context::FileManager>`.
 *
 *           ### Multi-file workflow
 *           If `Context::FileManager` satisfies
 *           `FileManagerConcept::CanHaveMultipleFilesOpened`, triggering the handler always creates a new file by calling
 *           `FileManager::newFile()`.
 *
 *           No replacement checks or user prompts are performed in this mode, because the manager is assumed to support
 *           multiple simultaneously opened files.
 *
 *           ### Single-file workflow
 *           If `Context::FileManager` does **not** satisfy
 *           `FileManagerConcept::CanHaveMultipleFilesOpened`, the handler assumes that only one file may be open at a time
 *           and performs replacement logic before creating a new file:
 *           - If nothing is currently opened (`FileManager::hasNothingOpened()` returns `true`) a new file is created immediately.
 *           - If a file is opened and has unsaved changes (`FileManager::hasUnsavedData()` returns `true`), the user is asked
 *             whether to **Save**, **Discard**, or **Cancel**.
 *           - If the user chooses **Save**, `Context::onSaveFile()` is called and then a new file is created.
 *           - If the user chooses **Discard**, a new file is created immediately.
 *           - If the user chooses **Cancel**, the operation is aborted.
 *           - If a file is opened and has no unsaved changes, the user is asked for confirmation before the current file is
 *             replaced with a new one.
 *
 *           Runtime safety: `Context::fileManager()` is expected to return a non-null pointer. This is checked with
 *           `Q_ASSERT_X` before any action is performed.
 *
 *           ## Required API
 *
 *           ### Always required
 *           `Context` must provide:
 *           - `using FileManager = ...;`
 *           - `FileManager* fileManager();`
 *
 *           `Context::FileManager` must provide:
 *           - `void newFile();`
 *
 *           ### Additionally required for single-file workflow
 *           If `Context::FileManager` does **not** satisfy
 *           `FileManagerConcept::CanHaveMultipleFilesOpened`, then `Context::FileManager` must also provide:
 *           - `bool hasNothingOpened();`
 *           - `bool hasUnsavedData();`
 *
 *           and `Context` must provide:
 *           - `static int askUser(QString title, QString message, QMessageBox::StandardButtons buttons);`
 *           - `void onSaveFile();`
 *
 *  @note In the single-file workflow, this handler delegates actual save logic to `Context::onSaveFile()` and only decides
 *        whether replacing the current file is allowed according to the user's choice.
 */

template<class Context>
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry>;

template<class Context>
    requires(FileManagerConcept::CanHaveMultipleFilesOpened<typename Context::FileManager>)
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry> :
    public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry>>
{
public:
    /*! @brief Constructs the handler.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandlerTemplate(Context* context) :
        p_context{context}
    {}

    /*! @brief Slot called when the "File → New" action is triggered. Implements logic for creating a new file, prompting the
     *         user if needed. */
    void onTriggered() {
        Q_ASSERT_X(p_context->fileManager() != nullptr,
                   "GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::FileNewEntry>::onTriggered",
                   "FileManager must be specified before.");
        p_context->fileManager()->newFile();
    }

private:
    Context* p_context;
};

template<class Context>
    requires(!FileManagerConcept::CanHaveMultipleFilesOpened<typename Context::FileManager>)
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry> :                                                                             public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileNewEntry>>
{
public:
    /*! @brief Constructs the handler.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandlerTemplate(Context* context) :
        p_context{context}
    {}

    /*! @brief Slot called when the "File → New" action is triggered. Implements logic for creating a new file, prompting the
     *         user if needed. */
    void onTriggered() {
        Q_ASSERT_X(p_context->fileManager() != nullptr,
            "GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::FileNewEntry>::onTriggered",
            "FileManager must be specified before.");

        const bool hasNothingOpened = p_context->fileManager()->hasNothingOpened();
        // If we have nothing opened - open the new file.
        if (hasNothingOpened) {
            p_context->fileManager()->newFile();
            return;
        }

        const bool managerHasUnsavedData = p_context->fileManager()->hasUnsavedData();
        if (managerHasUnsavedData)     // If we have sth opened, BUT not saved
            _replaceWhenUnsaved();
        else                           // If we have sth opened, and saved
            _replaceWhenSaved();
    }

private:
    Context* p_context;
    void _replaceWhenUnsaved() {
        const int userSelection = Context::askUser(
            QObject::tr("Replace current file?"),
            QObject::tr("Current file was modified. Do you want to save your changes or discard them?"),
            QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel
        );

        if (userSelection == QMessageBox::Discard) {
            p_context->fileManager()->newFile();
        } else if (userSelection == QMessageBox::Save) {
            p_context->template triggerEntryHandler<Ui::FileSaveEntry>();
            p_context->fileManager()->newFile();
        } else {
            return;
        }
    }
    void _replaceWhenSaved() {
        const int userSelection = Context::askUser(
            QObject::tr("Replace current file?"),
            QObject::tr("This action will close current file and create a new one. Continue?"),
            QMessageBox::Yes | QMessageBox::Cancel
            );

        if (userSelection == QMessageBox::Yes)
            p_context->fileManager()->newFile();
        else
            return;
    }
};

}; // namespace Draupnir::Menus

#endif // FILENEWENTRYHANDLER_H
