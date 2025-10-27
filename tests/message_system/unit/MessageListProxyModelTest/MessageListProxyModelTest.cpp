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

#include "draupnir/core/Message.h"
#include "draupnir/models/MessageListModel.h"
#include "draupnir/models/MessageListProxyModel.h"
#include "draupnir/traits/messages/DefaultMessageTraits.h"

namespace Draupnir::Messages
{

/*! @class MessageListProxyModelTest tests/message_system/unit/MessageListProxyModelTest.cpp
 *  @brief This test class tests MessageListProxyModel class.
 *
 * @todo Add test cases for testing display options for message objects (brief, text, icon, date).
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

class MessageListProxyModelTest : public QObject
{
    Q_OBJECT
public:
    MessageListProxyModelTest() :
        sourceModel{new MessageListModel}
    {}
    ~MessageListProxyModelTest() {
        sourceModel->deleteLater();
    };

    MessageListModel* sourceModel;
    Message* debugOne;
    Message* infoOne;
    Message* infoTwo;

private slots:
    void initTestCase() {
        // Populate test model with some random stuff
        debugOne = Message::fromTrait<DebugMessageTrait>("Debug");
        infoOne = Message::fromTrait<InfoMessageTrait>("Info One");
        infoTwo = Message::fromTrait<InfoMessageTrait>("Info Two");

        sourceModel->append(
            { debugOne, infoOne, infoTwo }
        );

    }

    void test_initialization() {
        MessageListProxyModel* testedProxy = new MessageListProxyModel;
        testedProxy->setSourceModel(sourceModel);

        QCOMPARE(testedProxy->messageTypeFilter().id(), MessageType::AllMessages);

        delete testedProxy;
    }

    void test_setMessageTypeFilter() {
        MessageListProxyModel* testedProxy = new MessageListProxyModel;
        Message* message = nullptr;
        testedProxy->setSourceModel(sourceModel);

        // Show all messages
        testedProxy->setMessageTypeFilter(MessageType::AllMessages);
        QCOMPARE(testedProxy->messageTypeFilter().id(), MessageType::AllMessages);
        QCOMPARE(testedProxy->columnCount(), sourceModel->columnCount());
        QCOMPARE(testedProxy->rowCount(), sourceModel->rowCount());

        // Show only debug messages
        testedProxy->setMessageTypeFilter(MessageType::Debug);
        QCOMPARE(testedProxy->messageTypeFilter().id(), MessageType::Debug);
        QCOMPARE(testedProxy->rowCount(), 1);
        message = static_cast<Message*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QCOMPARE(message, debugOne);

        // Show only info messages
        testedProxy->setMessageTypeFilter(MessageType::Info);
        QCOMPARE(testedProxy->messageTypeFilter().id(), MessageType::Info);
        QCOMPARE(testedProxy->rowCount(), 2);
        message = static_cast<Message*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QVERIFY(message == infoOne || message == infoTwo);

        // Show Warning messages (which are not present)
        testedProxy->setMessageTypeFilter(MessageType::Warning);
        QCOMPARE(testedProxy->messageTypeFilter().id(), MessageType::Warning);
        QCOMPARE(testedProxy->rowCount(), 0);

        delete testedProxy;
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageListProxyModelTest)

#include "MessageListProxyModelTest.moc"
