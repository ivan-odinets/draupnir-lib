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

#ifndef ABSTRACTMESSAGEHANDLER_H
#define ABSTRACTMESSAGEHANDLER_H

#include <QObject>

#include <QMap>

#include "draupnir/message_system/core/Message.h"
#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/message_system/core/Notification.h"

class QSystemTrayIcon;

namespace Draupnir::Messages
{

class MessageDisplayDialog;
class MessageListModel;

/*! @class AbstractMessageHandler draupnir/message_system/core/AbstractMessageHandler.h
 *  @ingroup MessageSystem
 *  @brief Abstract base class for processing and displaying application messages.
 *
 *  @details This class provides an interface for managing how messages are processed, stored, and displayed to the user. Messages
 *           can be handled immediately or in grouped batches via `MessageGroup`.
 *
 *           @ref AbstractMessageHandler defines the runtime API while @ref Draupnir::Messages::MessageHandlerTemplate supplies a
 *           concrete implementation that maps compile-time message traits to notification types. @ref Logger sends messages to
 *           the current @ref Draupnir::Messages::AbstractMessageHandler / @ref Draupnir::Messages::MessageHandlerTemplate instance,
 *           which consults these policies to show or queue notifications.
 *
 * @todo AbstractMessageHandler::_showMessageBox(Message* message) and AbstractMessageHandler::_showMessageBox(const QList<Message*>&
 *       messageList) methods can be refractored.
 * @todo Allow configuration of dialogs (blocking vs non-blocking) somewhere.
 * @todo Add Q_ASSERT_X / reasonable debug messages when trying to show Message object with type unknown to this AbstractMessageHandler. */

class AbstractMessageHandler : public QObject
{
    Q_OBJECT
public:
    /*! @brief Destructor. Deletes internal @ref Draupnir::Messages::MessageListModel. */
    virtual ~AbstractMessageHandler() override;

#ifndef QT_NO_SYSTEMTRAYICON
    /*! @brief This method is used to specify tray icon used to show notifications in systemtray.
     * @note This method is available only when building with systemtray support. */
    void setTrayIcon(QSystemTrayIcon* trayIcon) { w_trayIcon = trayIcon; }

#endif // QT_NO_SYSTEMTRAYICON

    /*! @brief Returns `true` if the specified @ref Draupnir::Messages::MessageType is known to this MessageHandlerTemplate
     *         instantiation. */
    virtual bool contains(MessageType type) const = 0;

    /*! @brief Sets the @ref Draupnir::Messages::Notification::Type for a specified @ref Draupnir::Messages::MessageType. This
     *         method is implemented within the @ref Draupnir::Messages::MessageHandlerTemplate class.
     * @see MessageHandlerTemplate::setNotification */
    virtual void setNotification(MessageType type, const Notification::Type notificationType) = 0;

    /*! @brief Returns @ref Draupnir::Messages::Notification::Type which is used for given @ref Draupnir::Messages::MessageType.
     *         This method is implemented within the @ref Draupnir::Messages::MessageHandlerTemplate class.
     * @see MessageHandlerTemplate::notification */
    virtual Notification::Type notification(uint64_t type) = 0;

    /*! @brief Gives access to @ref Draupnir::Messages::MessageListModel, containing logs about stuff happened. */
    MessageListModel* messages() { return p_messageListModel; }

    /*! @brief Shows to user how specific @ref Draupnir::Messages::Notification::Type things will be displayed. */
    void showDummy(Notification::Type type);

    /*! @brief Handles a single @ref Draupnir::Messages::Message object. Provided message is added to the internal @ref
     *         Draupnir::Messages::MessageListModel and then the appropriate notification is displayed to the user. */
    void handleMessage(Draupnir::Messages::Message* message);

    /*! @brief Processes a list of @ref Draupnir::Messages::Message objects. Each @ref Draupnir::Messages::Message is added to the
     *         @ref Draupnir::Messages::MessageListModel and then the proper notification is displayed to the user.
     * @note If different notification methods are required, each group of messages will use its own notification type. */
    void handleMessageList(const QList<Draupnir::Messages::Message*>& messageList);

    /*! @brief Shows the given @ref Draupnir::Messages::Message using the @ref Draupnir::Messages::Notification::Type stored
     *         for its type.
     * @note During execution the @ref Draupnir::Messages::Message object is **not** added to the @ref Draupnir::Messages::MessageListModel. */
    void showMessage(Message* message);

    /*! @brief Shows the given @ref Draupnir::Messages::Message using the provided @ref Draupnir::Messages::Notification::Type.
     * @note During execution the @ref Draupnir::Messages::Message object is **not** added to the @ref Draupnir::Messages::MessageListModel. */
    void showMessage(Message* message, const Notification::Type type);

    /*! @brief Shows a list of @ref Draupnir::Messages::Message objects using the saved @ref Draupnir::Messages::Notification::Type
     *         for their type.
     * @note During execution the @ref Draupnir::Messages::Message objects are **not** added to the @ref Draupnir::Messages::MessageListModel. */
    void showMessageList(const QList<Message*>& messageList);

    /*! @brief Shows a list of @ref Draupnir::Messages::Message objects using the provided @ref Draupnir::Messages::Notification::Type.
     * @note During execution the @ref Draupnir::Messages::Message objects are **not** added to the @ref Draupnir::Messages::MessageListModel. */
    void showMessageList(const QList<Draupnir::Messages::Message*>& messageList, const Notification::Type type);

signals:
    /*! @brief This signal is emitted when a type of notification for specific @ref Draupnir::Messages::Message::Type has changed. */
    void notificationTypeChanged(quint64 messageType, Draupnir::Messages::Notification::Type notification);

protected:
    /*! @brief Default constructor. Creates the underlying @ref Draupnir::Messages::MessageListModel. */
    explicit AbstractMessageHandler(QObject* parent = nullptr);

private:
    friend class MessageSystemGeneralIT;
    friend class MessageHandlerTemplateTest;

#ifndef QT_NO_SYSTEMTRAYICON
    /*! @brief Shows tray notification with @ref Draupnir::Messages::Message object. */
    void _showMessageInSystray(Message* message);

    /*!< @brief Shows tray notification about several @ref Draupnir::Messages::Message objects placed in the log. */
    void _showMessageListTray(const QList<Message*>& messageList);
#endif // QT_NO_SYSTEMTRAYICON

    static MessageDisplayDialog* createMessageDialog(const QString& title);
    void _showMessageBox(Message* message);
    void _showMessageBox(const QList<Message*>& messageList);

#ifndef QT_NO_SYSTEMTRAYICON
    QSystemTrayIcon* w_trayIcon;
#endif //QT_NO_SYSTEMTRAYICON

    Message* p_dummy;
    MessageListModel* p_messageListModel;
};

}; // namespace Draupnir::Messages

#endif // ABSTRACTMESSAGEHANDLER_H
