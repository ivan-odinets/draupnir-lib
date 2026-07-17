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

#ifndef DIALOGNOTIFICATIONTRAIT_H
#define DIALOGNOTIFICATIONTRAIT_H

#include "draupnir/notifications/core/NotificationTypes.h"

namespace Draupnir::Notifications
{

/*! @class DialogNotificationTrait include/draupnir/notifications/traits/notifications/DialogNotificationTrait.h
 *  @ingroup Notifications
 *  @brief Describes the dialog notification type.
 *
 *  @details Provides the notification identifier, configuration key and user-visible name used by the notification system. */

class DialogNotificationTrait
{
public:
    /*! @brief Returns the notification type represented by this trait. */
    static constexpr NotificationType value() { return NotificationType::DialogNotification; }

    /*! @brief Returns the key used to store this notification type in configuration. */
    static QLatin1String configString() { return QLatin1String{"dialog"}; }

    /*! @brief Returns the translated user-visible name of this notification type. */
    static QString displayName() { return QObject::tr("Dialog"); }
};

}; // namespace Draupnir::Notifications

#endif // DIALOGNOTIFICATIONTRAIT_H
