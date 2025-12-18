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

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/settings_registry/traits/settings/main_window/StartHiddenSetting.h"

namespace Draupnir::Ui
{

/*! @class ViewManager draupnir/ui_bricks/core/ViewManager.h
 *  @ingroup UiBricks
 *  @brief Manages the main UI components such as `MainWindow` and `TrayIcon`.
 *  @tparam MainWindowClass  `QWidget`-derived class representing the main application window. Must be constructible without
 *                           arguments and may accept a `TrayIconClass` pointer via `void MainWindowClass::setTrayIcon(TrayIconClass*)
 *                           method.
 *  @tparam TrayIconClass    `QSystemTrayIcon`-compatible class representing the tray icon UI.
 *
 *  @details This templated class is responsible for creating, showing, and cleaning up the primary user interface components:
 *           the main window and tray icon. It supports both automatic instantiation via `createUi()` and manual injection via
 *           `setMainWindow()` / `setTrayIcon()`. It also handles UI-related settings, such as whether the main window starts
 *           hidden.
 *
 *           The expected interfaces:
 *           - MainWindowClass must provide:
 *              - void show();
 *              - [optionally] void setTrayIcon(TrayIconClass*);
 *           - TrayIconClass must provide:
 *              - void show(); */

template<class MainWindowClass, class TrayIconClass>
class ViewManager
{
    /*! @brief Trait to check if a class provides `void show()` method. */
    template<class, class = std::void_t<>>
    struct has_show : std::false_type {};

    template<class T>
    struct has_show<
        T,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<T>().show())>
        )>
    > : std::true_type {};

    /*! @brief Trait to check if a class provides `void setTrayIcon(TrayIconClass*)` method. */
    template<class, class = std::void_t<>>
    struct has_setTrayIcon : std::false_type {};

    template<class T>
    struct has_setTrayIcon<
        T,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<T>().template setTrayIcon<TrayIconClass>(std::declval<TrayIconClass*>()))>
        )>
    > : std::true_type {};

    template<class T, class SettingsRegistry, class = std::void_t<>>
    struct has_loadSettings : std::false_type {};

    template<class T, class SettingsSource>
    struct has_loadSettings<
        T,
        SettingsSource,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<T>().template loadSettings<SettingsSource>(std::declval<SettingsSource*>()))>
        )>
    > : std::true_type {};

    using _SettingsBundleInternal = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Settings::MainWindow::StartHiddenSetting
    >;

public:
    /*! @brief This is public SettingsBundleTemplate instantiation which contains settings from all UI components managed by this
     *         ViewManager. */
    using SettingsBundle = typename Draupnir::Settings::SettingsTraitsConcatenator<
        _SettingsBundleInternal,
        MainWindowClass
    >::toSettingsBundle;

    /*! @brief Constructs an empty ViewManager with null-initialized elements.
     *  @details Also verifies at compile-time that the provided UI classes conform to the expected interfaces. */
    ViewManager() :
        m_elementsTuple{}
    {
        static_assert(has_show<MainWindowClass>(),
                "Provided MainWindowClass argument must have void show() method.");
        static_assert(has_show<TrayIconClass>(),
                "Provided TrayIconClass argument must have void show() method.");
    }

    /*! @brief Destructor. Cleans up created UI elements (if any). */
    ~ViewManager() {
        std::apply([](auto elements...){
            delete elements;
        },m_elementsTuple);
    }

    /*! @brief Loads UI-related settings from a settings source (typically @ref Draupnir::Settings::SettingsRegistry).
     *  @tparam SettingsSource    type of specific source of settings. Typically this will be @ref Draupnir::Settings::SettingsRegistryTemplate,
     *                            but @ref Draupnir::Settings::SettingsBundleTemplate can be used as well. The povided SettingsSource
     *                            must be capable to populate `SettingsBundle` of the `ViewManager` object.
     *  @param settings Pointer to a SettingsSource instance.
     *  @details This must be called exactly once before calling `showUi()` method. Will assert if called more than once. */
    template<class SettingsSource>
    void loadSettings(SettingsSource* settings) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>(),
                "Provided SettingsSource can not populate the SettingsBundle required by this ViewManager.");
        Q_ASSERT_X(settings, "ViewManager::loadSettings",
                   "Provided SettingsSource pointer is nullptr");
        Q_ASSERT_X(!m_settings.isValid(), "ViewManager::loadSettings",
                   "This method must be called only once.");

        m_settings = settings->template getSettingsBundle<_SettingsBundleInternal>();

        if constexpr (has_loadSettings<MainWindowClass, SettingsSource>::value) {
            mainWindow()->template loadSettings<SettingsSource>(settings);
        }
    }

    /*! @brief Sets the "start hidden" flag and updates the corresponding persistent setting.
     *  @param state If `true`, the main window will be hidden on application startup.
     * @note Must be called after loadSettings(). */
    void setStartHidden(bool state) {
        Q_ASSERT_X(m_settings.isValid(),"ViewManager::setStartHidden",
                   "This method must be called after UiManager::loadSettings method.");
        if (startHidden() == state)
            return;

        m_settings.template set<Draupnir::Settings::MainWindow::StartHiddenSetting>(state);
    }

    /*! @brief Checks whether the main window is configured to start hidden.
     *  @return `True` if the UI should not show the main window initially.
     * @note Must be called after `loadSettings()`. */
    bool startHidden() const {
        Q_ASSERT_X(m_settings.isValid(),"ViewManager::startHidden",
                   "This method must be called after UiManager::loadSettings method.");

        return m_settings.template get<Draupnir::Settings::MainWindow::StartHiddenSetting>();
    }

    /*! @brief Creates instances of `MainWindowClass` and `TrayIconClass`, and connects them together if MainWindowClass has
     *         `void MainWindowClass::setTrayIcon(TrayIconClass*)` method. This method must be called only once, and is mutually
     *          exclusive with manual injection via `setMainWindow()` and `setTrayIcon()`. */
    void createUi() {
        Q_ASSERT_X(!mainWindow(),"ViewManager::setMainWindow",
                   "Only one of the either this method or ViewManager::setMainWindow must be called only once.");
        Q_ASSERT_X(!trayIcon(),"ViewManager::setTrayIcon",
                   "Only one of the either this method or ViewManager::setTrayIcon must be called only once.");

        std::get<MainWindowClass*>(m_elementsTuple) = new MainWindowClass{};
        std::get<TrayIconClass*>(m_elementsTuple) = new TrayIconClass;

        if constexpr (has_setTrayIcon<MainWindowClass>::value) {
            std::get<MainWindowClass*>(m_elementsTuple)->setTrayIcon(
                std::get<TrayIconClass*>(m_elementsTuple)
            );
        }
    }

    /*! @brief Injects an existing `MainWindowClass` instance into the `ViewManager`.
     *  @param newWindow Pointer to a `MainWindowClass` object.
     * @note The ViewManager takes ownership and will delete the object in its destructor.
     * @note Must be called exactly once, and only if `createUi()` was not used. */
    void setMainWindow(MainWindowClass* newWindow) {
        Q_ASSERT_X(newWindow,"ViewManager::setMainWindow",
                   "Provided MainWindowClass* pointer is nullptr.");
        Q_ASSERT_X(!mainWindow(),"ViewManager::setMainWindow",
                   "Only one of the either this method or ViewManager::createUi must be called only once.");

        std::get<MainWindowClass*>(m_elementsTuple) = newWindow;
    }

    /*! @brief Injects an existing `TrayIconClass` instance into the `ViewManager`.
     *  @param newTrayIcon Pointer to a `TrayIconClass` object.
     * @note The `ViewManager` takes ownership and will delete the object in its destructor.
     * @note Must be called exactly once, and only if `createUi()` was not used. */
    void setTrayIcon(TrayIconClass* newTrayIcon) {
        Q_ASSERT_X(newTrayIcon,"ViewManager::setTrayIcon",
                   "Provided TrayIconClass* pointer is nullptr.");
        Q_ASSERT_X(!trayIcon(),"ViewManager::setTrayIcon",
                   "Only one of the either this method or ViewManager::createUi must be called only once.");

        std::get<TrayIconClass*>(m_elementsTuple) = newTrayIcon;
    }

    /*! @brief Shows the main UI elements. The tray icon is always shown. The main window is shown only if the "start hidden"
     *         flag is `false`. Must be called after loadSettings().*/
    void showUi() {
        Q_ASSERT_X(mainWindow(), "ViewManager::showUi()",
                   "MainWindowClass instance is not set. Either ViewManager::createUi or ViewManager::setMainWindow methods "
                   "must be called before this.");
        Q_ASSERT_X(trayIcon(), "ViewManager::showUi()",
                   "TraiIconClass instance is not set. Either ViewManager::createUi or ViewManager::setTrayIcon methods "
                   "must be called before this.");
        Q_ASSERT_X(m_settings.isValid(),"ViewManager::showUi",
                   "This method must be called after UiManager::loadSettings method.");
        trayIcon()->show();

        if (!startHidden())
            mainWindow()->show();
    }

    /*! @brief Returns a pointer to the stored UI element of a given type.
     *  @tparam Element Type of the element to retrieve. Must match either `MainWindowClass` or `TrayIconClass`.
     *  @return Pointer to the requested element, or `nullptr` if it has not been set. */
    template<class Element>
    Element* getElement() { return std::get<Element*>(m_elementsTuple); }

    /*! @brief Returns the pointer to the tray icon instance.
     *  @return Pointer to `TrayIconClass`. */
    TrayIconClass* trayIcon() { return std::get<TrayIconClass*>(m_elementsTuple); }

    /*! @brief Returns the pointer to the main window instance.
     *  @return Pointer to `MainWindowClass`. */
    MainWindowClass* mainWindow() { return std::get<MainWindowClass*>(m_elementsTuple); }

private:
    _SettingsBundleInternal m_settings;
    std::tuple<
        MainWindowClass*,
        TrayIconClass*
    > m_elementsTuple;
};

};

#endif // VIEWMANAGER_H
