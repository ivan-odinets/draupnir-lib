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

#ifndef FILESAVEENTRYHANDLER_H
#define FILESAVEENTRYHANDLER_H

#include "draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h"

#include "draupnir/ui_bricks/concepts/FileContextConcept.h"
#include "draupnir/ui_bricks/concepts/FileManagerConcept.h"
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers
{

template<class Context, class Entry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericMenuEntryHandler<FileContext,Draupnir::Ui::FileSaveEntry>
 *  @headerfile draupnir/handlers/file_menu/FileSaveEntryHandler.h
 *  @ingroup UiBricks
 *  @tparam FileContext Context class providing file management interface.
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
 *           required `saveCurrentFile()` method. */

template<class Context>
class GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileSaveEntry> :
    public ActionHandlerTemplate<GenericMenuEntryHandlerTemplate<Context,Draupnir::Ui::FileSaveEntry>>
{
public:
    /*! @brief Constructs the handler, statically asserting `FileManager` interface compliance.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandlerTemplate(Context& context) :
        m_context{context}
    {};

    /*! @brief Slot called when the "Save File" menu entry is triggered. If the current file has a name, saves directly;
     *         otherwise, initiates "Save As". */
    void onTriggered() {
        Q_ASSERT(m_context.fileManager());

        if (m_context.fileManager()->currentFileHasName()) {
            m_context.fileManager()->saveCurrentFile();
        } else {
            m_context.onSaveFileAs();
        }
    }

private:
    Context& m_context;
};

}; // namespace Draupnir::Menus

#endif // FILESAVEENTRYHANDLER_H
