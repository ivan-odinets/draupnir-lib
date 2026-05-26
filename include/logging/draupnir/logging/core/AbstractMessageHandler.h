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

#include <QObject>

#include "draupnir/logging/messages/Message.h"

namespace Draupnir::Logging
{

/*! @class AbstractMessageHandler include/draupnir/logging/core/AbstractMessageHandler.h
 *  @ingroup Logging
 *  @brief Interface for classes that receive and process logged messages.
 *
 *  @details @ref Draupnir::Logging::AbstractMessageHandler is used by @ref Draupnir::Logging::Logger to forward created @ref
 *           Draupnir::Logging::Message objects to an external processing backend.
 *
 *           Implementations may store messages, forward them to log sinks, write them to files, expose them through Qt models,
 *           dispatch notifications, or perform any other application-specific processing.
 *
 * @note Implementations are responsible for defining the final lifetime management strategy for received messages. Unless
 *       explicitly documented otherwise by a concrete implementation, passing a message to this interface transfers ownership
 *       of the message to the handler. */

class AbstractMessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMessageHandler(QObject* parent = nullptr) :
        QObject{parent}
    {}

    virtual ~AbstractMessageHandler() = default;

    /*! @brief Handles a single @ref Draupnir::Logging::Message object.
     *  @param message Pointer to the message object to handle.
     * @note This method should take ownership of the provided `message`. The caller must not delete the message. Accessing it
     *       in theory is possible, but depends on implementation. */
    virtual void handleMessage(Message* message) = 0;

    /*! @brief Handles a list of @ref Draupnir::Logging::Message objects.
     *  @param messageList List of message pointers to handle.
     * @note This method should take ownership of all message objects contained in `messageList`. The caller must not delete the
     *       messages. Accessing them in theory is possible, but depends on implementation. */
    virtual void handleMessageList(const QList<Message*>& messageList) = 0;
};

}; // namespace Draupnir::Logging

#endif // ABSTRACTMESSAGEHANDLER_H
