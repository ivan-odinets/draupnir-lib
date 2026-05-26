/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/logging/Logger.h"

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    #include <QMutexLocker>
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

#include "draupnir/logging/core/AbstractMessageHandler.h"

namespace Draupnir::Logging {

Logger::~Logger()
{
    if (p_messageHandler == nullptr)
        qDebug() << "Logger::~Logger() - AbstractMessageHandler was not set.";

    if (p_tempMessageStorage != nullptr) {
        qDeleteAll(*p_tempMessageStorage);
        delete p_tempMessageStorage;
        p_tempMessageStorage = nullptr;
    }

    for (const auto& messageList : m_messageGroupsMap)
        qDeleteAll(messageList);

    m_messageGroupsMap.clear();
}

void Logger::setMessageHandler(Draupnir::Logging::AbstractMessageHandler* handler)
{
    Q_ASSERT(handler);

    MessageList messagesToDeliver;

    _synchronized([&] {
        Q_ASSERT(p_messageHandler == nullptr);
        Q_ASSERT(p_tempMessageStorage != nullptr);

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
        // Connect signals for a multithreading environment
        // Qt::QueuedConnection is specified explicitly to avoid direct handler calls from Logger::logMessage(). Without it,
        // logging from the handler thread would call AbstractMessageHandler immediately while Logger's mutex is still locked.
        connect(this, &Logger::messageReceived, handler, &AbstractMessageHandler::handleMessage, Qt::QueuedConnection);
        connect(this, &Logger::messageListReceived, handler, &AbstractMessageHandler::handleMessageList, Qt::QueuedConnection);
#endif

        p_messageHandler = handler;

        messagesToDeliver.swap(*p_tempMessageStorage);

        delete p_tempMessageStorage;
        p_tempMessageStorage = nullptr;
    });

    _deliverMessageListUnsafe(messagesToDeliver);
}

Draupnir::Logging::MessageGroup Logger::beginMessageGroup()
{
    return _synchronized([&](){
        return _beginMessageGroupUnsafe();
    });
}

bool Logger::isGroupExisting(Draupnir::Logging::MessageGroup group) const
{
    return _synchronized([&,group](){
        return m_messageGroupsMap.contains(group);
    });
}

void Logger::flush(Draupnir::Logging::MessageGroup group)
{
    MessageList messagesToDeliver;

    const bool shouldDeliver = _synchronized([&] {
        return _takeGroupMessagesForDeliveryUnsafe(group, false, messagesToDeliver);
    });

    if (shouldDeliver)
        _deliverMessageListUnsafe(messagesToDeliver);
}

void Logger::endMessageGroup(Draupnir::Logging::MessageGroup group)
{
    MessageList messagesToDeliver;

    const bool shouldDeliver = _synchronized([&] {
        return _takeGroupMessagesForDeliveryUnsafe(group, true, messagesToDeliver);
    });

    if (shouldDeliver)
        _deliverMessageListUnsafe(messagesToDeliver);
}

void Logger::logMessage(Draupnir::Logging::Message* message)
{
    Q_ASSERT(message);

    const bool shouldDeliver = _synchronized([&] {
        if (Q_UNLIKELY(p_messageHandler == nullptr)) {
            Q_ASSERT(p_tempMessageStorage);
            p_tempMessageStorage->append(message);
            return false;
        }
        return true;
    });

    if (shouldDeliver)
        _deliverMessageUnsafe(message);
}

void Logger::logMessage(Draupnir::Logging::Message* message, Draupnir::Logging::MessageGroup group)
{
    Q_ASSERT(message);

    const bool accepted = _synchronized([&] {
        auto it = m_messageGroupsMap.find(group);

        if (it == m_messageGroupsMap.end())
            return false;

        it.value().append(message);
        return true;
    });

    if (!accepted) {
        qDebug() << "Logger::logMessage() - non-existing message group.";
        delete message;
    }
}

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
Logger::Logger(QObject* parent) :
    QObject{parent},
#else
Logger::Logger() :
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
    p_tempMessageStorage{new QList<Draupnir::Logging::Message*>},
    p_messageHandler{nullptr}
{
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    qRegisterMetaType<Draupnir::Logging::MessageList>();
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
}

MessageGroup Logger::_beginMessageGroupUnsafe()
{
    const auto newGroup = MessageGroup::generateUniqueGroup();

    if (m_messageGroupsMap.contains(newGroup)) {
        return Logger::_beginMessageGroupUnsafe();
    }

    m_messageGroupsMap.insert(newGroup, QList<Message*>{});
    return newGroup;
}

bool Logger::_takeGroupMessagesForDeliveryUnsafe(MessageGroup group, bool removeGroup, MessageList& out)
{
    auto it = m_messageGroupsMap.find(group);

    if (it == m_messageGroupsMap.end()) {
        qDebug() << "Logger - non-existing message group.";
        return false;
    }

    if (p_messageHandler != nullptr) {
        out.swap(it.value());
    } else {
        Q_ASSERT(p_tempMessageStorage);
        p_tempMessageStorage->append(it.value());
        it.value().clear();
    }

    if (removeGroup) m_messageGroupsMap.erase(it);

    return !out.isEmpty();
}

void Logger::_deliverMessageUnsafe(Message* message)
{
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    emit messageReceived(message);
#else
    Q_ASSERT(p_messageHandler);
    p_messageHandler->handleMessage(message);
#endif
}

void Logger::_deliverMessageListUnsafe(const MessageList& messages)
{
    if (messages.isEmpty())
        return;

#ifndef QT_NO_DEBUG
    for (Message* message : messages)
        Q_ASSERT(message);
#endif

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    emit messageListReceived(messages);
#else
    Q_ASSERT(p_messageHandler);
    p_messageHandler->handleMessageList(messages);
#endif
}

}; // namespace Draupnir::Logging
