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

    enum Value : uint8_t {
        Brief      = 0b00000001,  /*!< @brief Brief summary of `Message` (@ref Draupnir::Logging::Message::brief). */
        What       = 0b00000010,  /*!< @brief Details what happened (@ref Draupnir::Logging::Message::what). */
        DateTime   = 0b00000100,  /*!< @brief DateTime of `Message` (@ref Draupnir::Logging::Message::dateTime). */
        Icon       = 0b00001000,  /*!< @brief Icon of `Message` (@ref Draupnir::Logging::Message::icon). */
    };

    static constexpr Value values[] = { Brief, What, DateTime, Icon };

    static QLatin1String briefConfigKey()    { return QLatin1String{"brief"}; }
    static QLatin1String whatConfigKey()     { return QLatin1String{"what"}; }
    static QLatin1String datetimeConfigKey() { return QLatin1String{"date-time"}; }
    static QLatin1String iconConfigKey()     { return QLatin1String{"icon"}; }

    /*! @brief This is a static function converting Fields value to user-friendly string. */
    static QString toDisplayString(Value value) {
        switch (value) {
        case Logging::MessageViewItemField::Brief:
            return QObject::tr("Brief");
        case Logging::MessageViewItemField::What:
            return QObject::tr("What");
        case Logging::MessageViewItemField::DateTime:
            return QObject::tr("Timestamp:");
        case Logging::MessageViewItemField::Icon:
            return QObject::tr("Icon");
        }
        Q_ASSERT(false);
        return QString{};
    }

    static std::optional<MessageViewItemField::Value> fromConfigString(const QString& string) {
        if (string == briefConfigKey())
            return Brief;
        if (string == whatConfigKey())
            return What;
        if (string == datetimeConfigKey())
            return DateTime;
        if (string == iconConfigKey())
            return Icon;
        return std::nullopt;
    }

    static QString toConfigString(MessageViewItemField::Value value) {
        switch (value) {
        case Brief:
            return briefConfigKey();
        case What:
            return whatConfigKey();
        case DateTime:
            return datetimeConfigKey();
        case Icon:
            return iconConfigKey();
        };
        Q_ASSERT(false);
        Q_UNREACHABLE();
        return QString{};
    }
};

/*! @ingroup Logging
 *  @brief Increment operator for the MessageField enum. */

inline MessageViewItemField::Value& operator++(MessageViewItemField::Value& type,int)
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

/*! @ingroup Logging */

class MessageViewItemFields final : public draupnir::utils::enum_flags<MessageViewItemField::Value>
{
    using _Base = draupnir::utils::enum_flags<MessageViewItemField::Value>;

public:
    using _Base::enum_flags;

    using _Base::operator=;

    static constexpr _Base::integer None = 0;

    static constexpr _Base::integer All =
        MessageViewItemField::Value::Brief |
        MessageViewItemField::Value::What |
        MessageViewItemField::Value::DateTime |
        MessageViewItemField::Value::Icon;

    static std::optional<MessageViewItemFields> fromConfigString(const QString& string) {
        if (string.isEmpty())
            return std::nullopt;

        const QStringList parts = string.split(',');
        const bool hasNone = parts.contains(noneConfigKey());
        const bool hasAll  = parts.contains(allConfigKey());

        if ((hasNone || hasAll) && parts.size() != 1)
            return std::nullopt;

        if (hasNone)
            return None;
        if (hasAll)
            return All;

        MessageViewItemFields result{None};
        for (const QString& part : parts) {
            auto maybeField = MessageViewItemField::fromConfigString(part);
            if (!maybeField)
                return std::nullopt;
            result.set_flag(maybeField.value(), true);
        }
        return std::optional<MessageViewItemFields>{result};
    }

    static QString toConfigString(const MessageViewItemFields& mask) {
        if (mask == None)
            return noneConfigKey();
        if (mask == All)
            return allConfigKey();

        QStringList resultingParts;
        for (auto field : MessageViewItemField::values) {
            if (mask.test_flag(field))
                resultingParts.append(MessageViewItemField::toConfigString(field));
        }

        return resultingParts.join(',');
    }

    static QLatin1String noneConfigKey()     { return QLatin1String{"none"}; }
    static QLatin1String allConfigKey()      { return QLatin1String{"all"}; }

private:
};

}; // namespace Draupnir::Logging

Q_DECLARE_METATYPE(Draupnir::Logging::MessageViewItemField::Value);
Q_DECLARE_METATYPE(Draupnir::Logging::MessageViewItemFields);

#endif // MESSAGEVIEWITEMFIELDS_H
