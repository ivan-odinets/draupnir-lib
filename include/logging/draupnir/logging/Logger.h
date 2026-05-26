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

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    #include <QMutex>
    #include <QMutexLocker>
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

#include "draupnir/logging/messages/Message.h"
#include "draupnir/logging/messages/MessageGroup.h"
#include "draupnir/logging/messages/MessageLevels.h"
#include "draupnir/logging/messages/MessageCategories.h"

namespace Draupnir::Logging {

class AbstractMessageHandler;

/*! @class Logger draupnir/logging/Logger.h
 *  @ingroup Logging
 *  @brief Central entry point for creating and submitting log messages.
 *
 *  @details The @ref Logger class is used to create and submit @ref Draupnir::Logging::Message objects from different parts
 *           of the application.
 *
 *           Messages can be logged immediately or associated with a @ref Draupnir::Logging::MessageGroup and flushed later.
 *
 *           If no @ref Draupnir::Logging::MessageHandlerInterface is installed, logged messages are stored internally and
 *           forwarded once @ref setMessageHandler is called.
 *
 *           In the default multithreaded mode public logging methods are guarded internally and messages are delivered to
 *           the handler through Qt signals.
 *
 * @todo Question: What to do if logging to non-existant group? Should we print something to debug? Or Q_ASSERT_X? -> or
 *       let user define this? */

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
class Logger final : public QObject {
    Q_OBJECT
#else
class Logger
{
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
public:
    /*! @brief Returns the global Logger instance.
     *  @return Reference to the global Logger singleton. */
    static Logger& get() {
        static Logger theOne;
        return theOne;
    }

    /*! @brief Destroys the logger. If no @ref Draupnir::Logging::MessageHandlerInterface was installed, the logger deletes
     *         messages stored in its internal temporary storage. Messages already transferred to a message handler are not
     *         owned by the logger anymore. */
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    ~Logger() final;
#else
    ~Logger();
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

    /*! @brief Sets the message handler used to process logged messages.
     *  @param handler Pointer to a valid message handler.
     *  @details After the handler is installed, all messages stored in the internal temporary storage are forwarded to it.
     *           Ownership of forwarded messages is transferred to the handler.
     * @note The logger does not take ownership of @p handler. The caller must ensure that the handler remains valid while
     *       it is  used by the logger.
     * @todo What should we do when trying to set @ref Draupnir::Messages::AbstractMessageHandler for the second time?
     *       Allow this? Ignore this? Q_ASSERT_X this? */
    void setMessageHandler(AbstractMessageHandler* handler);

///@name This group of methods allows manipulating with message groups.
///@{
    /*! @brief Starts a new message group.
     *  @return New @ref Draupnir::Logging::MessageGroup identifier. */
    MessageGroup beginMessageGroup();

    /*! @brief Checks whether the specified message group exists.
     *  @param group Message group to check.
     *  @return `true` if the group exists; otherwise `false`. */
    bool isGroupExisting(MessageGroup group) const;

    /*! @brief Flushes messages stored in the given group. Messages stored in the group are submitted for normal processing.
     *         The group itself remains valid and can be used to collect more messages later.
     *  @param group Message group to flush. */
    void flush(MessageGroup group);

    /*! @brief Ends a message group. Messages stored in the group are submitted for normal processing. After this call the
     *         group is removed and must not be reused.
     *  @param group Message group to end. */
    void endMessageGroup(MessageGroup group);
///@}

///@name This group of methods allows logging custom @ref Draupnir::Logging::Message objects.
///@{
    /*! @brief Logs a preconstructed @ref Draupnir::Logging::Message object. This method takes ownership of `message`. The
     *         caller must not delete the message after passing it to this method.
     *  @param message Message object to log.
     * @note The pointer must not be null.  */
    void logMessage(Message* message);

    /*! @brief Logs a preconstructed message into a message group. This method takes ownership of `message`. The caller must
     *         not delete the message after passing it to this method.
     *  @param message Message object to log.
     *  @param group Message group identifier.
     * @note The pointer must not be null.  */
    void logMessage(Message* message, MessageGroup group);
///@}

///@name This group of methods allows logging the default levels of messages.
///@{
    /*! @brief Logs a message with the specified level and category.
     *  @param what Message text.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(const QString& what, MessageLevel::Value messageLevel, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(Message::create(what, messageLevel, messageCategory));
    }

    /*! @brief Logs a message with the specified level and category into a group.
     *  @param what Message text.
     *  @param group Message group identifier.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(const QString& what, MessageGroup group, MessageLevel::Value messageLevel, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(Message::create(what, messageLevel, messageCategory), group);
    }

    /*! @brief Logs a message with brief and full text.
     *  @param brief Short message summary.
     *  @param what Full message text.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(const QString& brief, const QString& what, MessageLevel::Value messageLevel, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(Message::create(brief, what, messageLevel, messageCategory));
    }

    /*! @brief Logs a message with brief and full text into a group.
     *  @param brief Short message summary.
     *  @param what Full message text.
     *  @param group Message group identifier.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(const QString& brief, const QString& what, MessageGroup group, MessageLevel::Value messageLevel, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(Message::create(brief, what, messageLevel, messageCategory), group);
    }

    /*! @brief Logs a debug message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logDebug(const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs a debug message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logDebug(const QString& what, MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, group, MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs a debug message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logDebug(const QString& brief, const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs debug message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logDebug(const QString& brief, const QString& what, Draupnir::Logging::MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, group, MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs an info message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logInfo(const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs an info message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logInfo(const QString& what, MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, group, MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs an info message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logInfo(const QString& brief, const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs an info message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logInfo(const QString& brief, const QString& what, Draupnir::Logging::MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, group, MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs a warning message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logWarning(const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs a warning message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logWarning(const QString& what, MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, group, MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs a warning message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logWarning(const QString& brief, const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs a warning message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logWarning(const QString& brief, const QString& what, Draupnir::Logging::MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, group, MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs an error message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logError(const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, MessageLevel::Error, messageCategory);
    }

    /*! @brief Logs an error message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logError(const QString& what, MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(what, group, MessageLevel::Error, messageCategory);
    }

    /*! @brief Logs an error message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logError(const QString& brief, const QString& what, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, MessageLevel::Error, messageCategory);
    }

    /*! @brief Logs an error message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logError(const QString& brief, const QString& what, Draupnir::Logging::MessageGroup group, MessageCategory messageCategory = MessageCategory::Default) {
        logMessage(brief, what, group, MessageLevel::Error, messageCategory);
    }

///@}

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
signals:
    /*! @brief Emitted when a single message is logged and ready to be processed.
     *  @param message Logged message.
     * @note Passing messages through this signal is used in the multithreaded mode, where logging methods may be called from
     *       different threads while final message processing is performed in the receiver thread.
     * @note Ownership of `message` is transferred to the connected handler. */
    void messageReceived(Draupnir::Logging::Message* message);

    /*! @brief Emitted when a list of messages is ready to be processed.
     *  @param messageList List of logged messages.
     * @note Passing messages through this signal is used in the multithreaded mode, where logging methods may be called from
     *       different threads while final message processing is performed in the receiver thread.
     * @note Ownership of all messages in `messageList` is transferred to the connected handler. */
    void messageListReceived(Draupnir::Logging::MessageList messageList);

#endif // DRAUPNIR_LOGGING_SINGLETHREAD

private:
    friend class LoggerTest;
    friend class LoggerMultithreadTest;

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    /*! @brief Mutex guarding access to logger internal state. */
    mutable QMutex m_resourceMutex;
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

    /*! @brief Temporary storage for messages logged before a handler is installed. The logger owns messages stored in this list.
     *         Once a message handler is installed, messages from this storage are forwarded to the handler and ownership is
     *         transferred. */
    QList<Message*>* p_tempMessageStorage;

    /*! @brief Stores messages associated with active message groups. The logger owns messages stored in this map until the
     *         corresponding group is flushed or ended. */
    QMap<MessageGroup,QList<Message*>> m_messageGroupsMap;

    /*! @brief Message handler used to process submitted messages.
     * @note This pointer is non-owning. */
    AbstractMessageHandler* p_messageHandler;

    /*! @brief Private constructor. Creates internal p_tempMessageStorage object. */
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    explicit Logger(QObject* parent = nullptr);
#else
    Logger();
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    /*! @brief In multithreaded mode this method locks the logger state mutex (`m_resourceMutex`), invokes the callable, and
     *         unlocks the mutex before returning. */
    template<class Func>
    decltype(auto) _synchronized(this auto& self, Func&& func) {
        QMutexLocker locker{&self.m_resourceMutex};
        return std::forward<Func>(func)();
    }
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

#ifdef DRAUPNIR_LOGGING_SINGLETHREAD
    /*! @brief Executes callable without locking in single-threaded mode. */
    template<class Func>
    static decltype(auto) _synchronized(Func&& func) {
        return std::forward<Func>(func)();
    }
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
///@}

    /*! @brief Thread-unsafe implementation of @ref beginMessageGroup. */
    MessageGroup _beginMessageGroupUnsafe();

    bool _takeGroupMessagesForDeliveryUnsafe(MessageGroup group, bool removeGroup, MessageList& out);

    /*! @brief Thread-unsafe implementation of single message delivery.
     *  @param message Message to deliver.
     * @note Takes ownership of `message`. */
    void _deliverMessageUnsafe(Message* message);

    /*! @brief Thread-unsafe implementation of message list delivery.
     *  @param messageList List of messages to log.
     * @note Takes ownership of all messages in `messageList`. */
    void _deliverMessageListUnsafe(const MessageList& messages);
};

}; // namespace Draupnir::Logging

#define logger Draupnir::Logging::Logger::get()

#endif // LOGGER_H
