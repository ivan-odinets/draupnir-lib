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

#include "draupnir/core/MessageHandler.h"

#include "draupnir/traits/messages/DefaultMessageTraits.h"

Logger::Logger() :
    p_messageHandler{nullptr}
{}

void Logger::setMessageHandler(Draupnir::Messages::MessageHandler* handler)
{
    Q_ASSERT_X(handler, "Logger::setMessageHandler","Provided MessageHandler is nullptr");
    p_messageHandler = handler;
}

Draupnir::Messages::MessageGroup Logger::beginMessageGroup()
{
    Q_ASSERT_X(p_messageHandler, "Logger::beginMessageGroup",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");

    return p_messageHandler->beginMessageGroup();
}

bool Logger::groupExisting(Draupnir::Messages::MessageGroup group) const
{
    Q_ASSERT_X(p_messageHandler, "Logger::groupExisting",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");

    return p_messageHandler->groupExisting(group);
}

void Logger::flush(Draupnir::Messages::MessageGroup group)
{
    Q_ASSERT_X(p_messageHandler, "Logger::flush",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");

    p_messageHandler->flush(group);
}

void Logger::endMessageGroup(Draupnir::Messages::MessageGroup group)
{
    Q_ASSERT_X(p_messageHandler, "Logger::endMessageGroup",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");

    p_messageHandler->endMessageGroup(group);
}

void Logger::logDebug(const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::DebugMessageTrait>(what)
    );
}

void Logger::logDebug(const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::DebugMessageTrait>(what), group
    );
}

void Logger::logDebug(const QString& brief, const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::DebugMessageTrait>(brief,what)
    );
}

void Logger::logDebug(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::DebugMessageTrait>(brief,what), group
    );
}

void Logger::logInfo(const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::InfoMessageTrait>(what)
    );
}

void Logger::logInfo(const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::InfoMessageTrait>(what), group
    );
}

void Logger::logInfo(const QString& brief, const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::InfoMessageTrait>(brief,what)
    );
}

void Logger::logInfo(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::InfoMessageTrait>(brief,what), group
    );
}

void Logger::logWarning(const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::WarningMessageTrait>(what)
    );
}

void Logger::logWarning(const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::WarningMessageTrait>(what), group
    );
}

void Logger::logWarning(const QString& brief, const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::WarningMessageTrait>(brief,what)
    );
}

void Logger::logWarning(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::WarningMessageTrait>(brief,what), group
    );
}

void Logger::logError(const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::ErrorMessageTrait>(what)
    );
}

void Logger::logError(const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::ErrorMessageTrait>(what), group
    );
}

void Logger::logError(const QString& brief, const QString& what)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::ErrorMessageTrait>(brief,what)
    );
}

void Logger::logError(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group)
{
    _logMessage(
        new Draupnir::Messages::MessageTemplate<Draupnir::Messages::ErrorMessageTrait>(brief,what), group
    );
}

void Logger::_logMessage(Draupnir::Messages::Message* message)
{
    Q_ASSERT_X(p_messageHandler, "Logger::logMessage",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");

    p_messageHandler->processMessage(message);
}

void Logger::_logMessage(Draupnir::Messages::Message* message, Draupnir::Messages::MessageGroup group)
{
    Q_ASSERT_X(p_messageHandler, "Logger::logMessage",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");

    p_messageHandler->processMessage(message, group);
}

void Logger::logMessageList(const QList<Draupnir::Messages::Message*>& messageList)
{
    Q_ASSERT_X(p_messageHandler, "Logger::logMessageList",
               "Logger::setMessageHandler with valid MessageHandler must be called before.");
    p_messageHandler->processMessageList(messageList);
}
