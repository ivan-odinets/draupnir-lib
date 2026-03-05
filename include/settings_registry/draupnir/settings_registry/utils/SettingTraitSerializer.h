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

#ifndef SETTINGTRAITSERIALIZER_H
#define SETTINGTRAITSERIALIZER_H

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/settings_registry/concepts/SettingsBackendConcept.h"
#include "draupnir/settings_registry/utils/EnumFlagsSerializer.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Settings
{

/*! @class SettingTraitSerializer draupnir/settings_registry/utils/SettingTraitSerializer.h
 *  @ingroup SettingsRegistry
 *  @brief Type-safe bridge for serializing and deserializing a specific `SettingTrait` using a backend.
 *  @tparam Backend Backend type used for persistence (e.g., `AppSettings`, `QSettings` wrapper).
 *  @tparam SettingTrait Trait that describes a setting (at minimum provides `Value` and `defaultValue()`).
 *
 *  @details This template is intentionally forward-declared to allow custom specializations for non-primitive or multi-key
 *           settings.
 *
 *           ## Default implementation
 *           A default implementation is provided only when `SettingTrait` satisfies @ref PrimitiveSettingTraitConcept, i.e.
 *           it defines:
 *           - `using Value`
 *           - `static QString key()`
 *           - `static Value defaultValue()`
 *
 *           The default implementation:
 *           - `get()` reads value from backend by `SettingTrait::key()`. If missing or invalid, returns `defaultValue()`.
 *           - `set()` writes value to backend under `SettingTrait::key()`.
 *
 *           ## Enum-flags special handling
 *           If `draupnir::utils::enum_flags_concept<typename SettingTrait::Value>` is satisfied, the value is stored as a
 *           string using @ref EnumFlagsSerializer.
 *
 *           ## Customization
 *           For complex types (e.g. multi-key settings, custom validation, non-QVariant-storable values), provide a full
 *           specialization:
 *           - `static Value get(Backend* settings);`
 *           - `static void set(Backend* settings, const Value& value);`
 *
 *  @note Default implementation assumes that `Backend` provides `contains(key)`, `value(key)`, `setValue(key, QVariant)`. */

template<SettingsBackendConcept Backend, SettingTraitConcept SettingTrait>
class SettingTraitSerializer;

template<SettingsBackendConcept Backend, PrimitiveSettingTraitConcept SettingTrait>
    requires(!draupnir::utils::enum_flags_concept<typename SettingTrait::Value>)
class SettingTraitSerializer<Backend,SettingTrait>
{
public:
    using Value = typename SettingTrait::Value;

    /*! @brief Loads the setting value from the backend.
     *  @param settings Pointer to the backend (must not be nullptr).
     *  @return The stored value if present and valid, otherwise the trait's default. */
    inline static Value get(Backend* settings) {
        Q_ASSERT_X(settings, "SettingTraitSerializer<Backend,SettingTrait>::get",
                   "Provided settings pointer is nullptr.");

        if (!settings->contains(SettingTrait::key()))
            return SettingTrait::defaultValue();

        const QVariant value = settings->value(SettingTrait::key());
        if (value.canConvert<Value>())
            return value.value<Value>();
        else
            return SettingTrait::defaultValue();
    }

    /*! @brief Stores the setting value into the backend.
     *  @param settings Pointer to the backend (must not be nullptr).
     *  @param value The value to persist. */
    inline static void set(Backend* settings, const Value& value) {
        Q_ASSERT_X(settings, "SettingTraitSerializer<Backend,SettingTrait>::set",
                   "Provided settings pointer is nullptr.");

        settings->setValue(SettingTrait::key(),QVariant::fromValue(value));
    }
};

template<SettingsBackendConcept Backend, PrimitiveSettingTraitConcept SettingTrait>
    requires(draupnir::utils::enum_flags_concept<typename SettingTrait::Value>)
class SettingTraitSerializer<Backend, SettingTrait>
{
public:
    using Value = typename SettingTrait::Value;

    /*! @brief Loads the setting value from the backend.
     *  @param settings Pointer to the backend (must not be nullptr).
     *  @return The stored value if present and valid, otherwise the trait's default. */
    inline static Value get(Backend* settings) {
        Q_ASSERT_X(settings, "SettingTraitSerializer<Backend,SettingTrait>::get",
                   "Provided settings pointer is nullptr.");

        if (!settings->contains(SettingTrait::key()))
            return SettingTrait::defaultValue();

        const QString valueString = settings->value(SettingTrait::key()).toString();
        const std::optional<Value> maybeValue = EnumFlagsSerializer<Value>::fromConfigString(valueString);
        return maybeValue.value_or(SettingTrait::defaultValue());
    }

    /*! @brief Stores the setting value into the backend.
     *  @param settings Pointer to the backend (must not be nullptr).
     *  @param value The value to persist. */
    inline static void set(Backend* settings, const Value& value) {
        Q_ASSERT_X(settings, "SettingTraitSerializer<Backend,SettingTrait>::set",
                   "Provided settings pointer is nullptr.");

        settings->setValue(SettingTrait::key(), EnumFlagsSerializer<Value>::toConfigString(value));
    }
};

}; // namespace Draupnir::Settings

#endif // SETTINGTRAITSERIALIZER_H
