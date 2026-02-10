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

#ifndef REMEMBERWINDOWSIZE_H
#define REMEMBERWINDOWSIZE_H

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/ui_bricks/traits/settings/main_window/WindowSizeSetting.h"

namespace Draupnir::Ui::MainWindow
{

/*! @struct RememberWindowSize draupnir/ui_bricks/traits/features/RememberWindowSize.h
 *  @ingroup UiBricks
 *  @brief Feature that persists and restores the main window size.
 *  @details This feature is a simple, stateless marker that:
 *           - contributes @ref Draupnir::Settings::MainWindow::WindowSizeSetting to the aggregated `SettingsBundle` of @ref
 *             Draupnir::Ui::MainWindowTemplate;
 *           - allows @ref Draupnir::Ui::MainWindowTemplate to:
 *             - restore the window size from settings in @ref Draupnir::Ui::MainWindowTemplate::loadSettings();
 *             - save the current window size back to settings in the destructor.
 *
 *           The actual logic of reading/writing the setting is implemented inside @ref Draupnir::Ui::MainWindowTemplate.
 *           This feature only declares which setting it uses via its @ref RememberWindowSize::SettingsBundle alias.
 *
 *           Example usage:
 *           @code
 *           using MainWindowBase = Draupnir::Ui::MainWindowTemplate<
 *               Draupnir::Ui::MainWindow::RememberWindowSize,
 *               Draupnir::Ui::MainWindow::MinimizableOnClose,
 *               Draupnir::Ui::MainWindow::MinimizableToTray
 *           >;
 *           @endcode */

struct RememberWindowSize
{
    /*! @brief Settings contributed by this feature. Adds @ref Draupnir::Settings::MainWindow::WindowSizeSetting to the main
     *         window’s `SettingsBundle`, allowing the window size to be saved and restored across application runs. */
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Settings::MainWindow::WindowSizeSetting
    >;
};

}; // namespace Draupnir::Ui::MainWindow

#endif // REMEMBERWINDOWSIZE_H
