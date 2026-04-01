/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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
#include "draupnir/message_system/traits/messages/DefaultMessageTraits.h" // IWYU pragma: keep

namespace Draupnir::Messages
{

/*! @class MessageListModelTest tests/modules/message_system/unit/MessageListModelTest.cpp
 *  @ingroup MessageSystem
 *  @ingroup Tests
 *  @brief Unit test for @ref Draupnir::Messages::MessageListModel class.
 * @todo Add tests for signals emission. */

class MessageListModelTest : public QObject
{
    Q_OBJECT
private:
    MessageListModel* model = nullptr;

    static void verifyRowsSignal(const QSignalSpy& spy, int expectedFirst, int expectedLast) {
        QTRY_COMPARE(spy.count(), 1);

        const QList<QVariant> arguments = spy.at(0);
        QCOMPARE(arguments.size(), 3);

        const QModelIndex parent = qvariant_cast<QModelIndex>(arguments.at(0));
        QVERIFY(!parent.isValid());

        QCOMPARE(arguments.at(1).toInt(), expectedFirst);
        QCOMPARE(arguments.at(2).toInt(), expectedLast);
    }

private slots:
    void init() {
        model = new MessageListModel;
    }

    void cleanup() {
        delete model; model = nullptr;
    }

    void test_count() {
        QCOMPARE(model->columnCount(),1);
        QCOMPARE(model->rowCount(), 0);

        model->append(Message::fromTrait<DebugMessageTrait>("Debug"));
        QCOMPARE(model->rowCount(), 1);
        QCOMPARE(model->columnCount(), 1);
    }

    void test_index() {
        Message* debugOne = Message::fromTrait<DebugMessageTrait>("Debug");
        model->append(debugOne);

        QModelIndex index = model->index(0,0);
        QCOMPARE(index.internalPointer(), static_cast<void*>(debugOne));
    }

    void test_data_method() {
        Message* debugOne = Message::fromTrait<DebugMessageTrait>("Debug");
        Message* infoOne = Message::fromTrait<InfoMessageTrait>("Info One");
        Message* infoTwo = Message::fromTrait<InfoMessageTrait>("Info Two");
        model->append({debugOne, infoOne, infoTwo});

        for (int i = 0; i < model->rowCount(); i++) {
            QModelIndex index = model->index(i,0);
            Message* message = static_cast<Message*>(index.internalPointer());

            // Qt::DisplayRole
            QString text = model->data(index,Qt::DisplayRole).toString();
            QVERIFY(text.contains(message->brief()));
            QVERIFY(text.contains(message->what()));
            QVERIFY(text.contains(message->dateTime().toString()));

            // Qt::DecorationRole
            QIcon icon = model->data(index, Qt::DecorationRole).value<QIcon>();
            QVERIFY(icon.cacheKey() == message->icon().cacheKey());

            // Qt::TooltipRole
            QString toolTip = model->data(index, Qt::ToolTipRole).toString();
            QVERIFY(toolTip.contains(message->what()));
        }
    }
};

}; // namespace Draupnir::Messages

QTEST_MAIN(Draupnir::Messages::MessageListModelTest)

#include "MessageListModelTest.moc"
