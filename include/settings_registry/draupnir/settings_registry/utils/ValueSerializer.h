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

#ifndef VALUESERIALIZER_H
#define VALUESERIALIZER_H

#include <QVariant>

namespace Draupnir::Settings
{

/*! @class ValueSerializer draupnir/setting_resgistry/utils/ValueSerializer.h
 *  @ingroup SettingsRegistry
 *  @brief Provides type-safe serialization and deserialization of values using a specified backend.
 *  @tparam Backend Type of the backend providing key-value storage interface (e.g., AppSettings, QSettings).
 *  @tparam Value   Type of the value being stored and retrieved (e.g., int, QString, custom enum...).
 *
 *  @details This class provides static `get()` and `set()` methods for reading/writing a value to a settings backend using
 *           a string key. It performs runtime type conversion via `QVariant` and supports a fallback to the default value
 *           if the key is not present or conversion fails.
 *
 *           This template may be specialized if the Value needs to be stored under one key, but QVarient convertion mechanism
 *           is not good. For example when storing enums as their string representation.
 *
 *           ### Usage example:
 *           @code
 *           QString path = ValueSerializer<QSettings, QString>::get(settings, "app/logPath", "default.txt");
 *           ValueSerializer<QSettings, QString>::set(settings, "app/logPath", path);
 *           @endcode
 *
 * @note The backend is expected to provide:
 *       - bool contains(const QString&) const;
 *       - QVariant value(const QString&) const;
 *       - void setValue(const QString&, const QVariant&);
 *
 * @todo Add test for this class. */

template<class Backend, class Value>
class ValueSerializer
{
public:
    /*! @brief Retrieves a value from the backend by key, or returns a default if not found or conversion fails.
     *  @param backend Pointer to the settings backend.
     *  @param key     Storage key used to retrieve the value.
     *  @param defaultValue Fallback value if key is missing or conversion fails.
     *  @return Retrieved value or fallback default. */
    static inline Value get(Backend* backend, const QString& key, const Value& defaultValue) {
        Q_ASSERT_X(backend, "ValueSerializer<Backend,Value>::get",
                   "Provided Backend* pointer is nullptr");

        if (!backend->contains(key))
            return defaultValue;

        const QVariant value = backend->value(key);
        return value.canConvert<Value>() ?
            value.value<Value>() :
            defaultValue;
    }

    /*! @brief Persists a value to the backend under the given key.
     *  @param backend Pointer to the settings backend.
     *  @param key     Storage key to write the value under.
     *  @param value   Value to be stored. */
    static inline void set(Backend* backend, const QString& key, const Value& value) {
        Q_ASSERT_X(backend, "ValueSerializer<Backend,Value>::set",
                   "Provided Backend* pointer is nullptr");

        backend->setValue(key,QVariant::fromValue(value));
    }

};

};

#endif // VALUESERIALIZER_H
