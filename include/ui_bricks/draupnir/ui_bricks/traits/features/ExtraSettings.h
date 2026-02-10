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

#ifndef EXTRASETTINGS_H
#define EXTRASETTINGS_H

#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"

namespace Draupnir::Ui::MainWindow
{

/*! @struct ExtraSettings include/draupnir/traits/features/ExtraSettings.h
 *  @ingroup UiBricks
 *  @brief Feature that contributes arbitrary user-defined settings to MainWindowTemplate.
 *  @tparam Things...   List of types settings from which should be included in the @ref Draupnir::Ui::MainWindowTemplate::SettingsBundle
 *                      This can be: simple setting traits, @ref Draunir::Settings::SettingsBundleTemplate instantiations or any
 *                      custom type which exposes `SettingsBundle` alias.
 *
 *  @details This feature does not define any runtime behavior and does not store state. It only contributes additional setting
 *           traits into the aggregated @ref Draupnir::Ui::MainWindowTemplate::SettingsBundle via the usual `SettingsTraitsConcatenator`.
 *
 *           Typical use case:
 *           - you want the main window to know about some custom settings (e.g. @c MyApp::Settings::SidebarWidthSetting),
 *           - but you do not need a full-blown "feature" with its own logic.
 *
 *           Example:
 *           @code
 *           using MyMainWindowBase = Draupnir::Ui::MainWindowTemplate<
 *               Draupnir::Ui::MainWindow::RememberWindowSize,
 *               Draupnir::Ui::MainWindow::MinimizableOnClose,
 *               Draupnir::Ui::MainWindow::ExtraSettings<
 *                   MyApp::Settings::SidebarWidthSetting,
 *                   MyApp::Settings::LastOpenedPageSetting
 *               >
 *           >;
 *           @endcode */

template<class... Things>
struct ExtraSettings
{
    using SettingsBundle = typename Draupnir::Settings::SettingsTraitsConcatenator<Things...>::toSettingsBundle;
};

} // namespace Draupnir::Ui::MainWindow

#endif // EXTRASETTINGS_H
