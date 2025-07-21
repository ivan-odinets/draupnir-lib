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

#include "MessageHandler.h"

#include "../../include/containers/fixed_map.h"
#include "MessageTraitsHelper.h"

#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    #include "AppSettings.h"
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS

#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    #include "MessageSettingsInterface.h"
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

/*! @class MessageHandlerTemplate draupnir-lib/src/messages/core/MessageHandlerTemplate.h
 *  @brief Templated implementation of MessageHandler based on statically known message types.
 *  @details It extends MessageHandler and therefore can be used wherever a MessageHandler pointer is expected. Policies for each
 *           message type are defined at compile time through MessageTraits... and can be persisted via AppSettings or a custom
 *           MessageSettingsInterface.
 *
 *           MessageSystemTemplate creates a single instance of this class and shares it with MessageUiBuilderTemplate so that UI
 *           elements reflect and modify the same notification policy map.
 *
 * @tparam MessageTraits... List of message types supported by this handler.
 * @see MessageHandler, Logger, fixed_map
 * @todo Decide how we will store settings. Where to add the prefix? Here? Or within the settings object? */

template<class... MessageTraits>
class MessageHandlerTemplate final : public MessageHandler
{
public:
    /*! @brief Default constructor. */
    MessageHandlerTemplate() :
#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)
        p_settings{nullptr},
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
        m_policyMap{}
    {
        _initPolicyMap<MessageTraits...>();
    }

    /*! @brief Destructor. */
    ~MessageHandlerTemplate() final = default;

#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)

    /*! @brief This method is used to load settings for this MessageHandlerTemplate object from the provided object.
     * @note This method will be present when either DRAUPNIR_MSGSYS_APP_SETTINGS or DRAUPNIR_MSGSYS_CUSTOM_SETTINGS macros are enabled.
     *       When first macro is enabled - this method will accept AppSettings pointer. When second - MessageSystemInterface. */
#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    void loadSettings(AppSettings* settings)
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    void loadSettings(MessageSettingsInterface* settings)
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    {
        Q_ASSERT_X(p_settings == nullptr,"MessageHandlerTemplate::loadSettings",
                   "This method must be called only once.");
        Q_ASSERT_X(settings,"MessageHandlerTemplate::loadSettings",
                   "Provided object pointer is nullptr");

        p_settings = settings;

        _loadSettingsImpl<MessageTraits...>();
    }
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

    /*! @brief Returns the current notification type associated with the given message type ID. */
    Notification::Type notification(uint64_t messageId) final {
        return m_policyMap[messageId];
    }

    /*! @brief Sets the notification type for the given message type ID and updates settings. */
    void setNotification(MessageType messageId, Notification::Type type) final {
#if defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS) || defined(DRAUPNIR_MSGSYS_APP_SETTINGS)
        Q_ASSERT_X(p_settings,"MessageHandlerTemplate::setNotification",
                   "MessageHandlerTemplate::loadSettings must have been called before.");
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS || DRAUPNIR_MSGSYS_APP_SETTINGS

        _setImpl<MessageTraits...>(messageId, type);

        emit notificationTypeChanged(messageId, type);
    };

    /*! @brief Returns iterator pointing to the beginning of the internal fixed_map. */
    auto notificationMapBegin() { return m_policyMap.begin(); }

    /*! @brief Returns iterator pointing to the end of the internal fixed_map. */
    auto notificationMapEnd() { return m_policyMap.end(); }

private:
#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    AppSettings* p_settings;
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    MessageSettingsInterface* p_settings;
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

    /*! @brief Mapping from message ID to its configured Notification::Type. */
    fixed_map<
        MessageTraitsHelper<MessageTraits...>::supportedMessageIds,
        Notification::Type
    > m_policyMap;

    template<class First,class... Rest>
    inline constexpr void _initPolicyMap() {
        m_policyMap[First::type] = First::defaultNotification;

        if constexpr (sizeof...(Rest) > 0)
            _initPolicyMap<Rest...>();
    }

#if defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS) || defined(DRAUPNIR_MSGSYS_APP_SETTINGS)
    /*! @brief Recursive template implementation to load settings for each message type. */
    template<class First,class... Rest>
    inline void _loadSettingsImpl() {
        m_policyMap.get(First::type) = p_settings->value<Notification>(
            QString("notifications/").append(First::settingsKey),
            First::defaultNotification
        );

        if constexpr (sizeof...(Rest) > 0) {
            _loadSettingsImpl<Rest...>();
        }
    }
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS || DRAUPNIR_MSGSYS_APP_SETTINGS

    /*! @brief Recursive template implementation to update notification type for a specific message. */
    template<class First,class... Rest>
    inline void _setImpl(MessageType messageId, Notification::Type type) {
        if (First::type == messageId) {
            m_policyMap.get(messageId) = type;
#if defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS) || defined(DRAUPNIR_MSGSYS_APP_SETTINGS)
            p_settings->setValue<Notification>(
                QString("notifications/").append(First::settingsKey), type
            );
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS || DRAUPNIR_MSGSYS_APP_SETTINGS
            return;
        }

        if constexpr (sizeof...(Rest) > 0) {
            _setImpl<Rest...>(messageId,type);
        }
    }
};

#endif // MESSAGEHANDLERTEMPLATE_H
