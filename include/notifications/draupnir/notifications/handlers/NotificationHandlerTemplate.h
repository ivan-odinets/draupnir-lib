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

#ifndef NOTIFICATIONHANDLERTEMPLATE_H
#define NOTIFICATIONHANDLERTEMPLATE_H

#include "draupnir/notifications/concepts/NotificationTypeConcept.h"

namespace Draupnir::Notifications
{

/*! @class NotificationHandlerTemplate
 *  @ingroup Notifications
 *  @brief Primary template for notification handlers.
 *  @tparam NotificationType Trait describing the notification type.
 *
 *  @details Must be explicitly specialized for every supported notification type trait. Instantiating the unspecialized
 *           template produces a compilation error. */

template<NotificationTypeTraitConcept NotificationType>
class NotificationHandlerTemplate
{
    static_assert(false, "NotificationHandlerTemplate must be specialized for every notification type trait.");
};

}; // namespace Draupnir::Notifications

#endif // NOTIFICATIONHANDLERTEMPLATE_H
