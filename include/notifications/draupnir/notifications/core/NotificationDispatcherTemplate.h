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

#ifndef NOTIFICATIONSDISPATCHERTEMPLATE_H
#define NOTIFICATIONSDISPATCHERTEMPLATE_H

#include "draupnir/notifications/concepts/NotificationChannelConcept.h"
#include "draupnir/utils/filter_if.h"
#include "draupnir/utils/type_presense.h"

namespace Draupnir::Notifications
{

/*! @class NotificationDispatcherTemplate draupnir/notifications/core/NotificationDispatcherTemplate.h
 *  @brief Dispatches messages to notification handlers selected by a type mask.
 *  @ingroup Notifications
 *  @tparam NotificationChannels Channels describing the supported notification types.
 *
 *  @details Combines handlers described by the supplied notification traits. Stateful handlers are stored in the dispatcher instance,
 *           while stateless handlers are invoked through their static interface. */

template<NotificationChannelConcept... NotificationChannels>
class NotificationDispatcherTemplate
{
private:
    /*! @brief Adapts InstanceNotificationChannelConcept for use with @ref draupnir::utils::filter_if. */
    template<class Candidate>
    struct _InstanceNotificationChannelPredicate : std::bool_constant<
        InstanceNotificationChannelConcept<Candidate>
    > {};

    /*! @brief Tuple containing the stateful notification handlers. */
    using _StatefulHandlers = draupnir::utils::filter_if<
        _InstanceNotificationChannelPredicate, NotificationChannels...
    >::template to_container<std::tuple>;

public:
    /*! @brief Indicates whether the dispatcher contains any stateful handlers. */
    static inline constexpr bool hasInstanceChannels_v =
        (InstanceNotificationChannelConcept<NotificationChannels> || ...);

    /*! @brief Returns a configured stateful notification handler.
     *  @tparam NotificationChannel Channel identifying the requested handler.
     *  @return Reference to the corresponding handler instance. */
    template<NotificationChannelConcept NotificationChannel>
    inline auto& getNotificationHandler() requires (hasInstanceChannels_v) {
        static_assert(draupnir::utils::is_type_in_tuple_v<NotificationChannel, _StatefulHandlers>);
        return std::get<NotificationChannel>(m_managers);
    }

    /*! @brief Dispatches a message to the notification types enabled by a mask.
     *  @param mask Notification types that should receive the message.
     *  @param message Message to dispatch. Must not be null. */
    void showMessage(NotificationTypes mask, const Draupnir::Messages::MessagePtr& message) requires (hasInstanceChannels_v) {
        _showMessageImpl<NotificationChannels...>(mask, message);
    }

    /*! @brief Dispatches a message using stateless notification handlers.
     *  @param mask Notification types that should receive the message.
     *  @param message Message to dispatch. Must not be null. */
    static void showMessage(NotificationTypes mask, const Draupnir::Messages::MessagePtr& message) requires (!hasInstanceChannels_v) {
        _showMessageImpl<NotificationChannels...>(mask, message);
    }

    /*! @brief Dispatches a message list to the notification types enabled by a mask.
     *  @param mask Notification types that should receive the messages.
     *  @param messageList Messages to dispatch. */
    void showMessageList(NotificationTypes mask, const Draupnir::Messages::MessageList& messageList) requires (hasInstanceChannels_v) {
        _showMessageListImpl<NotificationChannels...>(mask, messageList);
    }

    /*! @brief Dispatches a message list using stateless notification handlers.
     *  @param mask Notification types that should receive the messages.
     *  @param messageList Messages to dispatch. */
    static void showMessageList(NotificationTypes mask, const Draupnir::Messages::MessageList& messageList) requires (!hasInstanceChannels_v) {
        _showMessageListImpl<NotificationChannels...>(mask, messageList);
    }

private:
    [[no_unique_address]] _StatefulHandlers m_managers;

    template<NotificationChannelConcept First, NotificationChannelConcept... Rest>
    inline void _showMessageImpl(NotificationTypes mask, const Draupnir::Messages::MessagePtr& message) requires (hasInstanceChannels_v) {
        if (mask.test_flag(First::Trait::value())) {
            if constexpr (InstanceNotificationChannelConcept<First>) {
                getNotificationHandler<First>().showMessage(message);
            } else {
                First::showMessage(message);
            };
        }

        if constexpr (sizeof...(Rest) > 0)
            _showMessageImpl<Rest...>(mask, message);
    }

    template<NotificationChannelConcept First, NotificationChannelConcept... Rest>
    static void _showMessageImpl(NotificationTypes mask, const Draupnir::Messages::MessagePtr& message) requires (!hasInstanceChannels_v) {
        if (mask.test_flag(First::Trait::value())) {
            First::showMessage(message);
        }

        if constexpr (sizeof...(Rest) > 0)
            _showMessageImpl<Rest...>(mask, message);
    }

    template<NotificationChannelConcept First, NotificationChannelConcept... Rest>
    inline void _showMessageListImpl(NotificationTypes mask, const Draupnir::Messages::MessageList& messageList) requires(hasInstanceChannels_v) {
        if (mask.test_flag(First::Trait::value())) {
            if constexpr (InstanceNotificationChannelConcept<First>) {
                getNotificationHandler<First>().showMessageList(messageList);
            } else {
                First::showMessageList(messageList);
            };
        }

        if constexpr (sizeof...(Rest) > 0)
            _showMessageListImpl<Rest...>(mask, messageList);
    }

    template<NotificationChannelConcept First, NotificationChannelConcept... Rest>
    static inline void _showMessageListImpl(NotificationTypes mask, const Draupnir::Messages::MessageList& messageList) requires(!hasInstanceChannels_v) {
        if (mask.test_flag(First::Trait::value()))
            First::showMessageList(messageList);

        if constexpr (sizeof...(Rest) > 0)
            _showMessageListImpl<Rest...>(mask, messageList);
    }
};

} // namespace Draupnir::Notifications

#endif // NOTIFICATIONSDISPATCHERTEMPLATE_H
