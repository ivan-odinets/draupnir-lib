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

#ifndef ABSTRACTMESSAGEHANDLER_H
#define ABSTRACTMESSAGEHANDLER_H

#include "draupnir/logging/core/AbstractMessageReceiver.h"

#include "draupnir/messages/core/MessageType.h"
#include "draupnir/notifications/core/NotificationTypes.h"

namespace Draupnir::MessageSystem
{

/*! @class AbstractMessageHandler draupnir/message_system/core/AbstractMessageHandler.h
 *  @ingroup MessageSystem
 *  @brief Interface for processing logged messages and configuring their notifications.
 *
 *  @details Extends @ref Draupnir::Logging::AbstractMessageReceiver with operations for querying and changing the notification
 *           types associated with individual message types. */

class AbstractMessageHandler : public Draupnir::Logging::AbstractMessageReceiver
{
    Q_OBJECT
public:
    /*! @brief Constructs a message handler.
     *  @param parent Parent object. */
    explicit AbstractMessageHandler(QObject* parent = nullptr) :
        AbstractMessageReceiver{parent}
    {}

    virtual ~AbstractMessageHandler() = default;

    /*! @brief Handles a single message.
     *  @param message Message to process.
     * @pre `message` must not be null. */
    virtual void handleMessage(const Draupnir::Messages::MessagePtr& message) = 0;

    /*! @brief Handles multiple messages.
     *  @param messageList Messages to process.
     * @pre `messageList` must not contain null message pointers. */
    virtual void handleMessageList(const Draupnir::Messages::MessageList& messageList) = 0;

    /*! @brief Returns the notification types configured for a message type.
     *  @param messageType Message type whose notification configuration should be queried.
     *  @return Notification types enabled for `messageType`. */
    [[nodiscard]] virtual Notifications::NotificationTypes notificationTypesFor(
        Draupnir::Messages::MessageType messageType
    ) const = 0;

    /*! @brief Sets the notification types for a message type.
     *  @param messageType Message type whose notification configuration should be changed.
     *  @param notificationTypes Notification types to enable. */
    virtual void setNotificationTypesFor(
        Draupnir::Messages::MessageType messageType,
        Draupnir::Notifications::NotificationTypes mask
    ) = 0;
};

} // namespace Draupnir::MessageSystem

#endif // ABSTRACTMESSAGEHANDLER_H
