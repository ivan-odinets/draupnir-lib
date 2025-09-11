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

#ifndef SETTINGTEMPLATE_H
#define SETTINGTEMPLATE_H

#include "SettingTraitValidator.h"

namespace Draupnir::Settings {

/*! @class SettingTemplate draupnir-lib/incldue/settings/SettingTemplate.h
 *  @brief Lightweight wrapper for a single setting trait.
 *  @details SettingTemplate<SettingTrait> provides storage and basic accessors for a setting defined by SettingTrait. It is
 *           not responsible for persistence itself — loading/saving is handled by SettingsRegistry in combination with
 *           SettingsSerializer.
 *
 *           For SettingTemplate template each SettingTrait must provide:
 *           - using Value — the underlying C++ type of the setting (e.g. bool, int, QString);
 *           - static Value defaultValue() — the default value if no entry is found in storage.
 *
 *           The wrapper stores the actual value as a public data member.
 *
 *  @tparam SettingTrait The trait type describing this setting.
 *
 *  @see SettingsRegistry, SettingsSerializer */

template<class SettingTrait>
class SettingTemplate
{
public:
    using Trait = SettingTrait;
    using Value = typename SettingTrait::Value;

    /*! @brief Constructs the setting with its default value. */
    SettingTemplate() :
        value{SettingTrait::defaultValue()}
    {
        static_assert(SettingTraitValidator::has_valueType<SettingTrait>(),
                "To be used as SettingTemplate argument SettingTrait must expose stored Value type.");
        static_assert(SettingTraitValidator::has_defaultValue<SettingTrait>(),
                "To be used as Setting template argument SettingTrait must provide static Value defaultValue() method.");
    }

    /*! @brief Current in-memory value of the setting.
     * @note Initialized to the trait’s defaultValue(). */
    Value value;
};

};

#endif // SETTINGTEMPLATE_H
