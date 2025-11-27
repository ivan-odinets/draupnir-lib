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

#ifndef WARNINGMESSAGETRAIT_H
#define WARNINGMESSAGETRAIT_H

#include <QApplication>
#include <QIcon>
#include <QStyle>

#include "draupnir/core/MessageType.h"
#include "draupnir/core/Notification.h"

namespace Draupnir::Messages
{

/*! @class WarningMessageTrait draupnir/traits/messages/WarningMessageTrait.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for warning messages.
 *
 *  @details Messages with this trait indicate that something might be wrong, but is not necessarily an error. */

class WarningMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Warning};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within Draupnir::Messages::MessageTypeSettingsTrait template to this string a notifications/ prefix will
     *       be prepended. Actual storage key: notifications/warning. */
    static constexpr const char* settingsKey = "warning";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Warning"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon() {
        static const QIcon icon = qApp->style()->standardIcon(QStyle::SP_MessageBoxWarning);
        return icon;
    }
};

}; // namespace Draupnir::Messages

#endif // WARNINGMESSAGETRAIT_H
