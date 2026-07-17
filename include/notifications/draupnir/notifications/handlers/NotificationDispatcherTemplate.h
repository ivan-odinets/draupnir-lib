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

#include "draupnir/notifications/concepts/NotificationTypeConcept.h"
#include "draupnir/notifications/concepts/NotificationHandlerConcepts.h"
#include "draupnir/notifications/handlers/NotificationHandlerTemplate.h"
#include "draupnir/utils/filter_if.h"

namespace Draupnir::Notifications
{

/*! @brief Dispatches messages to notification handlers selected by a type mask.
 *  @ingroup Notifications
 *  @tparam NotificationTraits Traits describing the supported notification types. * *
 *
 *  @details Combines handlers described by the supplied notification traits. Stateful handlers are stored in the dispatcher instance,
 *           while stateless handlers are invoked through their static interface. */

template<NotificationTypeTraitConcept... NotificationTraits>
class NotificationDispatcherTemplate
{
private:
    /*! @brief Adapts StatefulHandlerConcept for use with @ref draupnir::utils::filter_if. */
    template<class Candidate>
    struct _StatefullHandlerAdapter : std::bool_constant<
        StatefulHandlerConcept<Candidate>
    > {};

    /*! @brief Tuple containing the stateful notification handlers. */
    using _RuntimeNotificationManagers = draupnir::utils::filter_if<
        _StatefullHandlerAdapter, NotificationHandlerTemplate<NotificationTraits>...
    >::template to_container<std::tuple>;

public:
    /*! @brief Indicates whether the dispatcher contains any stateful handlers. */
    static inline constexpr bool hasAnyStatefullHandlers_v =
        (StatefulHandlerConcept<NotificationHandlerTemplate<NotificationTraits>> || ...);

    /*! @brief Returns a configured stateful notification handler.
     *  @tparam NotificationTrait Trait identifying the requested handler.
     *  @return Reference to the corresponding handler instance. */
    template<NotificationTypeTraitConcept NotificationTrait>
    inline auto& getNotificationHandler() requires (hasAnyStatefullHandlers_v) {
        return std::get<NotificationHandlerTemplate<NotificationTrait>>(m_managers);
    }

    /*! @brief Dispatches a message to the notification types enabled by a mask.
     *  @param mask Notification types that should receive the message.
     *  @param message Message to dispatch. Must not be null. */
    void showMessage(NotificationTypes mask, Draupnir::Logging::Message* message) requires (hasAnyStatefullHandlers_v) {
        _showMessageImpl<NotificationTraits...>(mask, message);
    }

    /*! @brief Dispatches a message using stateless notification handlers.
     *  @param mask Notification types that should receive the message.
     *  @param message Message to dispatch. Must not be null. */
    static void showMessage(NotificationTypes mask, Draupnir::Logging::Message* message) requires (!hasAnyStatefullHandlers_v) {
        _showMessageImpl<NotificationTraits...>(mask, message);
    }

    /*! @brief Dispatches a message list to the notification types enabled by a mask.
     *  @param mask Notification types that should receive the messages.
     *  @param messageList Messages to dispatch. */
    void showMessageList(NotificationTypes mask, const Draupnir::Logging::MessageList& messageList) requires (hasAnyStatefullHandlers_v) {
        _showMessageListImpl<NotificationTraits...>(mask, messageList);
    }

    /*! @brief Dispatches a message list using stateless notification handlers.
     *  @param mask Notification types that should receive the messages.
     *  @param messageList Messages to dispatch. */
    static void showMessageList(NotificationTypes mask, const Draupnir::Logging::MessageList& messageList) requires (!hasAnyStatefullHandlers_v) {
        _showMessageListImpl<NotificationTraits...>(mask, messageList);
    }

private:
    [[no_unique_address]] _RuntimeNotificationManagers m_managers;

    template<NotificationTypeTraitConcept First, NotificationTypeTraitConcept... Rest>
    inline void _showMessageImpl(NotificationTypes mask, Draupnir::Logging::Message* message) requires (hasAnyStatefullHandlers_v) {
        if (mask.test_flag(First::value())) {
            if constexpr (StatefulHandlerConcept<NotificationHandlerTemplate<First>>) {
                getNotificationHandler<First>().showMessage(message);
            } else {
                NotificationHandlerTemplate<First>::showMessage(message);
            };
        }

        if constexpr (sizeof...(Rest) > 0)
            _showMessageImpl<Rest...>(mask, message);
    }

    template<NotificationTypeTraitConcept First, NotificationTypeTraitConcept... Rest>
    static void _showMessageImpl(NotificationTypes mask, Draupnir::Logging::Message* message) requires (!hasAnyStatefullHandlers_v) {
        if (mask.test_flag(First::value())) {
            NotificationHandlerTemplate<First>::showMessage(message);
        }

        if constexpr (sizeof...(Rest) > 0)
            _showMessageImpl<Rest...>(mask, message);
    }

    template<NotificationTypeTraitConcept First, NotificationTypeTraitConcept... Rest>
    inline void _showMessageListImpl(NotificationTypes mask, const Draupnir::Logging::MessageList& messageList) requires(hasAnyStatefullHandlers_v) {
        if (mask.test_flag(First::value())) {
            if constexpr (StatefulHandlerConcept<NotificationHandlerTemplate<First>>) {
                getNotificationHandler<First>().showMessageList(messageList);
            } else {
                NotificationHandlerTemplate<First>::showMessageList(messageList);
            };
        }

        if constexpr (sizeof...(Rest) > 0)
            _showMessageListImpl<Rest...>(mask, messageList);
    }

    template<NotificationTypeTraitConcept First, NotificationTypeTraitConcept... Rest>
    static inline void _showMessageListImpl(NotificationTypes mask, const Draupnir::Logging::MessageList& messageList) requires(!hasAnyStatefullHandlers_v) {
        if (mask.test_flag(First::value()))
            NotificationHandlerTemplate<First>::showMessageList(messageList);

        if constexpr (sizeof...(Rest) > 0)
            _showMessageListImpl<Rest...>(mask, messageList);
    }
};

}; // namespace Draupnir::Notifications

#endif // NOTIFICATIONSDISPATCHERTEMPLATE_H
