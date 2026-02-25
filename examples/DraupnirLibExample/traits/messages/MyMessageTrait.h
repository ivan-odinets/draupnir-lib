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

#ifndef MYMESSAGETRAIT_H
#define MYMESSAGETRAIT_H

#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/message_system/core/Notification.h"

#include "draupnir/utils/ResourceHelper.h"

class MyMessageTrait
{
public:
    // type member, which is a flag ingeter (only one bit set). Custom flags must have bits set higher than within
    // Draupnir::Messages::MessageType::FirstCustomType constant.
    static constexpr Draupnir::Messages::MessageType type{
        Draupnir::Messages::MessageType::nextType(Draupnir::Messages::MessageType::FirstCustomType)
    };

    // Settings key under which notification type for this messages will be stored. Note that to this key a prefix
    // "notifications" will be prepended.
    static constexpr const char* settingsKey = "my_message_type";

    // This type of notification will be used if none are present within the config file
    static constexpr Draupnir::Messages::Notification::Type defaultNotification =
            Draupnir::Messages::Notification::None;

    // How our message will be displayed to user.
    static QString displayName() { return QObject::tr("My Message"); }

    // Icon of our message. May be empty.
    static const QIcon& icon() {
        return Draupnir::Resources::ResourceHelper::get().draupnirIcon();
    }
};

#endif // MYMESSAGETRAIT_H
