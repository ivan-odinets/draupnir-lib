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

#include <QApplication>
#include <QAction>
#include <QIcon>

/*! @file draupnir-lib/templateMenus/traits/entries/HelpMenuEntries.h
 *  @brief Menu action and menu entry trait classes for the draupnir UI framework.
 *  @details This file contains trait classes describing standard help menu actions.
 *           All menu trait classes must provide:
 *           - using Type = QAction (or QMenu, or thier child classes)
 *           - static Type* createElement(QWidget* parent = nullptr)
 *           - static QString displayName() */

namespace Draupnir::Menus {

/*! @var helpMenuName
 *  @brief Inline constexpr lambda returning the localized display name for the "Help" menu. */

inline constexpr auto helpMenuName = []() -> QString {
    return QObject::tr("Help");
};

/*! @class HelpEntryMenuTrait draupnir-lib/templateMenus/traits/entries/HelpMenuEntries.h
 *  @brief Trait for the "Help" menu action (application help). Provides a QAction with the main application icon and
 *         localized name. */

class HelpEntryMenuTrait
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for the "About" action, with app icon.
     *  @param parent Optional parent object.
     *  @return Newly allocated QAction pointer.
     *
     * @note Uses the application's window icon as the action icon. Should only be called after QApplication is created. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        return result;
    }

    /*! @brief Returns the localized display name ("Help").
     *  @return QString. */
    static QString displayName() { return QObject::tr("Help"); }
};

/*! @class AboutAppMenuTrait draupnir-lib/templateMenus/traits/entries/HelpMenuEntries.h
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

    /*! @brief Returns the localized display name ("About").
     *  @return QString. */
    static QString displayName() { return QObject::tr("About"); }
};

/*! @class AboutDraupnirLibMenuTrait draupnir-lib/templateMenus/traits/entries/HelpMenuEntries.h
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
        return result;
    }

    /*! @brief Returns the localized display name ("About Qt").
     *  @return QString. */
    static QString displayName() { return QObject::tr("About DraupnirLib"); }
};

/*! @class AboutQtMenuTrait draupnir-lib/templateMenus/traits/entries/HelpMenuEntries.h
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

    /*! @brief Returns the localized display name ("About Qt").
     *  @return QString. */
    static QString displayName() { return QObject::tr("About Qt"); }
};

}; // namespace Draupnir::Menus

#endif // HELPMENUENTRIES_H
