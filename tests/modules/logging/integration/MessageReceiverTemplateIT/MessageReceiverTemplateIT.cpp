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

#include "draupnir/logging/core/MessageReceiverTemplate.h"
#include "draupnir/messages/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/messages/traits/categories/NetworkMessageCategoryTrait.h"

using namespace Draupnir::Logging;
using namespace Draupnir::Messages;

/*! @class MessageReceiverTemplateIT tests/modules/logging/integration/MessageReceiverTemplateIT.cpp
 *  @ingroup LoggingTests
 *  @brief This is an integration test for the @ref Draupnir::Logging::MessageReceiverTemplate class. */

class MessageReceiverTemplateIT final : public QObject
{
    Q_OBJECT
private:
    using MessageReceiver = MessageReceiverTemplate<
        DefaultMessageCategoryTrait, NetworkMessageCategoryTrait
    >;
    MessageReceiver* receiver = nullptr;

    MessagePtr dummyMessage = Message::create(
        "dummy", MessageLevel::Debug, DefaultMessageCategoryTrait::value()
    );
    MessageList dummyMessageList = MessageList{
        Message::create("default-debug", MessageLevel::Debug, DefaultMessageCategoryTrait::value()),
        Message::create("default-info", MessageLevel::Info, DefaultMessageCategoryTrait::value()),
        Message::create("network-debug", MessageLevel::Debug, NetworkMessageCategoryTrait::value()),
        Message::create("network-info", MessageLevel::Info, NetworkMessageCategoryTrait::value()),
    };

private slots:

    void init() { receiver = new MessageReceiver; }
    void cleanup() { delete receiver; receiver = nullptr; }

    void test_handle_message() {
        QCOMPARE(receiver->messages()->rowCount(), 0);
        receiver->handleMessage(dummyMessage);
        QCOMPARE(receiver->messages()->rowCount(), 1);
        QCOMPARE(receiver->messages()->messageAt(0), dummyMessage);
    }

    void test_handle_message_list() {
        QCOMPARE(receiver->messages()->rowCount(), 0);
        receiver->handleMessageList(dummyMessageList);
        QCOMPARE(receiver->messages()->rowCount(), dummyMessageList.count());
    }
};

QTEST_MAIN(MessageReceiverTemplateIT)

#include "MessageReceiverTemplateIT.moc"
