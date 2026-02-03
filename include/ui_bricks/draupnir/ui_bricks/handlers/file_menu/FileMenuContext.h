/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef FILEMENUCONTEXT_H
#define FILEMENUCONTEXT_H

#include <functional>

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include "draupnir/settings_registry/utils/OptionalSettingsBundle.h"
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h"
#include "draupnir/ui_bricks/handlers/templates/GenericMenuEntryHandlerTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"

namespace Draupnir::Handlers
{

/*! @class GenericFileMenuContext draupnir/ui_bricks/handlers/file_menu/GenericFileMenuContext.h
 *  @brief This is a generic context template to be used within FileMenuEntriesHandler.
 * @todo Validate FileManager through concepts. */

template<class FileManagerClass, class... Entries>
class GenericFileMenuContext :
    public Settings::NonEmptySettingsBundle<
        typename Settings::SettingsTraitsConcatenator<GenericMenuEntryHandlerTemplate<class Empty,Entries>...>::toSettingsBundle
    >
{
private:
    // Lets find out which things we will need.
    static inline constexpr bool openingHandlersPresent =
        draupnir::utils::is_one_of_v<Ui::FileOpenEntry,Entries...> ||
        draupnir::utils::is_one_of_v<Ui::RecentFileEntry,Entries...>;

    static inline constexpr bool saveHandlerPresent =
        draupnir::utils::is_one_of_v<Ui::FileSaveEntry,Entries...>;

    static inline constexpr bool saveAsHandlerPresent =
        draupnir::utils::is_one_of_v<Ui::FileSaveAsEntry,Entries...>;

    static inline constexpr bool savingHandlersPresent =
        saveHandlerPresent || saveAsHandlerPresent;

public:
    using FileManager = FileManagerClass;

    GenericFileMenuContext() :
        p_fileManager{nullptr}
    {}

    /*! @brief Assigns the FileManager instance to be used by this context.
     *  @param fileManager Pointer to the FileManager object. */
    void setFileManager(FileManager* fileManager) {
        Q_ASSERT(fileManager);
        Q_ASSERT(p_fileManager != nullptr);

        p_fileManager = fileManager;
    }

    /*! @brief Returns a pointer to the current file manager. */
    FileManager* fileManager() { return p_fileManager; }

    /*! @brief Shows a QFileDialog for opening a single file.
     *  @return Selected file path, or empty if cancelled. */
    QString getOpenFileName() requires (openingHandlersPresent) {
        return QFileDialog::getOpenFileName(
            nullptr,
            fileManager()->openDialogTitle(),
            this->template get<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for opening multiple files.
     *  @return List of selected file paths, or empty if cancelled. */
    QStringList getOpenFileNames() requires (openingHandlersPresent) {
        return QFileDialog::getOpenFileNames(
            nullptr,
            fileManager()->openDialogTitle(),
            this->template getSetting<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for saving a file.
     *  @return Selected file path, or empty if cancelled. */
    QString getSaveFileName() requires (savingHandlersPresent) {
        return QFileDialog::getSaveFileName(
            nullptr,
            fileManager()->saveDialogTitle(),
            this->template getSetting<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Registers a callback to be called when "Save" is triggered.
     *  @param method The function to be called. */
    void registerSaveMethod(std::function<void()> method) requires (saveHandlerPresent) {
        Q_ASSERT_X(!m_saveMethod,"FileEntriesHandlersContext::registerSaveMethod",
                   "This method should be called only once.");
        m_saveMethod = method;
    }

    /*! @brief Calls the registered "Save" method. */
    void onSaveFile() requires (saveHandlerPresent) {
        Q_ASSERT_X(m_saveMethod,"FileEntriesHandlersContext::onSaveFile",
                   "FileEntriesHandlersContext::registerSaveMethod must have been called before.");
        m_saveMethod();
    };

    /*! @brief Registers a callback to be called when "Save As" is triggered.
     *  @param method The function to be called. */
    void registerSaveAsMethod(std::function<void()> method) requires (saveAsHandlerPresent) {
        Q_ASSERT_X(!m_saveAsMethod, "FileEntriesHandlersContext::registerSaveAsMethod",
                   "This method should be called only once.");
        m_saveAsMethod = method;
    }

    /*! @brief Calls the registered "Save As" method. */
    void onSaveFileAs() requires (saveAsHandlerPresent) {
        Q_ASSERT_X(m_saveAsMethod,"FileEntriesHandlersContext::onSaveFileAs",
                   "FileEntriesHandlersContext::registerSaveAsMethod must have been called before.");
        m_saveAsMethod();
    }

    /*! @brief Helper to show a QMessageBox with given title, text, and button set.
     *  @param title Title for the dialog.
     *  @param text Main message text.
     *  @param buttons Standard buttons to use.
     *  @return The button code selected by the user.
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
     * @todo Make this method dependent on the list of traits. */
    ReplaceDecision confirmReplaceIfNeeded() {
        if (p_fileManager->hasNothcingOpened())
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

private:
    FileManager* p_fileManager;

    std::function<void()> m_saveMethod;
    std::function<void()> m_saveAsMethod;
};

}; // namespace Draupnir::Handlers

#endif // FILEMENUCONTEXT_H
