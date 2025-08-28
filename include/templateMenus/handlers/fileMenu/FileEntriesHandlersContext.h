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

#ifndef FILEENTRIESHANDLERSCONTEXT_H
#define FILEENTRIESHANDLERSCONTEXT_H

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QString>

#include "AppSettings.h"
#include "FileManagerValidator.h"

namespace Draupnir::Menus {

/*! @class FileEntriesHandlersContext draupnir-lib/include/templateMenus/handlers/fileMenu/FileEntriesHandlersContext.h
 *  @brief Context and utility class for managing file menu actions and file manager integration.
 *  @details This class serves as a glue layer between the application's file manager, persistent settings, and file-related
 *           menu entry handlers. It provides helpers for showing dialogs, tracking and updating the last used directory,
 *           confirming destructive actions, and delegating save/save as callbacks.
 *
 *           This template also:
 *           - Holds pointers to AppSettings and the current FileManager.
 *           - Manages the last used directory and persists it in AppSettings.
 *           - Provides helpers for displaying standard open/save dialogs and user prompts.
 *           - Exposes methods to register save/save as callbacks (used by menu handlers).
 *           - Offers an internal state machine for confirming file replacements, which distinguishes between unsaved/saved
 *             states and provides user-friendly dialog flows.
 *
 *  @tparam FileManagerClass - Type of the file manager being integrated. Must provide methods for file state checks, dialog
 *          title/filters, and file actions.
 * @note You must call setFileManager() before using file operations.
 * @note You must call loadSettings() exactly once before first file operation.
 * @note Designed for use as a context in menu entry handler templates.
 *
 * @see AppSettings */

template<class FileManagerClass>
class FileEntriesHandlersContext
{
public:
    using FileManager = FileManagerClass;

    /*! @brief Default constructor. Initializes context state. */
    FileEntriesHandlersContext() :
        p_settings{nullptr},
        p_fileManager{nullptr},
        m_lastUsedDirectory{QDir::homePath()}
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
    }

    /*! @brief Loads application settings and initializes last used directory.
     *  @param settings Pointer to the application settings object. */
    void loadSettings(AppSettings* settings) {
        Q_ASSERT_X(settings, "FileEntriesHandlersContext::loadSettings",
                   "Provided AppSettings pointer is nullptr");
        Q_ASSERT_X(p_settings == nullptr, "FileEntriesHandlersContext::loadSettings",
                   "This method must be called only once");

        p_settings = settings;

        m_lastUsedDirectory = p_settings->value(
            AppSettings::Files, lastUsedDirectory_settingsKey,
            QDir::homePath()
        ).toString();
    }

    /*! @brief Sets the file manager for this context.
     *  @param fileManager Pointer to the file manager instance. */
    void setFileManager(FileManagerClass* fileManager) {
        Q_ASSERT_X(fileManager, "FileEntriesHandlersContext::setFileManager",
                   "Provided FileManager pointer is nullptr");

        p_fileManager = fileManager;
    }

    /*! @brief Returns a pointer to the current file manager. */
    FileManagerClass* fileManager() { return p_fileManager; }

    /*! @brief Updates the last used directory and persists it to AppSettings.
     *  @param newDirectory Path to be set as the last used directory. */
    void updateLastUsedDirectory(const QString& newDirectory) {
        Q_ASSERT_X(p_settings,"FileEntriesHandlersContext::updateLastUsedDirectory",
                   "FileEntriesHandlersContext::loadSettings method must have been called before. This should be done through "
                   "the FileMenuEntriesHandler class.");

        m_lastUsedDirectory = newDirectory;

        p_settings->setValue(
            AppSettings::Files, lastUsedDirectory_settingsKey,
            m_lastUsedDirectory
        );
    }

    /*! @brief Helper to show a QMessageBox with given title, text, and button set.
     *  @param title Title for the dialog.
     *  @param text Main message text.
     *  @param buttons Standard buttons to use.
     *  @return The button code selected by the user. */
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
     *  @return Decision indicating whether to proceed, save, or cancel. */
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
     *  @return Selected file path, or empty if cancelled. */
    QString getOpenFileName() {
        return QFileDialog::getOpenFileName(
            nullptr,
            fileManager()->openDialogTitle(),
            m_lastUsedDirectory,
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for opening multiple files.
     *  @return List of selected file paths, or empty if cancelled. */
    QStringList getOpenFileNames() {
        return QFileDialog::getOpenFileNames(
            nullptr,
            fileManager()->openDialogTitle(),
            m_lastUsedDirectory,
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for saving a file.
     *  @return Selected file path, or empty if cancelled. */
    QString getSaveFileName() {
        return QFileDialog::getSaveFileName(
            nullptr,
            fileManager()->saveDialogTitle(),
            m_lastUsedDirectory,
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Registers a callback to be called when "Save" is triggered.
     *  @param method The function to be called. */
    void registerSaveMethod(std::function<void()> method) {
        Q_ASSERT_X(!m_saveMethod,"FileEntriesHandlersContext::registerSaveMethod",
                   "This method should be called only once.");
        m_saveMethod = method;
    }

    /*! @brief Registers a callback to be called when "Save As" is triggered.
     *  @param method The function to be called. */
    void registerSaveAsMethod(std::function<void()> method) {
        Q_ASSERT_X(!m_saveAsMethod, "FileEntriesHandlersContext::registerSaveAsMethod",
                   "This method should be called only once.");
        m_saveAsMethod = method;
    }

    /*! @brief Calls the registered "Save" method. */
    void onSaveFile() {
        Q_ASSERT_X(m_saveMethod,"FileEntriesHandlersContext::onSaveFile",
                   "FileEntriesHandlersContext::registerSaveMethod must have been called before.");
        m_saveMethod();
    };

    /*! @brief Calls the registered "Save As" method. */
    void onSaveFileAs() {
        Q_ASSERT_X(m_saveAsMethod,"FileEntriesHandlersContext::onSaveFileAs",
                   "FileEntriesHandlersContext::registerSaveAsMethod must have been called before.");
        m_saveAsMethod();
    }

private:
    Q_DISABLE_COPY_MOVE(FileEntriesHandlersContext);

    inline static const QLatin1String lastUsedDirectory_settingsKey{"lastUsedDirectory"};

    AppSettings*        p_settings;
    FileManagerClass*   p_fileManager;
    QString             m_lastUsedDirectory;

    std::function<void()> m_saveMethod;
    std::function<void()> m_saveAsMethod;
};

}; // namespace Draupnir::Menus

#endif // FILEENTRIESHANDLERSCONTEXT_H
