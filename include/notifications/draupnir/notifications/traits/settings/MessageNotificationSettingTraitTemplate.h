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

#ifndef MESSAGENOTIFICATIONSETTINGTRAITTEMPLATE_H
#define MESSAGENOTIFICATIONSETTINGTRAITTEMPLATE_H

#include "draupnir/messages/concepts/MessageTypeTraitConcept.h"
#include "draupnir/notifications/core/NotificationTypes.h"

namespace Draupnir::Notifications
{

/*! @struct MessageNotificationSettingTrait draupnir/notifications/traits/settings/MessageNotificationSettingTrait.h
 *  @ingroup Notifications
 *  @brief Settings trait for notification types assigned to a message type.
 *  @tparam MessageTypeTrait Trait describing the message type whose notification setting is represented.
 *
 *  @details Uses the message type configuration token as the setting key. By default, notifications are disabled for the represented
 *           message type. */

template<Draupnir::Messages::MessageTypeTraitConcept MessageTypeTrait>
struct MessageNotificationSettingTraitTemplate
{
public:
    /*! @brief Value type stored by this setting. */
    using Value = Draupnir::Notifications::NotificationTypes;

    /*! @brief Returns the configuration key for this setting.
     *  @return Configuration token provided by `MessageTypeTrait`. */
    [[nodiscard]] static QString key() { return MessageTypeTrait::configString(); }

    /*! @brief Returns the default notification type mask.
     *  @return Empty notification mask representing disabled notifications.
     * @todo Question: Maybe there is a way to setup default value somehow through the preprocessor for different MessageTypeTraits. */
    [[nodiscard]]static constexpr Draupnir::Notifications::NotificationTypes defaultValue() {
        return Draupnir::Notifications::NotificationType::NoNotification;
    }
};

} // namespace Draupnir::Notifications

#endif // MESSAGENOTIFICATIONSETTINGTRAITTEMPLATE_H
