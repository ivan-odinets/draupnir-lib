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

#include "draupnir/logging/core/AbstractMessageHandler.h"

/*! @class MessageHandlerMock draupnir-test/mocks/MessageHandlerMock.h
 *  @ingroup TestHelpers
 *  @brief This is a mock class implementing @ref Draupnir::Logging::AbstractMessageHandler. */

class MessageHandlerMock final :
    public Draupnir::Logging::AbstractMessageHandler
{
    Q_OBJECT
public:
    explicit MessageHandlerMock(QObject* parent = nullptr) :
        Draupnir::Logging::AbstractMessageHandler{parent}
    {}

    ~MessageHandlerMock() final { clear(); };

    void clear() {
        for (auto* message : messagesReceived) { delete message; }
        messagesReceived.clear();
    }

    void handleMessage(Draupnir::Logging::Message* message) final {
        messagesReceived.append(message);
    }

    void handleMessageList(const QList<Draupnir::Logging::Message*>& messageList) final {
        messagesReceived.append(messageList);
    };

    QList<Draupnir::Logging::Message*> messagesReceived;
};

#endif // MESSAGEHANDLERMOCKTEMPLATE_H
