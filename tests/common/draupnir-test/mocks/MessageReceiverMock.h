/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MESSAGEHANDLERMOCKTEMPLATE_H
#define MESSAGEHANDLERMOCKTEMPLATE_H

#include "draupnir/logging/core/AbstractMessageReceiver.h"

/*! @class MessageReceiverMock draupnir-test/mocks/MessageReceiverMock.h
 *  @ingroup TestHelpers
 *  @brief Mock implementation of @ref Draupnir::Logging::AbstractMessageReceiver.
 *
 *  @details Stores every received message in @ref messagesReceived, allowing tests to inspect the number, order, and contents
 *           of messages delivered by a logger or another message-producing component. */

class MessageReceiverMock final :
    public Draupnir::Logging::AbstractMessageReceiver
{
    Q_OBJECT
public:
    /*! @brief Constructs an empty message receiver mock.
     *  @param parent Optional parent `QObject`, or `nullptr`. */
    explicit MessageReceiverMock(QObject* parent = nullptr) :
        Draupnir::Logging::AbstractMessageReceiver{parent}
    {}

    /*! @brief Trivial destructor. */
    ~MessageReceiverMock() override final = default;

    /*! @brief Clears all messages previously received by this mock. */
    void clear() {
        m_messagesReceived.clear();
    }

    /*! @brief Stores a single received message.
     *  @param message Shared pointer to the received message. */
    void handleMessage(const Draupnir::Messages::MessagePtr& message) override final {
        m_messagesReceived.append(message);
    }

    /*! @brief Stores all messages from a received message list.
     * @param messageList List of messages to store.
     * @details Messages are appended in their original order. */
    void handleMessageList(const Draupnir::Messages::MessageList& messageList) override final {
        m_messagesReceived.append(messageList);
    };

    /*! @brief Messages received by this mock, stored in delivery order. */
    const Draupnir::Messages::MessageList& messagesReceived() const { return m_messagesReceived; }

private:
    Draupnir::Messages::MessageList m_messagesReceived;
};

#endif // MESSAGEHANDLERMOCKTEMPLATE_H
