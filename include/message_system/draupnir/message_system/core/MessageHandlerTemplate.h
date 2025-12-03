/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/message_system/core/AbstractMessageHandler.h"
#include "draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h"

#include "draupnir/SettingsRegistry.h"

namespace Draupnir::Messages
{

/*! @class MessageHandlerTemplate<class... MessageTraits> draupnir/message_system/core/MessageHandlerTemplate.h
 *  @ingroup MessageSystem
 *  @tparam MessageTraits... List of message types supported by this handler.
 *  @brief Templated implementation of @ref Draupnir::Messages::AbstractMessageHandler based on message types specified.
 *
 *  @details It extends @ref Draupnir::Messages::AbstractMessageHandler and stores policies for each message type.
 *
 *           @ref Draupnir::Messages::MessageSystemTemplate creates a single instance of this class and shares it with @ref
 *           Draupnir::Messages::AbstractMessageUiBuilder / @ref Draupnir::Messages::MessageUiBuilderTemplate so that UI
 *           elements reflect and modify the same notification policy map. */


template<class... MessageTraits>
class MessageHandlerTemplate final : public AbstractMessageHandler
{
public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        MessageTypeSettingsTrait<MessageTraits>...
    >;

    /*! @brief Returns `true` if the specified `MessageTrait` is known to this MessageHandlerTemplate instantiation. */
    template<class MessageTrait>
    static constexpr bool contains() { return draupnir::utils::is_one_of_v<MessageTrait,MessageTraits...>; }

    /*! @brief Alias constexpr variable containing `true` if this MessageHandlerTemplate instantiation knows the
     *         specified `MessageTrait`. */
    template<class MessageTrait>
    static constexpr bool contains_v = contains<MessageTrait>();

    /*! @brief Returns `true` if the specified @ref Draupnir::Messages::MessageType is known to this MessageHandlerTemplate
     *         instantiation. */
    bool contains(MessageType type) const final { return _containsImpl<MessageTraits...>(type); }

    /*! @brief Default constructor. */
    MessageHandlerTemplate() :
        m_settings{}
    {}

    /*! @brief Destructor. */
    ~MessageHandlerTemplate() final = default;

    /*! @brief Loads notification settings from the provided SettingsSource. */
    template<class SettingsSource>
    void loadSettings(SettingsSource* registry) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>(),
                "Provided SettingsSource can not provide all settings needed for this MessageHandlerTemplate.");
        m_settings = registry->template getSettingsBundle<SettingsBundle>();
    }

    /*! @brief Returns the current notification type associated with the given message type ID. */
    Notification::Type notification(uint64_t messageId) final {
        return _notificationImpl<MessageTraits...>(messageId);
    }

    /*! @brief Returns the current notification type associated with the provided `MessageTrait`. */
    template<class MessageTrait>
    Notification::Type notification() {
        return _notificationImpl<MessageTrait,MessageTraits...>();
    }

    /*! @brief Sets the notification type for the given message type ID and updates settings.
     * @todo Maybe it make sense to emit signal only if the notification type has changes. */
    void setNotification(MessageType messageId, Notification::Type type) final {
        _setImpl<MessageTraits...>(messageId, type);

        emit notificationTypeChanged(messageId, type);
    };

private:
    SettingsBundle m_settings;

    template<class First, class... Rest>
    inline bool _containsImpl(MessageType type) const {
        if (First::type == type)
            return true;

        if constexpr (sizeof...(Rest) > 0)
            return _containsImpl<Rest...>(type);
        else
            return false;
    }

    template<class First, class... Rest>
    Notification::Type _notificationImpl(uint64_t messageId) {
        if (uint64_t(First::type) == messageId) {
            return m_settings.template get<MessageTypeSettingsTrait<First>>();
        }

        if constexpr (sizeof...(Rest) > 0)
            return _notificationImpl<Rest...>(messageId);
        else
            return Notification::UnknownType;
    }

    template<class MessageTrait, class First, class... Rest>
    Notification::Type _notificationImpl() {
        if constexpr (std::is_same_v<First,MessageTrait>) {
            return m_settings.template get<MessageTypeSettingsTrait<First>>();
        }

        if constexpr (sizeof...(Rest) > 0)
            return _notificationImpl<MessageTrait,Rest...>();
        else
            return Notification::UnknownType;
    }

    /*! @brief Recursive template implementation to update notification type for a specific message. */
    template<class First,class... Rest>
    inline void _setImpl(MessageType messageId, Notification::Type type) {
        if (First::type == messageId) {
            m_settings.template set<MessageTypeSettingsTrait<First>>(type);
            return;
        }

        if constexpr (sizeof...(Rest) > 0) {
            _setImpl<Rest...>(messageId,type);
        }
    }
};

}; // namespace Draupnir::Messages

#endif // MESSAGEHANDLERTEMPLATE_H
