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
#include <QCoreApplication>

#include "draupnir/message_system/core/Message.h"
#include "draupnir/message_system/models/MessageListModel.h"
#include "draupnir/message_system/models/MessageListProxyModel.h"
#include "draupnir/message_system/traits/messages/DebugMessageTrait.h"
#include "draupnir/message_system/traits/messages/InfoMessageTrait.h"

namespace Draupnir::Messages
{

/*! @class MessageListProxyModelTest tests/modules/message_system/unit/MessageListProxyModelTest.cpp
 *  @ingroup MessageSystem
 *  @ingroup Tests
 *  @brief Unit test for @ref Draupnir::Messages::MessageListProxyModel class. */

class MessageListProxyModelTest : public QObject
{
    Q_OBJECT
private:
    MessageListModel* sourceModel = nullptr;
    Message* debugOne = nullptr;
    Message* infoOne = nullptr;
    Message* infoTwo = nullptr;

private slots:
    void initTestCase() {
        sourceModel = new MessageListModel{this};
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

        QCOMPARE(testedProxy->displayedMessageTypesMask().id(), MessageType::AllMessages);
        QCOMPARE(testedProxy->displayedMessageFieldsMask(), MessageField::All);

        delete testedProxy;
    }

    void test_setting_message_types() {
        MessageListProxyModel* testedProxy = new MessageListProxyModel;
        Message* message = nullptr;
        testedProxy->setSourceModel(sourceModel);

        // Show all messages
        testedProxy->setDisplayedMessageTypesMask(MessageType::AllMessages);
        QCOMPARE(testedProxy->displayedMessageTypesMask().id(), MessageType::AllMessages);
        QCOMPARE(testedProxy->columnCount(), sourceModel->columnCount());
        QCOMPARE(testedProxy->rowCount(), sourceModel->rowCount());

        // Show only debug messages
        testedProxy->setDisplayedMessageTypesMask(MessageType::Debug);
        QCOMPARE(testedProxy->displayedMessageTypesMask().id(), MessageType::Debug);
        QCOMPARE(testedProxy->rowCount(), 1);
        message = static_cast<Message*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QCOMPARE(message, debugOne);

        // Show only info messages
        testedProxy->setDisplayedMessageTypesMask(MessageType::Info);
        QCOMPARE(testedProxy->displayedMessageTypesMask().id(), MessageType::Info);
        QCOMPARE(testedProxy->rowCount(), 2);
        message = static_cast<Message*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QVERIFY(message == infoOne || message == infoTwo);

        // Show Warning messages (which are not present)
        testedProxy->setDisplayedMessageTypesMask(MessageType::Warning);
        QCOMPARE(testedProxy->displayedMessageTypesMask().id(), MessageType::Warning);
        QCOMPARE(testedProxy->rowCount(), 0);

        delete testedProxy;
    }

    void test_settings_message_fields() {
        MessageListProxyModel* testedProxy = new MessageListProxyModel;
        Message* message = nullptr;
        testedProxy->setSourceModel(sourceModel);

        // Hide all Message Fields
        testedProxy->setDisplayedMessageFieldsMask(MessageFields{MessageField::None});
        QCOMPARE(testedProxy->displayedMessageFieldsMask(), MessageField::None);
        QCOMPARE(testedProxy->columnCount(), sourceModel->columnCount());
        QCOMPARE(testedProxy->rowCount(), sourceModel->rowCount());
        QModelIndex proxyIndex = testedProxy->index(0,0);
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), QString{});
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), QIcon{});

        // Show only icons
        testedProxy->setDisplayedMessageFieldsMask(MessageFields{MessageField::Icon});
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Icon), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Brief), false);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::What), false);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::DateTime), false);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<Message*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), QString{});

        // Show icons and brief
        testedProxy->setMessageFieldDisplayed(MessageField::Brief, true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Icon), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Brief), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::What), false);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::DateTime), false);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<Message*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), message->brief());

        // Show icons, brief and what
        testedProxy->setMessageFieldDisplayed(MessageField::What, true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Icon), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Brief), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::What), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::DateTime), false);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<Message*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), message->brief() + "\n" + message->what());

        // Show icons, brief, what and datetime
        testedProxy->setMessageFieldDisplayed(MessageField::DateTime, true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Icon), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::Brief), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::What), true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::DateTime), true);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<Message*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), message->brief() + "\n" + message->what() + "\n" + message->dateTime().toString());

        delete testedProxy;
    }

    void test_setting_message_types_extended() {
        MessageListProxyModel* testedProxy = new MessageListProxyModel;
        testedProxy->setSourceModel(sourceModel);

        testedProxy->setDisplayedMessageTypesMask(MessageType::AllMessages);
        // Test multiple disabling calls
        testedProxy->setMessageTypeDisplayed(MessageType::Debug, false);
        testedProxy->setMessageTypeDisplayed(MessageType::Debug, false);
        QCOMPARE(testedProxy->isMessageTypeDisplayed(MessageType::Debug), false);

        // Test multiple enabling calls
        testedProxy->setMessageTypeDisplayed(MessageType::Debug, true);
        testedProxy->setMessageTypeDisplayed(MessageType::Debug, true);
        QCOMPARE(testedProxy->isMessageTypeDisplayed(MessageType::Debug), true);

        delete testedProxy;
    }

    void test_setting_message_fields_extended() {
        MessageListProxyModel* testedProxy = new MessageListProxyModel;
        testedProxy->setSourceModel(sourceModel);

        testedProxy->setDisplayedMessageFieldsMask(MessageFields{MessageField::All});
        // Test multiple disabling calls
        testedProxy->setMessageFieldDisplayed(MessageField::DateTime, false);
        testedProxy->setMessageFieldDisplayed(MessageField::DateTime, false);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::DateTime), false);

        // Test multiple enabling calls
        testedProxy->setMessageFieldDisplayed(MessageField::DateTime, true);
        testedProxy->setMessageFieldDisplayed(MessageField::DateTime, true);
        QCOMPARE(testedProxy->isMessageFieldDisplayed(MessageField::DateTime), true);

        delete testedProxy;
    }
};

}; // namespace Draupnir::Messages

QTEST_MAIN(Draupnir::Messages::MessageListProxyModelTest)

#include "MessageListProxyModelTest.moc"
