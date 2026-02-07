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
 *  @brief This concept requires type Candidate to have tested Value type. */

template<class Candidate>
concept HasValueType = requires { typename Candidate::Value; };

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type Candidate to have public static method `QString Candidate::key()`*/

template<class Candidate>
concept HasKeyMethod = requires {
    { Candidate::key() } -> std::convertible_to<QString>;
};

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type `Candidate` to have public static method `typename Candidate::Value Candidate::defaultValue()`*/

template<class Candidate>
concept HasDefaultValueMethod = requires {
    { Candidate::defaultValue() } -> std::convertible_to<typename Candidate::Value>;
};

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type `Candidate` to have public static method `typename Candidate::Value Candidate::minimalValue()`*/

template<class Candidate>
concept HasMinimalValue = requires {
    { Candidate::minimalValue() } -> std::same_as<typename Candidate::Value>;
};

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type `Candidate` to have public static method `typename Candidate::Value Candidate::maximalValue()`*/

template<class Candidate>
concept HasMaximalValue = requires {
    { Candidate::maximalValue() } -> std::same_as<typename Candidate::Value>;
};

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept requires type `Candidate` to have public static method `QString Candidate::settingDescription()` */

template<class Candidate>
concept HasSettingDescription = requires {
    { Candidate::settingDescription() } -> std::same_as<QString>;
};

}; // namespace SettingTrait

/*! @headerfile draupnir/settings_registry/concepts/SettingTraitConcept.h
 *  @ingroup SettingsRegistry
 *  @brief This concept is combination of three other concepts: @ref Draupnir::Settings::SettingTrait::HasValueType,
 *         @ref Draupnir::Settings::SettingTrait::HasKeyMethod, @ref Draupnir::Settings::SettingTrait::HasDefaultValueMethod.
 *         Every type to be used as SettingTrait - must fulfill these requirements. */

template<class Candidate>
concept SettingTraitConcept =
    SettingTrait::HasValueType<Candidate> &&
    SettingTrait::HasKeyMethod<Candidate> &&
    SettingTrait::HasDefaultValueMethod<Candidate>;

}; // namespace Draupnir::Settings

#endif // SETTINGTRAITCONCEPT_H
