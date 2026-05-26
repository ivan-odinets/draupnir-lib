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

#include <QtTest>

#include "draupnir/logging/Logger.h"

#include "draupnir-test/mocks/MessageHandlerMock.h"

namespace Draupnir::Logging {

/*! @class LoggerTest tests/modules/message_system/unit/LoggerTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::Logger class running in a single-thread mode. */

class LoggerTest final : public QObject
{
    Q_OBJECT
private:
    Logger* dummyLogger = nullptr;
    MessageHandlerMock dummyHandler;

private slots:
    void init() { dummyLogger = new Logger; }
    void cleanup() { delete dummyLogger; dummyLogger = nullptr; dummyHandler.clear(); }

    void test_initilization() {
        QVERIFY(dummyLogger->p_messageHandler == nullptr);
        QVERIFY(dummyLogger->p_tempMessageStorage != nullptr);
        QVERIFY(dummyLogger->m_messageGroupsMap.isEmpty());

        // Singltone version needs to be checked as well
        QVERIFY(Logger::get().p_messageHandler == nullptr);
        QVERIFY(Logger::get().p_tempMessageStorage != nullptr);
        QVERIFY(Logger::get().m_messageGroupsMap.isEmpty());

        // To suppress qDebug output from Logger destructor
        Logger::get().setMessageHandler(&dummyHandler);
        dummyLogger->setMessageHandler(&dummyHandler);
    }

    void test_handler_setup() {
        auto group = dummyLogger->beginMessageGroup();
        dummyLogger->logDebug("text");
        dummyLogger->logDebug("text", group);

        dummyLogger->setMessageHandler(&dummyHandler);

        // After we set the message handler the following should happen:
        // - p_tempMessage storage is transfered to handler and container is deleted
        // - any groupped messages - stay in their containers untill the group is flushed / ended
        QVERIFY(dummyLogger->p_tempMessageStorage == nullptr);
        QVERIFY(dummyLogger->isGroupExisting(group));
        // As passing Message object from Logger to handler is done via signal / slot mechanism use QTRY_COMPARE here
        QTRY_COMPARE(dummyHandler.messagesReceived.count(), 1);
        // Here QCOMPARE would be ok
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 1);
    }

    void test_group_logging_without_handler() {
        // Create group
        auto group = dummyLogger->beginMessageGroup();
        QVERIFY(dummyLogger->isGroupExisting(group));
        QVERIFY(dummyLogger->m_messageGroupsMap[group].isEmpty());

        // This message should go to p_tempMessageStorage
        dummyLogger->logDebug("text");
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(), 1);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 0);

        // This message should go to p_tempMessageStorage
        dummyLogger->logDebug("brief", "what");
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(), 2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 0);

        // This should go to group
        dummyLogger->logDebug("group text", group);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(), 2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 1);

        // This should go to group as well
        dummyLogger->logDebug("group brief", "group what", group);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(), 2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 2);

        // Flush group. After flushing the group should be kept, while all Message from it - redirected to p_tempMessageStorage
        dummyLogger->flush(group);
        QVERIFY(dummyLogger->isGroupExisting(group));
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 0);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(), 4);

        // Log something to group and end the group
        dummyLogger->logDebug("text", group);
        dummyLogger->endMessageGroup(group);
        QVERIFY(dummyLogger->isGroupExisting(group) == false);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(), 5);

        // To suppress qDebug output from Logger destructor
        dummyLogger->setMessageHandler(&dummyHandler);
    }

    void test_group_logging_with_handler() {
        dummyLogger->setMessageHandler(&dummyHandler);

        // Create group
        auto group = dummyLogger->beginMessageGroup();
        QVERIFY(dummyLogger->isGroupExisting(group));
        QVERIFY(dummyLogger->m_messageGroupsMap[group].isEmpty());

        // This message should go directly to the handler
        dummyLogger->logDebug("text");
        QCOMPARE(dummyHandler.messagesReceived.count(), 1);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 0);

        // This as well
        dummyLogger->logDebug("brief", "what");
        QCOMPARE(dummyHandler.messagesReceived.count(), 2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 0);

        // This should go to group
        dummyLogger->logDebug("group text", group);
        QCOMPARE(dummyHandler.messagesReceived.count(), 2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 1);

        // This as well
        dummyLogger->logDebug("group brief", "group what", group);
        QCOMPARE(dummyHandler.messagesReceived.count(), 2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 2);

        // Flush group.
        dummyLogger->flush(group);
        QVERIFY(dummyLogger->isGroupExisting(group));
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(), 0);
        QTRY_COMPARE(dummyHandler.messagesReceived.count(), 4);

        // Log something to group and end the group
        dummyLogger->logDebug("text", group);
        dummyLogger->endMessageGroup(group);
        QVERIFY(dummyLogger->isGroupExisting(group) == false);
        QTRY_COMPARE(dummyHandler.messagesReceived.count(), 5);
    }

    void test_log_debug_method_group() {
        auto group = dummyLogger->beginMessageGroup();
        auto emptyGroup = dummyLogger->beginMessageGroup();
        const QString messageText = "text";
        const QString messageBrief = "brief";
        const auto expectedType = Draupnir::Logging::MessageType{
            Draupnir::Logging::MessageLevel::Debug,
            Draupnir::Logging::MessageCategory::Default
        };
        Draupnir::Logging::Message* messagePtr = nullptr;

        // Check if logDebug(const QString& text) is producing propper debug messages
        dummyLogger->logDebug(messageText);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),1);
        QVERIFY(dummyLogger->m_messageGroupsMap[group].isEmpty());
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->p_tempMessageStorage->last();
        // Check if Message if what we expect
        // QCOMPARE(messagePtr->brief(), Draupnir::Messages::DebugMessageTrait::displayName());
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // Check if logDebug(const QString& brief, const QString& what) is producing propper debug messages
        dummyLogger->logDebug(messageBrief,messageText);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),2);
        QVERIFY(dummyLogger->m_messageGroupsMap[group].isEmpty());
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->p_tempMessageStorage->last();
        // Check if Message if what we expect
        QCOMPARE(messagePtr->brief(), messageBrief);
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // Check if logDebug(const QString& text, MessageGroup group) is producing propper debug messages
        dummyLogger->logDebug(messageText, group);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(),1);
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->m_messageGroupsMap[group].last();
        // Check if Message if what we expect
        // QCOMPARE(messagePtr->brief(), Draupnir::Messages::DebugMessageTrait::displayName());
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // Check if logDebug(const QString& brief, const QString& what, MessageGroup group) is producing propper debug
        // messages
        dummyLogger->logDebug(messageBrief,messageText,group);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(),2);
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->m_messageGroupsMap[group].last();
        // Check if Message if what we expect
        QCOMPARE(messagePtr->brief(), messageBrief);
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // To suppress qDebug output from Logger destructor
        dummyLogger->setMessageHandler(&dummyHandler);
    }

    void test_log_info_method_group() {
        auto group = dummyLogger->beginMessageGroup();
        auto emptyGroup = dummyLogger->beginMessageGroup();
        const QString messageText = "text";
        const QString messageBrief = "brief";
        const auto expectedType = Draupnir::Logging::MessageType{
            Draupnir::Logging::MessageLevel::Info,
            Draupnir::Logging::MessageCategory::Default
        };
        Draupnir::Logging::Message* messagePtr = nullptr;

        // Check if logInfo(const QString& text) is producing propper info messages
        dummyLogger->logInfo(messageText);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),1);
        QVERIFY(dummyLogger->m_messageGroupsMap[group].isEmpty());
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->p_tempMessageStorage->last();
        // Check if Message if what we expect
        // QCOMPARE(messagePtr->brief(), Draupnir::Messages::InfoMessageTrait::displayName());
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // Check if logInfo(const QString& brief, const QString& what) is producing propper info messages
        dummyLogger->logInfo(messageBrief,messageText);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),2);
        QVERIFY(dummyLogger->m_messageGroupsMap[group].isEmpty());
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->p_tempMessageStorage->last();
        // Check if Message if what we expect
        QCOMPARE(messagePtr->brief(), messageBrief);
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // Check if logInfo(const QString& text, MessageGroup group) is producing propper info messages
        dummyLogger->logInfo(messageText,group);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(),1);
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->m_messageGroupsMap[group].last();
        // Check if Message if what we expect
        // QCOMPARE(messagePtr->brief(), Draupnir::Messages::InfoMessageTrait::displayName());
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // Check if logInfo(const QString& brief, const QString& what, MessageGroup group) is producing propper info
        // messages
        dummyLogger->logInfo(messageBrief,messageText,group);
        QCOMPARE(dummyLogger->p_tempMessageStorage->count(),2);
        QCOMPARE(dummyLogger->m_messageGroupsMap[group].count(),2);
        QVERIFY(dummyLogger->m_messageGroupsMap[emptyGroup].isEmpty());
        messagePtr = dummyLogger->m_messageGroupsMap[group].last();
        // Check if Message if what we expect
        QCOMPARE(messagePtr->brief(), messageBrief);
        QCOMPARE(messagePtr->what(), messageText);
        QCOMPARE(messagePtr->type(), expectedType);

        // To suppress qDebug output from Logger destructor
        dummyLogger->setMessageHandler(&dummyHandler);
        dummyHandler.clear();
    }
};

} // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::LoggerTest)

#include "LoggerTest.moc"
