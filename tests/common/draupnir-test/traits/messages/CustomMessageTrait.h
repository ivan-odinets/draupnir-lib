/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef CUSTOMMESSAGETRAIT_H
#define CUSTOMMESSAGETRAIT_H

#include <QIcon>

#include "draupnir/core/MessageType.h"
#include "draupnir/core/Notification.h"

class CustomMessageTrait
{
public:
    static constexpr Draupnir::Messages::MessageType type =
            Draupnir::Messages::MessageType::nextType(Draupnir::Messages::MessageType::FirstCustomType);
    static constexpr const char* settingsKey = "CUSTOM";
    static constexpr Draupnir::Messages::Notification::Type defaultNotification =
            Draupnir::Messages::Notification::MessageBoxType;
    static QString displayName() { return QObject::tr("TEST"); }
    static const QIcon& icon() {
        static const QIcon msgIcon = QIcon{};
        return msgIcon;
    }
};

#endif // CUSTOMMESSAGETRAIT_H
