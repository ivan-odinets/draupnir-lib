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

#ifndef NONENOTIFICATIONTRAIT_H
#define NONENOTIFICATIONTRAIT_H

#include <QObject>

#include "draupnir/notifications/core/NotificationTypes.h"

namespace Draupnir::Notifications
{

/*! @struct NoneNotificationTrait draupnir/notifications/traits/notifications/NoneNotificationTrait.h
 *  @ingroup Notifications
 *  @brief Trait describing the absence of notification delivery.
 *
 *  @details Represents the special @ref Draupnir::Notifications::NotificationTypes::None value used when no notification channel
 *           should be selected.
 *
 *           The trait provides:
 *           - the corresponding notification mask;
 *           - its persistent configuration string;
 *           - the user-visible translated name. */

struct NoneNotificationTrait
{
    /*! @brief Returns the notification mask represented by this trait.
     *  @return Mask indicating that no notification channel is selected. */
    [[nodiscard]] static constexpr NotificationTypes value() { return NotificationTypes::None; }

    /*! @brief Returns the persistent configuration string.
     *  @return Stable string used to serialize the no-notification value. */
    [[nodiscard]] static QLatin1String configString() { return QLatin1String("none"); }

    /*! @brief Returns the translated user-visible name.
     *  @return Display name for the no-notification option. */
    [[nodiscard]] static QString displayName() { return QObject::tr("None"); }
};

} // namespace Draupnir::Notifications

#endif // NONENOTIFICATIONTRAIT_H
