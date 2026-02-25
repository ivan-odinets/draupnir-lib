/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef APPLICATIONTEMPLATE_H
#define APPLICATIONTEMPLATE_H

#include <QApplication>

#include "draupnir/Logger.h"
#include "draupnir/application_template/utils/MenuBarHandlersHelper.h"
#include "draupnir/application_template/utils/UiMainEntryHelper.h"
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/utils/sfinae_detector_macro.h"

namespace Draupnir
{

/*! @class ApplicationTemplate draupnir/ApplicationTemplate.h
 *  @brief Compile-time configurable Qt application skeleton.
 *  @tparam ApplicationConfig A configuration struct that defines nested types described in
 *
 *  @details `ApplicationTemplate` builds an application's component graph at compile time from an `ApplicationConfig` "schema"
 *           (nested types). Optional parts are represented as `void` (via `extract_*_or_void_t`) and then removed from the
 *           final storage tuple.
 *
 *           The class is responsible for:
 *           - owning an internal `QApplication`;
 *           - building a unified `SettingsRegistry` out of settings-providing components;
 *           - instantiating all active components into a single `std::tuple`;
 *           - wiring everything together in @ref setup() (load settings, create/show UI, connect message system, connect menu
 *             handlers).
 *
 *           ## ConfigurationSchema Configuration schema
 *           `ApplicationConfig` is expected to provide:
 *
 *           **Required**
 *           - `struct ApplicationConfig::Core { ... };`
 *           - `struct ApplicationConfig::Ui   { ... };`
 *
 *           **Optional inside `Core`**
 *           - `Core::ExtraSettingsBundle`: An instantiation of `Draupnir::Settings::SettingsBundleTemplate<...>`. Included into
 *              the final @ref SettingsRegistry.
 *           - `Core::MessageSystem`: An instantiation of `Draupnir::Messages::MessageSystemTemplate<...>`. Included into @ref
 *              SettingsRegistry and configured during @ref setup().
 *
 *           **Optional inside `Ui`**
 *           - `Ui::MainWindow`: An instantiation of `Draupnir::Ui::MainWindowTemplate<...>`. Used as the main UI element.
 *           - `Ui::TrayIcon`: A tray icon type. If both MainWindow and TrayIcon exist, a ViewManager will be used.
 *           - `Ui::MenuBar`: An instantiation of `Draupnir::Ui::MenuBarTemplate<...>`. Used to extract Settings/Help menus.
 *           - `struct Ui::MenuHandlers { using HelpSource = ...; };` Provides a source object/type for Help menu handler (if used). */

template<class ApplicationConfig>
class ApplicationTemplate
{
    // Using this macros - a SFINAE-based detector for a specified nested type will be created.
    // DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(NestedType) gives the following:
    // - template<class T, class = std::void_t<>> struct has_NestedType : std::false_type {};
    // - template<class T> struct has_NestedType<T, std::void_t<typename T::NestedType>> : std::true_type {};
    // - template<class T> inline constexpr bool has_NestedType_v = has_NestedType<T>::value;
    DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(Core);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(ExtraSettingsBundle);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(MessageSystem);

    DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(Ui);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(CentralWidget);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(MainWindow);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(MenuBar);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(MenuHandlers);
            DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(HelpSource);
        DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(TrayIcon);


    /*! @brief Extracted `ApplicationConfig::Core`. Must be present. */
    using Core                         = extract_Core_or_void_t<ApplicationConfig>;
    static_assert(!std::is_void_v<Core>);

    /*! @brief Optional extra settings bundle (added to SettingsRegistry). */
    using ExtraSettingsBundle_or_void  = extract_ExtraSettingsBundle_or_void_t<Core>;

    /*! @brief Optional message system type. */
    using MessageSystem_or_void        = extract_MessageSystem_or_void_t<Core>;

    /*! @brief Extracted `ApplicationConfig::Ui`. Must be present. */
    using Ui                      = extract_Ui_or_void_t<ApplicationConfig>;
    static_assert(!std::is_void_v<Ui>);

    /*! @brief Optional main window type. */
    using MainWindow_or_void      = extract_MainWindow_or_void_t<Ui>;

    /*! @brief Optional tray icon type. */
    using TrayIcon_or_void        = extract_TrayIcon_or_void_t<Ui>;

    /*! @brief Helper that decides how UI is orchestrated.
     *  @details If both MainWindow and TrayIcon are provided, the helper may select a ViewManager. The ViewManager is then
     *           responsible for creating UI elements, loading settings into them and showing/hiding them. Otherwise,
     *           MainWindow is used directly without ViewManager. */
    using UiMainEntryDeductor     = Application::Utils::UiMainEntryHelper<MainWindow_or_void,TrayIcon_or_void>;

    /*! @brief Either ViewManager type or MainWindow type (or void if none). */
    using UiMainEntryType_or_void = UiMainEntryDeductor::MainUiEntry;

    /*! @brief ViewManager type if used, otherwise void. */
    using ViewManager_or_void     = UiMainEntryDeductor::ViewManager;

    /*! @brief Settings registry used by this application.
     *  @details Built by concatenating settings traits from:
     *           - the main UI entry (ViewManager or MainWindow);
     *           - optional ExtraSettingsBundle;
     *           - optional MessageSystem;
     * @note `SettingsTraitsConcatenator` is expected to ignore `void` parameters. */
    using SettingsRegistry = Draupnir::Settings::SettingsTraitsConcatenator<
        UiMainEntryType_or_void,
        ExtraSettingsBundle_or_void,
        MessageSystem_or_void
    >::toSettingsRegistry;

    /*! @brief Menu-bar helper used to extract menus and build handler types.
     *  @details If `Ui::MenuBar` is not specified, helper specialization for `void` should yield `void` menus/handlers. */
    using MenuBarHandlersHelper = Application::Utils::MenuBarHandlersHelper<extract_MenuBar_or_void_t<Ui>>;

    /*! @brief Settings menu type extracted from MenuBar or `void`. */
    using SettingsMenu = MenuBarHandlersHelper::SettingsMenu_or_void;

    /*! @brief Handler for Settings menu. Depends on `SettingsRegistry`. */
    using SettingsMenuHandler = typename MenuBarHandlersHelper::template SettingsMenuHandler<SettingsRegistry>;

    /*! @brief Help menu type extracted from MenuBar or `void`. */
    using HelpMenu = MenuBarHandlersHelper::HelpMenu_or_void;

    /*! @brief Handler for Help menu. */
    using HelpMenuHandler = typename MenuBarHandlersHelper::template HelpMenuHandler<extract_HelpSource_or_void_t<extract_MenuHandlers_or_void_t<Ui>>>;

    /*! @brief `std::tuple` storing all enabled components of the application.
     *  @details The tuple is created from a `draupnir::utils::type_list`:
     *           - includes potentially `void` types;
     *           - removes `void`;
     *           - converts to `std::tuple`. */
    using ApplicationElementsTuple =
        // Lets build a draupnir::utils::type_list of stuff.
        typename draupnir::utils::type_list<
            // Either ViewManager or MainWindow
            UiMainEntryType_or_void,
            MessageSystem_or_void,
            SettingsMenuHandler,
            HelpMenuHandler
        >
        // Remove any possible void types
        ::template remove_all_t<void>
        // And convert to the std::tuple
    ::template convert_to_t<std::tuple>;

public:
    /*! @brief Default constructor. Similarly to QApplication requires `int& argc` and `char* argv[]` parameters to be
     *         passed (for internal QApplication instance.
     *  @param argc Qt-style argc (passed by reference, as required by QApplication).
     *  @param argv Qt-style argv.
     *  @details Initializes the internal `QApplication`. Components inside `m_elementsTuple` and `m_settingsRegistry`
     *           are default-constructed. Call @ref setup() to wire everything. */
    ApplicationTemplate(int& argc, char* argv[]) :
        qtApplication{argc,argv}
    {}

    /*! @brief Wires components together, loads settings, and shows UI.
     *  @details Execution order:
     *           1) Loads settings via @ref settingsRegistry().
     *           2) Initializes and shows UI:
     *             - If ViewManager exists: creates UI via ViewManager, loads settings into UI
     *               elements, shows them;
     *             - Otherwise: loads settings into MainWindow and shows it;
     *           3) Initializes MessageSystem (if present):
     *             - loads settings;
     *             - injects TrayIcon (if available);
     *             - installs message handler into Logger;
     *           4) Connects menu handlers (Settings/Help) if menus and handlers exist. */
    void setup() {
        // Load settings
        settingsRegistry()->loadSettings();

        // Depending on if we have ViewManager or pure MainWindow - we need either to configure ViewManager or to configure MainWindow
        if constexpr (!std::is_void_v<ViewManager_or_void>) {
            // Configure ViewManager_or_void responsible for handling Ui elements, create UiElements and load their settings
            auto& viewManager = std::get<ViewManager_or_void>(m_elementsTuple);
            viewManager.createUi();
            viewManager.template loadSettings<SettingsRegistry>(settingsRegistry());
            // Show Ui elements
            viewManager.showUi();
        } else {
            // We dont have any ViewManager, just pure MainWindow. Lets load settings
            mainWindow()->template loadSettings<SettingsRegistry>(settingsRegistry());
            // And show window
            mainWindow()->show();
        }

        // Setup MessageSystem
        if constexpr (!std::is_void_v<MessageSystem_or_void>) {
            messageSystem()->template loadSettings<SettingsRegistry>(settingsRegistry());
            messageSystem()->handler()->setTrayIcon(trayIcon());
            Logger::get().setMessageHandler(messageSystem()->handler());
        }

        // Setup Settings Menu handlers
        if constexpr (!std::is_void_v<SettingsMenu> && !std::is_void_v<SettingsMenuHandler>) {
            _connectMenuHandler<SettingsMenu,SettingsMenuHandler>();
            std::get<SettingsMenuHandler>(m_elementsTuple).template loadSettings<SettingsRegistry>(settingsRegistry());
        }
        if constexpr (!std::is_void_v<HelpMenu> && !std::is_void_v<HelpMenuHandler>)
            _connectMenuHandler<HelpMenu,HelpMenuHandler>();
    }

    /*! @brief Runs Qt event loop.
     *  @return Exit code of `QApplication::exec()`. */
    int exec() { return qtApplication.exec(); }

///@name Conditional getters
///@{
    /*! @brief Returns a pointer to the settings registry used by this application. */
    auto* settingsRegistry() requires (!std::is_void_v<SettingsRegistry>) {
        return &m_settingsRegistry;
    }

    /*! @brief Returns a pointer to MainWindow instance.
     *  @details If ViewManager is used, returns ViewManager::mainWindow(). Otherwise returns a pointer to the MainWindow
     *           stored directly in @ref m_elementsTuple. */
    auto* mainWindow() requires (!std::is_void_v<MainWindow_or_void>) {
        if constexpr (!std::is_void_v<ViewManager_or_void>) {
            return std::get<ViewManager_or_void>(m_elementsTuple).mainWindow();
        } else {
            return &std::get<MainWindow_or_void>(m_elementsTuple);
        }
    }

    /*! @brief Returns a pointer to TrayIcon instance (when ViewManager is used). */
    auto* trayIcon() requires (!std::is_void_v<TrayIcon_or_void> && !std::is_void_v<ViewManager_or_void>) {
        return std::get<ViewManager_or_void>(m_elementsTuple).trayIcon();
    }

    /*! @brief Returns a pointer to MessageSystem instance stored in @ref m_elementsTuple. */
    auto* messageSystem() requires(!std::is_void_v<MessageSystem_or_void>) {
        return &std::get<MessageSystem_or_void>(m_elementsTuple);
    }
///@}

private:
    QApplication qtApplication;
    SettingsRegistry m_settingsRegistry;
    ApplicationElementsTuple m_elementsTuple;

    /*! @brief Connects a menu handler to menu actions.
     *  @tparam Menu Menu type to request from MainWindow's menu bar.
     *  @tparam Handler Handler type stored in @ref m_elementsTuple.
     *  @details Retrieves the concrete menu instance from `MainWindow::customMenuBar()` and forwards it to handler's
     *           `connectActions(menu)`. */
    template<class Menu,class Handler>
    inline void _connectMenuHandler() {
        auto menu = mainWindow()->customMenuBar()
            ->template getUiElement<Menu>();
        std::get<Handler>(m_elementsTuple).connectActions(menu);
    }
};

}; // namespace Draupnir

#endif // APPLICATIONTEMPLATE_H
