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

#include "draupnir/logging/core/AbstractMessageViewIconProvider.h"
#include "draupnir/logging/messages/MessageViewItem.h"
#include "draupnir/logging/models/MessageListModel.h"
#include "draupnir/logging/models/MessageListProxyModel.h"


namespace Draupnir::Logging
{

/*! @class MessageListProxyModelTest tests/modules/logging/unit/MessageListProxyModelTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageListProxyModel class. */

class MessageListProxyModelTest : public QObject
{
    Q_OBJECT
private:
    static inline constexpr MessageCategory dummyCategory       = 0b1000'0000'0000'0000;
    static inline constexpr MessageCategory nonExistingCategory = 0b0001'0000'0000'0000;

    MessageListModel* sourceModel = nullptr;
    Message* debugOne = nullptr;
    Message* infoOne = nullptr;
    Message* infoTwo = nullptr;
    Message* errorOne = nullptr;
    MessageListProxyModel* testedProxy = nullptr;

private slots:
    void initTestCase() {
        sourceModel = new MessageListModel{this};
        debugOne = Message::create("Debug", MessageLevel::Debug);
        infoOne = Message::create("Info One", MessageLevel::Info);
        infoTwo = Message::create("Info TWo", MessageLevel::Info);
        errorOne = Message::create("ERROR!", MessageLevel::Error, dummyCategory);

        sourceModel->append(
            { debugOne, infoOne, infoTwo, errorOne }
        );

        MessageViewItem::registerIconProvider(new AbstractMessageViewIconProvider);
    }

    void init() {
        testedProxy = new MessageListProxyModel;
        testedProxy->setSourceModel(sourceModel);
    }

    void cleanup() { delete testedProxy; testedProxy = nullptr; }

    void test_initialization() {
        QCOMPARE(testedProxy->displayedMessageCategoriesMask(), MessageCategories::All);
        QCOMPARE(testedProxy->displayedMessageLevelsMask(), MessageLevels::All);
        QCOMPARE(testedProxy->displayedMessageViewItemFieldsMask(), MessageViewItemFields::All);
    }

    void test_setting_message_levels() {
        MessageViewItem* msgView = nullptr;

        // Show all messages
        QCOMPARE(testedProxy->columnCount(), sourceModel->columnCount());
        QCOMPARE(testedProxy->rowCount(), sourceModel->rowCount());

        // Show only debug messages
        testedProxy->setDisplayedMessageLevelsMask(MessageLevel::Debug);
        QCOMPARE(testedProxy->displayedMessageLevelsMask(), MessageLevel::Debug);
        QCOMPARE(testedProxy->rowCount(), 1);
        msgView = static_cast<MessageViewItem*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QCOMPARE(msgView->message(), debugOne);

        // Show only info messages
        testedProxy->setDisplayedMessageLevelsMask(MessageLevel::Info);
        QCOMPARE(testedProxy->displayedMessageLevelsMask(), MessageLevel::Info);
        QCOMPARE(testedProxy->rowCount(), 2);
        msgView = static_cast<MessageViewItem*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QVERIFY(msgView->message() == infoOne || msgView->message() == infoTwo);

        // Show Warning messages (which are not present)
        testedProxy->setDisplayedMessageLevelsMask(MessageLevel::Warning);
        QCOMPARE(testedProxy->displayedMessageLevelsMask(), MessageLevel::Warning);
        QCOMPARE(testedProxy->rowCount(), 0);
    }

    void test_setting_message_categories() {
        MessageViewItem* msgView = nullptr;

        // Show all messages
        QCOMPARE(testedProxy->columnCount(), sourceModel->columnCount());
        QCOMPARE(testedProxy->rowCount(), sourceModel->rowCount());

        // Show only messages from default category
        testedProxy->setDisplayedMessageCategoriesMask(MessageCategory::Default);
        QCOMPARE(testedProxy->displayedMessageCategoriesMask(), MessageCategory::Default);
        QCOMPARE(testedProxy->rowCount(), 3);
        msgView = static_cast<MessageViewItem*>(testedProxy->mapToSource(testedProxy->index(0,0)).internalPointer());
        QCOMPARE(msgView->type().category(), MessageCategory::Default);

        // Show some dummy category
        testedProxy->setDisplayedMessageCategoriesMask(dummyCategory);
        QCOMPARE(testedProxy->displayedMessageCategoriesMask(), dummyCategory);
        QCOMPARE(testedProxy->rowCount(), 1);

        // Show some non-existing category
        testedProxy->setDisplayedMessageCategoriesMask(nonExistingCategory);
        QCOMPARE(testedProxy->displayedMessageCategoriesMask(), nonExistingCategory);
        QCOMPARE(testedProxy->rowCount(), 0);
    }

    void test_setting_message_fields() {
        MessageViewItem* message = nullptr;

        // Hide all Message Fields
        testedProxy->setDisplayedMessageViewItemFieldsMask(MessageViewItemFields::None);
        QCOMPARE(testedProxy->displayedMessageViewItemFieldsMask(), MessageViewItemFields::None);
        QCOMPARE(testedProxy->columnCount(), sourceModel->columnCount());
        QCOMPARE(testedProxy->rowCount(), sourceModel->rowCount());
        QModelIndex proxyIndex = testedProxy->index(0,0);
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), QString{});
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), QIcon{});

        // Show only icons
        testedProxy->setDisplayedMessageViewItemFieldsMask(MessageViewItemField::Icon);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Icon), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), false);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::What), false);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::DateTime), false);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<MessageViewItem*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), QString{});

        // Show icons and brief
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::Brief, true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Icon), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::What), false);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::DateTime), false);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<MessageViewItem*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(), message->brief());

        // Show icons, brief and what
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::What, true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Icon), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::What), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::DateTime), false);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<MessageViewItem*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(),
                 message->getViewString(testedProxy->displayedMessageViewItemFieldsMask()));

        // Show icons, brief, what and datetime
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::DateTime, true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Icon), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::Brief), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::What), true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::DateTime), true);
        proxyIndex = testedProxy->index(0,0);
        message = static_cast<MessageViewItem*>(testedProxy->mapToSource(proxyIndex).internalPointer());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DecorationRole).value<QIcon>(), message->icon());
        QCOMPARE(testedProxy->data(proxyIndex,Qt::DisplayRole).toString(),
                 message->getViewString(testedProxy->displayedMessageViewItemFieldsMask()));
    }

    void test_setting_message_levels_extended() {
        // Test multiple disabling calls
        testedProxy->setMessageLevelDisplayed(MessageLevel::Debug, false);
        testedProxy->setMessageLevelDisplayed(MessageLevel::Debug, false);
        QCOMPARE(testedProxy->isMessageLevelDisplayed(MessageLevel::Debug), false);

        // Test multiple enabling calls
        testedProxy->setMessageLevelDisplayed(MessageLevel::Debug, true);
        testedProxy->setMessageLevelDisplayed(MessageLevel::Debug, true);
        QCOMPARE(testedProxy->isMessageLevelDisplayed(MessageLevel::Debug), true);
    }

    void test_setting_message_fields_extended() {
        // Test multiple disabling calls
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::DateTime, false);
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::DateTime, false);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::DateTime), false);

        // Test multiple enabling calls
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::DateTime, true);
        testedProxy->setMessageViewItemFieldDisplayed(MessageViewItemField::DateTime, true);
        QCOMPARE(testedProxy->isMessageViewItemFieldDisplayed(MessageViewItemField::DateTime), true);
    }
};

}; // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::MessageListProxyModelTest)

#include "MessageListProxyModelTest.moc"
