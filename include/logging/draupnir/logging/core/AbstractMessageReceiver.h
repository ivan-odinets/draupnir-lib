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

#ifndef ABSTRACTMESSAGERECEIVER_H
#define ABSTRACTMESSAGERECEIVER_H

#include <QObject>

#include "draupnir/messages/core/Message.h"

namespace Draupnir::Logging
{

/*! @class AbstractMessageReceiver draupnir/logging/core/AbstractMessageReceiver.h
 *  @ingroup Logging
 *  @brief Abstract base class for classes that receive and process logged messages.
 *
 *  @details @ref Draupnir::Logging::AbstractMessageReceiver is used by @ref Draupnir::Logging::Logger to forward created
 *           @ref Draupnir::Messages::MessagePtr objects to an external processing backend.
 *
 *           Implementations may store messages, forward them to log sinks, write them to files, expose them through Qt models,
 *           dispatch notifications, or perform any other application-specific processing.
 *
 * @see Draupnir::MessageSystem::AbstractMessageHandler
 * @see Draupnir::MessageSystem::MessageHandlerTemplate
 *
 * @todo Documentation: Write a page how and when inherit from this class. */

class AbstractMessageReceiver : public QObject
{
    Q_OBJECT
public:
    /*! @brief Constructs a message receiver.
     *  @param parent Optional parent `QObject`, or `nullptr`. */
    explicit AbstractMessageReceiver(QObject* parent = nullptr) :
        QObject{parent}
    {}

    // No copying.
    Q_DISABLE_COPY(AbstractMessageReceiver);

    /*! @brief Default destructor. */
    virtual ~AbstractMessageReceiver() override = default;

    /*! @brief Handles a single @ref Draupnir::Messages::MessagePtr object.
     *  @param message @ref Draupnir::Messages::MessagePtr referencing the message object to be handled. */
    virtual void handleMessage(const Draupnir::Messages::MessagePtr& message) = 0;

    /*! @brief Handles a @ref Draupnir::Messages::MessageList.
     *  @param messageList @ref Draupnir::Messages::MessageList object containing @ref Draupnir::Messages::MessagePtr
     *         objects to be handled. */
    virtual void handleMessageList(const Draupnir::Messages::MessageList& messageList) = 0;
};

} // namespace Draupnir::Logging

#endif // ABSTRACTMESSAGERECEIVER_H
