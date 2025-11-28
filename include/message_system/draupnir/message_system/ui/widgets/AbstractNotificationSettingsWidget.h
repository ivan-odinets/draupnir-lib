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

#ifndef ABSTRACTNOTIFICATIONSETTINGSWIDGET_H
#define ABSTRACTNOTIFICATIONSETTINGSWIDGET_H

#include <QWidget>

#include <QLabel>

class QFormLayout;
class QPushButton;

#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/message_system/core/Notification.h"
#include "draupnir/message_system/ui/widgets/NotificationTypeComboBox.h"

namespace Draupnir::Messages
{

class AbstractMessageHandler;

/*! @class AbstractNotificationSettingsWidget draupnir/ui/widgets/AbstractNotificationSettingsWidget.h
 *  @ingroup MessageSystem
 *  @brief Abstract base widget for configuring notification settings for the @ref Draupnir::Messages::AbstractMessageHandler
 *         / @ref Draupnir::Messages::MessageHandlerTemplate.
 *
 *  @details This widget serves as a user interface for displaying and modifying notification settings associated with
 *           different message types. It is intended to be subclassed by `MessageNotificationSettingsWidgetTemplate`
 *           which populates this widget with actual trait-based controls.
 *
 *           The widget includes a test row (label + combo box + button) and a layout for dynamic rows generated per
 *           message type.
 *
 * @todo Maybe it will make sense to make this widget Qt-only and populate it with specific QCheckBoxes like this is done for
 *       menus?
 * @todo Write test for this class. */

class AbstractNotificationSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Virtual method to display the current notification type for a specific message type.
     *  @param type The message type identifier.
     *  @param notification The notification type to show for the message type. */
    virtual void showNotificationType(MessageType type, Notification::Type notification) = 0;

    /*! @brief Virtual method to return the displayed notification type for a specific message type.
     *  @param type The message type identifier. */
    virtual Notification::Type notificationType(MessageType type) = 0;

signals:
    /*! @brief Emitted when the user changes the notification type for a given message.
     *  @param type The message type.
     *  @param notification The newly selected notification type. */
    void messageNotificationTypeChanged(MessageType type, Notification::Type notification);

protected:
    /*! @brief Default constructor. Initializes all internal UI elements.
     *  @param parent Optional parent widget. */
    explicit AbstractNotificationSettingsWidget(QWidget *parent = nullptr);

    /*! @brief Qt event handler for language change, used for dynamic retranslation.
     *  @param event The event object. */
    void changeEvent(QEvent* event) final;

    /*! @brief Adds a labeled combo box row to the layout.
     *  @param label Pointer to a QLabel.
     *  @param selector Pointer to a NotificationTypeComboBox. */
    void addRow(QLabel* label, NotificationTypeComboBox* selector);

    /*! @brief Virtual helper to retranslate the labels of each message type. */
    virtual void _retranslateTypeLabels() = 0;

    /*! @brief Access the internal MessageHandlerTemplate via its interface. */
    virtual AbstractMessageHandler* handler() = 0;

private slots:
    void _onShowDummyClicked();

private:
    void _retranslateUi();
    QLabel* w_testLabel;
    NotificationTypeComboBox* w_testNotiticationSelector;
    QPushButton* w_testButton;
    QFormLayout* p_notificationTypesLayout;
};

}; // namespace Draupnir::Messages

#endif // ABSTRACTNOTIFICATIONSETTINGSWIDGET_H
