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
#include <QVBoxLayout>

#include "draupnir/logging/core/AbstractMessageViewIconProvider.h"
#include "draupnir/logging/messages/Message.h"
#include "draupnir/logging/messages/MessageViewItem.h"
#include "draupnir/logging/models/MessageListModel.h"
#include "draupnir/logging/models/MessageListProxyModel.h"
#include "draupnir/logging/ui/widgets/MessageListView.h"
#include "draupnir/logging/ui/widgets/MessageDisplayWidget.h"

#include "draupnir-test/helpers/UiTestHelper.h"

namespace Draupnir::Logging
{

/*! @class MessageListViewTest tests/modules/logging/unit/MessageListViewTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageListView class. */

class MessageListViewTest final : public QObject
{
    Q_OBJECT
private:
    MessageListView* widget = nullptr;
    MessageListModel* dummyModel = nullptr;

    Message* debugOne = nullptr;
    Message* infoOne = nullptr;
    Message* infoTwo = nullptr;
    Message* errorOne = nullptr;

private slots:
    void initTestCase() {
        MessageViewItem::registerIconProvider(new AbstractMessageViewIconProvider);
        dummyModel = new MessageListModel;

        debugOne = Message::create("Debug", MessageLevel::Debug);
        infoOne = Message::create("Info One", MessageLevel::Info);
        infoTwo = Message::create("Info TWo", MessageLevel::Info);
        errorOne = Message::create("ERROR!", MessageLevel::Error);

        dummyModel->append({
            debugOne, infoOne, infoTwo, errorOne
        });
    }

    void init() {
        widget = new MessageListView;
        widget->setModel(dummyModel);
    }
    void cleanup() {
        delete widget; widget = nullptr;
    }

    void test_initialization() {
        QVERIFY(widget->model() != nullptr);
        QVERIFY(qobject_cast<QSortFilterProxyModel*>(widget->model()) != nullptr);

        QCOMPARE(widget->displayedMessageViewItemFieldsMask(), MessageListProxyModel::DefaultDisplayedMessageItemFields);
        // QCOMPARE(widget->displayedMessageTypesMask(), randomProxyModel.displayedMessageTypesMask());
    }

    void test_setting_displayed_message_fields() {
        widget->setDisplayedMessageViewItemFieldsMask(MessageViewItemFields::All);
        QVERIFY(widget->displayedMessageViewItemFieldsMask() == MessageViewItemFields::All);

        // Signal spy
        QSignalSpy messageViewItemFieldVisibilityChangedSpy{ widget, &MessageListView::messageViewItemFieldVisibilityChanged};
        // Try setting masks
        widget->setDisplayedMessageViewItemFieldsMask(MessageViewItemField::Brief);
        QTRY_COMPARE(messageViewItemFieldVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageViewItemFieldsMask(), MessageViewItemField::Brief);
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), true);
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::What), false);
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::Icon), false);

        // Try setting more masks
        widget->setDisplayedMessageViewItemFieldsMask(MessageViewItemField::What|MessageViewItemField::Icon);
        QTRY_COMPARE(messageViewItemFieldVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageViewItemFieldsMask(), MessageViewItemField::What | MessageViewItemField::Icon);
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), false);
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::What), true);
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::Icon), true);

        widget->setMessageViewItemFieldDisplayed(MessageViewItemField::Brief, true);
        // Check if visibility changed
        QCOMPARE(widget->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), true);
        // Wait for signal to be emitted
        QTRY_COMPARE(messageViewItemFieldVisibilityChangedSpy.count(),1);
        // Check if corretct things were emitted
        auto signalArgs = messageViewItemFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(),2);
        QCOMPARE(signalArgs.at(0).value<MessageViewItemField::Value>(), MessageViewItemField::Brief);
        QCOMPARE(signalArgs.at(1).toBool(), true);
    }

    void test_setting_displayed_message_categoriesMask() {
        // widget->setDisplayedMessageCategoriesMask(MessageCategories::All);
        // QVERIFY(widget->displayedMessageCategoriesMask() == MessageCategories::All);

        // // Signal spy
        // QSignalSpy messageCategoriesVisibilityChangedSpy{ widget, &MessageListView:: };
        //     // Try setting masks
        //     widget->setDisplayedMessageTypesMask(MessageType::Debug);
        //     QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(),0);
        //     QCOMPARE(widget->displayedMessageTypesMask(), MessageType::Debug);
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Debug), true);
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Info), false);
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Warning), false);
        //     // Try setting more masks
        //     widget->setDisplayedMessageTypesMask(MessageType::Info|MessageType::Warning);
        //     QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(),0);
        //     QCOMPARE(widget->displayedMessageTypesMask(), MessageType::Info|MessageType::Warning);
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Debug), false);
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Info), true);
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Warning), true);

        //     // Trigger method emitting signal
        //     widget->setMessageTypeDisplayed(MessageType::Info, false);
        //     // Check if visibility changed
        //     QCOMPARE(widget->isMessageTypeDisplayed(MessageType::Info), false);
        //     // Wait for signal to be emitted
        //     QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(),1);
        //     // Check if corretct things were emitted
        //     auto signalArgs = messageTypeVisibilityChangedSpy.takeFirst();
        //     QCOMPARE(signalArgs.count(),2);
        //     QCOMPARE(signalArgs.at(0).value<MessageType>(), MessageType::Info);
        //     QCOMPARE(signalArgs.at(1).toBool(), false);
        // }
    }

    void test_mouse_double_click_single_item() {
        // Show widget
        widget->show();
        QVERIFY(QTest::qWaitForWindowExposed(widget));
        // Get index for the element number 1
        const QModelIndex index = widget->model()->index(1, 0);
        QVERIFY(index.isValid());
        auto* proxyModel = qobject_cast<MessageListProxyModel*>(widget->model());
        QVERIFY(proxyModel);
        const MessageViewItem* viewedElement = static_cast<MessageViewItem*>(proxyModel->mapToSource(index).internalPointer());
        // We need to verify this due to two reasons:
        // - If static_cast above returned sth wrong (UB) - we will crash here.
        // - And all of the "Dummy" messages have some "whats", so if by any chance it is empty - sth is still wrong
        QVERIFY(viewedElement != nullptr);
        QVERIFY(!viewedElement->what().isEmpty());
        // Scroll...
        widget->scrollTo(index, QAbstractItemView::PositionAtCenter);
        QCoreApplication::processEvents();
        // Lets find the location where to click
        const QRect rect = widget->visualRect(index);
        QVERIFY(rect.isValid());
        QVERIFY(!rect.isEmpty());
        const QPoint pos = rect.center();
        QVERIFY(widget->indexAt(pos) == index);
        // Lets click to select things
        QTest::mouseClick(widget->viewport(), Qt::LeftButton, Qt::NoModifier, pos);
        // And check if this happened
        QVERIFY(widget->selectionModel()->isSelected(index));
        QVERIFY(widget->currentIndex() == index);

        // "Register handler"
        bool isHandlerSuccessfull = false;
        UiTestHelper::scheduleForAllWidgets<MessageDisplayWidget>([viewedElement, &isHandlerSuccessfull](MessageDisplayWidget* widget) {
            QCOMPARE(widget->message(), viewedElement);
            isHandlerSuccessfull = widget->message() == viewedElement;
        });

        // And lets do a double click
        QTest::mouseDClick(widget->viewport(), Qt::LeftButton, Qt::NoModifier, pos);
        // And final check..
        QTRY_COMPARE(isHandlerSuccessfull, true);
    }

    // void test_mouse_double_click_multiple_items() {
    //     constexpr int numberSelections = 2;

    //     // Show widget
    //     widget->show();
    //     QVERIFY(QTest::qWaitForWindowExposed(widget));
    //     QVERIFY(widget->selectionMode() != QAbstractItemView::SingleSelection);
    //     //
    //     auto* proxyModel = qobject_cast<MessageListProxyModel*>(widget->model());
    //     QVERIFY(proxyModel);

        // const QModelIndex proxyIndex0 = widget->model()->index(0, 0);
        // const QModelIndex proxyIndex1 = widget->model()->index(1, 0);

        // QVERIFY(proxyIndex0.isValid());
        // QVERIFY(proxyIndex1.isValid());

        // const QModelIndex sourceIndex0 = proxyModel->mapToSource(proxyIndex0);
        // const QModelIndex sourceIndex1 = proxyModel->mapToSource(proxyIndex1);

        // QVERIFY(sourceIndex0.isValid());
        // QVERIFY(sourceIndex1.isValid());

        // const auto* viewedElement0 =
        //     static_cast<MessageViewItem*>(sourceIndex0.internalPointer());

        // const auto* viewedElement1 =
        //     static_cast<MessageViewItem*>(sourceIndex1.internalPointer());

        // QVERIFY(viewedElement0 != nullptr);
        // QVERIFY(viewedElement1 != nullptr);

        // QVERIFY(!viewedElement0->what().isEmpty());
        // QVERIFY(!viewedElement1->what().isEmpty());

        // // Прокручуємо до першого/другого. Для 0 та 1 це зазвичай не потрібно,
        // // але хай тест буде стабільний.
        // widget->scrollTo(proxyIndex0, QAbstractItemView::PositionAtCenter);
        // widget->scrollTo(proxyIndex1, QAbstractItemView::PositionAtCenter);
        // QCoreApplication::processEvents();

        // const QRect rect0 = widget->visualRect(proxyIndex0);
        // const QRect rect1 = widget->visualRect(proxyIndex1);

        // QVERIFY(rect0.isValid());
        // QVERIFY(rect1.isValid());
        // QVERIFY(!rect0.isEmpty());
        // QVERIFY(!rect1.isEmpty());

        // const QPoint pos0 = rect0.center();
        // const QPoint pos1 = rect1.center();

        // QCOMPARE(widget->indexAt(pos0), proxyIndex0);
        // QCOMPARE(widget->indexAt(pos1), proxyIndex1);

        // // Варіант максимально близький до користувача:
        // // 1. клік по першому
        // // 2. Ctrl+клік по другому
        // QTest::mouseClick(
        //     widget->viewport(),
        //     Qt::LeftButton,
        //     Qt::NoModifier,
        //     pos0
        //     );

        // QTest::mouseClick(
        //     widget->viewport(),
        //     Qt::LeftButton,
        //     Qt::ControlModifier,
        //     pos1
        //     );

        // QVERIFY(widget->selectionModel()->isSelected(proxyIndex0));
        // QVERIFY(widget->selectionModel()->isSelected(proxyIndex1));

        // const QSet<const MessageViewItem*> expectedMessages{
        //     viewedElement0,
        //     viewedElement1
        // };

        // QSet<const MessageViewItem*> receivedMessages;

        // UiTestHelper::scheduleForAllWidgets<MessageDisplayWidget>(
        //     [&expectedMessages, &receivedMessages](MessageDisplayWidget* displayWidget) {
        //         const auto* message = displayWidget->message();

        //         QVERIFY(expectedMessages.contains(message));

        //         receivedMessages.insert(message);
        //     }
        //     );

        // // Дабл-клік по одному з уже виділених item-ів.
        // // Якщо production-код бере selectionModel()->selection(),
        // // у діалог мають потрапити обидва.
        // QTest::mouseDClick(
        //     widget->viewport(),
        //     Qt::LeftButton,
        //     Qt::NoModifier,
        //     pos1
        //     );

        // QTRY_COMPARE(receivedMessages.size(), expectedMessages.size());

        // QCOMPARE(receivedMessages, expectedMessages);
    // }
};

}; // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::MessageListViewTest)

#include "MessageListViewTest.moc"
