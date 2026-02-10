/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MINIMIZABLETOTRAY_H
#define MINIMIZABLETOTRAY_H

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/settings_registry/traits/settings/main_window/MinimizeToTraySetting.h"

namespace Draupnir::Ui::MainWindow
{

/*! @struct MinimizableToTray draupnir/ui_bricks/traits/features/MinimizableToTray.h
 *  @ingroup UiBricks
 *  @brief Feature that enables "minimize to tray" behavior for @ref Draupnir::Ui::MainWindowTemplate.
 *  @details This feature is a simple, stateless marker that:
 *           - contributes @ref Draupnir::Settings::MainWindow::MinimizeToTraySetting to the aggregated `SettingsBundle` of @ref
 *             Draupnir::Ui::MainWindowTemplate;
 *           - indicates to @ref Draupnir::Ui::MainWindowTemplate that the window may be minimized to the system tray instead of using the
 *             default minimize behavior.
 *
 *           The actual behavior (when to hide the window, how to toggle it back, etc.) is implemented in @ref Draupnir::Ui::MainWindowTemplate
 *           itself, which consults both:
 *           - presence of this feature type in the `Features...` list;
 *           - the runtime value of `MinimizeToTraySetting` stored inside the shared @ref Draupnir::Ui::MainWindowTemplate::SettingsBundle.
 *
 *           Example usage:
 *           @code
 *           using MainWindowBase = Draupnir::Ui::MainWindowTemplate<
 *               Draupnir::Ui::MainWindow::MinimizableToTray,
 *               Draupnir::Ui::MainWindow::TrayIconSupported<Draupnir::Ui::TrayIcon>,
 *               Draupnir::Ui::MainWindow::RememberWindowSize
 *           >;
 *           @endcode */

struct MinimizableToTray
{
public:
    /*! @brief Settings contributed by this feature. Adds @ref Draupnir::Settings::MainWindow::MinimizeToTraySetting to the main window’s
     *         @ref Draupnir::Ui::MainWindowTemplate::SettingsBundle, controlling whether "minimize to tray" behavior is enabled at runtime. */
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Settings::MainWindow::MinimizeToTraySetting
    >;
};

}; // namespace Draupnir::Ui::MainWindow

#endif // MINIMIZABLETOTRAY_H
