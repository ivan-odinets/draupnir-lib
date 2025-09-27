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

#include "draupnir/SettingsBundleTemplate.h"
#include "draupnir/core/MessageHandler.h"
#include "draupnir/traits/settings/StartHiddenSetting.h"
#include "draupnir/MessageSystemInterface.h"

namespace Draupnir::Ui
{

/*! @class UiManager draupnir/core//UiManager.h
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
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Settings::StartHiddenSetting
    >;

    /*! @brief Constructs the UiManager, stores a pointer to the message system and initializes internal pointers to nullptr.
     *  @param messageSystem Pointer to the already initialized MessageSystemInterface. */
    UiManager(Draupnir::Messages::MessageSystemInterface* messageSystem) :
        p_messageSystem{messageSystem},
        w_mainWindow{nullptr},
        w_trayIcon{nullptr}
    {}

    /*! @brief Destructor. Cleans up created UI elements (if they exist) via QObject::deleteLater. */
    ~UiManager() {
        if (w_mainWindow) {
            delete w_mainWindow;
            w_mainWindow = nullptr;
        }

        if (w_trayIcon) {
            delete w_trayIcon;
            w_trayIcon = nullptr;
        }
    }

    /*! @brief Loads UI-related settings.
     *  @param settings Pointer to the SettingsRegistry instance.
     *  @details This must be called once after createUi() to initialize state (e.g. whether the window starts hidden). Will assert
     *           if called more than once or before UI creation. */
    template<class SettingsRegistry>
    void loadSettings(SettingsRegistry* settings) {
        Q_ASSERT_X(settings, "UiManager::loadSettings",
                   "Provided SettingsRegistry pointer is nullptr");

        m_settings = settings->template getSettingsBundle<SettingsBundle>();
    }

    /*! @brief @brief Sets the flag indicating whether MainWindow should start hidden. Also stores the state in SettingsBundle
     *  @param state True if MainWindow should be hidden at startup. */
    void setStartHidden(bool state) {
        if (startHidden() == state)
            return;

        m_settings.template set<Draupnir::Settings::StartHiddenSetting>(state);
    }

    /*! @brief Returns whether the MainWindow is configured to start hidden.
     *  @return True if the UI should not show the MainWindow initially. */
    bool startHidden() const {
        return m_settings.template get<Draupnir::Settings::StartHiddenSetting>();
    }

    /*! @brief Creates MainWindow and TrayIcon instances.
     *  @details Instantiates the UI components using the given types and wires TrayIcon to the message system. */
    void createUi() {
        w_mainWindow = new MainWindowClass{p_messageSystem->uiBuilder()};
        w_trayIcon = new TrayIconClass;

        p_messageSystem->handler()->setTrayIcon(w_trayIcon);

        w_mainWindow->setTrayIcon(w_trayIcon);
    }

    /*! @brief Shows the MainWindow and TrayIcon (depending on settings). The TrayIcon is always shown. The MainWindow is shown
     *         unless startHidden() returns true. */
    void showUi() {
        w_trayIcon->show();

        if (!m_settings.template get<Draupnir::Settings::StartHiddenSetting>())
            w_mainWindow->show();
    }

    /*! @brief Returns the pointer to the tray icon instance.
     *  @return Pointer to TrayIconClass. */
    TrayIconClass* trayIcon() { return w_trayIcon; }

    /*! @brief Returns the pointer to the main window instance.
     *  @return Pointer to MainWindowClass. */
    MainWindowClass* mainWindow() { return w_mainWindow; }

private:
    SettingsBundle m_settings;
    Draupnir::Messages::MessageSystemInterface* p_messageSystem;
    MainWindowClass* w_mainWindow;
    TrayIconClass* w_trayIcon;
};

};

#endif // UIMANAGER_H
