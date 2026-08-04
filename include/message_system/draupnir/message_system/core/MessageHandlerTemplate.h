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

#include "draupnir/messages/concepts/MessageTypeTraitConcept.h"
#include "draupnir/messages/models/MessageListModel.h"
#include "draupnir/message_system/core/AbstractMessageHandler.h"
#include "draupnir/notifications/concepts/NotificationChannelConcept.h"
#include "draupnir/notifications/core/NotificationDispatcherTemplate.h"
#include "draupnir/notifications/traits/settings/MessageNotificationSettingTraitTemplate.h"
#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/utils/class_macros.h"

namespace Draupnir::MessageSystem
{

namespace TemplateArgs
{

template<Draupnir::Messages::MessageTypeTraitConcept...>
struct MessageTypeTraitsWrapper { DRAUPNIR_DEFINE_COMPILE_TIME(MessageTypeTraitsWrapper); };

template<Draupnir::Notifications::NotificationChannelConcept...>
struct NotificationChannelsWrapper { DRAUPNIR_DEFINE_COMPILE_TIME(NotificationChannelsWrapper); };

}; // namespace TemplateArgs


template<class, class>
class MessageHandlerTemplate
{
    static_assert(false);
};

template<
    Draupnir::Messages::MessageTypeTraitConcept... MessageTypeTraits,
    Draupnir::Notifications::NotificationChannelConcept... NotificationChannels
>
class MessageHandlerTemplate<
    TemplateArgs::MessageTypeTraitsWrapper<MessageTypeTraits...>,
    TemplateArgs::NotificationChannelsWrapper<NotificationChannels...>
> : public AbstractMessageHandler
{
    using _NotificationDispatcher = Draupnir::Notifications::NotificationDispatcherTemplate<
        NotificationChannels...
    >;

public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Notifications::MessageNotificationSettingTraitTemplate<MessageTypeTraits>...
    >;

    static constexpr bool isMessageTypeKnown(const Draupnir::Messages::MessageType& type) {
        return ((MessageTypeTraits::type() == type) || ...);
    }

    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>());
        Q_ASSERT(source);
        m_settings = source->template getSettingsBundle<SettingsBundle>();
    }

    Draupnir::Messages::MessageListModel* messages() { return &m_messageListModel; }

    void handleMessage(const Draupnir::Messages::MessagePtr& message) final {
        m_messageListModel.append(message);

        auto notificationMask = notificationTypesFor(message->type());
        if (notificationMask.none())
            return;

        if constexpr (_NotificationDispatcher::hasInstanceChannels_v) {
            m_notificationDispatcher.showMessage(notificationMask, message);
        } else {
            _NotificationDispatcher::showMessage(notificationMask, message);
        }
    };

    void handleMessageList(const Draupnir::Messages::MessageList& messageList) final {
        m_messageListModel.append(messageList);

        QMap<Draupnir::Notifications::NotificationTypes, Draupnir::Messages::MessageList> notificationTypesMap;
        for (Draupnir::Messages::MessagePtr message : messageList) {
            const auto mask = notificationTypesFor(message->type());
            if (mask.any())
                notificationTypesMap[mask].append(message);
        }

        for (auto i = notificationTypesMap.cbegin(); i != notificationTypesMap.cend(); i++) {
            if constexpr (_NotificationDispatcher::hasInstanceChannels_v) {
                m_notificationDispatcher.showMessageList(i.key(), i.value());
            } else {
                _NotificationDispatcher::showMessageList(i.key(), i.value());
            }
        }
    }

    Notifications::NotificationTypes notificationTypesFor(
        Draupnir::Messages::MessageType messageType
    ) const final {
        return _notificationTypesForImpl<MessageTypeTraits...>(messageType);
    };

    void setNotificationTypesFor(
        Draupnir::Messages::MessageType messageType,
        Draupnir::Notifications::NotificationTypes notificationTypes
    ) final {
        _setNotificationTypesForImpl<MessageTypeTraits...>(messageType, notificationTypes);
    };

private:
    SettingsBundle m_settings;
    Messages::MessageListModel m_messageListModel;
    [[no_unique_address]] _NotificationDispatcher m_notificationDispatcher;

    template<Draupnir::Messages::MessageTypeTraitConcept First, Draupnir::Messages::MessageTypeTraitConcept... Rest>
    inline Draupnir::Notifications::NotificationTypes _notificationTypesForImpl(
        Draupnir::Messages::MessageType type
    ) const {
        if (First::type() == type) {
            return m_settings.template get<Draupnir::Notifications::MessageNotificationSettingTraitTemplate<First>>();
        }

        if constexpr (sizeof...(Rest) > 0)
            return _notificationTypesForImpl<Rest...>(type);
        else
            return Draupnir::Notifications::NotificationType::NoNotification;
    }

    template<Draupnir::Messages::MessageTypeTraitConcept First, Draupnir::Messages::MessageTypeTraitConcept... Rest>
    inline void _setNotificationTypesForImpl(
        Draupnir::Messages::MessageType messageType,
        Draupnir::Notifications::NotificationTypes notificationTypes
    ) {
        if (First::type() == messageType) {
            m_settings.template set<Draupnir::Notifications::MessageNotificationSettingTraitTemplate<First>>(notificationTypes);
            return;
        }

        if constexpr (sizeof...(Rest) > 0)
            _setNotificationTypesForImpl<Rest...>(messageType, notificationTypes);
    }
};

} // namespace Deaupnir::MessageSystem

#endif // MESSAGEHANDLERTEMPLATE_H
