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

#ifndef MESSAGEFIELDS_H
#define MESSAGEFIELDS_H

#include <optional>

#include <QObject>

#include "draupnir/settings_registry/utils/EnumFlagsSerializer.h"
#include "draupnir/utils/advance_enum.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Messages
{

/*! @brief This enum holds flag for different fields of @ref Draupnir::Messages::Message objects.
 *  @ingroup MessageSystem */
enum MessageField {
    None       = 0b00000000,  /*!< @brief None. */
    Brief      = 0b00000001,  /*!< @brief Brief summary of `Message` (@ref Draupnir::Messages::Message::brief). */
    What       = 0b00000010,  /*!< @brief Details what happened ((@ref Draupnir::Messages::Message::what). */
    DateTime   = 0b00000100,  /*!< @brief DateTime of `Message` ((@ref Draupnir::Messages::Message::dateTime). */
    Icon       = 0b00001000,  /*!< @brief Icon of `Message` ((@ref Draupnir::Messages::Message::icon). */
    All        = Brief | What | DateTime | Icon,  /*!< @brief All of the above. */
    Unknown    = 0b10000000
};

/*! @brief Increment operator for the MessageField enum. */

inline MessageField& operator++(MessageField& type,int)
{
    draupnir::utils::enum_values<
        MessageField,
        Brief,
        Icon,
        All
    >::advance(type);
    return type;
}




















/*! @brief This static constexpr array holds values from enum @ref Draupnir::Messages::Message::Fields, which are
     *         representing individual fields of the @ref Draupnir::Messages::Message objects. */
static constexpr MessageField individualFieldsArray[] = { Brief, What, DateTime, Icon };

using MessageFields = draupnir::utils::enum_flags<MessageField>;

/*! @brief This is a static comverting Fields valur to user-friendly string. */
[[maybe_unused]] static QString fieldsDisplayString(Messages::MessageField value) {
    switch (value) {
    case Messages::MessageField::None:
        return QObject::tr("None");
    case Messages::MessageField::Brief:
        return QObject::tr("Brief");
    case Messages::MessageField::What:
        return QObject::tr("What");
    case Messages::MessageField::DateTime:
        return QObject::tr("Timestamp:");
    case Messages::MessageField::Icon:
        return QObject::tr("Icon");
    case Messages::MessageField::All:
        return QObject::tr("All");
    case Messages::MessageField::Unknown:
        break;
    }
    Q_ASSERT(false);
    return QString{};
}


}; // namespace Draupnir::Messages

Q_DECLARE_METATYPE(Draupnir::Messages::MessageField);
Q_DECLARE_METATYPE(Draupnir::Messages::MessageFields);

namespace Draupnir::Settings
{

template<>
class EnumFlagsSerializer<draupnir::utils::enum_flags<Draupnir::Messages::MessageField>>
{
public:
    static std::optional<Messages::MessageFields> fromConfigString(const QString& string) {
        if (string.isEmpty())
            return std::nullopt;

        const QStringList parts = string.split(',');
        if (parts.contains(none_configKey))
            return std::optional<Messages::MessageFields>{Messages::MessageField::None};
        if (parts.contains(all_configKey))
            return std::optional<Messages::MessageFields>{Messages::MessageField::All};

        Messages::MessageFields result{Messages::MessageField::None};
        for (const QString& part : parts) {
            Messages::MessageField field = _fieldFromConfigString(part);
            if (field == Messages::MessageField::Unknown)
                return std::nullopt;
            result.setFlag(field,true);
        }
        return std::optional<Messages::MessageFields>{result};
    }

    static QString toConfigString(const Messages::MessageFields& mask) {
        if (mask == Messages::MessageField::None)
            return none_configKey;
        if (mask == Messages::MessageField::All)
            return all_configKey;

        QStringList resultingParts;
        for (Messages::MessageField field : Messages::individualFieldsArray) {
            if (mask.testFlag(field))
                resultingParts.append(_fieldToConfigString(field));
            }

        return resultingParts.join(',');
    }
private:
    inline static const QLatin1String none_configKey{QLatin1String{"none"}};
    inline static const QLatin1String brief_configKey{QLatin1String{"brief"}};
    inline static const QLatin1String what_configKey{QLatin1String{"what"}};
    inline static const QLatin1String datetime_configKey{QLatin1String{"date-time"}};
    inline static const QLatin1String icon_configKey{QLatin1String{"icon"}};
    inline static const QLatin1String all_configKey{QLatin1String{"all"}};

    static Messages::MessageField _fieldFromConfigString(const QString& field) {
        if (field == none_configKey)
            return Messages::MessageField::None;
        if (field == brief_configKey)
            return Messages::MessageField::Brief;
        if (field == what_configKey)
            return Messages::MessageField::What;
        if (field == datetime_configKey)
            return Messages::MessageField::DateTime;
        if (field == icon_configKey)
            return Messages::MessageField::Icon;
        return Messages::MessageField::Unknown;
    }

    static QString _fieldToConfigString(Messages::MessageField field) {
        switch (field) {
        case Messages::MessageField::None:
            return none_configKey;
        case Messages::MessageField::Brief:
            return brief_configKey;
        case Messages::MessageField::What:
            return what_configKey;
        case Messages::MessageField::DateTime:
            return datetime_configKey;
        case Messages::MessageField::Icon:
            return icon_configKey;
        case Messages::MessageField::All:
        case Messages::MessageField::Unknown:
            break;
        };
        Q_ASSERT(false);
        return QString{};
    }
};

}; // namespace Draupnir::Settings







#endif // MESSAGEFIELDS_H
