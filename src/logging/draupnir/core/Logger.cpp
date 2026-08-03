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

#include "draupnir/logging/core/Logger.h"

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    #include <QMutexLocker>
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

#include "draupnir/logging/core/AbstractMessageReceiver.h"

using namespace Draupnir::Messages;

namespace Draupnir::Logging
{

Logger::~Logger()
{
#ifndef QT_NO_DEBUG
    bool hasGroupedMessages = false;

    for (auto it = m_messageGroupsMap.constBegin(); it != m_messageGroupsMap.constEnd(); it++) {
        if (!it.value().isEmpty()) {
            hasGroupedMessages = true;
            break;
        }
    }

    if (!m_tempMessageStorage.isEmpty() || hasGroupedMessages)
        qDebug() << "Logger::~Logger() - undelivered messages remain.";
#endif
}

void Logger::setMessageReceiver(Draupnir::Logging::AbstractMessageReceiver* handler)
{
    Q_ASSERT(handler);

    MessageList messagesToDeliver;

    _synchronized([&] {
        Q_ASSERT(p_messageReceiver == nullptr);

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
        // Connect signals for a multithreading environment
        // Qt::QueuedConnection is specified explicitly to avoid direct handler calls from Logger::logMessage(). Without it,
        // logging from the handler thread would call AbstractMessageHandler immediately while Logger's mutex is still locked.
        connect(this, &Logger::messageReceived, handler, &AbstractMessageReceiver::handleMessage, Qt::QueuedConnection);
        connect(this, &Logger::messageListReceived, handler, &AbstractMessageReceiver::handleMessageList, Qt::QueuedConnection);
#endif

        p_messageReceiver = handler;

        messagesToDeliver.swap(m_tempMessageStorage);
    });

    _deliverMessageListUnsafe(messagesToDeliver);
}

MessageGroupId Logger::beginMessageGroup()
{
    return _synchronized([&](){
        return _beginMessageGroupUnsafe();
    });
}

bool Logger::isGroupExisting(MessageGroupId group) const
{
    return _synchronized([&,group](){
        return m_messageGroupsMap.contains(group);
    });
}

void Logger::flush(MessageGroupId group)
{
    Q_ASSERT_X(m_messageGroupsMap.contains(group), Q_FUNC_INFO, "Non-existing message group.");
    MessageList messagesToDeliver;

    const bool shouldDeliver = _synchronized([&] {
        return _takeGroupMessagesForDeliveryUnsafe(group, false, messagesToDeliver);
    });

    if (shouldDeliver)
        _deliverMessageListUnsafe(messagesToDeliver);
}

void Logger::endMessageGroup(MessageGroupId group)
{
    MessageList messagesToDeliver;

    const bool shouldDeliver = _synchronized([&] {
        Q_ASSERT_X(m_messageGroupsMap.contains(group), Q_FUNC_INFO, "Non-existing message group.");
        return _takeGroupMessagesForDeliveryUnsafe(group, true, messagesToDeliver);
    });

    if (shouldDeliver)
        _deliverMessageListUnsafe(messagesToDeliver);
}

void Logger::logMessage(MessagePtr message)
{
    Q_ASSERT(message);

    const bool shouldDeliver = _synchronized([&] {
        if (Q_UNLIKELY(p_messageReceiver == nullptr)) {
            m_tempMessageStorage.append(message);
            return false;
        }
        return true;
    });

    if (shouldDeliver)
        _deliverMessageUnsafe(message);
}

void Logger::logMessage(MessagePtr message, MessageGroupId group)
{
    Q_ASSERT(message);

    const bool accepted = _synchronized([&] {
        auto it = m_messageGroupsMap.find(group);

        if (it == m_messageGroupsMap.end()) {
            return false;
        }

        it.value().append(message);
        return true;
    });

    Q_ASSERT_X(accepted, Q_FUNC_INFO, "Non-existing message group.");
}

void Logger::logMessageList(const Draupnir::Messages::MessageList& list)
{
#ifndef QT_NO_DEBUG
    for (const MessagePtr& message : list)
        Q_ASSERT(message);
#endif

    const bool shouldDeliver = _synchronized([&] {
        if (Q_UNLIKELY(p_messageReceiver == nullptr)) {
            m_tempMessageStorage.append(list);
            return false;
        }
        return true;
    });

    if (shouldDeliver)
        _deliverMessageListUnsafe(list);
};

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
Logger::Logger(QObject* parent) :
    QObject{parent},
#else
Logger::Logger() :
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
    p_messageReceiver{nullptr}
{
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    qRegisterMetaType<Draupnir::Messages::MessagePtr>();
    qRegisterMetaType<Draupnir::Messages::MessageList>();
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
}

MessageGroupId Logger::_beginMessageGroupUnsafe()
{
    const auto newGroup = MessageGroupId::generate();

    if (m_messageGroupsMap.contains(newGroup))
        return Logger::_beginMessageGroupUnsafe();

    m_messageGroupsMap.insert(newGroup, MessageList{});
    return newGroup;
}

bool Logger::_takeGroupMessagesForDeliveryUnsafe(MessageGroupId group, bool removeGroup, MessageList& out)
{
    auto it = m_messageGroupsMap.find(group);

    if (it == m_messageGroupsMap.end()) {
        return false;
    }

    if (p_messageReceiver != nullptr) {
        out.swap(it.value());
    } else {
        m_tempMessageStorage.append(it.value());
        it.value().clear();
    }

    if (removeGroup) m_messageGroupsMap.erase(it);

    return !out.isEmpty();
}

void Logger::_deliverMessageUnsafe(MessagePtr message)
{
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    emit messageReceived(message);
#else
    Q_ASSERT(p_messageReceiver);
    p_messageReceiver->handleMessage(message);
#endif
}

void Logger::_deliverMessageListUnsafe(const MessageList& messageList)
{
    if (messageList.isEmpty())
        return;

#ifndef QT_NO_DEBUG
    for (MessagePtr message : messageList)
        Q_ASSERT(message);
#endif

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    emit messageListReceived(messageList);
#else
    Q_ASSERT(p_messageReceiver);
    p_messageReceiver->handleMessageList(messageList);
#endif
}

} // namespace Draupnir::Logging
