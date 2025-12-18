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

#ifndef UIBRICKS_H
#define UIBRICKS_H

/*! @file draupnir/UiBricks.h"
 *  @ingroup UiBricks
 *  @brief This file is a general include for all entities from the @ref UiBricks module. */

#include "draupnir/ui_bricks/ui/TrayIcon.h"                // IWYU pragma: keep

// Animations
#include "draupnir/ui_bricks/animations/AnimationBuilder.h"          // IWYU pragma: keep
#include "draupnir/ui_bricks/animations/FadeOut.h"                   // IWYU pragma: keep
#include "draupnir/ui_bricks/animations/ParralelAnimationTemplate.h" // IWYU pragma: keep
#include "draupnir/ui_bricks/animations/ShrinkToCenter.h"            // IWYU pragma: keep

// Core
#include "draupnir/ui_bricks/core/ViewManager.h"           // IWYU pragma: keep

// Traits - Features
#include "draupnir/ui_bricks/traits/features/AnimatedMinimizableToTray.h" // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/ExtraSettings.h"             // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/MinimizableToTray.h"         // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/MinimizableOnClose.h"        // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/TrayIconSupported.h"         // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/RememberWindowSize.h"        // IWYU pragma: keep

// Traits - Menu Entries
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"                 // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"                 // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h"             // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SectionEntry.h"         // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"       // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h"     // IWYU pragma: keep

// Ui - Menus
#include "draupnir/ui_bricks/ui/menus/MenuBarTemplate.h"   // IWYU pragma: keep
#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"      // IWYU pragma: keep
#include "draupnir/ui_bricks/ui/menus/RecentFilesMenu.h"   // IWYU pragma: keep

// Ui - Widgets
#include "draupnir/ui_bricks/ui/widgets/AnimationOverlayWidget.h"         // IWYU pragma: keep
#include "draupnir/ui_bricks/ui/widgets/FixedCentralTabWidgetTemplate.h"  // IWYU pragma: keep
#include "draupnir/ui_bricks/ui/widgets/FixedTabWidget.h"                 // IWYU pragma: keep
#include "draupnir/ui_bricks/ui/widgets/FixedTabWidgetTemplate.h"         // IWYU pragma: keep

// Ui - Windows
#include "draupnir/ui_bricks/ui/windows/MainWindowTemplate.h"   // IWYU pragma: keep

#endif // UIBRICKS_H
