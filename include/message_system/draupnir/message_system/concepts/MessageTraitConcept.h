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

#ifndef MESSAGETRAITCONCEPT_H
#define MESSAGETRAITCONCEPT_H

#include <QString>

#include "draupnir/message_system/core/Notification.h"

namespace Draupnir::Messages
{

namespace MessageTrait
{

template<class Candidate>
concept HasMessageType = requires {
    { Candidate::type } -> std::convertible_to<quint64>;
};

template<class Candidate>
concept HasSettingsKey = requires {
    { Candidate::settingsKey } -> std::convertible_to<QString>;
};

template<class Candidate>
concept HasDefaultNotification = requires {
    { Candidate::defaultNotification } -> std::convertible_to<Draupnir::Messages::Notification::Type>;
};

template<class Candidate>
concept HasDisplayName = requires {
    { Candidate::displayName() } -> std::convertible_to<QString>;
};

template<class Candidate>
concept HasIcon = requires {
    { Candidate::icon() } -> std::convertible_to<QIcon>;
};

}; // namespace Draupnir::Messages::MessageTrait

template<class Candidate>
concept MessageTraitConcept =
    MessageTrait::HasMessageType<Candidate> &&
    MessageTrait::HasSettingsKey<Candidate> &&
    MessageTrait::HasDefaultNotification<Candidate> &&
    MessageTrait::HasDisplayName<Candidate> &&
    MessageTrait::HasIcon<Candidate>;

}; // namespace Draupnir::Messages

#endif // MESSAGETRAITCONCEPT_H
