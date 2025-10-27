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

#ifndef SETTINGSMENUENTRIES_H
#define SETTINGSMENUENTRIES_H

/*! @file draupnir/traits/entries/SettingsMenuEntries.h
 *  @ingroup MenuTemplates
 *  @brief Menu action and menu entry trait classes for the draupnir UI framework.
 *
 *  @details This file contains trait classes describing standard settings menu actions.
 *
 * @todo Finalize naming, file (which will contain these traits) for settings within the  namespace @ref
 *       Draupnir::Settings::MainWindow
 *
 * @todo Split this file into multiple subfiles and include these new files by using this file. */

#include <QAction>

namespace Draupnir::Menus {

/*! @var settingsMenuName draupnir/traits/entries/SettingsMenuEntries.h
 *  @ingroup MenuTemplates
 *  @brief Inline constexpr lambda returning the localized display name for the "Settings" menu. */

inline constexpr auto settingsMenuName = []() -> QString {
    return QObject::tr("Settings");
};

/*! @class OpenSettingsEntry draupnir/traits/entries/SettingsMenuEntries.h
 *  @ingroup MenuTemplates
 *  @brief Trait for "Open Settings" action. Provides a non-checkable action for opening the settings dialog or window. */

class OpenSettingsEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction for "Open Settings".
     *  @param parent Optional parent object.
     *  @return Pointer to the created QAction. */
    static QAction* createElement(QWidget* parent = nullptr) {
        return new QAction{displayName(), parent};
    }

    /*! @brief Returns the localized display name.
     *  @return QString ("Open Settings"). */
    static QString displayName() { return QObject::tr("Open Settings"); }
};

/*! @class MinimizeOnCloseEntry draupnir/traits/entries/SettingsMenuEntries.h
 *  @ingroup MenuTemplates
 *  @brief Trait for "Minimize on close" checkable action. Provides a checkable action for toggling "minimize on close"
 *         behavior. */

class MinimizeOnCloseEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a checkable QAction for "Minimize on close".
     *  @param parent Optional parent object.
     *  @return Pointer to the created QAction. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        result->setCheckable(true);
        return result;
    }

    /*! @brief Returns the localized display name.
     *  @return QString ("Minimize on close"). */
    static QString displayName() { return QObject::tr("Minimize on close"); }
};

#ifndef QT_NO_SYSTEMTRAYICON

/*! @class MinimizeToTrayEntry draupnir/traits/entries/SettingsMenuEntries.h
 *  @ingroup MenuTemplates
 *  @brief Trait for "Minimize to tray" checkable action. Provides a checkable action for toggling "minimize to tray" behavior.
 * @note Available only when building without the QT_NO_SYSTEMTRAYICON macro. */

class MinimizeToTrayEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a checkable QAction for "Minimize to tray".
     *  @param parent Optional parent object.
     *  @return Pointer to the created QAction. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        result->setCheckable(true);
        return result;
    }

    /*! @brief Returns the localized display name.
     *  @return QString ("Minimize to tray"). */
    static QString displayName() { return QObject::tr("Minimize to tray"); }
};

#endif // QT_NO_SYSTEMTRAYICON

/*! @class StartHiddenMenuEntry draupnir/traits/entries/SettingsMenuEntries.h
 *  @ingroup MenuTemplates
 *  @brief Trait for "Start Hidden" checkable action. Provides a checkable action for toggling "start hidden" behavior on
 *         application launch. */

class StartHiddenMenuEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a checkable QAction for "Start hidden".
     *  @param parent Optional parent object.
     *  @return Pointer to the created QAction. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        result->setCheckable(true);
        return result;
    }

    /*! @brief Returns the localized display name.
     *  @return QString ("Start hidden"). */
    static QString displayName() { return QObject::tr("Start Hidden"); }
};

}; // namespace Draupnir::Menus

#endif // SETTINGSMENUENTRIES_H
