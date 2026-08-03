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

#ifndef NOTIFICATIONCHANNELCONCEPT_H
#define NOTIFICATIONCHANNELCONCEPT_H

#include <concepts>

#include "draupnir/notifications/core/NotificationTypes.h"
#include "draupnir/messages/core/Message.h"

namespace Draupnir::Notifications
{

/*! @concept StaticNotificationChannelConcept draupnir/notifications/concepts/NotificationChannelConcept.h
 *  @ingroup Notifications
 *  @brief Defines the interface required for statically invoked notification channels.
 *
 *  @details A matching channel must provide static methods for displaying a single message and a list of messages. */

template<class Candidate>
concept StaticNotificationChannelConcept = requires(
    Draupnir::Messages::MessagePtr message, const Draupnir::Messages::MessageList& messageList
) {
    { Candidate::showMessage(message) } -> std::same_as<void>;
    { Candidate::showMessageList(messageList) } -> std::same_as<void>;
};

/*! @concept InstanceNotificationChannelConcept draupnir/notifications/concepts/NotificationChannelConcept.h
 *  @ingroup Notifications
 *  @brief Defines the interface required for instance-based notification channels.
 *
 *  @details A matching channel must provide instance methods for displaying a single message and a list of messages.
 *
 *           Types satisfying @ref StaticNotificationChannelConcept are explicitly excluded because static methods can also be
 *           invoked through an object instance. */

template<class Candidate>
concept InstanceNotificationChannelConcept = requires(
    Candidate& obj,
    Draupnir::Messages::MessagePtr message, const Draupnir::Messages::MessageList& messageList
) {
    { obj.showMessage(message) } -> std::same_as<void>;
    { obj.showMessageList(messageList) } -> std::same_as<void>;
    requires(!StaticNotificationChannelConcept<Candidate>);
};

/*! @concept NotificationTraitConcept draupnir/notifications/concepts/NotificationChannelConcept.h
 *  @ingroup Notifications
 *  @brief Defines the interface required for notification traits.
 *
 *  @details A matching trait provides a notification type identifier, translated user-visible name, and configuration key. */

template<class Candidate>
concept NotificationTraitConcept = requires {
    { Candidate::value() } -> std::same_as<NotificationType>;
    { Candidate::displayName() } -> std::same_as<QString>;
    { Candidate::configString() } -> std::same_as<QLatin1String>;
};

/*! @concept NotificationChannelConcept draupnir/notifications/concepts/NotificationChannelConcept.h
 *  @ingroup Notifications
 *  @brief Defines the complete interface required for notification channels.
 *
 *  @details A matching channel must provide a nested `Trait` type satisfying @ref Draupnir::Notifications::NotificationTraitConcept
 *           and must satisfy one of the following concepts: either @ref Draupnir::Notifications::StaticNotificationChannelConcept
 *           or @ref Draupnir::Notifications::InstanceNotificationChannelConcept. */

template<class Candidate>
concept NotificationChannelConcept = requires {
    requires(NotificationTraitConcept<typename Candidate::Trait>);
    { Candidate::Trait::value() } -> std::same_as<NotificationType>;
    { Candidate::Trait::displayName() } -> std::same_as<QString>;
    { Candidate::Trait::configString() } -> std::same_as<QLatin1String>;
    requires(StaticNotificationChannelConcept<Candidate> || InstanceNotificationChannelConcept<Candidate>);
};

} // namespace Draupnir::Notifications

#endif // NOTIFICATIONCHANNELCONCEPT_H
