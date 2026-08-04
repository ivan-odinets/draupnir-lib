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

#ifndef VALUESERIALIZERTEMPLATE_H
#define VALUESERIALIZERTEMPLATE_H

#include <QVariant>

#include "draupnir/settings_registry/concepts/SettingsSerializationConcepts.h"
#include "draupnir/utils/concepts/flags_concepts.h"

namespace Draupnir::Settings
{

/*! @class ValueSerializerTemplate draupnir/settings_registry/utils/ValueSerializerTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Static template class used within the @ref Draupnir::Settings::SettingsTraitSerializer for value serialization
 *         to / from `QVariant`.
 *
 * @todo Documentation: Write reasonable documentation page when this class needs to be specialized. And write a manual how to
 *       do so.
 * @todo Question: Maybe there is a way how to reasonably restrict this template? (Convertability Value <=> QVariant). */

template<class Value>
class ValueSerializerTemplate
{
public:
    /*! @brief Deserializes a value from `QVariant`.
     *  @param value Variant containing the serialized value.
     *  @return The converted value, or `std::nullopt` when the variant cannot be converted to `Value`.
     *  @details The default implementation uses `QVariant::canConvert()` and `QVariant::value()` to perform the conversion.  */
    [[nodiscard]] static std::optional<Value> fromQVariant(const QVariant& value) {
        return (value.canConvert<Value>()) ?
                   std::optional<Value>{value.value<Value>()} :
                   std::nullopt;
    }
    /*! @brief Serializes a value to `QVariant`.
     *  @param value Value to serialize.
     *  @return Variant containing the supplied value.
     *  @details The default implementation delegates to `QVariant::fromValue()`. */
    [[nodiscard]] static QVariant toQVariant(const Value& value) {
        return QVariant::fromValue(value);
    }

};

/*! @class ValueSerializerTemplate draupnir/settings_registry/utils/ValueSerializerTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Serializes enum-flags values as binary strings stored in `QVariant`.
 *  @tparam EnumFlags An enum-flags type satisfying `draupnir::utils::enum_flags_like_concept`.
 *
 *  @details This specialization is used for enum-flags types that do not provide custom configuration serialization.
 *
 *           The flags mask is converted to its underlying integer representation and stored as a base-2 `QString`. During
 *           deserialization, the string is parsed back into the underlying integer type and used to construct the enum-flags
 *           value.
 *
 *           A value that cannot be converted to `QString`, or a string that cannot be parsed as a valid base-2 integer,
 *           results in `std::nullopt`.
 *
 * @note This specialization preserves the numeric flags mask rather than symbolic configuration names.
 *
 * @todo Feature: Improve handling of the cases when enum_flags contains sth very very wrong.
 * @todo Cleanup: Move interval _Helper class to some external util. Maybe in Utils module?
 * @todo Tests: Write tests for this class. */

template<draupnir::utils::enum_flags_like_concept EnumFlags>
class ValueSerializerTemplate<EnumFlags>
{
    template<typename Integer, class Unused = void>
    struct _Helper {};

    template<class Unused>
    struct _Helper<short, Unused> {
        static short fromString(const QString& string, bool* ok, int base)
        { return string.toShort(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned short, Unused> {
        static unsigned short fromString(const QString& string, bool* ok, int base)
        { return string.toUShort(ok,base); }
    };

    template<class Unused>
    struct _Helper<int, Unused> {
        static int fromString(const QString& string, bool* ok, int base)
        { return string.toInt(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned int, Unused> {
        static unsigned int fromString(const QString& string, bool* ok, int base)
        { return string.toUInt(ok,base); }
    };

    template<class Unused>
    struct _Helper<long, Unused> {
        static long fromString(const QString& string, bool* ok, int base)
        { return string.toLong(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned long, Unused> {
        static unsigned long fromString(const QString& string, bool* ok, int base)
        { return string.toULong(ok,base); }
    };

    template<class Unused>
    struct _Helper<long long, Unused> {
        static long long fromString(const QString& string, bool* ok, int base)
        { return string.toLongLong(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned long long, Unused> {
        static long long fromString(const QString& string, bool* ok, int base)
        { return string.toULongLong(ok,base); }
    };

public:
    /*! @brief Enum type represented by the serialized flags value. */
    using Enum = typename EnumFlags::enum_type;

    /*! @brief Deserializes an enum-flags value from a binary string stored in `QVariant`.
     *  @param value Variant containing the serialized binary flags mask.
     *  @return The deserialized enum-flags value, or `std::nullopt` when the variant cannot be converted to `QString` or the string
     *          cannot be parsed as a valid binary integer.
     *  @details The variant must be convertible to `QString`. The resulting string is interpreted as a base-2 integer using the
     *           underlying type of @ref Enum. */
    [[nodiscard]] static std::optional<EnumFlags> fromQVariant(const QVariant& value) {
        if (!value.canConvert<QString>())
            return std::nullopt;

        bool ok = false;
        const std::underlying_type_t<Enum> preparsedValue = _Helper<std::underlying_type_t<Enum>>::fromString(value.toString(),&ok,2);
        if (!ok)
            return std::nullopt;

        return (ok) ?
            std::optional<EnumFlags>{EnumFlags{preparsedValue}} :
            std::nullopt;
    }

    /*! @brief Serializes an enum-flags value as a binary string stored in `QVariant`.
     *  @param value Enum-flags value to serialize.
     *  @return Variant containing the numeric flags mask represented as a base-2 `QString`. */
    [[nodiscard]] static QVariant toQVariant(const EnumFlags& value) {
        return QVariant{QString::number(value.value(),2)};
    }
};

/*! @class ValueSerializerTemplate draupnir/settings_registry/utils/ValueSerializerTemplate.h
 *  @ingroup SettingsRegistry
 *  @tparam EnumFlags Enum-flags type providing custom configuration serialization.
 *  @brief Serializes enum-flags values using their custom configuration-string API.
 *
 *  @details This specialization is selected for enum-flags types satisfying both `draupnir::utils::enum_flags_like_concept` and
 *           `HasCustomEnumFlagsConfigSerialization`.
 *
 *           Serialization is delegated to `EnumFlags::toConfigString()`, while deserialization is delegated to
 *           `EnumFlags::fromConfigString()`.
 *
 * @note The exact string format and validation rules are defined by `EnumFlags`. */

template<draupnir::utils::enum_flags_like_concept EnumFlags>
    requires HasCustomEnumFlagsConfigSerialization<EnumFlags>
class ValueSerializerTemplate<EnumFlags>
{
public:
    /*! @brief Enum type represented by the serialized flags value. */
    using Enum = typename EnumFlags::enum_type;

    /*! @brief Deserializes an enum-flags value using its custom configuration format.
     *  @param value Variant containing the configuration string.
     *  @return The result of `EnumFlags::fromConfigString()`, or `std::nullopt` when the variant cannot be converted to `QString` or
     *          the configuration string is invalid. */
    [[nodiscard]] static std::optional<EnumFlags> fromQVariant(const QVariant& value) {
        if (!value.canConvert<QString>())
            return std::nullopt;
        return EnumFlags::fromConfigString(value.toString());
    }

    /*! @brief Serializes an enum-flags value using its custom configuration format.
     *  @param value Enum-flags value to serialize.
     *  @return Variant containing the result of `EnumFlags::toConfigString()`. */
    [[nodiscard]] static QVariant toQVariant(const EnumFlags& value) {
        return EnumFlags::toConfigString(value);
    }
};

} // namespace Draupnir::Settings

#endif // VALUESERIALIZERTEMPLATE_H
