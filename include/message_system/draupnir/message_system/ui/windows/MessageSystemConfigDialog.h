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

#ifndef MESSAGESYSTEMCONFIGDIALOG_H
#define MESSAGESYSTEMCONFIGDIALOG_H

#include <QDialog>

#include <QDialogButtonBox>
#include <QVBoxLayout>

class QDialogButton;

#include "draupnir/message_system/core/Message.h"
#include "draupnir/message_system/traits/widgets/MessageFieldsSelectorTrait.h"
#include "draupnir/message_system/traits/widgets/MessageTypesSelectorTrait.h"
#include "draupnir/message_system/traits/widgets/NotificationSettingsEditorWidgetTrait.h"
#include "draupnir/ui/widgets/FixedTabWidgetTemplate.h"

namespace Draupnir::Messages {
    class AbstractMessageUiBuilder;
    class AbstractNotificationSettingsWidget;
}

namespace Draupnir::Messages
{

/*! @class MessageSystemConfigDialog draupnir/message_system/ui/windows/MessageSystemConfigDialog.h
 *  @brief This dialog allows configuring some settings of the @ref Draupnir::Messages::AbstractMessageSystem / @ref
 *         Draupnir::Messages::MessageSystemTemplate and @ref Draupnir::Messages::LogWidget
 *
 * @todo After refractoring the AbstractMessageUiBuilder / MessageUiBuilderTemplate - this thing should be refractored as
 *       well.
 * @todo Expand the documentation for this class.
 * @todo Add test for this class. */

class MessageSystemConfigDialog : public QDialog
{
    Q_OBJECT

public:
    /*! @brief Trivial destructor. */
    ~MessageSystemConfigDialog() = default;

    /*! @brief Display the notification type for a specific message type.
     *  @param type The message type identifier.
     *  @param notification The notification type to show for the message type. */
    void setNotificationType(MessageType type, Notification::Type notification) {
        w_mainWidget->getWidget<AbstractNotificationSettingsWidget>()->showNotificationType(type,notification);
    };

    /*! @brief Return the displayed notification type for a specific message type.
     *  @param type The message type identifier. */
    Notification::Type notificationType(MessageType type) {
        return w_mainWidget->getWidget<AbstractNotificationSettingsWidget>()->notificationType(type);
    }

    /*! @brief Updates internal @ref Draupnir::Messages::MessageFieldsSelectorWidget according to the specified mask.
     *  @param mask Bitmask of fields to be shown.
     * @note No signals are emitted after calling this method. */
    void setDisplayedMessageFieldsMask(const std::underlying_type_t<Message::Fields> mask) {
        w_mainWidget->getWidget<MessageFieldsSelectorWidget>()->setDisplayedMessageFieldsMask(mask);
    }

    /*! @brief This method returns mask of the fields of the @ref Draupnir::Messages::Message object which are marked as
     *         displayed. */
    std::underlying_type_t<Message::Fields> displayedMessageFieldsMask() const {
        return w_mainWidget->getWidget<MessageFieldsSelectorWidget>()->displayedMessageFieldsMask();
    }

    /*! @brief Sets visibility flag of specific element of @ref Draupnir::Messages::Message objects.
     *  @param field Field to modify.
     *  @param isShown Whether the field should be marked as visible.
     * @note No signals are emitted after calling this method. */
    void setMessagePartDisplayed(Message::Fields field, bool isShown) {
        w_mainWidget->getWidget<MessageFieldsSelectorWidget>()->setMessageFieldDisplayed(field,isShown);
    }

    /*! @brief Returns whether specific element of @ref Draupnir::Messages::Message objects is marked as visible.
     *  @param field Field to query.
     *  @return True if field is visible. */
    bool isMessagePartDisplayed(Message::Fields field) const {
        return w_mainWidget->getWidget<MessageFieldsSelectorWidget>()->isMessageFieldDisplayed(field);
    }

    /*! @brief Updates all message type entries based on the given filter bitmask.
     *  @param config Bitmask of message types to mark as visible.
     * @note No signals should be emitted. */
    void setDisplayedMessageTypesMask(MessageType config) {
        w_mainWidget->getWidget<AbstractMessageTypesSelectorWidget>()->setDisplayedMessageTypesMask(config);
    };

    /*! @brief Returns current visibility mask for @ref Draupnir::Messages::MessageType entries. */
    MessageType displayedMessageTypesMask() const {
        return w_mainWidget->getWidget<AbstractMessageTypesSelectorWidget>()->displayedMessageTypesMask();
    };

    /*! @brief Marks a specific @ref Draupnir::Messages::MessageType as displayed/hidden.
     *  @param type Message type to configure.
     *  @param isSelected True to mark as visible.
     * @note No signals should be emitted. */
    void setMessageTypeDisplayed(MessageType type, bool isSelected) {
        w_mainWidget->getWidget<AbstractMessageTypesSelectorWidget>()->setMessageTypeDisplayed(type, isSelected);
    };

    /*! @brief Returns whether the specified @ref Draupnir::Messages::MessageType is currently selected for display. */
    bool isMessageTypeDisplayed(MessageType type) const {
        return w_mainWidget->getWidget<AbstractMessageTypesSelectorWidget>()->isMessageTypeDisplayed(type);
    };

signals:
    /*! @brief Emitted when the user changes the notification type for a given message.
     *  @param type The message type.
     *  @param notification The newly selected notification type. */
    void messageNotificationTypeChanged(MessageType type, Notification::Type notification);

    /*! @brief Emitted when user toggles visibility of a message type.
     *  @param messageType The message type being toggled.
     *  @param isVisible New visibility state. */
    void messageFieldVisibilityChanged(Draupnir::Messages::Message::Fields field, bool isVisible);

    /*! @brief Emitted when user toggles field visibility.
     *  @param field The field whose visibility changed.
     *  @param isVisible Whether the field is now visible. */
    void messageTypeVisibilityChanged(Draupnir::Messages::MessageType messageType, bool isVisible);

protected:
    template<class... MessageTraits>
    friend class MessageUiBuilderTemplate;

    /*! @brief Constructs MessageSystemConfigDialog using the provided @ref Draupnir::Messages::AbstractMessageUiBuilder
     *         instance as a source of template-dependant Ui components. */
    explicit MessageSystemConfigDialog(AbstractMessageUiBuilder* uiBuilder, QWidget* parent = nullptr);

private:
    using CentralWidget = Draupnir::Ui::FixedTabWidgetTemplate<
        void,
        NotificationSettingsEditorTrait,
        MessageTypesSelectorTrait,
        MessageFieldsSelectorTrait
    >;

    AbstractMessageUiBuilder* p_uiBuilder;
    CentralWidget* w_mainWidget;
    QDialogButtonBox* p_buttons;
};

}; // namespace Draupnir::Messages

#endif // MESSAGESYSTEMCONFIGDIALOG_H
