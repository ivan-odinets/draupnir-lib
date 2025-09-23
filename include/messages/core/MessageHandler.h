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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>

#include <QMap>

#include "core/Message.h"
#include "MessageType.h"
#include "MessageGroup.h"
#include "Notification.h"

class QSystemTrayIcon;

namespace Draupnir::Messages
{

class MessageListModel;

/*! @class MessageHandler draupnir-lib/src/messages/core/MessageHandler.h
 *  @brief Abstract base class for processing and displaying application messages.
 *  @details This class provides an interface for managing how messages are processed, stored, and displayed to the user. Messages
 *           can be handled immediately or in grouped batches via MessageGroup.
 *
 *           Interaction with MessageHandlerTemplate MessageHandler defines the runtime API while MessageHandlerTemplate supplies a
 *           concrete implementation that maps compile-time message traits to Notification types. Logger sends messages to the
 *           current MessageHandler instance, which consults these policies to show or queue notifications.
 *
 * @note The actual policy for each message type is defined in derived class (MessageHandlerTemplate).
 * @note MessageHandler is intended to be used together with the Logger singleton as its primary backend.
 * @note Message objects processed via MessageHandler::processMessage are also stored in a MessageListModel to display within
 *       LogWidget / MessageListView
 *
 * @see MessageHandlerTemplate, Logger, MessageGroup, Message, Notification */

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    /*! @brief Default constructor. Creates the underlying MessageListModel. */
    explicit MessageHandler(QObject* parent = nullptr);

    /*! @brief Destructor. Deletes internal MessageListModel. */
    virtual ~MessageHandler() override;

#ifndef QT_NO_SYSTEMTRAYICON
    /*! @brief This method is used to specify tray icon used to show notifications in systemtray.
     * @note This method is available only when building with systemtray support. */
    void setTrayIcon(QSystemTrayIcon* trayIcon) { w_trayIcon = trayIcon; }

#endif // QT_NO_SYSTEMTRAYICON

    /*! @brief General method to set the Notification::Type for a certain MessageType. */
    virtual void setNotification(MessageType type, const Notification::Type notificationType) = 0;

    /*! @brief Returns Notification::Type which is used for given MessageType. */
    virtual Notification::Type notification(uint64_t type) = 0;

    /*! @brief Gives access to MessageListModel, containing logs about stuff happened. */
    MessageListModel* messages() { return p_messageListModel; }

    /*! @brief Starts a new message group for batch logging.
     *  @return A MessageGroup identifier. */
    MessageGroup beginMessageGroup();

    /*! @brief Returns true if specified MessageGroup is existing within this MessageHandler.
     *  @param group The group to be checked. */
    bool groupExisting(MessageGroup group) const { return m_messageGroupsMap.contains(group); }

    /*! @brief Flushes the messages stored in the given group.
     *  @param group The group to flush. */
    void flush(MessageGroup group);

    /*! @brief Finalizes a message group and releases its resources.
     *  @param group The group to end. */
    void endMessageGroup(MessageGroup group);

public slots:
    /*! @brief Shows to user how specific Notification::Type things will be displayed. */
    void showDummy(Notification::Type type);

    /*! @brief Processes a single Message object. The message is added to the MessageListModel and then the appropriate
     *         notification is displayed to the user. */
    void processMessage(Message* message);

    /*! @brief Processes a Message object as part of a batch. The message is added to the MessageListModel, but the notification
     *         is shown only after MessageHandler::flush or MessageHandler::endMessageGroup is called. */
    void processMessage(Message* message, MessageGroup group);

    /*! @brief Processes a list of Message objects. Each Message is added to the MessageListModel and then the proper notification
     *         is displayed to the user.
     * @note If different notification methods are required, each group of messages will use its own notification type. */
    void processMessageList(const QList<Message*>& messageList);

    /*! @brief Shows the given Message using the Notification::Type stored for its type.
     * @note During execution the Message object is **not** added to the MessageListModel. */
    void showMessage(Message* message);

    /*! @brief Shows the given Message using the provided Notification::Type.
     * @note During execution the Message object is **not** added to the MessageListModel. */
    void showMessage(Message* message, const Notification::Type type);

    /*! @brief Shows a list of Message objects using the saved Notification::Type for their type.
     * @note During execution the Message objects are **not** added to the MessageListModel. */
    void showMessageList(const QList<Message*>& messageList);

    /*! @brief Shows a list of Message objects using the provided Notification::Type.
     * @note During execution the Message objects are **not** added to the MessageListModel. */
    void showMessageList(const QList<Message*>& messageList, const Notification::Type type);

signals:
    /*! @brief This signal is emitted when a type of notification for specific Message::Type has changed. */
    void notificationTypeChanged(uint64_t messageType, Notification::Type notification);

private:
    friend class MessageSystemGeneralIT;

#ifndef QT_NO_SYSTEMTRAYICON
    void _showMessageInSystray(Message* message);                    /*!< @brief Shows tray notification with Message object. */
    void _showMessageListTray(const QList<Message*>& messageList);   /*!< @brief Shows tray notification about several Message
                                                                                 objects placed in the log. */
#endif // QT_NO_SYSTEMTRAYICON

    void _showMessageBox(Message* message);
    void _showMessageBox(const QList<Message*>& messageList);

#ifndef QT_NO_SYSTEMTRAYICON
    // Outer objects
    QSystemTrayIcon* w_trayIcon;
#endif //QT_NO_SYSTEMTRAYICON

    // Dummy message to show
    Message* p_dummy;

    // Message "endpoint"
    MessageListModel* p_messageListModel;

    QMap<MessageGroup,QList<Message*> > m_messageGroupsMap;
};

}; // namespace Draupnir::Messages

#endif // MESSAGEHANDLER_H
