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

#ifndef NOTIFICATIONSETTINGSWIDGETTEMPLATE_H
#define NOTIFICATIONSETTINGSWIDGETTEMPLATE_H

#include "draupnir/ui/widgets/AbstractNotificationSettingsWidget.h"

#include "draupnir/containers/fixed_tuple_map.h"
#include "draupnir/core/MessageHandlerTemplate.h"

namespace Draupnir::Messages
{

/*! @class NotificationSettingsWidgetTemplate draupnir/ui/widgets/NotificationSettingsWidgetTemplate.h
 *  @ingroup MessageSystem
 *  @brief Concrete implementation of `MessageNotificationSettingsWidget` for a fixed set of message traits.
 *  @tparam MessageTraits... A parameter pack of message trait types (e.g., DebugMessageTrait, InfoMessageTrait).
 *
 *  @details This templated class initializes the UI rows for each message trait and handles interactions with a
 *           `MessageHandlerTemplate`. It maps each message type to a pair of widgets (label + combo box) using `fixed_tuple_map`.
 *
 * @todo Write test for this class. */


template<class... MessageTraits>
class NotificationSettingsWidgetTemplate final : public AbstractNotificationSettingsWidget
{
    using MessageHandler = MessageHandlerTemplate<MessageTraits...>;

    static constexpr MessageType supportedMessageIds[] = { MessageTraits::type... };

public:
    /*! @brief Constructs the widget and populates it based on the provided message traits.
     *  @param parent Optional parent widget. */
    explicit NotificationSettingsWidgetTemplate(QWidget* parent = nullptr) :
        AbstractNotificationSettingsWidget{parent}
    {
        _setupUiImpl<MessageTraits...>();
    }

    /*! @brief Updates the UI combo box corresponding to a given message type.
     *  @param type The message type.
     *  @param notification The notification type to set. */
    void showNotificationType(MessageType type, Notification::Type notification) final {
        std::get<NotificationTypeComboBox*>(m_widgetMap[type])->setNotificationType(notification);
    }

    /*! @brief Returns the displayed notification type for a MessageType specified
     *  @param type The message type. */
    Notification::Type notificationType(MessageType type) {
        return std::get<NotificationTypeComboBox*>(m_widgetMap[type])->notificationType();
    };

    /*! @brief Displays the current notification settings from the message handler.
     *  @param handler A pointer to the `MessageHandlerTemplate` for the current trait set.
     *  @details This populates the combo boxes based on the values in the handler’s internal notification map and
     *           connects the widgets to update the handler on change.
     * @todo Add Q_ASSERT_X with reasonable error message. */
    void showNotificationSettings(MessageHandlerTemplate<MessageTraits...>* handler) {
        p_handler = handler;

        _showNotificationSettingsImpl<MessageTraits...>();

        connect(handler, &MessageHandler::notificationTypeChanged,
                this,    &AbstractNotificationSettingsWidget::showNotificationType);
    }

protected:
    /*! @brief Retranslates all labels using the localized display name from each trait. */
    void _retranslateTypeLabels() final {
        _retranslateTypeLabelsImpl<MessageTraits...>();
    }

    AbstractMessageHandler* handler() {
        return p_handler;
    }

private:
    friend class NotificationSettingsWidgetTemplateTest;

    MessageHandler* p_handler;

    draupnir::containers::fixed_tuple_map<
        supportedMessageIds,
        QLabel*,
        NotificationTypeComboBox*
    > m_widgetMap;

    /*! @brief Recursive compile-time UI setup using trait parameter pack.
     *  @details Adds a row for each trait to the form layout. */
    template<class First, class... Rest>
    inline void _setupUiImpl() {
        constexpr MessageType type = First::type;

        QLabel* displayLabel = new QLabel{First::displayName()};

        NotificationTypeComboBox* selector = new NotificationTypeComboBox;
        connect(selector,&NotificationTypeComboBox::notificationTypeSelected,[this,type](Notification::Type notificationType){
            p_handler->setNotification(type,notificationType);
        });

        m_widgetMap.template get<QLabel*>(type) = displayLabel;
        m_widgetMap.template get<NotificationTypeComboBox*>(type) = selector;

        addRow(displayLabel,selector);

        if constexpr (sizeof...(Rest) > 0)
            _setupUiImpl<Rest...>();
    }

    template<class First, class... Rest>
    inline void _showNotificationSettingsImpl() {
        m_widgetMap.template get<NotificationTypeComboBox*>(First::type)->setNotificationType(
            p_handler->notification(First::type)
        );

        if constexpr (sizeof...(Rest) > 0)
            _showNotificationSettingsImpl<Rest...>();
    }

    /*! @brief Recursive compile-time retranslation for trait labels. */
    template<class First, class... Rest>
    inline void _retranslateTypeLabelsImpl() {
        std::get<QLabel*>(m_widgetMap[First::type])->setText(First::displayName());

        if constexpr (sizeof...(Rest) > 0)
            _retranslateTypeLabelsImpl<Rest...>();
    }
};

}; // namespace Draupnir::Messages

#endif // NOTIFICATIONSETTINGSWIDGETTEMPLATE_H
