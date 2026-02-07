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

#ifndef FILERECENTENTRYHANDLER_H
#define FILERECENTENTRYHANDLER_H

#include <QMessageBox>

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h"
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers {

template<class Context, class Entry>
class GenericMenuEntryHandlerTemplate;

/*! @class GenericMenuEntryHandler<FileContext, Draupnir::Ui::RecentFileEntry>
 *  @headerfile draupnir/handlers/file_menu/FileRecentEntryHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Specialization of the menu entry handler for "Open Recent File" actions.
 *
 *  @details This template specialization handles selection of recent files from a RecentFilesMenu. It inherits connection
 *           logic from CustomMenuHandler and provides slots for integrating with a RecentFilesMenu UI. Upon construction,
 *           the handler statically asserts that the FileManager provided by FileContext implements the required method
 *           `openFile(const QFileInfo&)`.
 *
 *           The core workflow:
 *           - The `connect()` method connects the handler to the `RecentFilesMenu`, binding the menu's `recentFileSelected` signal to a slot.
 *           - When a recent file is selected (`onRecentFileSelected`), the handler:
 *               - If the application supports only a single opened file at a time, prompts the user to confirm replacement if there is an unsaved or open file.
 *               - If there are unsaved changes, offers the user a choice to save, discard, or cancel.
 *               - If the user cancels, the operation is aborted.
 *               - If the user chooses to save, the current file is saved before opening the recent file.
 *               - The handler then calls `openFile(filePath)` on the file manager.
 *  @tparam FileContext Context class providing file management and dialog interface.
 *
 * @note This handler expects FileContext to provide:
 *       - `fileManager()`: Pointer to the FileManager.
 *       - `onSaveFile()`: Triggers a save operation for the current file.
 *       - `askUser(...)`: Presents a confirmation dialog to the user.
 *       The FileManager must implement:
 *       - `openFile(const QFileInfo&)`
 *       - `hasNothingOpened()`
 *       - `isCurrentFileSaved()`
 *       - Static method `canHaveMultipleFilesOpened()`. */

template<class FileContext>
class GenericMenuEntryHandlerTemplate<FileContext,Draupnir::Ui::RecentFileEntry>
{
public:
    using SettingsBundle = Settings::SettingsBundleTemplate<Settings::RecentFileListSetting>;

    /*! @brief Constructs the handler, asserting FileManager compliance.
     *  @param context Reference to the file context. */
    GenericMenuEntryHandlerTemplate(FileContext& context) :
        m_context(context)
    {}

    /*! @brief Connects this handler to the RecentFilesMenu's selection signal.
     *  @param entry Pointer to the RecentFilesMenu instance. */
    void connect(Draupnir::Ui::RecentFilesMenu* entry) {
        p_menu = entry;

        QObject::connect(entry, &Draupnir::Ui::RecentFilesMenu::recentFileSelected, [this](const QFileInfo& fileInfo){
            onRecentFileSelected(fileInfo);
        });

        QObject::connect(entry, &Draupnir::Ui::RecentFilesMenu::recentFilesMenuCleared, [this](){
            onRecentFilesCleared();
        });
    }

    /*! @brief Slot called when a recent file is selected from the menu. Handles user confirmation and opens the selected file,
     *         respecting unsaved changes. */
    void onRecentFileSelected(const QFileInfo& fileInfo) {
        if constexpr (!FileContext::FileManager::canHaveMultipleFilesOpened()) {
            // If we have sth opened, and saved
            if ( (!m_context.fileManager()->hasNothingOpened()) && m_context.fileManager()->isCurrentFileSaved()) {
                const int userSelection = FileContext::askUser(
                    QObject::tr("Replace current file?"),
                    QObject::tr("This action will close current file and open another file. Continue?"),
                    QMessageBox::Yes|QMessageBox::Cancel
                );

                if (userSelection == QMessageBox::Cancel)
                    return;
            }

            // If we have sth opened, BUT not saved
            if ( (!m_context.fileManager()->hasNothingOpened()) && (!m_context.fileManager()->isCurrentFileSaved()) ) {
                const int userSelection = FileContext::askUser(
                    QObject::tr("Replace current file?"),
                    QObject::tr("Current file was modified. Do you want to save your changes or discard them?"),
                    QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel
                );

                if (userSelection == QMessageBox::Cancel) {
                    return;
                } else if (userSelection == QMessageBox::Save) {
                    m_context.onSaveFile();
                }
            }
        }

        m_context.fileManager()->openFile(fileInfo.filePath());
    };

    void onRecentFilesCleared() {
        m_context.template setSetting<Draupnir::Settings::RecentFileListSetting>(QStringList{});
    }

    void onSettingsLoaded() {
        p_menu->loadRecentFiles(
            m_context.template getSetting<Draupnir::Settings::RecentFileListSetting>()
        );
    }

private:
    Ui::RecentFilesMenu* p_menu;
    FileContext& m_context;
};

};

#endif // FILERECENTENTRYHANDLER_H
