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

#ifndef DRAUPNIR_SETTINGSREGISTRY_H
#define DRAUPNIR_SETTINGSREGISTRY_H

/*! @file draupnir/SettingsRegistry.h
 *  @ingroup SettingsRegistry
 *  @brief Umbrella-header for the @ref SettingsRegistry module. */

// Concepts
#include "draupnir/settings_registry/concepts/SettingsBackendConcept.h"        // IWYU pragma: keep
#include "draupnir/settings_registry/concepts/SettingsBundleConcept.h"         // IWYU pragma: keep
#include "draupnir/settings_registry/concepts/SettingsSerializationConcepts.h" // IWYU pragma: keep
#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"           // IWYU pragma: keep

// Core
#include "draupnir/settings_registry/core/SettingsRegistryTemplate.h"  // IWYU pragma: keep
#include "draupnir/settings_registry/core/SettingsBundleTemplate.h"    // IWYU pragma: keep

// To work with SettingTraits
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h" // IWYU pragma: keep

// Utils
#include "draupnir/settings_registry/utils/FlagSerializerTemplate.h"      // IWYU pragma: keep
#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h" // IWYU pragma: keep
#include "draupnir/settings_registry/utils/OptionalSettingsBundle.h"      // IWYU pragma: keep
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"  // IWYU pragma: keep
#include "draupnir/settings_registry/utils/SettingTraitPrinter.h"         // IWYU pragma: keep
#include "draupnir/settings_registry/utils/SettingTraitSerializer.h"      // IWYU pragma: keep
#include "draupnir/settings_registry/utils/ValueSerializerTemplate.h"     // IWYU pragma: keep


#endif // DRAUPNIR_SETTINGSREGISTRY_H
