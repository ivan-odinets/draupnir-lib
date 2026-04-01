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

#include "draupnir/ui_bricks/handlers/templates/MenuEntryHandlerContainer.h"

#include <QFileDialog>
#include <QMessageBox>

#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h"     // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileOpenEntryHandler.h"    // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileSaveEntryHandler.h"    // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileSaveAsEntryHandler.h"  // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileCloseEntryHandler.h"   // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileRecentEntryHandler.h"  // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/file_menu/FileExitEntryHandler.h"    // IWYU pragma: keep

template<class Candidate>
concept HasSaveDialogTitle = requires {
    { Candidate::saveDialogTitle() } -> std::convertible_to<QString>;
};

template<class Candidate>
concept HasOpenDialogTitle = requires {
    { Candidate::openDialogTitle() } -> std::convertible_to<QString>;
};

template<class Candidate>
concept HasSupportedExtensions = requires {
    { Candidate::supportedExtensions() } -> std::convertible_to<QString>;
};

namespace Draupnir::Handlers {

/*! @class FileMenuEntriesHandler
 *  @headerfile draupnir/ui_bricks/handlers/file_menu/FileMenuEntriesHandler.h
 *  @ingroup UiBricks
 *  @brief Handler for File menu entries, managing their interaction with a specific `FileManager`.
 *  @tparam FileManager The concrete file manager type used in this menu handler. Must provide the expected interface.
 *  @tparam HandledEntries... Variadic parameter pack of all file menu entry trait types that this handler should manage.
 *
 *  @details This template aggregates handlers for all specified file-related menu entries (such as "New", "Open", "Save",
 *           "Close", etc.).
 *
 * @note Before using this handler, you **must** call setFileManager() to assign a valid FileManager instance. */

template<class FileManagerClass, class UiHelper, class... Entries>
class FileMenuHandler
{
    using _Container = MenuEntryHandlerContainer<
        FileMenuHandler<FileManagerClass, UiHelper, Entries...>,
        GenericMenuEntryHandlerTemplate,
        Entries...
    >;

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
    using SettingsBundle = Settings::SettingsTraitsConcatenator<
        GenericMenuEntryHandlerTemplate<class Empty,Entries>...
    >::toSettingsBundle;
    using FileManager = FileManagerClass;

    FileMenuHandler() :
        p_fileManager{nullptr},
        m_container{}
    {
        m_container.initialize(this);
    }

    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom_v<SettingsSource>);
        m_settings = source->template getSettingsBundle<SettingsBundle>();

        m_container.onSettingsLoaded();
    }

    /*! @brief Retrieves the value of a specific setting trait.
     *  @tparam SettingTrait A setting trait that must be contained in `SettingsBundle`.
     *  @return A const reference to the value associated with `SettingTrait`.
     *  @details This function performs a compile-time check that `Bundle` contains the specified `SettingTrait`. If the trait
     *           is not part of the bundle, compilation fails with a `static_assert`. */
    template<class SettingTrait>
    const typename SettingTrait::Value& get() const {
        static_assert(SettingsBundle::template contains<SettingTrait>());
        return m_settings.template get<SettingTrait>();
    }

    /*! @brief Updates the value of a specific setting trait in the bundle.
     *  @tparam SettingTrait A setting trait that must be contained in `Bundle`.
     *  @param value New value to assign to the trait.
     *  @details As with @ref get, this function enforces at compile time that `SettingTrait is a member of `Bundle`. The new
     *           value is then forwarded to the underlying bundle's `set` member. */
    template<class SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        static_assert(SettingsBundle::template contains<SettingTrait>());
        m_settings.template set<SettingTrait>(value);
    }

    template<class MenuContainer>
    void connectActions(MenuContainer* container) {
        m_container.template connectActions<MenuContainer>(container);
    }

    /*! @brief Assigns the FileManager instance to be used by this handler.
     *  @param fileManager Pointer to the FileManager object. */
    void setFileManager(FileManager* fileManager) {
        Q_ASSERT(fileManager);
        Q_ASSERT(p_fileManager == nullptr);

        p_fileManager = fileManager;
    }

    /*! @brief Returns a pointer to the current file manager. */
    FileManager* fileManager() { return p_fileManager; }

    static QString openDialogTitle() requires (openingHandlersPresent) {
        if constexpr (HasOpenDialogTitle<UiHelper>)
            return UiHelper::openDialogTitle();
        else
            return QObject::tr("Open.");
    }

    static QString saveDialogTitle() {
        if constexpr (HasSaveDialogTitle<UiHelper>)
            return UiHelper::saveDialogTitle();
        else
            return QObject::tr("Save.");
    }

    static QString supportedExtensions() {
        if constexpr (HasSupportedExtensions<UiHelper>)
            return UiHelper::openDialogTitle();
        else
            return QObject::tr("Extensions.");
    }

    /*! @brief Shows a QFileDialog for opening a single file.
     *  @return Selected file path, or empty if cancelled. */
    QString getOpenFileName() requires (openingHandlersPresent) {
        return QFileDialog::getOpenFileName(
            nullptr,
            openDialogTitle(),
            this->template get<Draupnir::Settings::LastUsedDirectorySetting>(),
            supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for opening multiple files.
     *  @return List of selected file paths, or empty if cancelled. */
    QStringList getOpenFileNames() requires (openingHandlersPresent) {
        return QFileDialog::getOpenFileNames(
            nullptr,
            fileManager()->openDialogTitle(),
            this->template get<Draupnir::Settings::LastUsedDirectorySetting>(),
            fileManager()->supportedExtensions()
        );
    }

    /*! @brief Shows a QFileDialog for saving a file.
     *  @return Selected file path, or empty if cancelled. */
    QString getSaveFileName() requires (savingHandlersPresent) {
        return QFileDialog::getSaveFileName(
            nullptr,
            saveDialogTitle(),
            this->template get<Draupnir::Settings::LastUsedDirectorySetting>(),
            supportedExtensions()
        );
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

    template<class Entry>
    void triggerEntryHandler() {
        m_container.template getHandlerForEntry<Entry>().onTriggered();
    }

    /*! @enum ReplaceDecision
     *  @brief Decision outcome for replacing an opened file. */
    enum ReplaceDecision { Proceed, SaveThenProceed, Cancel };

    /*! @brief Confirms with the user if replacing the current file is allowed.
     *  @return Decision indicating whether to proceed, save, or cancel.
     * @todo Make this method dependent on the list of traits. */
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

private:
    FileManager* p_fileManager;
    SettingsBundle m_settings;
    _Container m_container;
};


} // namespace Draupnir::Menus

#endif // FILEMENUENTRIESHANDLER_H
