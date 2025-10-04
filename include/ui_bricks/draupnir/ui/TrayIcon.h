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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

class QAction;
class QMenu;

namespace Draupnir::Ui
{

/*! @class TrayIcon draupnir/ui/TrayIcon.h
 *  @brief Provides system tray integration for the application, including message popups and localized tray menu.
 *  @details This class extends `QSystemTrayIcon` to offer:
 *           - Support for showing localized messages or message summaries.
 *           - A customizable context menu (`QMenu`) with support for dynamic translation.
 *           - An integrated "Exit" action bound to `QApplication::exit()`.
 *
 *           It handles language change events to dynamically update its menu content.
 *
 * @note Automatically assigns the application's main window icon as the tray icon. */

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    /*! @brief Constructs the tray icon and initializes the context menu.
     *  @details If the system tray is not available, a warning is logged. The tray icon will reuse the main application's
     *           window icon (`qApp->windowIcon()`).
     *  @param parent Optional QObject parent. */
    explicit TrayIcon(QObject *parent = nullptr);

    /*! @brief Destructor. Cleans up the tray menu and its actions. */
    ~TrayIcon();

    using QSystemTrayIcon::showMessage;

    /*! @brief Adds an action to the tray context menu.
     *  @details The action will appear as an item in the context menu shown when right-clicking the tray icon.
     *  @param action Pointer to a QAction to be added. */
    void addAction(QAction* action);

    /*! @brief Adds a submenu to the tray context menu.
     *  @details The menu will appear as a nested item inside the tray icon’s context menu.
     *  @param menu Pointer to a QMenu to be added. */
    void addMenu(QMenu* menu);

private:
    QMenu* w_trayMenu;
};

}; // namespace Draupnir::Ui

#endif // TRAYICON_H
