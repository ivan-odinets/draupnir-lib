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

#ifndef NOTIFICATIONTYPESSERIALIZERTEMPLATE_H
#define NOTIFICATIONTYPESSERIALIZERTEMPLATE_H

#include "draupnir/notifications/concepts/NotificationTypeConcept.h"
#include "draupnir/notifications/core/NotificationTypeSerializerInterface.h"
#include "draupnir/notifications/traits/NoNotificationTrait.h"
#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"

namespace Draupnir::Notifications
{

/*! @brief This is a class
 *  @ingroup Notifications
 * @todo Documentation: Write documentation */

template<NotificationTypeTraitConcept... Traits>
class NotificationTypesSerializerTemplate final : public NotificationTypeSerializerInterface
{
public:
    ~NotificationTypesSerializerTemplate() final = default;
    using SingleNotificationSerializer = Draupnir::Settings::FlagSerializerTemplate<
        NotificationType,
        Traits...
    >;

    using Serialzier = Draupnir::Settings::FlagsMaskSerializerTemplate<
        NotificationTypes,
        SingleNotificationSerializer,
        NoNotificationTrait
    >;

    std::optional<NotificationTypes> fromConfigString(const QString& string) const final {
        return Serialzier::fromConfigString(string);
    }

    QString toConfigString(NotificationTypes mask) const final {
        return Serialzier::toConfigString(mask);
    }
};

}; // namespace Draupnir::Notifications

#endif // NOTIFICATIONTYPESSERIALIZERTEMPLATE_H
