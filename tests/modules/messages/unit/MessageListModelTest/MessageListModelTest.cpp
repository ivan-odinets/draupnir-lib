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
#include <QAbstractItemModelTester>
#include <QCoreApplication>

#include "draupnir/messages/core/MessageTypeIconProvider.h"
#include "draupnir/messages/core/MessageViewItem.h"
#include "draupnir/messages/models/MessageListModel.h"

using namespace Draupnir::Messages;

/*! @class MessageListModelTest tests/modules/messages/unit/MessageListModelTest.cpp
 *  @ingroup MessagesTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageListModel class. */

class MessageListModelTest : public QObject
{
    Q_OBJECT
private:
    QAbstractItemModelTester* modelTester = nullptr;
    MessageListModel* model = nullptr;

    MessagePtr debugOne = Message::create("Debug",MessageLevel::Debug);
    MessagePtr infoOne = Message::create("Info One",MessageLevel::Info);
    MessagePtr infoTwo = Message::create("Info Two",MessageLevel::Info);

private slots:
    void initTestCase() {
        MessageViewItem::registerIconProvider(new MessageTypeIconProvider);
    }

    void init() {
        model = new MessageListModel;
        modelTester = new QAbstractItemModelTester{model, QAbstractItemModelTester::FailureReportingMode::QtTest};
    }
    void cleanup() {
        delete model; model = nullptr;
        delete modelTester; modelTester = nullptr;
    }

    void test_append_count_and_clear() {
        QCOMPARE(model->rowCount(), 0);

        model->append(debugOne);
        QCOMPARE(model->rowCount(), 1);

        model->clear();
        QCOMPARE(model->rowCount(), 0);

        model->append({debugOne, infoOne, infoTwo});
        QCOMPARE(model->rowCount(), 3);
    }

    void test_data_method() {
        model->append({debugOne, infoOne, infoTwo});

        for (int i = 0; i < model->rowCount(); i++) {
            QModelIndex index = model->index(i,0);
            const auto& item = model->itemAt(i);

            // Qt::DisplayRole
            QString text = model->data(index,Qt::DisplayRole).toString();
            QVERIFY(text.contains(item.brief()));
            QVERIFY(text.contains(item.what()));
            QVERIFY(text.contains(item.dateTime().toString()));

            // Qt::DecorationRole
            QIcon icon = model->data(index, Qt::DecorationRole).value<QIcon>();
            QVERIFY(icon.cacheKey() == item.icon().cacheKey());

            // Qt::TooltipRole
            QString toolTip = model->data(index, Qt::ToolTipRole).toString();
            QVERIFY(toolTip.contains(item.what()));
        }
    }
};

QTEST_MAIN(MessageListModelTest)

#include "MessageListModelTest.moc"
