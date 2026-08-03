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

#include "draupnir/notifications/core/NotificationTypesSerializerInterface.h"

namespace Draupnir::Settings
{

/*! @class ValueSerializerTemplate<Draupnir::Notifications::NotificationTypes>
 *  @headerfile draupnir/notifications/utils/NotificationTypeSerializer.h
 *  @ingroup Notifications
 *  @brief Serializes notification type masks to and from QVariant.
 *
 *  @details Uses a registered @ref Draupnir::Notifications::NotificationTypeSerializerInterface to convert notification type masks
 *           to their configuration string representation. */

template<>
class ValueSerializerTemplate<Draupnir::Notifications::NotificationTypes>
{
public:
    /*! @brief Owning pointer to the registered serializer implementation. */
    using ImplementationPtr = std::unique_ptr<Notifications::NotificationTypesSerializerInterface>;

    /*! @brief Notification type mask handled by this serializer. */
    using Value = Draupnir::Notifications::NotificationTypes;

    /*! @brief Registers the serializer used for notification type conversion.
     *  @param serializer Serializer interface to register. Must not be null. */
    static void registerImplementation(Notifications::NotificationTypesSerializerInterface* serializer) {
        Q_ASSERT(serializer);
        Q_ASSERT(p_serializer == nullptr);
        p_serializer = ImplementationPtr{serializer};
    }

    /*! @brief Registers the serializer used for notification type conversion.
     *  @param serializer Serializer interface to register. Must not be null. */
    static void registerImplementation(ImplementationPtr serializer) {
        Q_ASSERT(serializer);
        Q_ASSERT(p_serializer == nullptr);
        p_serializer = std::move(serializer);
    }

    /*! @brief Returns the registered serializer implementation.
     *  @return Non-owning pointer to the registered implementation, or `nullptr` if no implementation has been registered. */
    [[nodiscard]] static auto implementation() {
        return p_serializer.get();
    }

    /*! @brief Converts a notification type mask to QVariant.
     *  @param value Notification type mask to serialize.
     *  @return QVariant containing its configuration string representation. */
    [[nodiscard]] static QVariant toQVariant(const Value& value) {
        Q_ASSERT(p_serializer);
        return QVariant::fromValue(p_serializer->toConfigString(value));
    }

    /*! @brief Converts a QVariant to a notification type mask.
     *  @param value QVariant containing a configuration string.
     *  @return Deserialized notification type mask, or std::nullopt if the value cannot be converted or parsed. */
    [[nodiscard]] static std::optional<Value> fromQVariant(const QVariant& value) {
        Q_ASSERT(p_serializer);
        if (!value.canConvert<QString>())
            return std::nullopt;

        return p_serializer->fromConfigString(value.toString());
    }

private:
    static inline ImplementationPtr p_serializer = nullptr;
};

/*! @brief Convenience alias for the notification type mask serializer. */
using NotificationTypesSerializer = ValueSerializerTemplate<Draupnir::Notifications::NotificationTypes>;

} // namespace Draupnir::Settings

#endif // NOTIFICATIONTYPESSERIALIZER_H
