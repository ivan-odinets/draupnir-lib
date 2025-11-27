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

#include <QtTest>
#include <QCoreApplication>
#include <QtConcurrent>

#include "draupnir/Logger.h"
#include "draupnir/models/MessageListModel.h"

#include "draupnir-test/mocks/DummyMessageHandler.h"

/*! @class LoggerMultithreadTest tests/message_system/unit/LoggerMultithreadTest.cpp
 *  @brief This test class tests Logger class in a multi-thread mode.
 *
 * @todo Extend this test. */

class LoggerMultithreadTest final : public QObject
{
    Q_OBJECT
public:
    // DummyMessageHandler to be used as blocker to suppress possible output from ~Logger()
    DummyMessageHandler<> m_dummyHandler;

    // Helper method which will create threadCount threads each calling the callable callCount times
    void performSpamCalls(const int threadCount, const int callCount, const std::function<void()>& callable) {
        QList<QFuture<void>> futureList;

        // Start threads
        for (int i = 0; i < threadCount; i++) {
            auto future = QtConcurrent::run([&callable,callCount](){
                for (int j = 0; j < callCount; j++)
                    callable();
            });
            futureList.append(future);
        }

        // Wait for threads to be finished
        for (auto& future : futureList)
            future.waitForFinished();
    }

private slots:
    void test_begin_message_group() {
        const int threadCount = 50;
        const int callCount = 100;
        const int expectedGroupCount = threadCount * callCount;

        Logger dummyLogger;

        // Try to call Logger::beginMessageGroup method from many threads
        performSpamCalls(threadCount,callCount,[&dummyLogger](){
            dummyLogger.beginMessageGroup();
        });

        QTRY_COMPARE(dummyLogger.m_messageGroupsMap.count(), expectedGroupCount);

        // To suppress qDebug output from Logger destructor
        dummyLogger.setMessageHandler(&m_dummyHandler);
    }

    void test_is_group_existing() {
        const int threadCount = 50;
        const int callCount = 100;
        const int expectedGroupCount = threadCount * callCount;

        Logger dummyLogger;

        // Prepeare message groups in a single thread
        QList<Draupnir::Messages::MessageGroup> groupList;
        for (int i = 0; i < expectedGroupCount; i++)
            groupList.append(dummyLogger.beginMessageGroup());

        // Try to call Logger::beginMessageGroup method from many threads
        performSpamCalls(threadCount,callCount,[&dummyLogger,&groupList](){
            int index = rand() % expectedGroupCount;
            QVERIFY(dummyLogger.isGroupExisting(groupList.at(index)));
        });

        // Well, if we have not crashed - maybe the Logger is thread-safe

        // To suppress qDebug output from Logger destructor
        dummyLogger.setMessageHandler(&m_dummyHandler);
    }

    void test_end_message_group() {
        const int threadCount = 50;
        const int callCount = 100;
        const int groupCount = threadCount * callCount;

        Logger dummyLogger;

        // Prepeare lists of message groups in a single thread. One list of groups per one thread.
        // groupCount the total cmount of groups to be created
        QList<QList<Draupnir::Messages::MessageGroup>> listOfGroupLists;
        for (int i = 0; i < threadCount; i++) {
            QList<Draupnir::Messages::MessageGroup> groupList;
            for (int j = 0; j < callCount; j++)
                groupList.append(dummyLogger.beginMessageGroup());
            listOfGroupLists.append(groupList);
        }

        // To be sure that we have proper amount of groups
        QVERIFY(dummyLogger.m_messageGroupsMap.count() == groupCount);

        // Will start threadCount of threads, each having its own list of groups to be removed.
        QList<QFuture<void>> futuresList;
        for (int i = 0; i < threadCount; i++) {
            const auto groupList = listOfGroupLists.at(i);
            auto future = QtConcurrent::run([&dummyLogger,groupList](){
                for (int j = 0; j < callCount; j++)
                    dummyLogger.endMessageGroup(groupList.at(j));
            });
            futuresList.append(future);
        }

        // Wait for threads to be finished
        for (auto& future : futuresList)
            future.waitForFinished();

        // We should have cleaned all groups
        QCOMPARE(dummyLogger.m_messageGroupsMap.count(), 0);

        // To suppress qDebug output from Logger destructor
        dummyLogger.setMessageHandler(&m_dummyHandler);
    }

    void test_multithread_logging_without_handler() {
        const int threadCount = 10;
        const int callCount = 10000;
        const auto expectedMessages = threadCount * callCount;

        Logger dummyLogger;

        // Try to call one of logging methods from many threads
        performSpamCalls(threadCount,callCount,[&dummyLogger](){
            dummyLogger.logDebug(QString{"Blah"});
        });

        QTRY_COMPARE(dummyLogger.p_tempMessageStorage->count(), expectedMessages);

        // To suppress qDebug output from Logger destructor
        dummyLogger.setMessageHandler(&m_dummyHandler);
    }

    void test_multithread_logging_with_handler() {
        const int threadCount = 50;
        const int callCount = 100;
        const auto expectedMessages = threadCount * callCount;

        Logger dummyLogger;
        DummyMessageHandler dummyHandler;
        dummyLogger.setMessageHandler(&dummyHandler);

        // Try to call one of logging methods from many threads
        performSpamCalls(threadCount, callCount, [&dummyLogger](){
            dummyLogger.logDebug(QString{"Blah"});
        });

        // We NEED QTRY_COMPARE here instead of QCOMPARE because passing of Messages from Logger to the AbstractMessageHandler
        // is happening via signal/slot mechanism.
        QTRY_COMPARE(dummyHandler.messages()->rowCount(), expectedMessages);
    }

    void test_multithread_batch_logging_with_handler() {
        const int threadCount = 50;
        const int callCount = 100;
        const auto totalMessageCount = threadCount * callCount;

        Logger dummyLogger;
        DummyMessageHandler dummyHandler;
        dummyLogger.setMessageHandler(&dummyHandler);

        const auto groupOne = dummyLogger.beginMessageGroup();
        const auto groupTwo = dummyLogger.beginMessageGroup();

        performSpamCalls(threadCount,callCount,[&dummyLogger,groupOne,groupTwo](){
            if (rand() % 2) {
                dummyLogger.logDebug("debug",groupOne);
            } else {
                dummyLogger.logInfo("info",groupTwo);
                dummyLogger.flush(groupOne);
            }
        });

        // Process events. Threads, etc.
        QCoreApplication::processEvents();

        int messagesSomewhere =
                dummyLogger.m_messageGroupsMap[groupOne].count() +
                dummyLogger.m_messageGroupsMap[groupTwo].count() +
                dummyHandler.messages()->rowCount();
        QTRY_COMPARE(messagesSomewhere, totalMessageCount);
    }
};

QTEST_MAIN(LoggerMultithreadTest)

#include "LoggerMultithreadTest.moc"
