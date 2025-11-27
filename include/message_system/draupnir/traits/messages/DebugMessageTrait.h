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

#ifndef DEBUGMESSAGETRAIT_H
#define DEBUGMESSAGETRAIT_H

#include <QIcon>

#include "draupnir/core/MessageType.h"
#include "draupnir/core/Notification.h"

namespace Draupnir::Messages
{

/*! @class DebugMessageTrait draupnir/traits/messages/DebugMessageTrait.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for debug-level messages.
 *
 *  @details This trait is intended for internal diagnostic messages. */

class DebugMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Debug};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within Draupnir::Messages::MessageTypeSettingsTrait template to this string a notifications/ prefix will
     *       be prepended. Actual storage key: notifications/debug. */
    static constexpr const char* settingsKey = "debug";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Debug"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon() {
        static const QIcon icon;
        return icon;
    }
};

}; // namespace Draupnir::Messages

#endif // DEBUGMESSAGETRAIT_H
