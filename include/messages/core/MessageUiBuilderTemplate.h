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

#include "ui/menus/NotificationTypeMenu.h"
#include "ui/widgets/LogWidget.h"
#include "ui/widgets/MessageNotificationSettingsWidget.h"

#include "SettingsBundleMerge.h"

namespace Draupnir::Messages
{

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
    using SettingsBundle = Draupnir::Settings::bundle_merge_all_t<
        typename LogWidgetTemplate<MessageTypes...>::SettingsBundle
    >;

    /*! @brief Creates and returns a LogWidget configured with the current handler and settings.
     *  @details This method is intended to be used by GUI code that needs a message log viewer.
     *  @param parent Optional parent widget for the created LogWidget.
     *  @return QWidget* Pointer to the fully configured LogWidget. */
    QWidget* createConfiguredLogWidget(QWidget* parent = nullptr) final {
        Q_ASSERT_X(m_settings.isValid(),"MessageUiBuilderTemplate<MessageTraits...>::createConfiguredLogWidget",
                   "Method MessageUiBuilderTemplate<MessageTraits...>::loadSettings must have been called before.");

        auto* result = new LogWidgetTemplate<MessageTypes...>{parent};

        result->setMessageListModel(p_handler->messages());
        result->template loadSettings<SettingsBundle>(&m_settings);

        return result;
    }

    /*! @brief Returns a QMenu containing NotificationTypeMenu entries for all available MessageType
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    QMenu* createConfiguredGlobalNotificationsMenu(QWidget* parent = nullptr) final {
        QMenu* result = new QMenu{parent};
        _populateGlobalNotificationsMenu<MessageTypes...>(result);
        return result;
    };

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

    template<class SettingsRegistry>
    void loadSettings(SettingsRegistry* settings) {
        m_settings = settings->template getSettingsBundle<SettingsBundle>();
    }

private:
    SettingsBundle m_settings;
    MessageHandlerTemplate<MessageTypes...>* p_handler;

    template<class First, class... Rest>
    inline void _populateGlobalNotificationsMenu(QMenu* dest) {
        auto menu = createConfiguredNotificationMenu(First::type);
        menu->setTitle(First::displayName());
        dest->addMenu(menu);

        if constexpr (sizeof...(Rest) > 0)
            return _populateGlobalNotificationsMenu<Rest...>(dest);
    }
};

}; // namespace Draupnir::Messages

#endif // MESSAGEUIBUILDERTEMPLATE_H
