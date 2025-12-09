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

#ifndef SETTINGSREGISTRY_H
#define SETTINGSREGISTRY_H

/*! @file draupnir/SettingsRegistry.h
 *  @ingroup SettingsRegistry
 *  @brief This file is a general include to use the @ref SettingsRegistry module. */


// Core things
#include "draupnir/settings_registry/SettingsRegistryTemplate.h"  // IWYU pragma: keep
#include "draupnir/settings_registry/SettingsBundleTemplate.h"    // IWYU pragma: keep

// To work with SettingTraits
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h" // IWYU pragma: keep

// Default traits provided:
// Uncategorized
#include "draupnir/settings_registry/traits/settings/CentralWidgetIndexSetting.h"             // IWYU pragma: keep

// Files
#include "draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h"        // IWYU pragma: keep
#include "draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h"          // IWYU pragma: keep

// MainWindow
#include "draupnir/settings_registry/traits/settings/main_window/MinimizeOnCloseSetting.h"    // IWYU pragma: keep
#include "draupnir/settings_registry/traits/settings/main_window/MinimizeToTraySetting.h"     // IWYU pragma: keep
#include "draupnir/settings_registry/traits/settings/main_window/StartHiddenSetting.h"        // IWYU pragma: keep
#include "draupnir/settings_registry/traits/settings/main_window/WindowSizeSetting.h"         // IWYU pragma: keep

// Templates
#include "draupnir/settings_registry/traits/settings/templates/SizeSettingTraitTemplate.h"    // IWYU pragma: keep
#include "draupnir/settings_registry/traits/settings/templates/SettingTraitTemplate.h"        // IWYU pragma: keep

#endif // SETTINGSREGISTRY_H
