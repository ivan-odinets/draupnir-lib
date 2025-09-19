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

#ifndef MESSAGENOTIFICATIONSETTINGSWIDGET_H
#define MESSAGENOTIFICATIONSETTINGSWIDGET_H

#include <QWidget>

#include <QLabel>

class QFormLayout;
class QPushButton;

#include "MessageType.h"
#include "Notification.h"
#include "NotificationTypeComboBox.h"

class MessageHandler;

/*! @class MessageNotificationSettingsWidget draupnir-lib/src/messages/widgets/MessageNotificationSettingsWidget.h
 *  @brief Abstract base widget for configuring notification settings for the MessageHandler / MessageHandlerTemplate.
 *  @details This widget serves as a user interface for displaying and modifying notification settings associated with
 *           different message types. It is intended to be subclassed by `MessageNotificationSettingsWidgetTemplate`
 *           which populates this widget with actual trait-based controls.
 *
 *           The widget includes a test row (label + combo box + button) and a layout for dynamic rows generated per
 *           message type.
 * @see MessageNotificationSettingsWidgetTemplate */

class MessageNotificationSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Default constructor. Initializes all internal UI elements.
     *  @param parent Optional parent widget. */
    explicit MessageNotificationSettingsWidget(QWidget *parent = nullptr);

    /*! @brief Virtual method to display the current notification type for a specific message type.
     *  @param type The message type identifier.
     *  @param notification The notification type to show for the message type. */
    virtual void showNotificationType(MessageType type, Notification::Type notification) = 0;

signals:
    /*! @brief Emitted when the user changes the notification type for a given message.
     *  @param type The message type.
     *  @param notification The newly selected notification type. */
    void messageNotificationTypeChanged(MessageType type, Notification::Type notification);

protected:
    /*! @brief Qt event handler for language change, used for dynamic retranslation.
     *  @param event The event object. */
    void changeEvent(QEvent* event) final;

    /*! @brief Adds a labeled combo box row to the layout.
     *  @param label Pointer to a QLabel.
     *  @param selector Pointer to a NotificationTypeComboBox. */
    void addRow(QLabel* label, NotificationTypeComboBox* selector);

    /*! @brief Virtual helper to retranslate the labels of each message type. */
    virtual void _retranslateTypeLabels() = 0;

    MessageHandler* p_handler;

private slots:
    void _onShowDummyClicked();

private:
    void _retranslateUi();
    QLabel* w_testLabel;
    NotificationTypeComboBox* w_testNotiticationSelector;
    QPushButton* w_testButton;
    QFormLayout* p_notificationTypesLayout;
};

#include "../../include/containers/fixed_tuple_map.h"
#include "../core/MessageHandlerTemplate.h"
#include "../core/MessageTraitsHelper.h"

/*! @class MessageNotificationSettingsWidgetTemplate draupnir-lib/src/messages/widgets/MessageNotificationSettingsWidget.h
 *  @brief Concrete implementation of `MessageNotificationSettingsWidget` for a fixed set of message traits.
 *  @details This templated class initializes the UI rows for each message trait and handles interactions with a
 *           `MessageHandlerTemplate`. It maps each message type to a pair of widgets (label + combo box) using `fixed_tuple_map`.
 *
 * @tparam MessageTraits... A parameter pack of message trait types (e.g., DebugMessageTrait, InfoMessageTrait). */


template<class... MessageTraits>
class MessageNotificationSettingsWidgetTemplate final : public MessageNotificationSettingsWidget
{
public:
    /*! @brief Constructs the widget and populates it based on the provided message traits.
     *  @param parent Optional parent widget. */
    explicit MessageNotificationSettingsWidgetTemplate(QWidget* parent = nullptr) :
        MessageNotificationSettingsWidget(parent)
    {
        _setupUiImpl<MessageTraits...>();
    }

    /*! @brief Updates the UI combo box corresponding to a given message type.
     *  @param type The message type.
     *  @param notification The notification type to set. */
    void showNotificationType(MessageType type, Notification::Type notification) final {
        std::get<NotificationTypeComboBox*>(m_widgetMap[type])->setNotificationType(notification);
    }

    /*! @brief Displays the current notification settings from the message handler.
     *  @details This populates the combo boxes based on the values in the handler’s internal notification map and
     *           connects the widgets to update the handler on change.
     *  @param handler A pointer to the `MessageHandlerTemplate` for the current trait set. */
    void showNotificationSettings(MessageHandlerTemplate<MessageTraits...>* handler) {
        p_handler = handler;

        auto policyBegin = handler->notificationMapBegin();
        const auto policyEnd = handler->notificationMapEnd();

        auto widgetMapBegin = m_widgetMap.begin();
        //const auto widgetMapEnd = m_widgetMap.end();

        while (policyBegin != policyEnd) {
            std::get<NotificationTypeComboBox*>(widgetMapBegin->second)->setNotificationType(
                policyBegin->second
            );

            policyBegin++;
            widgetMapBegin++;
        }

        connect(handler, &MessageHandler::notificationTypeChanged,
                this, &MessageNotificationSettingsWidget::showNotificationType);
    }

protected:
    /*! @brief Retranslates all labels using the localized display name from each trait. */
    void _retranslateTypeLabels() final {
        _retranslateTypeLabelsImpl<MessageTraits...>();
    }

private:
    fixed_tuple_map<
        MessageTraitsHelper<MessageTraits...>::supportedMessageIds,
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
        connect(selector,&NotificationTypeComboBox::notificationTypeChanged,[this,type](Notification::Type notificationType){
            p_handler->setNotification(type,notificationType);
        });

        m_widgetMap.template get<QLabel*>(type) = displayLabel;
        m_widgetMap.template get<NotificationTypeComboBox*>(type) = selector;

        addRow(displayLabel,selector);

        if constexpr (sizeof...(Rest) > 0)
            _setupUiImpl<Rest...>();
    }

    /*! @brief Recursive compile-time retranslation for trait labels. */
    template<class First, class... Rest>
    inline void _retranslateTypeLabelsImpl() {
        std::get<QLabel*>(m_widgetMap[First::type])->setText(First::displayName());

        if constexpr (sizeof...(Rest) > 0)
            _retranslateTypeLabelsImpl<Rest...>();
    }

};

#endif // MESSAGENOTIFICATIONSETTINGSWIDGET_H
