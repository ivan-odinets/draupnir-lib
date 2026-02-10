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

#ifndef MINIMIZABLEONCLOSE_H
#define MINIMIZABLEONCLOSE_H

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/settings_registry/traits/settings/main_window/MinimizeOnCloseSetting.h"

namespace Draupnir::Ui::MainWindow
{

/*! @struct MinimizableOnClose draupnir/ui_bricks/traits/features/MinimizableOnClose.h
 *  @ingroup UiBricks
 *  @brief Feature that turns the "close" action into "minimize" for @ref Draupnir::Ui::MainWindowTemplate.
 *  @details This feature is a stateless marker that:
 *           - contributes @ref Draupnir::Settings::MainWindow::MinimizeOnCloseSetting to the aggregated @ref SettingsBundle of @ref
 *             Draupnir::Ui::MainWindowTemplate;
 *           - signals to @ref Draupnir::Ui::MainWindowTemplate that, when this setting is enabled, attempts to close the window (e.g.
 *             clicking the window close button) should minimize the window instead of actually closing it.
 *
 *           The concrete behavior is implemented inside @ref MainWindowTemplate::closeEvent(), which checks both:
 *           - whether this feature type is present in the `Features...` list;
 *           - whether `MinimizeOnCloseSetting` is enabled in the settings bundle.
 *
 *           Example usage:
 *           @code
 *           using MainWindowBase = Draupnir::Ui::MainWindowTemplate<
 *               Draupnir::Ui::MainWindow::MinimizableOnClose,
 *               Draupnir::Ui::MainWindow::MinimizableToTray,
 *               Draupnir::Ui::MainWindow::TrayIconSupported<Draupnir::Ui::TrayIcon>
 *           >;
 *           @endcode */

struct MinimizableOnClose
{
    /*! @brief Settings contributed by this feature. Adds @ref Draupnir::Settings::MainWindow::MinimizeOnCloseSetting to the main
     *         window’s @ref SettingsBundle. When this setting is `true`, the window will be minimized instead of closed on a
     *         close request. */
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Settings::MainWindow::MinimizeOnCloseSetting
    >;
};


}; // Draupnir::Ui::MainWindow

#endif // MINIMIZABLEONCLOSE_H
