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

#ifndef SETTINGTRAITCONCEPT_H
#define SETTINGTRAITCONCEPT_H

#include <QString>

namespace Draupnir::Settings
{

/*! @namespace SettingTrait
 *  @ingroup SettingsRegistry
 *  @brief Namespace containing concepts of individual elements of the setting traits objects. */

namespace SettingTrait
{

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type C to have tested Value type. */

template<class C>
concept HasValueType = requires { typename C::Value; };

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type C to have public static method `QString C::key()`*/

template<class C>
concept HasKeyMethod = requires {
    { C::key() } -> std::convertible_to<QString>;
};

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type C to have public static method `typename C::Value C::defaultValue()`*/

template<class C>
concept HasDefaultValueMethod = requires {
    { C::defaultValue() } -> std::convertible_to<typename C::Value>;
};

}; // namespace SettingTrait

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept is combination of three other concepts: @ref Draupnir::Settings::SettingTrait::HasValueType,
 *         @ref Draupnir::Settings::SettingTrait::HasKeyMethod, @ref Draupnir::Settings::SettingTrait::HasDefaultValueMethod.
 *         Every type to be used as SettingTrait - must fulfill these requirements. */

template<class C>
concept SettingTraitConcept =
    SettingTrait::HasValueType<C> &&
    SettingTrait::HasKeyMethod<C> &&
    SettingTrait::HasDefaultValueMethod<C>;

}; // namespace Draupnir::Settings

#endif // SETTINGTRAITCONCEPT_H
