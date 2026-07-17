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

#ifndef NOTIFICATIONHANDLERCONCEPTS_H
#define NOTIFICATIONHANDLERCONCEPTS_H

#include "draupnir/logging/messages/Message.h"

namespace Draupnir::Notifications
{

template<class Candidate>
concept StatelessHandlerConcept = requires(
    Draupnir::Logging::Message* message, const Draupnir::Logging::MessageList& messageList
    ) {
    { Candidate::showMessage(message) } -> std::same_as<void>;
    { Candidate::showMessageList(messageList) } -> std::same_as<void>;
};

template<class Candidate>
concept StatefulHandlerConcept = requires(
    Candidate& obj,
    Draupnir::Logging::Message* message, const Draupnir::Logging::MessageList& messageList
) {
    { obj.showMessage(message) } -> std::same_as<void>;
    { obj.showMessageList(messageList) } -> std::same_as<void>;
    requires(!StatelessHandlerConcept<Candidate>);
};

}; // namespace Draupnir::Notifications

#endif // NOTIFICATIONHANDLERCONCEPTS_H
