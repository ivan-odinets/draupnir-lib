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
#include <QDebug>
#include <QCoreApplication>

#include "draupnir/logging/messages/AbstractMessageViewIconProvider.h"
#include "draupnir/logging/ui/widgets/MessageDisplayWidget.h"
#include "draupnir/notifications/handlers/MessageHandlerTemplate.h"
#include "draupnir/notifications/traits/messages/MessageTypeTraitTemplate.h"
#include "draupnir/logging/messages/categories/MessageCategories.h"
#include "draupnir/logging/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/logging/messages/MessageViewItem.h"
#include "draupnir/notifications/traits/notifications/DialogNotificationTrait.h"
#include "draupnir/notifications/handlers/DialogNotificationHandler.h"

#include "draupnir-test/helpers/UiTestHelper.h"
#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"

using namespace Draupnir::Logging;
using namespace Draupnir::Notifications;

/*! @class MessageHandlerTemplateIT tests/modules/notifications/integration/MessageHandlerTemplateIT.cpp
 *  @ingroup NotificationsTests */

class MessageHandlerTemplateIT final : public QObject
{
    Q_OBJECT
private:
    using MessageHandler = MessageHandlerTemplate<
        MessageTypesWrapper<
            MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Info>,
            MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Warning>,
            MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Error>
        >,
        NotificationTypesWrapper<DialogNotificationTrait>
    >;
    using SettingsRegistryMocked = Draupnir::Settings::SettingsSourceMockTemplate<
        MessageNotificationSettingTrait<MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Info>>,
        MessageNotificationSettingTrait<MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Warning>>,
        MessageNotificationSettingTrait<MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Error>>
    >;

    Message* dummyMessage = nullptr;
    MessageList dummyMessageList = MessageList{};
    AbstractMessageViewIconProvider* iconProvider = nullptr;
    SettingsRegistryMocked* settingsSource = nullptr;
    MessageHandler* messageHandler = nullptr;

    void verify_single_message_dialog_shown_correctly(Message* message) {
        auto displayedDialog = QPointer(UiTestHelper::waitForFirstTopLevelWidget<MessageDisplayDialog>(1500));
        QVERIFY(displayedDialog);
        auto messageWidget = UiTestHelper::findFirstWidget<MessageDisplayWidget>();
        QVERIFY(messageWidget);
        QCOMPARE(messageWidget->message()->message(), message);

        displayedDialog->close();
        QTRY_VERIFY(displayedDialog.isNull());
    }

    void verify_several_message_dialog_shown_correctly() {
        // auto displayedDialog = QPointer(UiTestHelper::waitForFirstTopLevelWidget<MessageDisplayDialog>(1500));
        // QVERIFY(displayedDialog);
        // auto messageWidetsList = UiTestHelper::findAllWidgets<MessageDisplayWidget>();
        // QCOMPARE(messageWidetsList.count(), dummyMessageList.count());
        // for (MessageDisplayWidget* messageWidget : messageWidetsList) {
        //     QVERIFY(dummyMessageList.contains(messageWidget->message()->message()));
        // }

        // displayedDialog->close();
        // QTRY_VERIFY(displayedDialog.isNull());
    }

private slots:
    void initTestCase() {
        iconProvider = new AbstractMessageViewIconProvider;
        MessageViewItem::registerIconProvider(iconProvider);
        dummyMessage = Message::create("Very Important Message", MessageLevel::Info);
        dummyMessageList = MessageList{
            Message::create("One",   MessageLevel::Debug),
            Message::create("two",   MessageLevel::Info),
            Message::create("Three", MessageLevel::Warning),
            Message::create("Four",  MessageLevel::Error)
        };

        settingsSource = new SettingsRegistryMocked;
        settingsSource->loadSettings();
    }

    void cleanupTestCase() {
        delete dummyMessage; dummyMessage = nullptr;
        delete iconProvider; iconProvider = nullptr;
        qDeleteAll(dummyMessageList);
        delete settingsSource; settingsSource = nullptr;
    }

    void init() { messageHandler = new MessageHandler; }
    void cleanup() { delete messageHandler; messageHandler = nullptr; }

    void test_handle_message() {
        messageHandler->loadSettings(settingsSource);
        messageHandler->setNotificationFor(dummyMessage->type(), NotificationType::DialogNotification);
        messageHandler->handleMessage(dummyMessage);

        verify_single_message_dialog_shown_correctly(dummyMessage);
    }

    void test_handle_message_list() {
        messageHandler->loadSettings(settingsSource);
        messageHandler->handleMessageList(dummyMessageList);
    }
};

QTEST_MAIN(MessageHandlerTemplateIT)

#include "MessageHandlerTemplateIT.moc"
