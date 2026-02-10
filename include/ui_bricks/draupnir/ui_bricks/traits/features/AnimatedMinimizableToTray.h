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

#ifndef ANIMATEDMINIMIZABLETOTRAY_H
#define ANIMATEDMINIMIZABLETOTRAY_H

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/settings_registry/traits/settings/main_window/MinimizeToTraySetting.h"

namespace Draupnir::Ui::MainWindow
{

/*! @struct AnimatedMinimizableToTray draupnir/ui_bricks/traits/features/AnimatedMinimizableToTray.h
 *  @ingroup UiBricks
 *  @brief Feature that enables "minimize to tray" behavior with a custom animation.
 *  @tparam AnimationTemplate     Animation type that will be used when the window is minimized to tray. The 'AnimationTemplate'
 *                                is expected to conform to the same contract as other animation templates in the library, e.g.:
 *                                @code
 *                                struct SomeAnimation {
 *                                    static QAbstractAnimation* create(QWidget* target);
 *                                };
 *                                @endcode
 *
 *  @details This feature is a small, stateless "tag" that:
 *           - contributes @ref Draupnir::Settings::MainWindow::MinimizeToTraySetting to the aggregated `SettingsBundle` of @ref
 *             Draupnir::Ui::MainWindowTemplate;
 *           - exposes a nested @ref AnimatedMinimizableToTray::Animation alias, which is used by @ref Draupnir::Ui::MainWindowTemplate
 *             to obtain the animation type and pass it into @ref Draupnir::Ui::AnimationBuilder.
 *
 *           It does not store any runtime state (no `hasState_v`), so it will not be instantiated inside the window’s internal
 *           `FeaturesStateTuple`.
 *
 *           Example usage:
 *           @code
 *           using MainWindowBase = Draupnir::Ui::MainWindowTemplate<
 *               Draupnir::Ui::MainWindow::MinimizableOnClose,
 *               Draupnir::Ui::MainWindow::AnimatedMinimizableToTray<Draupnir::Ui::Animations::ShrinkToCenter>,
 *               Draupnir::Ui::MainWindow::TrayIconSupported<Draupnir::Ui::TrayIcon>,
 *               Draupnir::Ui::MainWindow::RememberWindowSize
 *           >;
 *           @endcode */

template<class AnimationTemplate>
struct AnimatedMinimizableToTray
{
    /*! @brief Settings bundle contributed by this feature. Adds @ref Draupnir::Settings::MainWindow::MinimizeToTraySetting, which
     *         controls whether the window should be minimized to tray instead of being minimized/hidden in the usual way. */
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Settings::MainWindow::MinimizeToTraySetting
    >;

    /*! @brief Animation type used by @ref Draupnir::Ui::MainWindowTemplate when minimizing to tray. */
    using Animation = AnimationTemplate;
};

}; // namespace Draiupnir::Ui::MainWindow

#endif // ANIMATEDMINIMIZABLETOTRAY_H
