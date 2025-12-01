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

#ifndef FILEOPENENTRYHANDLER_H
#define FILEOPENENTRYHANDLER_H

#include "draupnir/handlers/templates/ActionHandler.h"

#include <QDir>
#include <QMessageBox>

#include "draupnir/SettingsBundleTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/traits/settings/files/LastUsedDirectorySetting.h"
#include "draupnir/utils/FileManagerValidator.h"

namespace Draupnir::Handlers {

template<class Context, class Entry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileOpenEntry>
 *  @headerfile draupnir/handlers/file_menu/FileOpenEntryHandler.h
 *  @brief Specialization for handling the "File → Open" menu entry.
 *  @ingroup HandlerTemplates
 *
 *  @details Implements flexible logic for opening one або several files, depending on capabilities of the provided FileManager
 *           (multi-file or single-file mode, batch open or not). Handles confirmation prompts if current file(s) may be replaced.
 *
 *           The behaviour is the following:
 *           - If FileManager allows multiple files and batch open (canHaveMultipleFilesOpened(), canOpenMultipleFilesAtOnce()):
 *             - Prompts user to select several files, updates last used directory, calls FileManager::openFiles(selectedFiles).
 *           - If only multiple files (but not batch), prompts for one file at a time, calls FileManager::openFile(filePath).
 *           - If only one file allowed (single-file mode):
 *             - If a file is open and saved: asks user for confirmation before replacement.
 *             - If a file is open and NOT saved: asks user whether to save, discard, or cancel; acts accordingly.
 *             - If nothing is open or after confirmation: opens selected file.
 *           - All user prompts go via FileContext::askUser.
 *
 *  @tparam FileContext The context class which provides access to the file manager and user interaction helpers.
 *
 * @note FileManager is expected to provide:
 *       - static constexpr bool canHaveMultipleFilesOpened();
 *       - static constexpr bool canOpenMultipleFilesAtOnce();
 *       - void openFile(const QFileInfo&);
 *       - void openFiles(const QStringList&);
 *       - bool hasNothingOpened();
 *       - bool isCurrentFileSaved();
 *
 * @todo Write a test for this class. */

template<class FileContext>
class GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileOpenEntry> :
        public ActionHandler<GenericMenuEntryHandler<FileContext,Draupnir::Menus::FileOpenEntry>>
{
public:
    using SettingsBundle = Settings::SettingsBundleTemplate<Settings::LastUsedDirectorySetting>;

    /*! @brief Constructs the handler, checks file manager interface compliance. Performs a static_assert to ensure FileManager
     *         has openFile method.
     *  @param context Reference to file context. */
    GenericMenuEntryHandler(FileContext& context) :
        m_context(context)
    {
        if constexpr (FileContext::FileManager::canOpenMultipleFilesAtOnce()) {
            static_assert(FileManagerValidator::has_openFiles<typename FileContext::FileManager>::value,
                    "FileManager should have void openFiles(const QStringList& ) method");
        } else {
            static_assert(FileManagerValidator::has_openFile<typename FileContext::FileManager>::value,
                    "FileManager should have void openFile(const QFileInfo& ) method");
        }
    };

    /*! @brief Slot called when the "File → Open" action is triggered. Handles all required prompting and opens selected files
     *         accordingly. */
    void onTriggered() {
        if constexpr (FileContext::FileManager::canHaveMultipleFilesOpened()) {
            if constexpr (FileContext::FileManager::canOpenMultipleFilesAtOnce()) {
                const QStringList selectedFiles = m_context.getOpenFileNames();

                if (selectedFiles.isEmpty())
                    return;

                m_context.updateLastUsedDirectory(QFileInfo{selectedFiles.last()}.dir().path());
                m_context.fileManager()->openFiles(selectedFiles);
            } else {
                const QString filePath = m_context.getOpenFileName();

                if (filePath.isEmpty())
                    return;

                const QFileInfo fileInfo(filePath);
                m_context.updateLastUsedDirectory(fileInfo.dir().path());
                m_context.fileManager()->openFile(fileInfo);
            }
        } else {
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

            // Finally if we are here - we can open something...
            const QString filePath = m_context.getOpenFileName();

            if (filePath.isEmpty())
                return;

            const QFileInfo fileInfo(filePath);
            m_context.template setSetting<Draupnir::Settings::LastUsedDirectorySetting>(fileInfo.dir().path());
            m_context.fileManager()->openFile(fileInfo);
        }
    }

private:
    FileContext& m_context;
};

} // namespace Draupnir::Menus

#endif // FILEOPENENTRYHANDLER_H
