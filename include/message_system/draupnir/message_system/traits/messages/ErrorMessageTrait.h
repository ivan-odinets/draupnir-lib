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

#ifndef ERRORMESSAGETRAIT_H
#define ERRORMESSAGETRAIT_H

#include <QApplication>
#include <QIcon>
#include <QStyle>

#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/message_system/core/Notification.h"

namespace Draupnir::Messages
{

/*! @class ErrorMessageTrait draupnir/message_system/traits/messages/ErrorMessageTrait.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for error messages. */

class ErrorMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Error};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within @ref Draupnir::Messages::MessageTypeSettingsTrait template to this string a `notifications/` prefix will
     *       be prepended. Actual storage key: `notifications/error`. */
    static constexpr const char* settingsKey = "error";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Error"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon() {
        static const QIcon icon = qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical);
        return icon;
    }
};

}; // namespace Draupnir::Messages

#endif // ERRORMESSAGETRAIT_H
