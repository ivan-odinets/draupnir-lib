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

#include "draupnir/core/MessageHandler.h"

#include "draupnir/traits/settings/MessageTypeSettingsTrait.h"

#include "draupnir/SettingsBundleTemplate.h"

namespace Draupnir::Messages
{

/*! @class MessageHandlerTemplate draupnir/core/MessageHandlerTemplate.h
 *  @brief Templated implementation of MessageHandler based on statically known message types.
 *  @details It extends MessageHandler and therefore can be used wherever a MessageHandler pointer is expected. Policies for each
 *           message type are defined at compile time through MessageTraits... and can be persisted via AppSettings or a custom
 *           MessageSettingsInterface.
 *
 *           MessageSystemTemplate creates a single instance of this class and shares it with MessageUiBuilderTemplate so that UI
 *           elements reflect and modify the same notification policy map.
 *
 * @tparam MessageTraits... List of message types supported by this handler.
 * @see MessageHandler, Logger, fixed_map */

template<class... MessageTraits>
class MessageHandlerTemplate final : public MessageHandler
{
public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        MessageTypeSettingsTrait<MessageTraits>...
    >;

    template<class MessageTrait>
    static constexpr bool contains() { return is_one_of_v<MessageTrait,MessageTraits...>; }

    /*! @brief Default constructor. */
    MessageHandlerTemplate() :
        m_settings{}
    {}

    /*! @brief Destructor. */
    ~MessageHandlerTemplate() final = default;

    template<class SettingsRegistry>
    void loadSettings(SettingsRegistry* registry) {
        m_settings = registry->template getSettingsBundle<SettingsBundle>();
    }

    /*! @brief Returns the current notification type associated with the given message type ID. */
    Notification::Type notification(uint64_t messageId) final {
        return _notificationImpl<MessageTraits...>(messageId);
    }

    template<class MessageTrait>
    Notification::Type notification() {
        return _notificationImpl<MessageTrait,MessageTraits...>();
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

    /*! @brief Sets the notification type for the given message type ID and updates settings. */
    void setNotification(MessageType messageId, Notification::Type type) final {
        _setImpl<MessageTraits...>(messageId, type);

        emit notificationTypeChanged(messageId, type);
    };

private:
    SettingsBundle m_settings;

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
