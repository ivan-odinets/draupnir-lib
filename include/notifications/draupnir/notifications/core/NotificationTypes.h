/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef NOTIFICATIONTYPES_H
#define NOTIFICATIONTYPES_H

#include <QCoreApplication>

#include "draupnir/utils/integer_wrapper.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Notifications
{

/*! @brief This is a class
 *  @ingroup Notifications
 * @todo Documentation: Write documentation */

class NotificationType : public draupnir::utils::integer_wrapper<uint16_t, NotificationType>
{
    using _Base = draupnir::utils::integer_wrapper<uint16_t, NotificationType>;
public:
    using underlying_type = typename _Base::underlying_type;

    using _Base::integer_wrapper;

    using _Base::operator=;

    enum DefaultTypes {
        /*! @brief None: No notification; silently logged. */
        NoNotification          = 0b0000'0000,
        /*! @brief DialogNotification: Show notification as @ref Draupnir::Logging::MessageDisplayDialog. */
        DialogNotification      = 0b0000'0001,
        /*! @brief TrayNotification: Show notification in system tray. */
        TrayNotification        = 0b0000'0010,
        /*! @brief Fallback for unrecognized/invalid types. */
        UnknownNotification     = 0b0000'0100
    };
};

[[nodiscard]] inline uint qHash(NotificationType key, uint seed = 0) noexcept {
    return ::qHash(key.value(), seed);
}

/*! @brief This is a class
 *  @ingroup Notifications
 * @todo Documentation: Write documentation */

class NotificationTypes : public draupnir::utils::flags<NotificationType>
{
    using _Base = draupnir::utils::flags<NotificationType>;
public:
    using _Base::flags;
    using _Base::operator=;
};

[[nodiscard]] inline uint qHash(NotificationTypes key, uint seed = 0) noexcept {
    return ::qHash(key.value(), seed);
}

}; // namespace Draupnir::Notifications

Q_DECLARE_METATYPE(Draupnir::Notifications::NotificationType);
Q_DECLARE_METATYPE(Draupnir::Notifications::NotificationType::DefaultTypes);
Q_DECLARE_METATYPE(Draupnir::Notifications::NotificationTypes);

#endif // NOTIFICATIONTYPES_H
