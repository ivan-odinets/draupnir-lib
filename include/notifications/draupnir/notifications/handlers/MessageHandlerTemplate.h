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

#ifndef MESSAGEHANDLERTEMPLATE_H
#define MESSAGEHANDLERTEMPLATE_H

#include "draupnir/logging/core/AbstractMessageHandler.h"
#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/notifications/concepts/MessageTypeTraitConcept.h"
#include "draupnir/notifications/concepts/NotificationTypeConcept.h"
#include "draupnir/notifications/handlers/NotificationDispatcherTemplate.h"
#include "draupnir/notifications/traits/settings/MessageNotificationSettingTrait.h"

namespace Draupnir::Notifications
{

template<class MessageTypesWrapper, class NotificationTypesWrapper>
class MessageHandlerTemplate;

template<MessageTypeTraitConcept... MessageType>
struct MessageTypesWrapper {};

template<Draupnir::Notifications::NotificationTypeTraitConcept... NotificationTypeTraits>
struct NotificationTypesWrapper {};

template<
    MessageTypeTraitConcept... MessageTypeTraits,
    Draupnir::Notifications::NotificationTypeTraitConcept... NotificationTraits
>
class MessageHandlerTemplate<MessageTypesWrapper<MessageTypeTraits...>, NotificationTypesWrapper<NotificationTraits...>>
    : public Draupnir::Logging::AbstractMessageHandler
{
    using _NotificationDispatcher = Draupnir::Notifications::NotificationDispatcherTemplate<
        NotificationTraits...
    >;

public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        MessageNotificationSettingTrait<MessageTypeTraits>...
    >;

    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>());
        m_settings = source->template getSettingsBundle<SettingsBundle>();
    }

    NotificationTypes notificationFor(Draupnir::Logging::MessageType type) {
        return _notificationForImpl<MessageTypeTraits...>(type);
    }

    void setNotificationFor(Draupnir::Logging::MessageType type, NotificationTypes mask) {
        _setNotificationForImpl<MessageTypeTraits...>(type, mask);
    }

    void handleMessage(Draupnir::Logging::Message* message) final {
        NotificationTypes notificationMask = notificationFor(message->type());
        if (notificationMask.none())
            return;

        if constexpr (_NotificationDispatcher::hasAnyStatefullHandlers_v) {
            m_notificationDispatcher.showMessage(notificationMask, message);
        } else {
            _NotificationDispatcher::showMessage(notificationMask, message);
        }
    };

    void handleMessageList(const Draupnir::Logging::MessageList& messageList) final {
        QMap<NotificationTypes, Draupnir::Logging::MessageList> notificationTypesMap;
        for (Draupnir::Logging::Message* message : messageList) {
            NotificationTypes mask = notificationFor(message->type());
            if (mask.any())
                notificationTypesMap[mask].append(message);
        }

        for (auto i = notificationTypesMap.cbegin(); i != notificationTypesMap.cend(); i++) {
            if constexpr (_NotificationDispatcher::hasAnyStatefullHandlers_v) {
                m_notificationDispatcher.showMessageList(i.key(), i.value());
            } else {
                _NotificationDispatcher::showMessageList(i.key(), i.value());
            }
        }
    }

private:
    SettingsBundle m_settings;
    [[no_unique_address]] _NotificationDispatcher m_notificationDispatcher;

    template<MessageTypeTraitConcept First, MessageTypeTraitConcept... Rest>
    inline NotificationTypes _notificationForImpl(Draupnir::Logging::MessageType type) {
        if (First::type() == type) {
            return m_settings.template get<MessageNotificationSettingTrait<First>>();
        }

        if constexpr (sizeof...(Rest) > 0)
            return _notificationForImpl<Rest...>(type);
        else
            return NotificationType::NoNotification;
    }

    template<MessageTypeTraitConcept First, MessageTypeTraitConcept... Rest>
    inline void _setNotificationForImpl(Draupnir::Logging::MessageType type, NotificationTypes mask) {
        if (First::type() == type) {
            m_settings.template set<MessageNotificationSettingTrait<First>>(mask);
            return;
        }

        if constexpr (sizeof...(Rest) > 0)
            _setNotificationForImpl<Rest...>(type, mask);
    }
};

};























// {






//     static constexpr bool isMessageTypeKnown(Draupnir::Logging::MessageType type) {
//         return _isMessageTypeKnownImpl<MessageTypeTraits...>(type);
//     }

//     template<MessageTypeTraitConcept First>
//     Draupnir::Notifications::NotificationTypes notificationsFor() {
//         return m_settings.template get<MessageTypeNotificationSettingTraitTemplate<First>>();
//     }

//     void handleMessage(Draupnir::Logging::Message* message) final {
//         if (!isMessageTypeKnown(message->type()))
//             return;
//     }

//     void handleMessageList(const Draupnir::Logging::MessageList& /*messageList*/) final {}

// private:


//     Draupnir::Notifications::NotificationsDisplayerTemplate<
//         NotificationTraits...
//     > m_notificationManager;

//     template<MessageTypeTraitConcept First, MessageTypeTraitConcept... Rest>
//     static inline bool _isMessageTypeKnownImpl(Draupnir::Logging::MessageType type) {
//         if (First::type() == type)
//             return true;

//         if constexpr (sizeof...(Rest) > 0)
//             return _isMessageTypeKnownImpl<Rest...>();
//         else
//             return false;
//     }

//     template<MessageTypeTraitConcept First, MessageTypeTraitConcept... Rest>
//     inline void _handleMessageImpl(Draupnir::Logging::Message* message) {
//         if (First::type() == message->type()) {
//             const auto mask = notificationsFor<First>();
//             if (mask.any()) {
//                 m_notificationManager.showMessage(mask, message);
//             }
//         }

//         if constexpr (sizeof...(Rest) > 0)
//             _handleMessageImpl<Rest...>(message);
//     }
// };

#endif // MESSAGEHANDLERTEMPLATE_H
