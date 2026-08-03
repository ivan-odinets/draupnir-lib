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

/*! @class NotificationType draupnir/notifications/core/NotificationTypes.h
 *  @ingroup Notifications
 *  @brief Identifies a notification delivery mechanism.
 *
 *  @details Notification type values occupy individual bits and may be combined into a @ref Draupnir::Notifications::NotificationTypes
 *           mask.
 *
 *           The type is a lightweight, copyable, assignable, comparable, and hashable integer wrapper. */

class NotificationType final : public draupnir::utils::integer_wrapper<uint16_t, NotificationType>
{
    using _Base = draupnir::utils::integer_wrapper<uint16_t, NotificationType>;
public:
    using underlying_type = typename _Base::underlying_type;

    using _Base::integer_wrapper;

    using _Base::operator=;

    /*! @enum DefaultTypes
     *  @brief Built-in and reserved notification type values. */
    enum DefaultTypes : underlying_type {
        /*! @brief No user-facing notification is produced. */
        NoNotification          = 0b0000'0000,
        /*! @brief Displays the message in a dialog window. */
        DialogNotification      = 0b0000'0001,
        /*! @brief Displays the message through the system tray. */
        TrayNotification        = 0b0000'0010,
        /*! @brief First value reserved for application-defined notification types. */
        FirstCustomNotification = 0b0000'0100
    };

    /*! @brief Returns the notification type following another type.
     *  @param previousType Previous non-zero notification type.
     *  @return Notification type whose value is shifted one bit to the left.
     * @pre `previousType` must contain a non-zero value.
     * @pre Its value must leave enough space for a one-bit left shift without overflowing the underlying integer. */
    [[nodiscard]] static constexpr NotificationType nextType(NotificationType previousType) noexcept {
        return NotificationType{previousType << 1};
    }
};

/*! @class NotificationTypes draupnir/notifications/core/NotificationTypes.h
 *  @ingroup Notifications
 *  @brief Flag mask containing zero or more notification types.
 *
 *  @details Combines @ref Draupnir::Notifications::NotificationType values into a single mask describing the notification mechanisms
 *           that should be used for a message.
 *
 *           An empty mask represents @ref NotificationType::NoNotification. The type is copyable, assignable, comparable, and hashable. */

class NotificationTypes : public draupnir::utils::flags<NotificationType>
{
    using _Base = draupnir::utils::flags<NotificationType>;
public:
    using _Base::flags;
    using _Base::operator=;
};

/*! @brief Calculates a Qt hash value for a notification type.
 *  @ingroup Notifications
 *  @param key Notification type to hash.
 *  @param seed Initial hash seed.
 *  @return Hash value of the underlying notification type identifier. */

[[nodiscard]] inline uint qHash(NotificationTypes key, uint seed = 0) noexcept {
    return ::qHash(key.value(), seed);
}

/*! @brief Calculates a Qt hash value for a notification type mask.
 *  @ingroup Notifications
 *  @param key Notification type mask to hash.
 *  @param seed Initial hash seed.
 *  @return Hash value of the underlying mask. */

[[nodiscard]] inline uint qHash(NotificationType key, uint seed = 0) noexcept {
    return ::qHash(key.value(), seed);
}

} // namespace Draupnir::Notifications

namespace std
{

/*! @ingroup Notifications
 *  @brief Standard hash implementation for @ref Draupnir::Notifications::NotificationType. */

template<>
struct hash<Draupnir::Notifications::NotificationType>
{
    [[nodiscard]] std::size_t operator()(Draupnir::Notifications::NotificationType type) const noexcept {
        return std::hash<Draupnir::Notifications::NotificationType::underlying_type>{}(type.value());
    }
};

/*! @ingroup Notifications
 *  @brief Standard hash implementation for @ref Draupnir::Notifications::NotificationTypes. */

template<>
struct hash<Draupnir::Notifications::NotificationTypes>
{
    [[nodiscard]] std::size_t operator()(Draupnir::Notifications::NotificationTypes types) const noexcept {
        return std::hash<Draupnir::Notifications::NotificationType::underlying_type>{}(types.value().value());
    }
};

} // namespace std

Q_DECLARE_METATYPE(Draupnir::Notifications::NotificationType);
Q_DECLARE_METATYPE(Draupnir::Notifications::NotificationType::DefaultTypes);
Q_DECLARE_METATYPE(Draupnir::Notifications::NotificationTypes);

#endif // NOTIFICATIONTYPES_H
