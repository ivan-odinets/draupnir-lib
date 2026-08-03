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

#ifndef NOTIFICATIONTYPESSERIALIZERINTERFACE_H
#define NOTIFICATIONTYPESSERIALIZERINTERFACE_H

#include "draupnir/notifications/core/NotificationTypes.h"

#include <QString>

namespace Draupnir::Notifications
{

/*! @class NotificationTypesSerializerInterface draupnir/notifications/core/NotificationTypesSerializerInterface.h
 *  @ingroup Notifications
 *  @brief Abstract interface for serializing notification type masks.
 *
 *  @details Implementations convert @ref Draupnir::Notifications::NotificationTypes masks between their runtime representation and
 *           a configuration string representation. */

class NotificationTypesSerializerInterface
{
public:
    /*! @brief Virtual destructor. */
    virtual ~NotificationTypesSerializerInterface() = default;

    /*! @brief Parses notification types from a configuration string.
     *  @param string Configuration string to parse.
     *  @return Parsed notification type mask, or `std::nullopt` if `string` is invalid or unsupported. */
    [[nodiscard]] virtual std::optional<Draupnir::Notifications::NotificationTypes> fromConfigString(const QString& string) const = 0;

    /*! @brief Converts notification types to a configuration string.
     *  @param types Notification type mask to serialize.
     *  @return Configuration string representing `types`. */
    [[nodiscard]] virtual QString toConfigString(Draupnir::Notifications::NotificationTypes types) const = 0;
};

}; // namespace Draupnir::Notifications

#endif // NOTIFICATIONTYPESSERIALIZERINTERFACE_H
