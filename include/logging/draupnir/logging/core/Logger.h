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

#include "draupnir/logging/core/MessageGroupId.h"
#include "draupnir/messages/core/Message.h"
#include "draupnir/messages/core/MessageLevels.h"
#include "draupnir/messages/categories/MessageCategories.h"

namespace Draupnir::Logging
{

class AbstractMessageReceiver;

/*! @class Logger draupnir/core/logging/Logger.h
 *  @ingroup Logging
 *  @brief Central entry point for creating and submitting log messages.
 *
 *  @details The @ref Logger class is used to create and submit @ref Draupnir::Messages::Message objects from different parts
 *           of the application.
 *
 *           Messages can be logged immediately or associated with a @ref Draupnir::Logging::MessageGroupId and flushed later.
 *
 *           If no @ref Draupnir::Logging::AbstractMessageReceiver is installed, logged messages are stored internally and
 *           forwarded once @ref setMessageHandler is called.
 *
 *           In the default multithreaded mode public logging methods are guarded internally and messages are delivered to
 *           the handler through Qt signals.
 * @todo Feature: Allow optional using of QPointer to monitor if the receiver was deleted before the logger. */

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
class Logger final : public QObject {
    Q_OBJECT
#else
class Logger
{
#endif // DRAUPNIR_LOGGING_SINGLETHREAD
public:
    Q_DISABLE_COPY_MOVE(Logger);

    /*! @brief Returns the global Logger instance.
     *  @return Reference to the global Logger singleton. */
    static Logger& get() {
        static Logger theOne;
        return theOne;
    }

    /*! @brief Destroys the logger. If no @ref Draupnir::Logging::AbstractMessageReceiver was installed, the logger deletes
     *         messages stored in its internal temporary storage. Messages already transferred to a message handler are not
     *         owned by the logger anymore. */
#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    ~Logger() final;
#else
    ~Logger();
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

    /*! @brief Sets the message receiver used to process logged messages.
     *  @param receiver Pointer to a valid message receiver.
     *  @details After the receiver is installed, all messages stored in the internal temporary storage are forwarded to it.
     * @note The logger does not take ownership of `receiver`. The caller must ensure that the receiver remains valid while
     *       it is  used by the logger.
     * @todo Feature: Allow choosing behaviour in the release build: UB vs some fallback. (E.g. with DRAUPNIR_ENABLE_SAFE_RELEASE
     *       macro. */
    void setMessageReceiver(AbstractMessageReceiver* receiver);

///@name This group of methods allows manipulating with message groups.
///@{
    /*! @brief Starts a new message group.
     *  @return New @ref Draupnir::Logging::MessageGroupId identifier. */
    [[nodiscard]] MessageGroupId beginMessageGroup();

    /*! @brief Checks whether the specified message group exists.
     *  @param group Message group to check.
     *  @return `true` if the group exists; otherwise `false`. */
    [[nodiscard]] bool isGroupExisting(MessageGroupId group) const;

    /*! @brief Flushes messages stored in the given group. Messages stored in the group are submitted for normal processing.
     *         The group itself remains valid and can be used to collect more messages later.
     *  @param group Message group to flush.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void flush(MessageGroupId group);

    /*! @brief Ends a message group. Messages stored in the group are submitted for normal processing. After this call the
     *         group is removed and must not be reused.
     *  @param group Message group to end.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void endMessageGroup(MessageGroupId group);
///@}

///@name This group of methods allows logging custom @ref Draupnir::Message::Message objects.
///@{
    /*! @brief Logs a preconstructed @ref Draupnir::Messages::Message object.
     *  @param message Message object to log.
     * @note The pointer must not be null.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void logMessage(Draupnir::Messages::MessagePtr message);

    /*! @brief Logs a preconstructed message into a message group.
     *  @param message Message object to log.
     *  @param group Message group identifier.
     * @note The pointer must not be null.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void logMessage(Draupnir::Messages::MessagePtr message, MessageGroupId group);


    /*! @brief Logs a @ref Draupnir::Messages::MessageList object.
     *  @param messageList List of @ref Draupnir::Messages::MessagePtr objects to log.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void logMessageList(const Draupnir::Messages::MessageList& messageList);
///@}

///@name This group of methods allows logging the default levels of messages.
///@{
    /*! @brief Logs a message with the specified level and category.
     *  @param what Message text.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(
        const QString& what,
        Draupnir::Messages::MessageLevel::Value messageLevel,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(
            Draupnir::Messages::Message::create(
                Messages::MessageLevels::toDisplayName(messageLevel),
                what,
                messageLevel,
                messageCategory
            )
        );
    }

    /*! @brief Logs a message with the specified level and category into a group.
     *  @param what Message text.
     *  @param group Message group identifier.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void logMessage(
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageLevel::Value messageLevel,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(
            Draupnir::Messages::Message::create(
                Messages::MessageLevels::toDisplayName(messageLevel),
                what,
                messageLevel,
                messageCategory
            ), group);
    }

    /*! @brief Logs a message with brief and full text.
     *  @param brief Short message summary.
     *  @param what Full message text.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(const QString& brief,
        const QString& what,
        Draupnir::Messages::MessageLevel::Value messageLevel,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(Draupnir::Messages::Message::create(brief, what, messageLevel, messageCategory));
    }

    /*! @brief Logs a message with brief and full text into a group.
     *  @param brief Short message summary.
     *  @param what Full message text.
     *  @param group Message group identifier.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category. */
    void logMessage(
        const QString& brief,
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageLevel::Value messageLevel,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(Draupnir::Messages::Message::create(brief, what, messageLevel, messageCategory), group);
    }

    /*! @brief Logs a debug message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logDebug(
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, Draupnir::Messages::MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs a debug message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logDebug(
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, group, Draupnir::Messages::MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs a debug message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logDebug(
        const QString& brief,
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, Draupnir::Messages::MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs debug message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logDebug(
        const QString& brief,
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default) {
        logMessage(brief, what, group, Draupnir::Messages::MessageLevel::Debug, messageCategory);
    }

    /*! @brief Logs an info message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logInfo(
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, Draupnir::Messages::MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs an info message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logInfo(
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, group, Draupnir::Messages::MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs an info message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logInfo(
        const QString& brief,
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, Draupnir::Messages::MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs an info message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logInfo(
        const QString& brief,
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, group, Draupnir::Messages::MessageLevel::Info, messageCategory);
    }

    /*! @brief Logs a warning message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logWarning(
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, Draupnir::Messages::MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs a warning message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logWarning(
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, group, Draupnir::Messages::MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs a warning message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logWarning(
        const QString& brief,
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, Draupnir::Messages::MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs a warning message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logWarning(
        const QString& brief,
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, group, Draupnir::Messages::MessageLevel::Warning, messageCategory);
    }

    /*! @brief Logs an error message immediately.
     *  @param what Message content.
     *  @param messageCategory Message category. */
    void logError(
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, Draupnir::Messages::MessageLevel::Error, messageCategory);
    }

    /*! @brief Logs an error message into a message group.
     *  @param what Message content.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logError(
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(what, group, Draupnir::Messages::MessageLevel::Error, messageCategory);
    }

    /*! @brief Logs an error message with brief and details.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param messageCategory Message category. */
    void logError(
        const QString& brief,
        const QString& what,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, Draupnir::Messages::MessageLevel::Error, messageCategory);
    }

    /*! @brief Logs an error message with brief and details into a group.
     *  @param brief Short summary.
     *  @param what Full message.
     *  @param group MessageGroup identifier.
     *  @param messageCategory Message category. */
    void logError(
        const QString& brief,
        const QString& what,
        MessageGroupId group,
        Draupnir::Messages::MessageCategory messageCategory = Draupnir::Messages::MessageCategory::Default
    ) {
        logMessage(brief, what, group, Draupnir::Messages::MessageLevel::Error, messageCategory);
    }

///@}

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
signals:
    /*! @brief Emitted when a single message is logged and ready to be processed.
     *  @param message Logged message.
     * @note Passing messages through this signal is used in the multithreaded mode, where logging methods may be called from
     *       different threads while final message processing is performed in the receiver thread. */
    void messageReceived(Draupnir::Messages::MessagePtr message);

    /*! @brief Emitted when a list of messages is ready to be processed.
     *  @param messageList List of logged messages.
     * @note Passing messages through this signal is used in the multithreaded mode, where logging methods may be called from
     *       different threads while final message processing is performed in the receiver thread. */
    void messageListReceived(Draupnir::Messages::MessageList messageList);

#endif // DRAUPNIR_LOGGING_SINGLETHREAD

private:
    friend class LoggerTest;
    friend class LoggerMultithreadTest;

#ifndef DRAUPNIR_LOGGING_SINGLETHREAD
    /*! @brief Mutex guarding access to logger internal state. */
    mutable QMutex m_resourceMutex;
#endif // DRAUPNIR_LOGGING_SINGLETHREAD

    /*! @brief Temporary storage for messages logged before a handler is installed. Once a message handler is installed,
     *         messages from this storage are forwarded to the handler. */
    Draupnir::Messages::MessageList m_tempMessageStorage;

    /*! @brief Stores messages associated with active message groups. */
    QMap<MessageGroupId, Draupnir::Messages::MessageList> m_messageGroupsMap;

    /*! @brief Message handler used to process submitted messages.
     * @note This pointer is non-owning. */
    AbstractMessageReceiver* p_messageReceiver;

    /*! @brief Private constructor. */
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
    MessageGroupId _beginMessageGroupUnsafe();

    /*! @brief Extracts messages from a message group for delivery.
     *  @param group Group whose messages should be extracted.
     *  @param removeGroup Whether the group should be removed after extraction.
     *  @param out Destination list receiving messages when a receiver is installed.
     *  @return `true` if `out` contains messages that should be delivered immediately; otherwise, `false`.
     *  @details If no receiver is installed, group messages are moved to temporary storage instead of `out`.
     * @note This method does not perform synchronization. */
    bool _takeGroupMessagesForDeliveryUnsafe(
        MessageGroupId group,
        bool removeGroup,
        Draupnir::Messages::MessageList& out
    );

    /*! @brief Thread-unsafe implementation of single message delivery.
     *  @param message Message to deliver. */
    void _deliverMessageUnsafe(Draupnir::Messages::MessagePtr message);

    /*! @brief Thread-unsafe implementation of message list delivery.
     *  @param messageList List of messages to log. */
    void _deliverMessageListUnsafe(const Draupnir::Messages::MessageList& messageList);
};

} // namespace Draupnir::Logging

#define logger Draupnir::Logging::Logger::get()

#endif // LOGGER_H
