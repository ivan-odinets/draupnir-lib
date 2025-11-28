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

#include <QObject>

#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    #include <QMutex>
    #include <QMutexLocker>
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

namespace Draupnir::Messages { class AbstractMessageHandler; }

class Logger;

#include "draupnir/message_system/core/Message.h"
#include "draupnir/message_system/core/MessageGroup.h"

/*! @class Logger draupnir/Logger.h
 *  @ingroup MessageSystem
 *  @brief A singleton class used to log messages from various parts of the application.
 *
 *  @details The @ref Logger class provides a global, interface for message logging. Messages can be logged immediately or
 *           grouped and shown later using the @ref Draupnir::Messages::MessageGroup objects together with @ref Logger methods.
 *
 * @todo Update and write reasonable documentation with examples, including the MessageGroup usage.
 * @todo Define what to do if logging to non-existant group? Should we print something to debug? Or Q_ASSERT_X? -> or let user define this?
 * @todo _logMessage method(s) - replace if else with something else. */

class Logger final : public QObject
{
    Q_OBJECT
public:
    /*! @brief Returns the global Logger instance.
     *  @return Reference to the global Logger singleton. */
    static Logger& get() {
        static Logger theOne;
        return theOne;
    }

    /*! @brief Final destructor. If no @ref Draupnir::Messages::AbstractMessageHandler object was setup - deletes internal
     *         p_tempMessageStorage object. */
    ~Logger() final;

    /*! @brief Sets the @ref AbstractMessageHandler instance which will be used to process messages and sends to this
     *         instance any @ref Draupnir::Messages::Message objects recieved before this call.
     *  @param handler Pointer to a valid @ref Draupnir::Messages::AbstractMessageHandler instance.
     * @todo What should we do when trying to set @ref Draupnir::Messages::AbstractMessageHandler for the second time?
     *       Allow this? Ignore this? Q_ASSERT_X this? */
    void setMessageHandler(Draupnir::Messages::AbstractMessageHandler* handler);

///@name This group of methods allows manipulating with message groups.
///@{
    /*! @brief Starts a new message group for batch logging.
     *  @return A Draupnir::Messages::MessageGroup identifier. */
    Draupnir::Messages::MessageGroup beginMessageGroup();

    /*! @brief Returns true if specified @ref Draupnir::Messages::MessageGroup is existing.
     *  @param group The group to be checked. */
    bool isGroupExisting(Draupnir::Messages::MessageGroup group) const;

    /*! @brief Flushes the messages stored in the given group. Group is kept anc can be used further to log more messages.
     *  @param group The group to be flushed. */
    void flush(Draupnir::Messages::MessageGroup group);

    /*! @brief Finalizes a message group and releases its resources. After calling this method - the group specified can
     *         not be reused.
     *  @param group The group to be ended. */
    void endMessageGroup(Draupnir::Messages::MessageGroup group);
///@}

///@name This group of methods allows logging custom @ref Draupnir::Messages::Message objects.
///@{
    /*! @brief Logs a preconstructed @ref Draupnir::Messages::Message object.
     * @note @ref Draupnir::Messages::Message object will be finaly sent to the @ref Draupnir::Messages::MessageListModel
     *       within the @ref Draupnir::Messages::AbstractMessageHandler. This model is responsible for deletion of the
     *       logged @ref Draupnir::Messages::Message object. If no handler was setup - deletion of the logged object will
     *       happen within the @ref Logger class. */
    void logMessage(Draupnir::Messages::Message* message);

    /*! @brief Logs a preconstructed @ref Draupnir::Messages::Message object to a specified group.
     * @note @ref Draupnir::Messages::Message object will be finaly sent to the @ref Draupnir::Messages::MessageListModel
     *       within the @ref Draupnir::Messages::AbstractMessageHandler. This model is responsible for deletion of the
     *       logged @ref Draupnir::Messages::Message object. If no handler was setup - deletion of the logged object will
     *       happen within the @ref Logger class. */
    void logMessage(Draupnir::Messages::Message* message, Draupnir::Messages::MessageGroup group);

    /*! @brief Logs a custom Message object described by the provided MessageTrait
     *  @tparam MessageTrait The trait describing the message type.
     *  @param what Message content. */
    template<class MessageTrait>
    void logMessage(const QString& what) {
        logMessage(Draupnir::Messages::Message::fromTrait<MessageTrait>(what));
    }

    /*! @brief Logs a custom Message object to a group.
     *  @tparam MessageTrait The trait describing the message type.
     *  @param what Message content.
     *  @param group Message group */
    template<class MessageTrait>
    void logMessage(const QString& what, Draupnir::Messages::MessageGroup group) {
        logMessage(Draupnir::Messages::Message::fromTrait<MessageTrait>(what), group);
    }

    /*! @brief Logs a custom Message object with brief and details to a group
     *  @tparam MessageTrait The trait describing the message type.
     *  @param brief Short summary.
     *  @param what Full message. */
    template<class MessageTrait>
    void logMessage(const QString& brief, const QString& what) {
        logMessage(Draupnir::Messages::Message::fromTrait<MessageTrait>(brief,what));
    }

    /*! @brief Logs a custom Message object with brief and details.
     *  @tparam MessageTrait The trait describing the message type.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group Group to be logged in. */
    template<class MessageTrait>
    void logMessage(const QString& brief, const QString& what, Draupnir::Messages::MessageGroup group) {
        logMessage(Draupnir::Messages::Message::fromTrait<MessageTrait>(brief,what),group);
    }
///@}

///@name This group of methods allows logging the default types of messages.
///@{
    /*! @brief Logs a debug message immediately.
     *  @param what Message content. */
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
     *  @param what Message content. */
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
     *  @param what Message content. */
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
     *  @param what Message content. */
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
///@}

signals:
    /*! @brief This signal is emitted when a single @ref Draupnir::Messages::Message object was logged and is ready to be
     *         processed by @ref Draupnir::Messages::AbstractMessageHandler.
     * @note Passing of message via signal is used in the multithreading context, when logX methods (e.g. @ref Logger::logMessage)
     *       can be called from many threads but final processing of @ref Draupnir::Messages::Message objects must be done
     *       within the GUI thread. */
    void messageReceived(Draupnir::Messages::Message* message);

    /*! @brief This signal is emitted when a pack of @ref Draupnir::Messages::Message objects was logged and is ready to be
     *         processed by @ref Draupnir::Messages::AbstractMessageHandler.
     * @note Passing of messages via signal is used in the multithreading context, when logX methods (e.g. @ref Logger::logMessage)
     *       can be called from many threads but final processing of @ref Draupnir::Messages::Message objects must be done
     *       within the GUI thread. */
    void messageListReceived(QList<Draupnir::Messages::Message*> messageList);

private:
    friend class LoggerTest;
    friend class LoggerMultithreadTest;

    /*! @brief Private constructor. Creates internal p_tempMessageStorage object. */
    explicit Logger(QObject* parent = nullptr);

    /*! @brief Thread-unsafe private implementation of the @ref Logger::beginMessageGroup method. */
    Draupnir::Messages::MessageGroup _beginMessageGroupImpl();

    /*! @brief Thread-unsafe private implementation of the @ref Logger::flush method. */
    void _flushImpl(Draupnir::Messages::MessageGroup group);

    /*! @brief Thread-unsafe private implementation of the @ref Logger::endMessageGroup method. */
    void _endMessageGroupImpl(Draupnir::Messages::MessageGroup group);

    /*! @brief Thread-unsafe private method to handle @ref Draupnir::Messages::Message objects. */
    void _logMessageImpl(Draupnir::Messages::Message* message);

    /*! @brief Thread-unsafe private method to handle @ref Draupnir::Messages::Message objects belonging to a specific
     *         group. */
    void _logMessageImpl(Draupnir::Messages::Message* message, Draupnir::Messages::MessageGroup group);

    /*! @brief Thread-unsafe private method to handle list of the @ref Draupnir::Messages::Message objects. */
    void _logMessageList(const QList<Draupnir::Messages::Message*>& messageList);

#ifndef DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD
    /*! @brief This QMutex guards access to private fields of the Logger class. */
    mutable QMutex m_resourceMutex;
#endif // DRAUPNIR_MESSAGE_SYSTEM_SINGLETHREAD

    /*! @brief This QList field is used as a temp storage of Message objects, to catch all of the Message objects which
     *         are logged before the Logger::setsetMessageHandler method was called. */
    QList<Draupnir::Messages::Message*>* p_tempMessageStorage;

    /*! @brief This map holds lists of @ref Draupnir::Messages::Message objects logged for a specific group. */
    QMap<Draupnir::Messages::MessageGroup,QList<Draupnir::Messages::Message*>> m_messageGroupsMap;

    /*! @brief Pointer to the @ref AbstractMessageHandler to which the @ref Draupnir::Messages::Message are sent. */
    Draupnir::Messages::AbstractMessageHandler* p_messageHandler;
};

#define logger Logger::get()

#endif // LOGGER_H
