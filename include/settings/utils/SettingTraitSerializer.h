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

#ifndef SETTINGTRAITSERIALIZER_H
#define SETTINGTRAITSERIALIZER_H

#include "SettingTraitValidator.h"
#include "ValueSerializer.h"

namespace Draupnir::Settings
{

/*! @class SettingTraitSerializer draupnir-lib/incldue/settings/utils/SettingTraitSerializer.h
 *  @brief Type-safe bridge for serializing and deserializing a specific SettingTrait using a backend.
 *  @details This class defines the default serialization logic for settings represented by a specific `SettingTrait`. It
 *           delegates low-level reading/writing to `ValueSerializer`, and extracts metadata (key, type, default) from the
 *           trait.
 *
 *           ### Requirements for SettingTrait:
 *           The SettingTrait must define the following:
 *           - `using Value`                        — the C++ type of the value (e.g., bool, int, QString, enum, ...);
 *           - `static QString key()`              — the storage key used in the backend;
 *           - `static Value defaultValue()`       — the default value if none is stored.
 *
 *           ### Serialization Logic:
 *           - `get()` loads the value from the backend using the trait's key. If missing or invalid, returns the trait's
 *              default value.
 *           - `set()` stores the value into the backend under the trait's key.
 *
 *           ### Customization:
 *           For complex types or multi-key settings, specialize this template. A specialization must implement the same static
 *           interface:
 *           - `static Value get(Backend* settings);`
 *           - `static void set(Backend* settings, const Value& value);`
 *
 *  @tparam Backend       Type of the backend used for persistence (e.g., AppSettings, QSettings).
 *  @tparam SettingTrait  A trait that describes a setting, including its value type, key, and default value.
 *
 * @see ValueSerializer, SettingsRegistry, AbstractSetting, SettingTraitValidator */

template<class Backend, class SettingTrait>
class SettingTraitSerializer
{
public:
    using Value = typename SettingTrait::Value;

    /*! @brief Loads the setting value from the backend.
     *  @param settings Pointer to the backend (must not be nullptr).
     *  @return The stored value if present and valid, otherwise the trait's default. */
    inline static Value get(Backend* settings) {
        static_assert(SettingTraitValidator::has_valueType<SettingTrait>(),
                      "SettingTrait must provide `using Value = ...`.");
        static_assert(SettingTraitValidator::has_key<SettingTrait>(),
                      "SettingTrait must provide static method `key()`.");
        static_assert(SettingTraitValidator::has_defaultValue<SettingTrait>(),
                      "SettingTrait must provide static method `defaultValue()`.");

        Q_ASSERT_X(settings, "SettingTraitSerializer<SettingTrait>::get",
                   "Provided settings backend pointer is nullptr.");

        return ValueSerializer<Backend,Value>::get(settings,SettingTrait::key(),SettingTrait::defaultValue());
    }

    /*! @brief Stores the setting value into the backend.
     *  @param settings Pointer to the backend (must not be nullptr).
     *  @param value    The value to persist. */
    inline static void set(Backend* settings, const Value& value) {
        static_assert(SettingTraitValidator::has_valueType<SettingTrait>(),
                      "SettingTrait must provide `using Value = ...`.");
        static_assert(SettingTraitValidator::has_key<SettingTrait>(),
                      "SettingTrait must provide static method `key()`.");
        static_assert(SettingTraitValidator::has_defaultValue<SettingTrait>(),
                      "SettingTrait must provide static method `defaultValue()`.");

        Q_ASSERT_X(settings, "SettingTraitSerializer<SettingTrait>::set",
                   "Provided settings backend pointer is nullptr.");

        ValueSerializer<Backend,Value>::set(settings,SettingTrait::key(),value);
    }
};

}; // namespace Draupnir::Settings

#endif // SETTINGTRAITSERIALIZER_H
