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

#include "draupnir/logging/messages/AbstractMessageViewIconProvider.h"
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
        widget->setDisplayedMessageCategoriesMask(MessageCategories::All);
        QVERIFY(widget->displayedMessageCategoriesMask() == MessageCategories::All);

        // Signal spy
        QSignalSpy messageCategoriesVisibilityChangedSpy{ widget, &MessageListView::messageCategoryVisibilityChanged };
        // Try setting masks
        widget->setDisplayedMessageCategoriesMask(MessageCategory::Default);
        widget->setDisplayedMessageCategoriesMask(MessageCategory::Default);
        QTRY_COMPARE(messageCategoriesVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageCategoriesMask(), MessageCategory::Default);
        QCOMPARE(widget->isMessageCategoryDisplayed(MessageCategory::Default), true);
        QCOMPARE(widget->isMessageCategoryDisplayed(MessageCategory::Network), false);
        // Try setting more masks
        widget->setDisplayedMessageCategoriesMask(MessageCategory::Default | MessageCategory::Network);
        QTRY_COMPARE(messageCategoriesVisibilityChangedSpy.count(),0);
        QCOMPARE(widget->displayedMessageCategoriesMask(), MessageCategory::Default | MessageCategory::Network);
        QCOMPARE(widget->isMessageCategoryDisplayed(MessageCategory::Default), true);
        QCOMPARE(widget->isMessageCategoryDisplayed(MessageCategory::Network), true);

        // Trigger method emitting signal
        widget->setMessageCategoryDisplayed(MessageCategory::Default, false);
        // Check if visibility changed
        QCOMPARE(widget->isMessageCategoryDisplayed(MessageCategory::Default), false);
        // Wait for signal to be emitted
        QTRY_COMPARE(messageCategoriesVisibilityChangedSpy.count(),1);
        // Check if corretct things were emitted
        auto signalArgs = messageCategoriesVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(),2);
        QCOMPARE(signalArgs.at(0).value<MessageCategory>(), MessageCategory::Default);
        QCOMPARE(signalArgs.at(1).toBool(), false);
    }

    void test_setting_message_levels() {
        widget->setDisplayedMessageLevelsMask(MessageLevels::All);
        QVERIFY(widget->displayedMessageLevelsMask() == MessageLevels::All);

        // Signal spy
        QSignalSpy messageLevelVisibilityChangedSignalSpy{ widget, &MessageListView::messageLevelVisibilityChanged };
        // Try setting masks
        widget->setDisplayedMessageLevelsMask(MessageLevel::Debug);
        QTRY_COMPARE(messageLevelVisibilityChangedSignalSpy.count(),0);
        QCOMPARE(widget->displayedMessageLevelsMask(), MessageLevel::Debug);
        QCOMPARE(widget->isMessageLevelDisplayed(MessageLevel::Debug), true);
        QCOMPARE(widget->isMessageLevelDisplayed(MessageLevel::Info), false);

        // Trigger method emitting signal
        widget->setMessageLevelDisplayed(MessageLevel::Debug, false);
        // Check if visibility changed
        QCOMPARE(widget->isMessageLevelDisplayed(MessageLevel::Debug), false);
        // Wait for signal to be emitted
        QTRY_COMPARE(messageLevelVisibilityChangedSignalSpy.count(),1);
        // Check if corretct things were emitted
        auto signalArgs = messageLevelVisibilityChangedSignalSpy.takeFirst();
        QCOMPARE(signalArgs.count(),2);
        QCOMPARE(signalArgs.at(0).value<MessageLevel::Value>(), MessageLevel::Debug);
        QCOMPARE(signalArgs.at(1).toBool(), false);
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
};

}; // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::MessageListViewTest)

#include "MessageListViewTest.moc"
