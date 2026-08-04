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

#ifndef SETTINGSBUNDLECONCEPT_H
#define SETTINGSBUNDLECONCEPT_H

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/utils/template_detectors.h"

namespace Draupnir::Settings
{

template<SettingTraitConcept... Traits>
class SettingsBundleTemplate;

/*! @concept HasNestedSettingsBundle
 *  @ingroup SettingsRegistry
 *  @brief Checks whether a type exposes a nested settings-bundle type.
 *  @tparam Candidate Type to inspect.
 *  @details The concept is satisfied when `Candidate` declares a nested `SettingsBundle` type that is an instantiation of @ref
 *           Draupnir::Settings::SettingsBundleTemplate. It is intended for components that publicly describe their settings
 *           requirements through a nested type alias: `using SettingsBundle = SettingsBundleTemplate<SomeSettingTrait>;` */

template<class Candidate>
concept HasNestedSettingsBundle =
    requires { typename Candidate::SettingsBundle; } &&
    draupnir::utils::is_instantiation_of_v<
        typename Candidate::SettingsBundle,
        Draupnir::Settings::SettingsBundleTemplate
    >;

/*! @concept SettingsBundleConcept draupnir/settings_registry/SettingsBundleConcept.h
 *  @ingroup SettingsRegistry
 *  @brief Checks whether a type is a settings-bundle specialization.
 *  @tparam Candidate Type to inspect.
 *  @details The concept is satisfied when `Candidate` is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate. */

template<class Candidate>
concept SettingsBundleConcept =
    draupnir::utils::is_instantiation_of_v<
        Candidate,
        Draupnir::Settings::SettingsBundleTemplate
    >;

}; // namespace Draupnir::Settings

#endif // SETTINGSBUNDLECONCEPT_H
