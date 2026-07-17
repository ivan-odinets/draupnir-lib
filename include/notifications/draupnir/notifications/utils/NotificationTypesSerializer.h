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

#ifndef NOTIFICATIONTYPESSERIALIZER_H
#define NOTIFICATIONTYPESSERIALIZER_H

#include "draupnir/settings_registry/utils/ValueSerializerTemplate.h"

#include "draupnir/notifications/core/NotificationTypeSerializerInterface.h"

namespace Draupnir::Settings
{

/*! @brief Serializes notification type masks to and from QVariant.
 *  @ingroup Notifications
 *
 *  @details Uses a registered NotificationTypeSerializerInterface to convert notification type masks to their configuration
 *           string representation. */

template<>
class ValueSerializerTemplate<Draupnir::Notifications::NotificationTypes>
{
private:
    static inline Notifications::NotificationTypeSerializerInterface* p_serializer = nullptr;

public:
    using Value = Draupnir::Notifications::NotificationTypes;

    /*! @brief Registers the serializer used for notification type conversion.
     *  @param serializer Serializer interface to register. Must not be null.
     *  @details The serializer is not owned by this class and must remain valid while serialization operations are performed. */
    static void registerSerializerInterface(Notifications::NotificationTypeSerializerInterface* serializer) {
        Q_ASSERT(serializer);
        p_serializer = serializer;
    }

    /*! @brief Converts a notification type mask to QVariant.
     *  @param value Notification type mask to serialize.
     *  @return QVariant containing its configuration string representation. */
    static QVariant toQVariant(const Value& value) {
        Q_ASSERT(p_serializer);
        return QVariant::fromValue(p_serializer->toConfigString(value));
    }

    /*! @brief Converts a QVariant to a notification type mask.
     *  @param value QVariant containing a configuration string.
     *  @return Deserialized notification type mask, or std::nullopt if the value cannot be converted or parsed. */
    static std::optional<Value> fromQVariant(const QVariant& value) {
        Q_ASSERT(p_serializer);
        if (!value.canConvert<QString>())
            return std::nullopt;

        const auto maybeValue = p_serializer->fromConfigString(value.toString());
        return (maybeValue) ? maybeValue : std::nullopt;
    }
};

}; // namespace Draupnir::Settings

#endif // NOTIFICATIONTYPESSERIALIZER_H
