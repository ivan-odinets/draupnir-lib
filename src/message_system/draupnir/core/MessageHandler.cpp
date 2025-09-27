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

#include "draupnir/core/MessageHandler.h"

#include <QApplication>
#include <QSystemTrayIcon>

#include "draupnir/traits/messages/DefaultMessageTraits.h"
#include "draupnir/core/MessageTemplate.h"
#include "draupnir/models/MessageListModel.h"
#include "draupnir/ui/windows/MessageDisplayDialog.h"

namespace Draupnir::Messages
{

MessageHandler::MessageHandler(QObject* parent) :
    QObject{parent},
    w_trayIcon{nullptr},
    p_dummy{new MessageTemplate<InfoMessageTrait>(tr("Test message"))},
    p_messageListModel{new MessageListModel}
{}

MessageHandler::~MessageHandler()
{
    p_messageListModel->deleteLater();
    delete p_dummy;
}

MessageGroup MessageHandler::beginMessageGroup()
{
    const MessageGroup newGroup = MessageGroup::generateUniqueGroup();

    if (m_messageGroupsMap.contains(newGroup)) {
        qDebug() << "Group is existing? O_o";
        return MessageHandler::beginMessageGroup();
    }

    m_messageGroupsMap.insert(newGroup, QList<Message*>{});
    return newGroup;
}

void MessageHandler::flush(MessageGroup group)
{
    if (!m_messageGroupsMap.contains(group)) {
        qDebug() << "MessageHandler::flush - trying to flush non-existant group.";
        return;
    }

    const QList<Message*>& messages = m_messageGroupsMap[group];
    showMessageList(messages);
    m_messageGroupsMap[group].clear();
}

void MessageHandler::endMessageGroup(MessageGroup group)
{
    if (!m_messageGroupsMap.contains(group)) {
        qDebug() << "MessageHandler::endMessageGroup - trying to close non-existant group.";
        return;
    }

    const QList<Message*>& messages = m_messageGroupsMap[group];
    showMessageList(messages);
    m_messageGroupsMap.remove(group);
}

void MessageHandler::showDummy(Notification::Type type)
{
    switch (type) {
    case Notification::None:
        return;
#ifndef QT_NO_SYSTEMTRAYICON
    case Notification::Systemtray:
        _showMessageInSystray(p_dummy);
        return;
#endif // QT_NO_SYSTEMTRAYICON
    case Notification::MessageBoxType:
        _showMessageBox(p_dummy);
        return;
    case Notification::UnknownType:
        Q_ASSERT_X(false, "MessageHandler::showDummy", "Unknown notification type");
        return;
    }
}

void MessageHandler::processMessage(Message* message)
{
    p_messageListModel->append(message);
    showMessage(message);
}

void MessageHandler::processMessage(Message* message, MessageGroup group)
{
    p_messageListModel->append(message);

    if (!m_messageGroupsMap.contains(group)) {
        qDebug() << "MessageHandler::processMessage - trying to use non-existant group.";
        return;
    }

    m_messageGroupsMap[group].append(message);
}

void MessageHandler::processMessageList(const QList<Message*>& messageList)
{
    p_messageListModel->append(messageList);
    showMessageList(messageList);
}

void MessageHandler::showMessage(Message* message)
{
    showMessage(message,notification(message->type()));
}

void MessageHandler::showMessage(Message* message, Notification::Type type)
{
    switch (type) {
    case Notification::None:
        return;
#ifndef QT_NO_SYSTEMTRAYICON
    case Notification::Systemtray:
        _showMessageInSystray(message);
        return;
#endif //QT_NO_SYSTEMTRAYICON
    case Notification::MessageBoxType:
        _showMessageBox(message);
        return;
    case Notification::UnknownType:
        Q_ASSERT_X(false, "MessageHandler::showMessage", "Unknown notification type");
        return;
    }
}

void MessageHandler::showMessageList(const QList<Message*>& messageList)
{
    // Temp containers for different Message types
#ifndef QT_NO_SYSTEMTRAYICON
    QList<Message*> trayMessages;
#endif // QT_NO_SYSTEMTRAYICON
    QList<Message*> messageBoxMessages;

    for (const auto& message : messageList) {
        const Notification::Type type = notification(message->type());
        if (type == Notification::MessageBoxType)
            messageBoxMessages.append(message);
#ifndef QT_NO_SYSTEMTRAYICON
        else if (type == Notification::Systemtray)
            trayMessages.append(message);
#endif // QT_NO_SYSTEMTRAYICON
    }

#ifndef QT_NO_SYSTEMTRAYICON
    // Show messages
    if (!trayMessages.isEmpty()) {
        trayMessages.count() == 1 ?
            _showMessageInSystray(trayMessages.first()) :
            _showMessageListTray(trayMessages);
    }
#endif // QT_NO_SYSTEMTRAYICON

    if (!messageBoxMessages.isEmpty())
        _showMessageBox(messageBoxMessages);
}

void MessageHandler::showMessageList(const QList<Message*>& messageList, Notification::Type type)
{
    switch (type) {
    case Notification::None:
        return;
#ifndef QT_NO_SYSTEMTRAYICON
    case Notification::Systemtray:
        _showMessageListTray(messageList);
        return;
#endif // QT_NO_SYSTEMTRAYICON
    case Notification::MessageBoxType:
        _showMessageBox(messageList);
        return;
    case Notification::UnknownType:
        Q_ASSERT_X(false, "MessageHandler::showMessageList", "Unknown notification type");
        return;
    }
}

#ifndef QT_NO_SYSTEMTRAYICON

void MessageHandler::_showMessageInSystray(Message* message)
{
    Q_ASSERT_X(w_trayIcon, "MessageHandler::_showMessageInSystray", "Tray icon is null");
    w_trayIcon->showMessage(
        message->brief(),
        message->what(),
        message->icon()
    );
}

void MessageHandler::_showMessageListTray(const QList<Message*>& messageList)
{
    Q_ASSERT_X(w_trayIcon, "MessageHandler::_showMessageListTray", "Tray icon is null");
    w_trayIcon->showMessage(
        qApp->applicationName(),
        tr("%1 new messages received. Check log for details.").arg(messageList.count()),
        QSystemTrayIcon::Information
    );
}

#endif // QT_NO_SYSTEMTRAYICON

void MessageHandler::_showMessageBox(Message* message)
{
    MessageDisplayDialog dialog;
    dialog.setWindowIcon(qApp->windowIcon());
    dialog.setWindowTitle(message->brief() + QLatin1String{" - "} + qApp->applicationName());
    dialog.addMessage(message);
    dialog.exec();
}

void MessageHandler::_showMessageBox(const QList<Message*>& messageList)
{
    MessageDisplayDialog dialog;
    dialog.setWindowIcon(qApp->windowIcon());
    dialog.setWindowTitle(qApp->applicationName());
    dialog.addMessageList(messageList);
    dialog.exec();
}

}; // namespace Draupnir::Messages
