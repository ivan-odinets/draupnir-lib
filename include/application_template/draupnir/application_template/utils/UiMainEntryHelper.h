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

#ifndef UIMAINENTRYHELPER_H
#define UIMAINENTRYHELPER_H

#include "draupnir/ui_bricks/core/ViewManager.h"

namespace Draupnir::Application::Utils
{

/*! @class UiMainEntryHelper draupnir/application_template/utils/UiMainEntryHelper.h
 *  @brief Compile-time helper that selects the main UI entry type for an application.
 *  @tparam MainWindow Main window type or `void` if not configured.
 *  @tparam TrayIcon   Tray icon type or `void` if not configured.
 *
 *  @details `UiMainEntryHelper` decides whether the application should use a dedicated ViewManager or operate directly on a
 *           MainWindow, based on the presence of `MainWindow` and `TrayIcon` configuration types.
 *
 *           Selection rules:
 *           - If both `MainWindow` and `TrayIcon` are provided (non-void), the helper selects `Ui::ViewManager<MainWindow,
 *             TrayIcon>` as the main UI entry and exposes it as both @ref MainUiEntry and @ref ViewManager.
 *           - If only `MainWindow` is provided and `TrayIcon` is `void`, the helper selects `MainWindow` as the main UI
 *             entry and sets @ref ViewManager to `void` (meaning “no view manager”).
 *           - If both are `void`, the helper yields `void` for both aliases. */

template<class MainWindow,class TrayIcon>
struct UiMainEntryHelper
{
    /*! @brief Selected main UI entry type. */
    using MainUiEntry = Ui::ViewManager<MainWindow,TrayIcon>;

    /*! @brief View manager type. @details Equal to @ref MainUiEntry in the case when both MainWindow and TrayIcon are present. */
    using ViewManager = MainUiEntry;
};

/*! @brief Specialization for "MainWindow only" configuration (no tray icon).
 *  @tparam MainWindow Main window type.
 *  @details When `TrayIcon` is `void`, the helper selects `MainWindow` as the main UI entry and indicates that no view manager
 *           is used by setting @ref ViewManager to `void`. */

template<class MainWindow>
struct UiMainEntryHelper<MainWindow,void>
{
    /*! @brief Selected main UI entry type (plain main window). */
    using MainUiEntry = MainWindow;

    /*! @brief No view manager is used in this configuration. */
    using ViewManager = void;
};

/*! @brief Specialization for “no UI” configuration (neither main window nor tray icon).
 *  @details This is a degenerate configuration where both `MainWindow` and `TrayIcon` are `void`. Both aliases resolve to `void`. */

template<>
struct UiMainEntryHelper<void,void>
{
    /*! @brief No main UI entry. */
    using MainUiEntry = void;

    /*! @brief No view manager. */
    using ViewManager = void;
};

}; // namespace Draupnir::Application::Utils

#endif // UIMAINENTRYHELPER_H
