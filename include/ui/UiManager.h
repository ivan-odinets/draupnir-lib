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

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "AppSettings.h"
#include "lib/draupnir-lib/src/messages/core/MessageHandlerTemplate.h"
#include "MessageSystemInterface.h"

/*! @class UiManager draupnir-lib/ui/UiManager.h
 *  @brief Manages the main UI components such as MainWindow and TrayIcon.
 *  @details This templated class is responsible for creating, showing, and disposing the primary user interface components.
 *           It integrates with the MessageSystemInterface to wire up messaging and UI behavior.
 *  @tparam MainWindowClass - A QWidget-derived class representing the main application window. Must accept MessageUiBuilder in
 *          its constructor.
 *  @tparam TrayIconClass - A QSystemTrayIcon / TrayIcon (or compatible) class representing the tray icon UI. */

template<class MainWindowClass, class TrayIconClass>
class UiManager
{
public:
    /*! @brief Constructs the UiManager, stores a pointer to the message system and initializes internal pointers to nullptr.
     *  @param messageSystem Pointer to the already initialized MessageSystemInterface. */
    UiManager(MessageSystemInterface* messageSystem) :
        p_settings{nullptr},
        m_startHidden{defaultStartHidden},
        p_messageSystem{messageSystem},
        w_mainWindow{nullptr},
        w_trayIcon{nullptr}
    {}

    /*! @brief Destructor. Cleans up created UI elements (if they exist) via QObject::deleteLater. */
    ~UiManager() {
        if (w_mainWindow) {
            w_mainWindow->deleteLater();
            w_mainWindow = nullptr;
        }

        if (w_trayIcon) {
            w_trayIcon->deleteLater();
            w_trayIcon = nullptr;
        }
    }

    /*! @brief Loads UI-related settings.
     *  @param settings Pointer to the AppSettings instance.
     *  @details This must be called once after createUi() to initialize state (e.g. whether the window starts hidden). Will assert
     *           if called more than once or before UI creation. */
    void loadSettings(AppSettings* settings) {
        Q_ASSERT_X(settings, "UiManager::loadSettings",
                   "Provided AppSettings pointer is nullptr");
        Q_ASSERT_X(p_settings == nullptr, "UiManager::loadSettings",
                   "This method must be called only once.");
        Q_ASSERT_X(w_mainWindow,"UiManager::loadSettings",
                   "UiManager::createUi must have been called before.");

        p_settings = settings;

        m_startHidden = p_settings->value(
            startHidden_settingsKey, defaultStartHidden
        ).toBool();

        w_mainWindow->loadSettings(settings);
    }

    /*! @brief @brief Sets the flag indicating whether MainWindow should start hidden. Also stores the state in AppSettings.
     *  @param state True if MainWindow should be hidden at startup. */
    void setStartHidden(bool state) {
        if (m_startHidden == state)
            return;

        m_startHidden = state;

        p_settings->setValue(
            startHidden_settingsKey, state
        );
    }

    /*! @brief Returns whether the MainWindow is configured to start hidden.
     *  @return True if the UI should not show the MainWindow initially. */
    bool startHidden() const { return m_startHidden; }

    /*! @brief Creates MainWindow and TrayIcon instances.
     *  @details Instantiates the UI components using the given types and wires TrayIcon to the message system. */
    void createUi() {
        w_mainWindow = new MainWindowClass{p_messageSystem->uiBuilder()};
        w_trayIcon = new TrayIconClass;

        p_messageSystem->handler()->setTrayIcon(w_trayIcon);
    }

    /*! @brief Shows the MainWindow and TrayIcon (depending on settings). The TrayIcon is always shown. The MainWindow is shown
     *         unless startHidden() returns true. */
    void showUi() {
        w_trayIcon->show();

        if (!m_startHidden)
            w_mainWindow->show();
    }

    /*! @brief Returns the pointer to the tray icon instance.
     *  @return Pointer to TrayIconClass. */
    TrayIconClass* trayIcon() { return w_trayIcon; }

    /*! @brief Returns the pointer to the main window instance.
     *  @return Pointer to MainWindowClass. */
    MainWindowClass* mainWindow() { return w_mainWindow; }

private:
    static const inline QLatin1String  startHidden_settingsKey{"startHidden"};
    static const inline bool           defaultStartHidden = false;

    AppSettings*             p_settings;
    bool                     m_startHidden;
    MessageSystemInterface*  p_messageSystem;
    MainWindowClass*         w_mainWindow;
    TrayIconClass*           w_trayIcon;
};

#endif // UIMANAGER_H
