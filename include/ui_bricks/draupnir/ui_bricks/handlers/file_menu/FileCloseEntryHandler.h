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

#ifndef FILECLOSEENTRYHANDLER_H
#define FILECLOSEENTRYHANDLER_H

#include "draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h"

#include <QApplication>
#include <QMessageBox>

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context, class Entry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::FileCloseEntry>
 *  @headerfile draupnir/ui_bricks/handlers/file_menu/FileCloseEntryHandler.h
 *  @ingroup UiBricks
 *  @brief Menu-entry handler specialization for the **File → Close** action.
 *  @tparam Context Context type used by this handler.
 *
 *  @details This handler implements the UX flow for closing the current file:
 *           - If no file is opened (`FileManager::hasNothingOpened()`), it does nothing.
 *           - If the current file has unsaved changes (`FileManager::isCurrentFileSaved() == false`), it prompts the user via
 *             `Context::askUser(...)` with **Save / Discard / Cancel**.
 *              - **Save** → calls `FileContext::onSaveFile()` and continues closing.
 *              - **Discard** → continues closing without saving.
 *              - **Cancel** → aborts the operation.
 *              - Finally calls `FileManager::closeCurrentFile()`.
 *
 *           The class performs **compile-time contract validation** via `static_assert` checks to ensure that `Context` and
 *           its nested `FileManager` satisfy the required interface.
 *
 *           `Context` must provide:
 *           - `using FileManager = ...;`
 *           - `FileManager* fileManager();`
 *           - `static int askUser(QString title, QString message, QMessageBox::StandardButtons buttons);`
 *           - `void onSaveFile();`
 *
 *           The nested `Context::FileManager` must provide:
 *           - `bool hasNothingOpened();`
 *           - `bool isCurrentFileSaved();`
 *           - `void closeCurrentFile();`
 *
 *           Runtime requirement: `fileManager()` must return a non-null pointer when the handler is used. This is enforced with
 *           `Q_ASSERT_X`. */

template<class Context>
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileCloseEntry> :
    public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileCloseEntry>>
{
public:
    /*! @brief Constructs the handler.
     *  @param context Reference to the context. */
    GenericMenuEntryHandlerTemplate(Context& context) :
        m_context{context}
    {};

    /*! @brief Slot called when the "Close File" menu entry is triggered. Prompts the user to save unsaved changes before
     *         closing the file. If the file is already saved, it is closed directly. Otherwise, the user is presented
     *         with options to Save, Discard, or Cancel. */
    void onTriggered() {
        Q_ASSERT_X(m_context.fileManager() != nullptr,
            "GenericMenuEntryHandlerTemplate<Context,FileCloseEntry>::onTriggered",
            "FileManager must be specified before by using FileMenuEntriesHandler::setFileManager method");

        if (m_context.fileManager()->hasNothingOpened())
            return;

        if (!m_context.fileManager()->isCurrentFileSaved()) {
            const int userSelection = Context::askUser(
                QObject::tr("Close current file?"),
                QObject::tr("Current file was modified. Do you want to save your changes or discard them?"),
                QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel
            );

            if (userSelection == QMessageBox::Save) {
                m_context.onSaveFile();
            } else if (userSelection == QMessageBox::Cancel) {
                return;
            }
            // Discard -> continue
        }

        m_context.fileManager()->closeCurrentFile();
    }

private:
    Context& m_context;
};

};

#endif // FILECLOSEENTRYHANDLER_H
