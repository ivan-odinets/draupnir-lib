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

#ifndef FILEMENUENTRIES_H
#define FILEMENUENTRIES_H

#include <QAction>

#include "SubmenuEntries.h"
#include "../menus/RecentFilesMenu.h"

/*! @file draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Menu action and menu entry trait classes for the draupnir UI framework.
 *  @details This file contains trait classes describing standard file menu actions.
 *           All menu trait classes must provide:
 *           - using Type = QAction (or QMenu, or thier child classes)
 *           - static Type* createElement(QWidget* parent = nullptr)
 *           - static QString displayName() */

namespace Draupnir::Menus {

/*! @var fileMenuName
 *  @brief Inline constexpr lambda returning the localized display name for the "File" menu. */

inline constexpr auto fileMenuName = []() -> QString {
    return QObject::tr("File");
};

/*! @class FileNewEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait for "File -> New" menu action. Provides an action with standard icon, display name, and shortcut (Ctrl+N). */

class FileNewEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "New" file action.
     *  @param parent Optional parent QObject.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{QIcon::fromTheme("document-new"), displayName(), parent};
        result->setShortcut(QKeySequence{"Ctrl+N"});
        return result;
    }

    /*! @brief Returns the localized display name for the action.
     *  @return Localized QString ("New"). */
    static QString displayName() { return QObject::tr("New"); }
};

/*! @class FileOpenEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait for "File -> Open" menu action. Provides an action with standard icon, display name, and shortcut (Ctrl+O). */

class FileOpenEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "Open" file action.
     *  @param parent Optional parent QObject.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{QIcon::fromTheme("document-open"), displayName(), parent};
        result->setShortcut(QObject::tr("Ctrl+O"));
        return result;
    }

    /*! @brief Returns the localized display name for the action.
     *  @return Localized QString ("Open"). */
    static QString displayName() { return QObject::tr("Open"); }
};

/*! @class FileSaveEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait for "File -> Save" menu action. Provides an action with standard icon, display name, and shortcut (Ctrl+S). */

class FileSaveEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "Save" file action.
     *  @param parent Optional parent QObject.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{QIcon::fromTheme("document-save"), displayName(), parent};
        result->setShortcut(QObject::tr("Ctrl+S"));
        return result;
    }

    /*! @brief Returns the localized display name for the action.
     *  @return Localized QString ("Save"). */
    static QString displayName() { return QObject::tr("Save"); }
};

/*! @class FileSaveAsEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait for "File -> Save As" menu action. Provides an action with standard icon and display name. */

class FileSaveAsEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "Save As" file action.
     *  @param parent Optional parent QObject.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        return new QAction{QIcon::fromTheme("document-save-as"), displayName(), parent};
    }

    /*! @brief Returns the localized display name for the action.
     *  @return Localized QString ("Save"). */
    static QString displayName() { return QObject::tr("Save As"); }
};

/*! @class FileCloseEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait for "File -> Close" menu action. Provides an action with standard icon and display name. */

class FileCloseEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "Close" file action.
     *  @param parent Optional parent QObject.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{QIcon::fromTheme("document-close"), displayName(), parent};
        return result;
    }

    /*! @brief Returns the localized display name for the action.
     *  @return Localized QString ("Close"). */
    static QString displayName() { return QObject::tr("Close"); }
};

/*! @var recentMenuEntryName draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Inline constexpr lambda returning the localized display name for the "Recent" submenu. To be used in menu trait
 *         template parameters. */

inline constexpr auto recentMenuEntryName = []() -> QString {
    return QObject::tr("Recent");
};

/*! @typedef RecentFileEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait type for the "Recent Files" submenu entry, using a custom menu class. Uses CustomMenuEntryTrait to adapt a
 *         custom menu widget for use as a menu entry trait. */

using RecentFileEntry = CustomMenuEntryTrait<recentMenuEntryName,RecentFilesMenu>;

/*! @class ExitApplicationEntry draupnir-lib/templateMenus/entries/FileMenuEntries.h
 *  @brief Trait for "File -> Exit" menu action. Provides an action with display name. */

class ExitApplicationEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "Exit" file action.
     *  @param parent Optional parent QObject.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        return new QAction{QIcon::fromTheme("application-exit"), displayName(), parent};
    }

    /*! @brief Returns the localized display name for the action.
     *  @return Localized QString ("Exit"). */
    static QString displayName() { return QObject::tr("Exit"); }
};

} // namespace Draupnir::Menus

#endif // FILEMENUENTRIES_H
