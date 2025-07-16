/*
 **********************************************************************************************************************
 *
 * This file is part of the crypto-stalker project.
 *
 * Copyright (c) 2025 Ivan Odinets <i_odinets@protonmail.com>
 *
 * crypto-stalker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * crypto-stalker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with crypto-stalker. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

class QAction;
class QMenu;

/*! @class TrayIcon draupnir-lib/include/trayIcon/TrayIcon.h
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

protected:
    /*! @brief Handles Qt events, in particular `LanguageChange` events for dynamic retranslation.
     *  @details When a `LanguageChange` event is received, the tray menu labels are updated.
     *  @param event The event being processed.
     *  @return true if the event was handled; otherwise passes it to the base class. */
    bool event(QEvent* event) final;

private:
    /*! @brief Updates UI elements with translated strings.
     *  @details Currently re-translates the text for the "Exit" menu action. */
    void _retranslateUi();
    QMenu* w_trayMenu;
    QAction* w_exitTrayAction;
};

#endif // TRAYICON_H
