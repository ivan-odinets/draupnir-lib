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

/*! @brief This is a class
 *  @ingroup Notifications
 * @todo Documentation: Write documentation */

class DialogNotification
{
public:
    static constexpr NotificationType value() { return NotificationType::DialogNotification; }
    static QLatin1String configString() { return QLatin1String{"dialog"}; }
    static QString displayName() { return QObject::tr("Dialog"); }
};

}; // namespace Draupnir::Notifications

#endif // DIALOGNOTIFICATIONTRAIT_H
