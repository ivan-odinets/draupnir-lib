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

#ifndef NOTIFICATIONTYPESSERIALIZERTEMPLATE_H
#define NOTIFICATIONTYPESSERIALIZERTEMPLATE_H

#include "draupnir/notifications/concepts/NotificationChannelConcept.h"
#include "draupnir/notifications/core/NotificationTypesSerializerInterface.h"
#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"

namespace Draupnir::Notifications
{

/*! @class NotificationTypesSerializerTemplate draupnir/notifications/core/NotificationTypesSerializerInterface.h
 *  @ingroup Notifications
 *  @brief Serializes notification type masks using a compile-time set of notification traits.
 *  @tparam Traits Notification traits describing the supported notification types and their configuration tokens.
 *
 *  @details Uses @ref Draupnir::Settings::FlagSerializerTemplate to serialize individual notification types and
 *           @ref Draupnir::Settings::FlagsMaskSerializerTemplate to serialize complete @ref Draupnir::Notifications::NotificationTypes masks.
 *
 *           The empty notification mask is represented using the configuration token provided by @ref Draupnir::Settings::NoneFlagsMaskTemplate. */

template<NotificationTraitConcept... Traits>
class NotificationTypesSerializerTemplate final : public NotificationTypesSerializerInterface
{
public:
    /*! @brief Destroys the notification types serializer. */
    ~NotificationTypesSerializerTemplate() final = default;

    /*! @brief Serializer used for individual notification type values. */
    using SingleNotificationSerializer = Draupnir::Settings::FlagSerializerTemplate<
        NotificationType,
        Traits...
    >;
    /*! @brief Serializer used for complete notification type masks. */
    using Serializer = Draupnir::Settings::FlagsMaskSerializerTemplate<
        NotificationTypes,
        SingleNotificationSerializer,
        Draupnir::Settings::NoneFlagsMaskTemplate<NotificationTypes>
    >;

    /*! @brief Parses notification types from a configuration string.
     *  @param string Configuration string to parse.
     *  @return Parsed notification type mask, or `std::nullopt` if `string` is invalid or contains unsupported notification
     *          type tokens. */
    [[nodiscard]] std::optional<NotificationTypes> fromConfigString(const QString& string) const final {
        return Serializer::fromConfigString(string);
    }

    /*! @brief Converts a notification type mask to a configuration string.
     *  @param mask Notification type mask to serialize.
     *  @return Configuration string representing `mask`. */
    [[nodiscard]] QString toConfigString(NotificationTypes mask) const final {
        return Serializer::toConfigString(mask);
    }
};

} // namespace Draupnir::Notifications

#endif // NOTIFICATIONTYPESSERIALIZERTEMPLATE_H
