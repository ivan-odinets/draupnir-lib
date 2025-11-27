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

#include "draupnir/Logger.h"

#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    #include <QMutexLocker>
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

#include "draupnir/core/AbstractMessageHandler.h"
#include "draupnir/traits/messages/DefaultMessageTraits.h"

Logger::~Logger()
{
    if (p_tempMessageStorage != nullptr) {
        if (p_messageHandler == nullptr)
            qDebug() << "Logger::~Logger() - deleting p_tempMessagsStorage. AbstractMessageHandler was not set.";

        for (auto i = p_tempMessageStorage->begin(); i != p_tempMessageStorage->end(); i++)
            delete *i;
        delete p_tempMessageStorage;
    }
}

void Logger::setMessageHandler(Draupnir::Messages::AbstractMessageHandler* handler)
{
    Q_ASSERT_X(handler, "Logger::setMessageHandler","Provided AbstractMessageHandler is nullptr.");
    Q_ASSERT_X(p_messageHandler == nullptr, "Logger::setMessageHandler",
               "This method can be called only once.");

#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};

    // Connect signals for a multithreading environment
    connect(this,       &Logger::messageReceived,
            handler,    &Draupnir::Messages::AbstractMessageHandler::handleMessage);
    connect(this,       &Logger::messageListReceived,
            handler,    &Draupnir::Messages::AbstractMessageHandler::handleMessageList, Qt::QueuedConnection);
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    p_messageHandler = handler;

    if (!p_tempMessageStorage->isEmpty()) {
        handler->handleMessageList(*p_tempMessageStorage);
        delete p_tempMessageStorage;
        p_tempMessageStorage = nullptr;
    }
}

Draupnir::Messages::MessageGroup Logger::beginMessageGroup()
{
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    return _beginMessageGroupImpl();
}

bool Logger::isGroupExisting(Draupnir::Messages::MessageGroup group) const
{
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};
#endif // #ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    return m_messageGroupsMap.contains(group);
}

void Logger::flush(Draupnir::Messages::MessageGroup group)
{
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    _flushImpl(group);
}

void Logger::endMessageGroup(Draupnir::Messages::MessageGroup group)
{
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    _endMessageGroupImpl(group);
}

void Logger::logMessage(Draupnir::Messages::Message* message)
{
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    _logMessageImpl(message);
}

void Logger::logMessage(Draupnir::Messages::Message* message, Draupnir::Messages::MessageGroup group)
{
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    QMutexLocker locker{&m_resourceMutex};
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    _logMessageImpl(message,group);
}

void Logger::logDebug(const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::DebugMessageTrait>(what)
    );
}

void Logger::logDebug(const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::DebugMessageTrait>(what), group
    );
}

void Logger::logDebug(const QString& brief, const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::DebugMessageTrait>(brief,what)
    );
}

void Logger::logDebug(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::DebugMessageTrait>(brief,what), group
    );
}

void Logger::logInfo(const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::InfoMessageTrait>(what)
    );
}

void Logger::logInfo(const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::InfoMessageTrait>(what), group
    );
}

void Logger::logInfo(const QString& brief, const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::InfoMessageTrait>(brief,what)
    );
}

void Logger::logInfo(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::InfoMessageTrait>(brief,what), group
    );
}

void Logger::logWarning(const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::WarningMessageTrait>(what)
    );
}

void Logger::logWarning(const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::WarningMessageTrait>(what), group
    );
}

void Logger::logWarning(const QString& brief, const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::WarningMessageTrait>(brief,what)
    );
}

void Logger::logWarning(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::WarningMessageTrait>(brief,what), group
    );
}

void Logger::logError(const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::ErrorMessageTrait>(what)
    );
}

void Logger::logError(const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::ErrorMessageTrait>(what), group
    );
}

void Logger::logError(const QString& brief, const QString& what)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::ErrorMessageTrait>(brief,what)
    );
}

void Logger::logError(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    logMessage(
        Draupnir::Messages::Message::fromTrait<Draupnir::Messages::ErrorMessageTrait>(brief,what), group
    );
}

Logger::Logger(QObject* parent) :
    QObject{parent},
    p_tempMessageStorage{new QList<Draupnir::Messages::Message*>},
    p_messageHandler{nullptr}
{}

Draupnir::Messages::MessageGroup Logger::_beginMessageGroupImpl()
{
    const auto newGroup = Draupnir::Messages::MessageGroup::generateUniqueGroup();

    if (m_messageGroupsMap.contains(newGroup)) {
        return Logger::_beginMessageGroupImpl();
    }

    m_messageGroupsMap.insert(newGroup, QList<Draupnir::Messages::Message*>{});
    return newGroup;
}

void Logger::_flushImpl(Draupnir::Messages::MessageGroup group)
{
    if (!m_messageGroupsMap.contains(group)) {
        qDebug() << "Trying to flush non-existant group.";
        return;
    }

    _logMessageList(m_messageGroupsMap[group]);
    m_messageGroupsMap[group].clear();
}

void Logger::_endMessageGroupImpl(Draupnir::Messages::MessageGroup group)
{
    _flushImpl(group);

    m_messageGroupsMap.remove(group);
}

void Logger::_logMessageImpl(Draupnir::Messages::Message* message)
{
    if (Q_LIKELY(p_messageHandler)) {
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
        emit messageReceived(message);
#else
        p_messageHandler->handleMessage(message);
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    } else {
        p_tempMessageStorage->append(message);
    }
}

void Logger::_logMessageImpl(Draupnir::Messages::Message* message, Draupnir::Messages::MessageGroup group)
{
    if (!m_messageGroupsMap.contains(group)) {
        qDebug() << "Non existant message group.";
        return;
    }

    m_messageGroupsMap[group].append(message);
}

void Logger::_logMessageList(const QList<Draupnir::Messages::Message*>& messageList)
{
    if (Q_LIKELY(p_messageHandler)) {
#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
        emit messageListReceived(messageList);
#else
        p_messageHandler->handleMessageList(messageList);
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    } else {
        p_tempMessageStorage->append(messageList);
    }
}
