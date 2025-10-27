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

#ifndef RECENTFILESMENU_H
#define RECENTFILESMENU_H

#include <QMenu>

#include <QFileInfo>

class QActionGroup;

namespace Draupnir::Menus
{

/*! @class RecentFilesMenu draupnir/ui/menus/RecentFilesMenu.h
 *  @ingroup MenuTemplates
 *  @brief A dynamic menu for displaying, managing, and interacting with a list of recently opened files.
 *
 *  @details This class provides a convenient interface for creating a menu with entries for recently used files. It supports
 *           adding, updating, removing, and clearing recent files; selecting files via QAction; and emits signals when a file
 *           is selected or the menu is cleared.
 *
 *           Usage:
 *           - Use addRecentAction() to append a file.
 *           - Use loadRecentFiles() to bulk-load or refresh the list.
 *           - Connect to recentFileSelected() to handle user selection.
 *           - Call reset() or trigger the "Clear" action to clear the menu. */

class RecentFilesMenu final : public QMenu
{
    Q_OBJECT
public:
    /*! @brief Constructs an empty RecentFilesMenu with an optional parent widget.
     *  @param parent Optional parent widget. */
    explicit RecentFilesMenu(QWidget* parent = nullptr);

    /*! @brief Constructs an empty RecentFilesMenu with the given title and optional parent widget.
     *  @param title Menu title (displayed as menu name).
     *  @param parent Optional parent widget. */
    explicit RecentFilesMenu(const QString& title, QWidget* parent = nullptr);

    /*! @brief Destructor. Cleans up all managed actions. */
    ~RecentFilesMenu() final = default;

    /*! @brief Checks if the menu contains an action for the given file.
     *  @param fileInfo QFileInfo describing the file.
     *  @return true if a QAction for this file exists in the menu; false otherwise. */
    bool hasFileAction(const QFileInfo& fileInfo) { return getFileAction(fileInfo) != nullptr; }

    /*! @brief Returns the QAction representing the given file, or nullptr if not found.
     *  @param fileInfo QFileInfo describing the file.
     *  @return Pointer to QAction, or nullptr if not found. */
    QAction* getFileAction(const QFileInfo& fileInfo);

    /*! @brief Populates the menu with recent files given as a list of file paths. Clears previous entries.
     *  @param filePathsList List of file paths. */
    void loadRecentFiles(const QStringList& filePathsList);

    /*! @brief Populates the menu with recent files given as a list of QFileInfo. Clears previous entries.
     *  @param fileInfoList List of QFileInfo objects. */
    void loadRecentFiles(const QList<QFileInfo>& fileInfoList);

    /*! @brief Populates the menu with recent files given as a list of QFileInfo. Clears previous entries.
     *  @param fileInfoList List of QFileInfo objects. */
    void loadRecentFiles(QList<QFileInfo>&& fileInfoList);

    /*! @brief Removes all recent file actions from the menu and clears the list. */
    void reset();

    /*! @brief Returns a list of recent files. */
    QList<QFileInfo> fileInfoList() const { return m_recentFiles; }

    /*! @brief Returns QStringList containing list of full paths to the recent file entries. */
    QStringList recentFilesPathsList() const;

signals:
    /*! @brief Emitted when the menu is cleared (via reset() or "Clear" action). */
    void recentFilesMenuCleared();

    /*! @brief Emitted when a user selects a file from the recent files menu.
     *  @param fileInfo QFileInfo of the selected file. */
    void recentFileSelected(const QFileInfo& fileInfo);

public slots:
    /*! @brief Adds a file as a recent entry in the menu. If the file already exists, a new entry will be added (duplicates
     *         are possible).
     *  @param fileInfo QFileInfo of the recent file. */
    void addRecentAction(const QFileInfo& fileInfo);

    /*! @brief Updates an existing recent file action, replacing its info with a new file.
     *  @param oldFileInfo QFileInfo of the file to be replaced.
     *  @param newFileInfo QFileInfo to use as replacement.
     * @note Asserts in debug if oldFileInfo not found. */
    void updateRecentAction(const QFileInfo& oldFileInfo, const QFileInfo& newFileInfo);

    /*! @brief Updates a specific QAction, replacing its file info and display text.
     *  @param action Pointer to the QAction to update.
     *  @param newFileInfo QFileInfo to use as replacement.
     * @note Asserts in debug if action is not managed by this menu. */
    void updateRecentAction(QAction* action, const QFileInfo& newFileInfo);

    /*! @brief Removes a specific QAction from the menu.
     *  @param action Pointer to QAction to remove.
     * @note Asserts in debug if action is not managed by this menu. */
    void removeRecentAction(QAction* action);

protected:
    /*! @brief Handles retranslation on QEvent::LanguageChange.
     *  @param event The event pointer. */
    void changeEvent(QEvent* event) final;

private slots:
    /*! @brief Internal slot triggered when a file action is selected.
     *  @param action Pointer to the QAction triggered. */
    void _onRecentFileSelected(QAction* action);

    /*! @brief Internal slot triggered when "Clear" action is selected. */
    void _onRecentClearRequested();

private:
    friend class RecentFilesMenuTest;

    QList<QFileInfo> m_recentFiles;

    /*! @brief Initializes the menu structure, connects signals/slots, and sets up actions. */
    void _setupUi();

    /*! @brief Updates all visible texts to match the current language. */
    void _retranslateUi();
    QActionGroup* w_recentFilesActions;
    QAction* w_recentFilesSeparator;
    QAction* w_clearRecentAction;
};

}; // namespace Draupnir::Menus

#endif // RECENTFILESMENU_H
