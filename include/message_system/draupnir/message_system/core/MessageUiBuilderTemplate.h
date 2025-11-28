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

#include "draupnir/message_system/core/AbstractMessageUiBuilder.h"
#include "draupnir/message_system/core/MessageHandlerTemplate.h"

#include "draupnir/message_system/ui/menus/NotificationTypeMenu.h"
#include "draupnir/message_system/ui/widgets/LogWidget.h"
#include "draupnir/message_system/ui/widgets/NotificationSettingsWidgetTemplate.h"
#include "draupnir/message_system/ui/windows/MessageSystemConfigDialog.h"
#include "draupnir/message_system/ui/widgets/MessageTypesSelectorWidgetTemplate.h"
#include "draupnir/message_system/ui/menus/MessageListViewConfigMenuTemplate.h"

#include "draupnir/SettingsBundleMerge.h"

namespace Draupnir::Messages
{

/*! @class MessageUiBuilderTemplate draupnir/core/MessageUiBuilderTemplate.h
 *  @ingroup MessageSystem
 *  @brief Default template-based implementation of MessageUiBuilder for a given list of message types.
 *  @tparam MessageTypes List of message type traits defining messages handled by this builder.
 *
 *  @details This class creates a LogWidget and binds it to the message handler and settings. The template parameters define the
 *           supported message types. */

template<class... MessageTypes>
class MessageUiBuilderTemplate final : public AbstractMessageUiBuilder
{
public:
    /*! @brief Defines @ref Draupnir::Settings::SettingsBundleTemplate template instantiation containing all settings which
     *         are required by this @ref Draupnir::Messages::MessageUiBuilderTemplate to work properly. To be used as one of
     *         the template arguments of the @ref SettingsRegistryTemplate. */
    using SettingsBundle = Draupnir::Settings::bundle_merge_all_t<
        typename LogWidget::SettingsBundle
    >;

    /*! @brief Returns a created and ready-to-use @ref LogWidget. This widget is capable of displaying messages from @ref
     *         MessageSystemTemplate to which this @ref MessageUiBuilderTemplate instantiation is belonging.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    LogWidget* createLogWidget(QWidget* parent = nullptr) final {
        Q_ASSERT_X(m_settings.isValid(),"MessageUiBuilderTemplate<MessageTraits...>::createLogWidget",
                   "Method MessageUiBuilderTemplate<MessageTraits...>::loadSettings must have been called before.");

        LogWidget* result = new LogWidget{this,parent};
        result->template loadSettings<SettingsBundle>(&m_settings);
        result->setMessageListModel(p_handler->messages());
        return result;
    };

    /*! @brief Returns a @ref LogWidget as a pointer to `QWidget`. This widget is ready to use and is capable to display
     *         messages from @ref MessageSystemTemplate to which this @ref MessageUiBuilderTemplate instantiation is
     *         belonging.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    QWidget* createLogWidgetAsQWidget(QWidget* parent = nullptr) final {
        Q_ASSERT_X(m_settings.isValid(),"MessageUiBuilderTemplate<MessageTraits...>::createLogWidgetAsQWidget",
                   "Method MessageUiBuilderTemplate<MessageTraits...>::loadSettings must have been called before.");
        return createLogWidget(parent);
    };

    /*! @brief Returns @ref NotificationTypeMenu which is ready-to-use for changing the type of notification for specified
     *         @ref MessageType.
     *  @param messageType Type of message notification for which will be configured by this NotificationTypeMenu.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    NotificationTypeMenu* createNotificationSettingsMenu(MessageType messageType, QWidget* parent = nullptr) final {
        Q_ASSERT_X(p_handler, "MessageUiBuilderTemplate<MessageTraits...>::createNotificationSettingsMenu",
                   "p_handler was not set.");
        NotificationTypeMenu* result = new NotificationTypeMenu{parent};
        result->setNotificationType(p_handler->notification(messageType));

        // MessageHandler is informing about changes of notification policies by emitting signal MessageHandler::notificationTypeChanged
        // with two arguments: type of message and type of notification. As we want this NotificationTypeMenu to be responsible only for
        // ONE specific type of messages - we use a lambda to catch signal from MessageHandler and IF it is related to this particular
        // notificationTypeMenu and IF this menu was not deleted - we can process this.
        QPointer<NotificationTypeMenu> targetMenu{result};
        QObject::connect(static_cast<AbstractMessageHandler*>(p_handler),&AbstractMessageHandler::notificationTypeChanged,
            [targetMenu,messageType](MessageType changedMsgType,Notification::Type notification) {
            if (changedMsgType != messageType)
                return;

            if (targetMenu.isNull())
                return;

            targetMenu->setNotificationType(notification);
        });

        QPointer<AbstractMessageHandler> handlerPtr{static_cast<AbstractMessageHandler*>(p_handler)};
        QObject::connect(result, &NotificationTypeMenu::notificationTypeChanged,[handlerPtr,messageType](Notification::Type notification) {
            if (handlerPtr.isNull())
                return;

            handlerPtr->setNotification(messageType,notification);
        });

        return result;
    };

    /*! @brief Returns a QMenu containing @ref NotificationTypeMenu entries for specified @ref MessageType entries. @ref MessageType
     *         entries must be specified as combination of the flags passed as variable of type @ref MessageType.
     *  @param messageType Optional type of messages which notifications will be handled by this menu.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly.
     * @todo Switch from QMenu to the other class in order to enable retranslation. */
    QMenu* createNotificationSettingsMenuForTypes(MessageType messageType = MessageType::AllMessages, QWidget* parent = nullptr) final {
        Q_ASSERT(p_handler);
        QMenu* result = new QMenu{parent};
        _populateNotificationSettingsMenuForTypes<MessageTypes...>(result,messageType);
        return result;
    };

protected:
    /*! @brief Defines @ref NotificationSettingsWidgetTemplate instantiation containing notification settings of all
     *         message types which are handled by @ref MessageSystemTemplate instantiation to which this @ref MessageUiBuilderTemplate
     *         is belonging. */
    using NotificationSettingsWidget = NotificationSettingsWidgetTemplate<MessageTypes...>;

    template<class... Args>
    friend class MessageSystemTemplate;
    friend class MessageUiBuilderTemplateIT;

    MessageUiBuilderTemplate() :
        p_handler{nullptr}
    {}

    /*! @brief Associates a specific MessageHandlerTemplate with this UI builder.
     *  @param handler Pointer to a MessageHandlerTemplate.
     *  @details This is required before any UI widgets can be created. */
    void setMessageHandlerTemplate(MessageHandlerTemplate<MessageTypes...>* handler) {
        Q_ASSERT_X(handler, "MessageUiBuilderTemplate::setMessageHandlerTemplate",
                   "Provided handler is nullptr.");
        Q_ASSERT_X(p_handler == nullptr, "MessageUiBuilderTemplate::setMessageHandlerTemplate",
                   "Provided handler is nullptr.");

        p_handler = handler;
    }

    /*! @brief Loads settings for this @ref MessageUiBuilderTemplate from the specified source. */
    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::canBeFullyPopulatedFrom<SettingsSource>(),
                "Provided SettingsSource can not populate the settings bundle required by this MessageUiBuilderTemplate");
        m_settings = source->template getSettingsBundle<SettingsBundle>();
    }

    /*! @brief This method returns the @ref Draupnir::Messages::MessageListViewConfigMenuTemplate as a pointer to the @ref
     *         Draupnir::Messages::AbstractMessageListViewConfigMenu, which can be used to configure different parameters
     *         of @ref Draupnir::Messages::MessageListView widget.
     *  @details This method is used by the @ref Draupnir::Messages::LogWidget internally to allow user configuration of MessageListView. */
    AbstractMessageListViewConfigMenu* createAbstractMessageListViewConfigMenu(QWidget* parent = nullptr) final {
        return new MessageListViewConfigMenuTemplate<MessageTypes...>{parent};
    };

    /*! @brief Return a widget for configuring notification settings within this MessageSystem. */
    AbstractNotificationSettingsWidget* createNotificationSettingsWidgetAsInterface(QWidget* parent = nullptr) final {
        return createNotificationSettingsWidget(parent);
    };

    NotificationSettingsWidget* createNotificationSettingsWidget(QWidget* parent = nullptr) {
        NotificationSettingsWidgetTemplate<MessageTypes...>* result =
                new NotificationSettingsWidgetTemplate<MessageTypes...>(parent);

        result->showNotificationSettings(p_handler);
        return result;
    }

    /*! @brief Returns a widget for selecting specific MessageTypes. */
    AbstractMessageTypesSelectorWidget* createAbstractMessageTypesSelectorWidget(QWidget* parent = nullptr) final {
        return new MessageTypesSelectorWidgetTemplate<MessageTypes...>{parent};
    };

    /*! @brief Returns MessageSystemConfigDialog capable of editing some of the settings within the MessageSystem. */
    MessageSystemConfigDialog* createAbstractMessageSystemConfigDialog(QWidget* parent = nullptr) final {
        MessageSystemConfigDialog* result = new MessageSystemConfigDialog{this,parent};
        return result;
    }

private:
    template<class First, class... Rest>
    inline void _populateNotificationSettingsMenuForTypes(QMenu* dest, MessageType messageTypes) {
        if (messageTypes & First::type) {
            auto menu = createNotificationSettingsMenu(First::type);
            menu->setTitle(First::displayName());
            dest->addMenu(menu);
        }

        if constexpr (sizeof...(Rest) > 0)
            _populateNotificationSettingsMenuForTypes<Rest...>(dest,messageTypes);
    }

    SettingsBundle m_settings;
    MessageHandlerTemplate<MessageTypes...>* p_handler;
};

}; // namespace Draupnir::Messages

#endif // MESSAGEUIBUILDERTEMPLATE_H
