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
#include "draupnir/message_system/ui/widgets/MessageListView.h"

namespace Draupnir::Messages
{

/*! @class MessageListViewTest tests/modules/message_system/unit/MessageListViewTest.cpp
 *  @brief This test class tests MessageListView class. */

class MessageListViewTest final : public QObject
{
    Q_OBJECT
public:
    MessageListView* widget = nullptr;

private slots:
    void initTestCase() { qRegisterMetaType<Draupnir::Messages::Message::Fields>(); }

    void init() {
        widget = new MessageListView;
    }

    void cleanup() {
        delete widget;
    }

    void test_initialization() {
        QVERIFY(widget->model() != nullptr);
        QVERIFY(qobject_cast<QSortFilterProxyModel*>(widget->model()) != nullptr);

        MessageListModel* dummyModel = new MessageListModel{widget};
        widget->setModel(dummyModel);

        MessageListProxyModel* randomProxyModel = new MessageListProxyModel{widget};

        QCOMPARE(widget->displayedMessageFieldsMask(), randomProxyModel->displayedMessageFieldsMask());
        QCOMPARE(widget->displayedMessageTypesMask(), randomProxyModel->displayedMessageTypesMask());
    }

    void test_setting_displayed_message_types() {
        MessageListModel* dummyModel = new MessageListModel{widget};
        widget->setModel(dummyModel);
        widget->setDisplayedMessageTypesMask(MessageType::AllMessages);
        QVERIFY(widget->displayedMessageTypesMask() == MessageType{MessageType::AllMessages});

        // Signal spy
        QSignalSpy messageTypeVisibilityChangedSpy{ widget, &MessageListView::messageTypeVisibilityChanged };
        // Try setting masks
        widget->setDisplayedMessageTypesMask(MessageType::Debug);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageTypesMask(), MessageType{MessageType::Debug});
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Debug), true);
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Info), false);
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Warning), false);
        // Try setting more masks
        widget->setDisplayedMessageTypesMask(MessageType::Info|MessageType::Warning);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageTypesMask(), MessageType{MessageType::Info|MessageType::Warning});
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Debug), false);
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Info), true);
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Warning), true);

        // Trigger method emitting signal
        widget->setMessageTypeDisplayed(MessageType::Info, false);
        // Check if visibility changed
        QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Info), false);
        // Wait for signal to be emitted
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(),1);
        // Check if corretct things were emitted
        auto signalArgs = messageTypeVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(),2);
        QCOMPARE(signalArgs.at(0).value<MessageType>(), MessageType{MessageType::Info});
        QCOMPARE(signalArgs.at(1).toBool(), false);
    }

    void test_setting_displayed_message_fields() {
        MessageListModel* dummyModel = new MessageListModel{widget};
        widget->setModel(dummyModel);
        widget->setDisplayedMessageFieldsMask(MessageFields{MessageField::All});
        QVERIFY(widget->displayedMessageFieldsMask() == MessageField::All);

        // Signal spy
        QSignalSpy messageFieldVisibilityChangedSpy{ widget, &MessageListView::messageFieldVisibilityChanged };
        // Try setting masks
        widget->setDisplayedMessageFieldsMask(MessageFields{MessageField::Brief});
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageFieldsMask(), MessageField::Brief);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Brief), true);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::What), false);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Icon), false);

        // Try setting more masks
        widget->setDisplayedMessageFieldsMask(MessageFields{MessageField::What|MessageField::Icon});
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageFieldsMask(), MessageField::What|MessageField::Icon);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Brief), false);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::What), true);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Icon), true);

        widget->setMessageFieldDisplayed(MessageField::Brief, true);
        // Check if visibility changed
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Brief), true);
        // Wait for signal to be emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(),1);
        // Check if corretct things were emitted
        auto signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(),2);
        QCOMPARE(signalArgs.at(0).value<Message::Field>(), MessageField::Brief);
        QCOMPARE(signalArgs.at(1).toBool(), true);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageListViewTest)

#include "MessageListViewTest.moc"
