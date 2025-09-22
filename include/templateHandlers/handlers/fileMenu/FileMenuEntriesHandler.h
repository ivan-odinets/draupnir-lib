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

#ifndef FILEMENUENTRIESHANDLER_H
#define FILEMENUENTRIESHANDLER_H

#include "../GenericMenuEntriesHandler.h"

#include <QFileDialog>
#include <QMessageBox>

#include "FileNewEntryHandler.h" // IWYU pragma: keep
#include "FileOpenEntryHandler.h" // IWYU pragma: keep
#include "FileSaveEntryHandler.h" // IWYU pragma: keep
#include "FileSaveAsEntryHandler.h" // IWYU pragma: keep
#include "FileCloseEntryHandler.h" // IWYU pragma: keep
#include "FileRecentEntryHandler.h" // IWYU pragma: keep
#include "FileExitEntryHandler.h" // IWYU pragma: keep

namespace Draupnir::Handlers {

/*! @class FileMenuEntriesHandler
 *  @headerfile draupnir-lib/include/templateHandlers/handlers/fileMenu/FileMenuEntriesHandler.h
 *  @brief Handler for File menu entries, managing their interaction with a specific FileManager.
 *  @details This template aggregates handlers for all specified file-related menu entries (such as "New", "Open", "Save",
 *           "Close", etc.) and delegates context and state management to FileEntriesHandlersContext. It inherits from
 *           GenericMenuEntriesHandler, instantiating one handler per HandledEntry type, and manages their lifecycle.
 *
 *           This class:
 *           - Is designed to be used as a ready-made handler for all file menu actions in the application.
 *           - Stores its context as a FileEntriesHandlersContext<FileManager>.
 *           - Provides a convenient setter to assign the FileManager instance at runtime.
 *           - All actual file menu logic and state is handled by the base context class and its handlers.
 *
 * @tparam FileManager The concrete file manager type used in this menu handler. Must provide the expected interface
 *         (see FileManagerValidator for requirements).
 * @tparam HandledEntries Variadic parameter pack of all file menu entry trait types that this handler should manage.
 *
 * @note Before using this handler, you **must** call setFileManager() to assign a valid FileManager instance.
 *
 * @see FileManagerValidator, FileEntriesHandlersContext, GenericMenuEntriesHandler
 * @todo Handling Recent file entries. */

template<class FileManagerClass, class... HandledEntries>
class FileMenuEntriesHandler :
        public GenericMenuEntriesHandler<FileMenuEntriesHandler<FileManagerClass,HandledEntries...>,HandledEntries...>
{
public:
    using Base = GenericMenuEntriesHandler<FileMenuEntriesHandler<FileManagerClass,HandledEntries...>,HandledEntries...>;

    using FileManager = FileManagerClass;

    FileMenuEntriesHandler() :
        GenericMenuEntriesHandler<FileMenuEntriesHandler<FileManager,HandledEntries...>,HandledEntries...>()
    {
        static_assert(FileManagerValidator::has_hasNothingOpened<FileManagerClass>::value,
                "Specified FileManager must provide bool hasNothingOpened() method.");
        static_assert(FileManagerValidator::has_isCurrentFileSaved<FileManagerClass>::value,
                "Specified FileManager must provide bool isCurrentFileSaved() method.");
        static_assert(FileManagerValidator::has_openDialogTitle<FileManagerClass>::value,
                "Specified FileManager must provide QString openDialogTitle() method.");
        static_assert(FileManagerValidator::has_supportedExtensions<FileManagerClass>::value,
                "Specified FileManager must provide QString supportedExtensions() method.");
        static_assert(FileManagerValidator::has_saveDialogTitle<FileManagerClass>::value,
                "Specified FileManager must provide QString saveDialogTitle() method.");

        this->registerSaveMethod([this](){
            Base::template getHandler<Draupnir::Menus::FileSaveEntry>().onTriggered();
        });
        this->registerSaveAsMethod([this](){
            Base::template getHandler<Draupnir::Menus::FileSaveAsEntry>().onTriggered();
        });

    }

    /*! @brief Assigns the FileManager instance to be used by this handler and its context. Delegates to the underlying context
     *         for actual storage.
     *  @param fileManager Pointer to the FileManager object. */
    void setFileManager(FileManager* fileManager) {
        Q_ASSERT_X(fileManager, "FileEntriesHandlersContext::setFileManager",
                   "Provided FileManager pointer is nullptr");

        p_fileManager = fileManager;
    }

    /*! @brief Returns a pointer to the current file manager. */
    FileManager* fileManager() { return p_fileManager; }

    /*! @brief Helper to show a QMessageBox with given title, text, and button set.
     *  @param title Title for the dialog.
     *  @param text Main message text.
     *  @param buttons Standard buttons to use.
     *  @return The button code selected by the user.
     *
     * @todo Make this method dependent on the list of traits. */
    static int askUser(const QString& title, const QString& text, QMessageBox::StandardButtons buttons) {
        QMessageBox messageBox{qApp->activeWindow()};
        messageBox.setWindowIcon(qApp->windowIcon());
        messageBox.setWindowTitle(title);
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setText(text);
        messageBox.setStandardButtons(buttons);
        return messageBox.exec();
    }

    /*! @enum ReplaceDecision
     *  @brief Decision outcome for replacing an opened file. */
    enum ReplaceDecision { Proceed, SaveThenProceed, Cancel };

    /*! @brief Confirms with the user if replacing the current file is allowed.
     *  @return Decision indicating whether to proceed, save, or cancel.
     * @brief Make this method dependent on the list of traits. */
    ReplaceDecision confirmReplaceIfNeeded() {
        if (p_fileManager->hasNothingOpened())
            return ReplaceDecision::Proceed;

        if (fileManager()->isCurrentFileSaved()) {
            const auto userSelection = askUser(
                QObject::tr("Replace current file?"),
                QObject::tr("This action will close current file and open another one. Continue?"),
                QMessageBox::Yes | QMessageBox::No
            );

            return (userSelection == QMessageBox::Yes) ?
                ReplaceDecision::Proceed : ReplaceDecision::Cancel;
        } else {
            const auto userSelection = askUser(
                QObject::tr("Replace current file?"),
                QObject::tr("This action will close current file and open another one. Continue?"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Save
            );

            switch (userSelection) {
            case QMessageBox::Yes:
                return ReplaceDecision::Proceed;
            case QMessageBox::Save:
                return ReplaceDecision::SaveThenProceed;
            case QMessageBox::Cancel:
                return ReplaceDecision::Cancel;
            }
            Q_UNREACHABLE();
            return ReplaceDecision::Cancel;
        }
    }

    /*! @brief Shows a QFileDialog for opening a single file.
     *  @return Selected file path, or empty if cancelled.
     * @todo Make this method dependent on traits. */
    QString getOpenFileName() {
        return QFileDialog::getOpenFileName(
            nullptr,
            fileManager()->openDialogTitle(),
            this->template getSetting<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for opening multiple files.
     *  @return List of selected file paths, or empty if cancelled.
     * @todo Make this method dependent on traits. */
    QStringList getOpenFileNames() {
        return QFileDialog::getOpenFileNames(
            nullptr,
            fileManager()->openDialogTitle(),
            this->template getSetting<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for saving a file.
     *  @return Selected file path, or empty if cancelled.
     * @todo Make this method dependent on traits. */
    QString getSaveFileName() {
        return QFileDialog::getSaveFileName(
            nullptr,
            fileManager()->saveDialogTitle(),
            this->template getSetting<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Registers a callback to be called when "Save" is triggered.
     *  @param method The function to be called.
     * @todo Make this method dependent on traits. */
    void registerSaveMethod(std::function<void()> method) {
        Q_ASSERT_X(!m_saveMethod,"FileEntriesHandlersContext::registerSaveMethod",
                   "This method should be called only once.");
        m_saveMethod = method;
    }

    /*! @brief Registers a callback to be called when "Save As" is triggered.
     *  @param method The function to be called.
     * @todo Make this method dependent on traits. */
    void registerSaveAsMethod(std::function<void()> method) {
        Q_ASSERT_X(!m_saveAsMethod, "FileEntriesHandlersContext::registerSaveAsMethod",
                   "This method should be called only once.");
        m_saveAsMethod = method;
    }

    /*! @brief Calls the registered "Save" method.
     * @todo Make this method dependent on traits. */
    void onSaveFile() {
        Q_ASSERT_X(m_saveMethod,"FileEntriesHandlersContext::onSaveFile",
                   "FileEntriesHandlersContext::registerSaveMethod must have been called before.");
        m_saveMethod();
    };

    /*! @brief Calls the registered "Save As" method.
     * @todo Make this method dependent on traits. */
    void onSaveFileAs() {
        Q_ASSERT_X(m_saveAsMethod,"FileEntriesHandlersContext::onSaveFileAs",
                   "FileEntriesHandlersContext::registerSaveAsMethod must have been called before.");
        m_saveAsMethod();
    }

private:
    FileManager* p_fileManager;

    std::function<void()> m_saveMethod;
    std::function<void()> m_saveAsMethod;
};

} // namespace Draupnir::Menus

#endif // FILEMENUENTRIESHANDLER_H
