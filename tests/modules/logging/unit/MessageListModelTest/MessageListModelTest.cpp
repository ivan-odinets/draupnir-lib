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

#include "draupnir/logging/core/AbstractMessageViewIconProvider.h"
#include "draupnir/logging/messages/MessageViewItem.h"
#include "draupnir/logging/models/MessageListModel.h"

namespace Draupnir::Logging
{

/*! @class MessageListModelTest tests/modules/logging/unit/MessageListModelTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageListModel class.
 * @todo Add testing of signals emission. */

class MessageListModelTest : public QObject
{
    Q_OBJECT
private:
    AbstractMessageViewIconProvider iconProvider;
    MessageListModel* model = nullptr;


private slots:
    void initTestCase() { MessageViewItem::registerIconProvider(&iconProvider); }
    void init() { model = new MessageListModel; }
    void cleanup() { delete model; model = nullptr; }

    void test_count() {
        QCOMPARE(model->columnCount(),1);
        QCOMPARE(model->rowCount(), 0);

        Message* msg = Message::create("Debug",MessageLevel::Debug);
        model->append(msg);
        QCOMPARE(model->rowCount(), 1);
        QCOMPARE(model->columnCount(), 1);
        delete msg;
    }

    void test_index() {
        Message* msg = Message::create("Debug",MessageLevel::Debug);
        MessageViewItem* msgItem = new MessageViewItem{msg};
        model->append(msgItem);

        QModelIndex index = model->index(0,0);
        QCOMPARE(index.internalPointer(), static_cast<void*>(msgItem));
        delete msg;
    }

    void test_data_method() {
        Message* debugOne = Message::create("Debug",MessageLevel::Debug);
        Message* infoOne = Message::create("Info One",MessageLevel::Info);
        Message* infoTwo = Message::create("Info Two",MessageLevel::Info);
        model->append({debugOne, infoOne, infoTwo});

        for (int i = 0; i < model->rowCount(); i++) {
            QModelIndex index = model->index(i,0);
            MessageViewItem* messageViewItem = static_cast<MessageViewItem*>(index.internalPointer());

            // Qt::DisplayRole
            QString text = model->data(index,Qt::DisplayRole).toString();
            QVERIFY(text.contains(messageViewItem->brief()));
            QVERIFY(text.contains(messageViewItem->what()));
            QVERIFY(text.contains(messageViewItem->dateTime().toString()));

            // Qt::DecorationRole
            QIcon icon = model->data(index, Qt::DecorationRole).value<QIcon>();
            QVERIFY(icon.cacheKey() == messageViewItem->icon().cacheKey());

            // Qt::TooltipRole
            QString toolTip = model->data(index, Qt::ToolTipRole).toString();
            QVERIFY(toolTip.contains(messageViewItem->what()));
        }
        delete debugOne;
        delete infoOne;
        delete infoTwo;
    }
};

}; // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::MessageListModelTest)

#include "MessageListModelTest.moc"
