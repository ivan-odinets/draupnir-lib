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

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

namespace Draupnir::Messages { class MessageHandler; }

class Logger;

#include "MessageGroup.h"
#include "core/MessageTemplate.h"

/*! @class Logger draupnir-lib/include/messages/Logger.h
 *  @brief A singleton class used to log messages from various parts of the application.
 *  @details The Logger class provides a global, interface for message logging. Messages can be logged immediately or grouped and
 *           shown later via MessageGroup. This is implemented within MessageHandler / MessageHandlerTemplate. This handler is
 *           specified by using Logger::setMessageHandler method.
 *
 * @see MessageHandler, MessageHandlerTemplate, MessageGroup, Message, MessageTemplate */

class Logger
{
public:
    /*! @brief Returns the global Logger instance.
     * @return Pointer to the global Logger singleton. */
    static Logger* get() {
        static Logger theOne;
        return &theOne;
    }

    /*! @brief Sets the MessageHandler instance used to process messages.
     *  @param handler Pointer to a valid MessageHandler instance.
     *  @note This method **must** be called before any logging occurs; otherwise a nullptr will be dereferenced.
     *  @note Not thread-safe. Must be called once during initialization. */
    void setMessageHandler(Draupnir::Messages::MessageHandler* handler);

    /*! @brief Starts a new message group for batch logging.
     *  @return A MessageGroup identifier. */
    Draupnir::Messages::MessageGroup beginMessageGroup();

    /*! @brief Returns true if specified MessageGroup is existing.
     *  @param group The group to be checked. */
    bool groupExisting(Draupnir::Messages::MessageGroup group) const;

    /*! @brief Flushes the messages stored in the given group.
     *  @param group The group to flush. */
    void flush(Draupnir::Messages::MessageGroup group);

    /*! @brief Finalizes a message group and releases its resources.
     *  @param group The group to end. */
    void endMessageGroup(Draupnir::Messages::MessageGroup group);

    /*! @brief Logs a debug message immediately.
     * @param what Message content. */
    void logDebug(const QString& what);

    /*! @brief Logs a debug message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier. */
    void logDebug(const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs a debug message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message. */
    void logDebug(const QString& brief, const QString& what);

    /*! @brief Logs debug message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier. */
    void logDebug(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs an informational message immediately.
     * @param what Message content. */
    void logInfo(const QString& what);

    /*! @brief Logs an informational message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier. */
    void logInfo(const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs an informational message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message. */
    void logInfo(const QString& brief, const QString& what);

    /*! @brief Logs an informational message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier. */
    void logInfo(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs a warning message immediately.
     * @param what Message content. */
    void logWarning(const QString& what);

    /*! @brief Logs a warning message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier. */
    void logWarning(const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs a warning message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message. */
    void logWarning(const QString& brief, const QString& what);

    /*! @brief Logs a warning message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier. */
    void logWarning(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs an error message immediately.
     * @param what Message content. */
    void logError(const QString& what);

    /*! @brief Logs an error message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier. */
    void logError(const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs an error message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message. */
    void logError(const QString& brief, const QString& what);

    /*! @brief Logs an error message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier. */
    void logError(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs a custom Message object.
     *  @param what Message content.
     *  @tparam MessageTrait The trait describing the message type. */
    template<class MessageTrait>
    void logMessage(const QString& what) {
        _logMessage(new Draupnir::Messages::MessageTemplate<MessageTrait>(what));
    }

    /*! @brief Logs a custom Message object with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @tparam MessageTrait The trait describing the message type. */
    template<class MessageTrait>
    void logMessage(const QString& brief, const QString& what) {
        _logMessage(new Draupnir::Messages::MessageTemplate<MessageTrait>(brief,what));
    }

    /*! @brief This method passes provided QList with the pointers to the Message objects to the configured MessageHandler. */
    [[deprecated]] void logMessageList(const QList<Draupnir::Messages::Message*>& messageList);

private:
    Logger();
    Q_DISABLE_COPY(Logger);
    Draupnir::Messages::MessageHandler* p_messageHandler;

    void _logMessage(Draupnir::Messages::Message* message);

    void _logMessage(Draupnir::Messages::Message* message, Draupnir::Messages::MessageGroup group);
};

#define logger Logger::get()

#endif // LOGGER_H
