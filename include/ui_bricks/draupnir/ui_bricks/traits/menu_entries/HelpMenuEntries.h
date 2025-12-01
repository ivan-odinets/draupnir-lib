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

#ifndef HELPMENUENTRIES_H
#define HELPMENUENTRIES_H

/*! @file draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Menu action and menu entry trait classes for the standart help menu actions.
 *
 *  @details This file contains trait classes describing standard help menu actions.
 *
 * @todo Split this file into multiple subfiles and include these new files by using this file.
 * @todo Entries within this file may and should be splited in the seperate namespace. */

#include <QApplication>
#include <QAction>
#include <QIcon>

#include "draupnir/utils/ResourceHelper.h"

namespace Draupnir::Ui {

/*! @var helpMenuName draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Inline constexpr lambda returning the localized display name for the "Help" menu. */

inline constexpr auto helpMenuName = []() -> QString {
    return QObject::tr("Help");
};

/*! @class HelpEntryMenuTrait draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for the "Help" menu action (application help). Provides a QAction with the main application icon and
 *         localized name. */

class HelpEntryMenuTrait
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "About" action, with app icon.
     *  @param parent Optional parent object.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        return result;
    }

    /*! @brief Returns the localized display name ("Help"). */
    static QString displayName() { return QObject::tr("Help"); }
};

/*! @class AboutAppMenuTrait draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for the "About" menu action (application about box). Provides a QAction with the main application icon and
 *         localized name. */

class AboutAppMenuTrait
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "About" action, with app icon.
     *  @param parent Optional parent object.
     *  @return Newly allocated QAction pointer.
     *
     * @note Uses the application's window icon as the action icon. Should only be called after QApplication is created. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{qApp->windowIcon(), displayName(), parent};
        return result;
    }

    /*! @brief Returns the localized display name ("About"). */
    static QString displayName() { return QObject::tr("About"); }
};

/*! @class AboutDraupnirLibMenuTrait draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for the "About DraupnirLib" menu action. Provides a QAction with localized name. */

class AboutDraupnirLibMenuTrait
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "About DraupnirLib" action.
     *  @param parent Optional parent object.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        result->setIcon(Resources::ResourceHelper::get().draupnirIcon());
        return result;
    }

    /*! @brief Returns the localized display name ("About Qt"). */
    static QString displayName() { return QObject::tr("About DraupnirLib"); }
};

/*! @class AboutQtMenuTrait draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for the "About Qt" menu action. Provides a QAction with localized name (for the standard Qt about dialog). */

class AboutQtMenuTrait
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "About Qt" action.
     *  @param parent Optional parent object.
     *  @return Newly allocated QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        return result;
    }

    /*! @brief Returns the localized display name ("About Qt"). */
    static QString displayName() { return QObject::tr("About Qt"); }
};

}; // namespace Draupnir::Ui

#endif // HELPMENUENTRIES_H
