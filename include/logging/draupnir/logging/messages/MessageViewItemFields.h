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

#ifndef MESSAGEVIEWITEMFIELDS_H
#define MESSAGEVIEWITEMFIELDS_H

#include <QObject>

#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"
#include "draupnir/utils/advance_enum.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Logging
{

/*! @class MessageViewItemField draupnir/logging/messages/MessageViewItemFields.h
 *  @ingroup Logging
 *  @brief This holds flags for different fields of @ref Draupnir::Logging::MessageViewItem objects. */

class MessageViewItemField
{
public:
    MessageViewItemField() = delete;
    Q_DISABLE_COPY(MessageViewItemField);

    /*! @brief Bit flags describing which `MessageViewItem` fields should be shown in the UI. */
    enum Value : uint8_t {
        Brief      = 0b00000001,  /*!< @brief Brief summary of `Message` (@ref Draupnir::Logging::Message::brief). */
        What       = 0b00000010,  /*!< @brief Details what happened (@ref Draupnir::Logging::Message::what). */
        DateTime   = 0b00000100,  /*!< @brief DateTime of `Message` (@ref Draupnir::Logging::Message::dateTime). */
        Icon       = 0b00001000,  /*!< @brief Icon of `Message` (@ref Draupnir::Logging::Message::icon). */
    };

    /*! @brief Returns config token for @ref Brief field. */
    static QLatin1String briefConfigToken()      { return QLatin1String{"brief"}; }

    /*! @brief Returns config token for @ref What field. */
    static QLatin1String whatConfigToken()       { return QLatin1String{"what"}; }

    /*! @brief Returns config token for @ref DateTime field. */
    static QLatin1String datetimeConfigToken()   { return QLatin1String{"date-time"}; }

    /*! @brief Returns config token for @ref Icon field. */
    static QLatin1String iconConfigToken()       { return QLatin1String{"icon"}; }

    using Serializer = Draupnir::Settings::FlagSerializerTemplate<
        Value,
        Draupnir::Settings::FlagWrapperTemplate<Value::Brief,    [](){ return briefConfigToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::What,     [](){ return whatConfigToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::DateTime, [](){ return datetimeConfigToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Icon,     [](){ return iconConfigToken(); }>
    >;

    /*! @brief Parses a message view field from its config string representation.
     *  @param string Config token to parse.
     *  @return Parsed field value, or `std::nullopt` if the token is unknown.
     *  @details Recognized tokens are returned by `briefConfigToken()`, `whatConfigToken()`, `datetimeConfigToken()`, and
     *           `iconConfigToken()`. */
    static std::optional<MessageViewItemField::Value> fromConfigString(const QString& string) {
        return Serializer::fromConfigString(string);
    }

    /*! @brief Converts a message view field to its config string representation.
     *  @param value Field value to serialize.
     *  @return Config token corresponding to @p value. */
    static QString toConfigString(MessageViewItemField::Value value) {
        return Serializer::toConfigString(value);
    }

    /*! @brief Returns user-facing display string for @ref Brief field. */
    static QString briefDisplayName();

    /*! @brief Returns user-facing display string for @ref What field. */
    static QString whatDisplayName();

    /*! @brief Returns user-facing display string for @ref DateTime field. */
    static QString dateTimeDisplayName();

    /*! @brief Returns user-facing display string for @ref Icon field. */
    static QString iconDisplayName();

    /*! @brief Converts a message view field to a user-facing display string.
     *  @tparam value Field value to convert.
     *  @return Human-readable field name suitable for UI labels. */
    template<MessageViewItemField::Value value>
    static inline QString toDisplayName() {
        if constexpr (value == Brief)
            return briefDisplayName();
        else if constexpr (value == What)
            return whatDisplayName();
        else if constexpr (value == DateTime)
            return dateTimeDisplayName();
        else if constexpr (value == Icon)
            return iconDisplayName();
        else
            static_assert(false);
    };

    /*! @brief Converts a message view field to a user-facing display string.
     *  @param value Field value to convert.
     *  @return Human-readable field name suitable for UI labels. */
    static QString toDisplayName(Value value);
};

/*! @ingroup Logging
 *  @brief Increment operator for the MessageField enum. */

inline MessageViewItemField::Value& operator++(MessageViewItemField::Value& type)
{
    draupnir::utils::enum_values<
        MessageViewItemField::Value,
        MessageViewItemField::Value::Brief,
        MessageViewItemField::Value::What,
        MessageViewItemField::Value::DateTime,
        MessageViewItemField::Value::Icon
    >::advance(type);
    return type;
}

inline MessageViewItemField::Value operator++(MessageViewItemField::Value& type, int)
{
    auto old = type;
    ++type;
    return old;
}

/*! @class MessageViewItemFields draupnir/logging/messages/MessageViewItemFields.h
 *  @ingroup Logging
 *  @brief Flag mask describing which fields of a message view item should be displayed.
 *
 *  @details This type wraps @ref MessageViewItemField::Value flags and provides helpers for config serialization, deserialization,
 *           and UI metadata used by selector widgets.
 *
 *           The config representation supports two special values:
 *           - `"none"` for an empty field mask;
 *           - `"all"` for all supported message view item fields.
 *
 *           Otherwise, masks are serialized as comma-separated field tokens produced by @ref MessageViewItemField::toConfigString. */

class MessageViewItemFields final : public draupnir::utils::enum_flags<MessageViewItemField::Value>
{
    using _Base = draupnir::utils::enum_flags<MessageViewItemField::Value>;

public:
    /*! @brief Integer storage type used by the underlying flag mask. */
    using integer = _Base::integer;

    /*! @brief Enum type used as individual message view item flags. */
    using enum_type = _Base::enum_type;

    using _Base::enum_flags;
    using _Base::operator=;

    /*! @brief Constructs an empty field mask. */
    MessageViewItemFields() = default;

    /*! @brief Empty field mask. No message view item fields are displayed. */
    static constexpr _Base::integer None = 0;

    /*! @brief Mask containing all supported message view item fields. */
    static constexpr _Base::integer All =
        MessageViewItemField::Value::Brief |
        MessageViewItemField::Value::What |
        MessageViewItemField::Value::DateTime |
        MessageViewItemField::Value::Icon;

    /*! @brief Returns config token representing all supported fields. */
    static QLatin1String allConfigToken()        { return QLatin1String{"all"}; }

    using Serializer = Draupnir::Settings::FlagsMaskSerializerTemplate<
        MessageViewItemFields,
        MessageViewItemField::Serializer,
        Settings::NoneFlagsMaskTemplate<MessageViewItemFields>,
        Settings::FlagsMaskWrapperTemplate<[]() { return All; }, [](){ return allConfigToken();}>
    >;

    /*! @brief Parses message view item fields from config string representation.
     *  @param string Config value to parse.
     *  @return Parsed field mask, or `std::nullopt` if the string is invalid.
     *  @details Recognized formats are:
     *           - `noneConfigToken()` for an empty mask;
     *           - `allConfigToken()` for all supported fields;
     *           - comma-separated field tokens recognized by @ref MessageViewItemField::fromConfigString.
     *
     *           The special tokens `"none"` and `"all"` must appear alone.*/
    static std::optional<MessageViewItemFields> fromConfigString(const QString& string) {
        return Serializer::fromConfigString(string);
    }

    /*! @brief Converts a field mask to its config string representation.
     *  @param mask Field mask to serialize.
     *  @return Config string representing @p mask.
     *  @details Empty mask is serialized as `noneConfigToken()`. Full mask is serialized as `allConfigToken()`.
     *           Other masks are serialized as comma-separated field tokens. */
    static QString toConfigString(const MessageViewItemFields& mask) {
        return Serializer::toConfigString(mask);
    }

    /*! @brief Converts a field mask to a user-facing display name.
     *  @tparam fields Field mask to convert.
     *  @return Human-readable name suitable for UI display.
     * @note This helper assumes that @p fields represents a value supported by @ref @Draupnir::Logging::MessageViewItemField::toDisplayString.
     *       If arbitrary flag combinations are allowed, prefer formatting the mask as a list of individual field names. */
    template<MessageViewItemFields::integer mask>
    static QString toDisplayName() {
        static_assert((mask & ~All) == 0, "Unknown MessageViewItemFields bits");
        static_assert(mask != None, "Empty field mask has no single display name");
        static_assert((mask & (mask - 1)) == 0, "Field mask must contain exactly one flag");

        return MessageViewItemField::template toDisplayName<
            static_cast<MessageViewItemField::Value>(mask)>();
    }

    /*! @brief Converts a field mask to a user-facing display name.
     *  @param fields Field mask to convert.
     *  @return Human-readable name suitable for UI display.
     * @note This helper assumes that @p fields represents a value supported by @ref Draupnir::Logging::MessageViewItemField::toDisplayString.
     *       If arbitrary flag combinations are allowed, prefer formatting the mask as a list of individual field names. */
    static QString toDisplayName(MessageViewItemFields fields);

    /*! @struct UiSelectorMetadata
     *  @brief Metadata used by UI selectors that expose message view item fields. */
    struct UiSelectorMetadata {
        /*! @brief Flags displayed by UI selectors. */
        static constexpr enum_type displayedFlags[] = {
            MessageViewItemField::Value::Brief, MessageViewItemField::Value::What,
            MessageViewItemField::Value::DateTime, MessageViewItemField::Value::Icon
        };
    };
};

}; // namespace Draupnir::Logging

Q_DECLARE_METATYPE(Draupnir::Logging::MessageViewItemField::Value);
Q_DECLARE_METATYPE(Draupnir::Logging::MessageViewItemFields);

#endif // MESSAGEVIEWITEMFIELDS_H
