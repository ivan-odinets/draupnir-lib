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

#include "draupnir/core/AbstractMessageHandler.h"

#include <QApplication>
#include <QSystemTrayIcon>

#include "draupnir/models/MessageListModel.h"
#include "draupnir/traits/messages/InfoMessageTrait.h"
#include "draupnir/ui/windows/MessageDisplayDialog.h"

namespace Draupnir::Messages
{

AbstractMessageHandler::~AbstractMessageHandler()
{
    p_messageListModel->deleteLater();
    delete p_dummy;
}

AbstractMessageHandler::AbstractMessageHandler(QObject* parent) :
    QObject{parent},
    w_trayIcon{nullptr},
    p_dummy{Message::fromTrait<InfoMessageTrait>(tr("Test message"))},
    p_messageListModel{new MessageListModel}
{}

void AbstractMessageHandler::showDummy(Notification::Type type)
{
    Q_ASSERT_X(type != Notification::UnknownType, "AbstractMessageHandler::showDummy",
               "Notification::UnknownType is not supposed to be shown to a user.");
    showMessage(p_dummy, type);
}

void AbstractMessageHandler::handleMessage(Message* message)
{
    Q_ASSERT_X(message, "AbstractMessageHandler::handleMessage",
               "Specified message pointer is nullptr. ");
    p_messageListModel->append(message);
    showMessage(message);
}

void AbstractMessageHandler::handleMessageList(const QList<Message*>& messageList)
{
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Message* message : messageList) {
        Q_ASSERT_X(message, "AbstractMessageHandler::handleMessageList", "One of the provided Message* is nullptr!");
    }
#endif // QT_NO_DEBUG

    p_messageListModel->append(messageList);
    showMessageList(messageList);
}

void AbstractMessageHandler::showMessage(Message* message)
{
    Q_ASSERT_X(message, "AbstractMessageHandler::showMessage",
               "Specified message pointer is nullptr. ");
    showMessage(message,notification(message->type()));
}

void AbstractMessageHandler::showMessage(Message* message, Notification::Type type)
{
    Q_ASSERT_X(message, "AbstractMessageHandler::showMessage",
               "Specified message pointer is nullptr. ");
    Q_ASSERT_X(type != Notification::UnknownType, "AbstractMessageHandler::showMessage",
               "Notification::UnknownType is not supposed to be shown to a user.");

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
        Q_ASSERT(false);
    }
}

void AbstractMessageHandler::showMessageList(const QList<Message*>& messageList)
{
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Message* message : messageList) {
        Q_ASSERT_X(message, "AbstractMessageHandler::showMessageList", "One of the provided Message* is nullptr!");
    }
#endif // QT_NO_DEBUG

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

void AbstractMessageHandler::showMessageList(const QList<Message*>& messageList, Notification::Type type)
{
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Message* message : messageList) {
        Q_ASSERT_X(message, "AbstractMessageHandler::showMessageList", "One of the provided Message* is nullptr!");
    }
#endif // QT_NO_DEBUG
    Q_ASSERT_X(type != Notification::UnknownType, "AbstractMessageHandler::showMessageList",
               "Notification::UnknownType is not supposed to be shown to a user.");

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

void AbstractMessageHandler::_showMessageInSystray(Message* message)
{
    Q_ASSERT_X(w_trayIcon, "MessageHandler::_showMessageInSystray",
               "Tray icon is null");
    Q_ASSERT_X(message, "AbstractMessageHandler::_showMessageInSystray",
               "Specified message pointer is nullptr. ");

    w_trayIcon->showMessage(
        message->brief(),
        message->what(),
        message->icon()
    );
}

void AbstractMessageHandler::_showMessageListTray(const QList<Message*>& messageList)
{
    Q_ASSERT_X(w_trayIcon, "MessageHandler::_showMessageListTray", "Tray icon is null");
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Message* message : messageList) {
        Q_ASSERT_X(message, "AbstractMessageHandler::_showMessageListTray", "One of the provided Message* is nullptr!");
    }
#endif // QT_NO_DEBUG

    w_trayIcon->showMessage(
        qApp->applicationName(),
        tr("%1 new messages received. Check log for details.").arg(messageList.count()),
        QSystemTrayIcon::Information
    );
}

#endif // QT_NO_SYSTEMTRAYICON

MessageDisplayDialog* AbstractMessageHandler::createMessageDialog(const QString& title)
{
    MessageDisplayDialog* result = new MessageDisplayDialog;
    result->setAttribute(Qt::WA_DeleteOnClose);
    result->setWindowIcon(qApp->windowIcon());
    result->setWindowTitle(title);
    return result;
}

void AbstractMessageHandler::_showMessageBox(Message* message)
{
    Q_ASSERT_X(message, "AbstractMessageHandler::_showMessageBox",
               "Specified message pointer is nullptr. ");

    auto dialog = createMessageDialog(message->brief() + QLatin1String{" - "} + qApp->applicationName());
    dialog->addMessage(message);
    dialog->show();
}

void AbstractMessageHandler::_showMessageBox(const QList<Message*>& messageList)
{
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Message* message : messageList) {
        Q_ASSERT_X(message, "AbstractMessageHandler::_showMessageBox", "One of the provided Message* is nullptr!");
    }
#endif // QT_NO_DEBUG

    auto dialog = createMessageDialog(qApp->applicationName());
    dialog->addMessageList(messageList);
    dialog->show();
}

}; // namespace Draupnir::Messages
