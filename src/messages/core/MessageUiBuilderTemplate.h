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

#ifndef MESSAGEUIBUILDERTEMPLATE_H
#define MESSAGEUIBUILDERTEMPLATE_H

#include "MessageUiBuilder.h"
#include "MessageHandlerTemplate.h"

#include "../menus/NotificationTypeMenu.h"
#include "../widgets/LogWidget.h"
#include "../widgets/MessageNotificationSettingsWidget.h"

/*! @class MessageUiBuilderTemplate draupnir-lib/src/messages/core/MessageUiBuilderTemplate.h
 *  @brief Default template-based implementation of MessageUiBuilder for a given list of message types.
 *  @details This class creates a LogWidget and binds it to the message handler and settings. The template parameters define the
 *           supported message types.
 *
 * @tparam MessageTypes List of message type traits defining messages handled by this builder.
 *
 * @see MessageSystemTemplate, MessageUiBuilder, LogWidget */

template<class... MessageTypes>
class MessageUiBuilderTemplate final : public MessageUiBuilder
{
public:
    /*! @brief Creates and returns a LogWidget configured with the current handler and settings.
     *  @details This method is intended to be used by GUI code that needs a message log viewer.
     *  @param parent Optional parent widget for the created LogWidget.
     *  @return QWidget* Pointer to the fully configured LogWidget. */
    QWidget* createConfiguredLogWidget(QWidget* parent = nullptr) const final {
#if defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS) || defined(DRAUPNIR_MSGSYS_APP_SETTINGS)
        Q_ASSERT_X(p_settings, "MessageUiBuilderTemplate::createConfiguredLogWidget",
                   "MessageUiBuilder::loadSettings method must have been called before.");
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS || DRAUPNIR_MSGSYS_APP_SETTINGS
        LogWidget* result = new LogWidgetTemplate<MessageTypes...>{parent};

        result->setMessageListModel(p_handler->messages());

#if defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS) || defined(DRAUPNIR_MSGSYS_APP_SETTINGS)
        result->loadSettings(p_settings);
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS || DRAUPNIR_MSGSYS_APP_SETTINGS
        return result;
    }

    /*! @brief Creates and returns NotificationTypeMenu (as a pointer to QMenu) which is ready-to-use for changing the type of
     *         notification for specified MessageType.
     *  @param messageType Type of message notification for which will be configured by this NotificationTypeMenu.
     *  @param parent Optional parent widget. */
    QMenu* createConfiguredNotificationMenu(MessageType messageType, QWidget* parent = nullptr) const final {
        NotificationTypeMenu* result = new NotificationTypeMenu{parent};
        result->setNotificationType(p_handler->notification(messageType));

        // MessageHandler is informing about changes of notification policies by emitting signal MessageHandler::notificationTypeChanged
        // with two arguments: type of message and type of notification. As we want this NotificationTypeMenu to be responsible only for
        // ONE specific type of messages - we use a lambda to catch signal from MessageHandler and IF it is related to this particular
        // notificationTypeMenu and IF this menu was not deleted - we can process this.
        QPointer<NotificationTypeMenu> targetMenu{result};
        QObject::connect(static_cast<MessageHandler*>(p_handler),&MessageHandler::notificationTypeChanged,[targetMenu,messageType](MessageType changedMsgType,Notification::Type notification){
            if (changedMsgType != messageType)
                return;

            if (targetMenu.isNull())
                return;

            targetMenu->setNotificationType(notification);
        });

        QPointer<MessageHandler> handlerPtr{static_cast<MessageHandler*>(p_handler)};
        QObject::connect(result, &NotificationTypeMenu::notificationTypeChanged,[handlerPtr,messageType](Notification::Type notification){
            if (handlerPtr.isNull())
                return;

            handlerPtr->setNotification(messageType,notification);
        });

        return result;
    };

    /*! @brief Return a ready-to-use widget for configuring notification settings within this MessageSystem.
     *  @param parent Optional parent widget.
     * @note Its caller responsibility to manage memory properly. */
    QWidget* createConfiguredNotificationSettingsWidget(QWidget* parent = nullptr) const final {
        MessageNotificationSettingsWidgetTemplate<MessageTypes...>* result =
                new MessageNotificationSettingsWidgetTemplate<MessageTypes...>(parent);

        result->showNotificationSettings(p_handler);
        return result;
    };

protected:
    template<class... Args>
    friend class MessageSystemTemplate;

    MessageUiBuilderTemplate() :
#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)
        p_settings{nullptr},
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
        p_handler{nullptr}
    {}

    /*! @brief Associates a specific MessageHandlerTemplate with this UI builder.
     *  @details This is required before any UI widgets can be created. The handler is used to feed messages into the UI.
     *  @param handler Pointer to a MessageHandlerTemplate. */
    void setMessageHandlerTemplate(MessageHandlerTemplate<MessageTypes...>* handler) {
        Q_ASSERT_X(handler, "MessageUiBuilderTemplate::setMessageHandlerTemplate",
                   "Provided handler is nullptr.");
        Q_ASSERT_X(p_handler == nullptr, "MessageUiBuilderTemplate::setMessageHandlerTemplate",
                   "Provided handler is nullptr.");

        p_handler = handler;
    }


#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined (DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)

    /*! @brief Loads persistent application settings into the UI builder.
     *  @details The settings are typically passed from the application and used to configure the LogWidget appearance and behavior.
     *  @param settings Pointer to the AppSettings or MessageSettingsInterface instance.
     * @note This method will be present when either DRAUPNIR_MSGSYS_APP_SETTINGS or DRAUPNIR_MSGSYS_CUSTOM_SETTINGS macros are enabled.
     *       When first macro is enabled - this method will accept AppSettings pointer. When second - MessageSystemInterface. */
#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    void loadSettings(AppSettings* settings)
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    void loadSettings(MessageSettingsInterface* settings)
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
    {
        Q_ASSERT_X(settings, "MessageUiBuilderTemplate::loadSettings",
                   "Provided settings is nullptr.");
        Q_ASSERT_X(p_settings == nullptr, "MessageUiBuilderTemplate::loadSettings",
                   "This method should be called only once.");

        p_settings = settings;
    }
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS || DRAUPNIR_MSGSYS_APP_SETTINGS

private:
#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    AppSettings* p_settings;
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    MessageSettingsInterface* p_settings;
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
    MessageHandlerTemplate<MessageTypes...>* p_handler;
};

#endif // MESSAGEUIBUILDERTEMPLATE_H
