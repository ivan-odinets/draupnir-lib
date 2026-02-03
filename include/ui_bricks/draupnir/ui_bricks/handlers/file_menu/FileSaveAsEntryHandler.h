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

#ifndef FILESAVEASENTRYHANDLER_H
#define FILESAVEASENTRYHANDLER_H

#include "draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h"

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h"
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context, class Entry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericMenuEntryHandler<FileContext, Draupnir::Ui::FileSaveAsEntry>
 *  @headerfile draupnir/handlers/file_menu/FileSaveAsEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Specialization of the menu entry handler for "Save As" actions.
 *
 *  @details This template specialization handles the "Save As" menu action, allowing users to save the current file under a
 *           new name. It inherits QAction connection logic from ActionHandler, and provides a concrete slot (`onTriggered()`)
 *           that invokes a file save dialog and triggers file saving logic through the file manager.
 *
 *           On triggering, the handler performs the following steps:
 *           - Asserts that the file manager is present in the context.
 *           - If no file is currently opened (`hasNothingOpened()`), the operation is aborted.
 *           - Prompts the user with a file save dialog (`getSaveFileName()`).
 *           - If a filename is selected, calls `saveCurrentFileAs(filePath)` on the file manager.
 *
 *  @tparam FileContext Context class providing file and dialog management interface.
 *
 * @note This handler expects FileContext to provide:
 *   - `fileManager()`: Returns a pointer to the FileManager.
 *   - `getSaveFileName()`: Opens a QFileDialog to get a filename from the user.
 *   - FileManager must implement `hasNothingOpened()` and `saveCurrentFileAs(const QString&)`. */

template<class FileContext>
class GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::FileSaveAsEntry> :
    public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::FileSaveAsEntry>>
{
public:
    using SettingsBundle = Settings::SettingsBundleTemplate<Settings::LastUsedDirectorySetting>;

    /*! @brief Constructs the handler with the given context reference.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandlerTemplate(FileContext& context) :
        m_context(context)
    {}

    /*! @brief Slot called when the "Save As" menu entry is triggered. Opens a dialog for file selection and saves the file
     *         with the selected name. */
    void onTriggered() {
        Q_ASSERT(m_context.fileManager());

        if (m_context.fileManager()->hasNothingOpened())
            return;

        const QString filePath = m_context.getSaveFileName();
        if (filePath.isEmpty())
            return;

        m_context.fileManager()->saveCurrentFileAs(filePath);
    }

private:
    FileContext& m_context;
};

};

#endif // FILESAVEASENTRYHANDLER_H
