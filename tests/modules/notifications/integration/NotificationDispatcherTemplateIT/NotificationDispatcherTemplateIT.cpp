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
#include "draupnir/notifications/handlers/DialogNotificationHandler.h"
#include "draupnir/notifications/handlers/TrayNotificationHandler.h"
#include "draupnir/notifications/handlers/NotificationDispatcherTemplate.h"

#include "draupnir-test/helpers/UiTestHelper.h"

using namespace Draupnir::Logging;
using namespace Draupnir::Notifications;

/*! @class NotificationDispatcherTemplateIT tests/modules/notifications/integration/NotificationDispatcherTemplateIT.cpp
 *  @ingroup NotificationsTests */

class NotificationDispatcherTemplateIT final : public QObject
{
    Q_OBJECT
private:
    using NotificationDispatcher = NotificationDispatcherTemplate<
        DialogNotificationTrait,
        TrayNotificationTrait
    >;

    using StatelessNotificationDispatcher = NotificationDispatcherTemplate<
        DialogNotificationTrait
    >;

    NotificationDispatcher* dispatcher = nullptr;

    Message* dummyMessage = nullptr;
    MessageList dummyMessageList = MessageList{};
    AbstractMessageViewIconProvider* iconProvider = nullptr;

    void verify_single_message_dialog_shown_correctly() {
        auto displayedDialog = QPointer(UiTestHelper::waitForFirstTopLevelWidget<MessageDisplayDialog>(1500));
        QVERIFY(displayedDialog);
        auto messageWidget = UiTestHelper::findFirstWidget<MessageDisplayWidget>();
        QVERIFY(messageWidget);
        QCOMPARE(messageWidget->message()->message(), dummyMessage);

        displayedDialog->close();
        QTRY_VERIFY(displayedDialog.isNull());
    }

    void verify_several_message_dialog_shown_correctly() {
        auto displayedDialog = QPointer(UiTestHelper::waitForFirstTopLevelWidget<MessageDisplayDialog>(1500));
        QVERIFY(displayedDialog);
        auto messageWidetsList = UiTestHelper::findAllWidgets<MessageDisplayWidget>();
        QCOMPARE(messageWidetsList.count(), dummyMessageList.count());
        for (MessageDisplayWidget* messageWidget : messageWidetsList) {
            QVERIFY(dummyMessageList.contains(messageWidget->message()->message()));
        }

        displayedDialog->close();
        QTRY_VERIFY(displayedDialog.isNull());
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
    }

    void cleanupTestCase() {
        delete dummyMessage; dummyMessage = nullptr;
        delete iconProvider; iconProvider = nullptr;
        qDeleteAll(dummyMessageList);
    }

    void init() { dispatcher = new NotificationDispatcher; }
    void cleanup() { delete dispatcher; dispatcher = nullptr; }

    void test_initial_state() {
        QVERIFY(dispatcher->getNotificationHandler<TrayNotificationTrait>().trayIcon() == nullptr);
        QVERIFY(NotificationDispatcher::hasAnyStatefullHandlers_v == true);
        QVERIFY(StatelessNotificationDispatcher::hasAnyStatefullHandlers_v == false);

        QCOMPARE(sizeof(StatelessNotificationDispatcher), 1);
        QVERIFY(sizeof(NotificationDispatcher) > sizeof(StatelessNotificationDispatcher));
    }

    void test_show_message() {
        // When
        dispatcher->showMessage(DialogNotificationTrait::value(), dummyMessage);
        // Than
        verify_single_message_dialog_shown_correctly();

        // When
        StatelessNotificationDispatcher::showMessage(DialogNotificationTrait::value(), dummyMessage);
        // Than
        verify_single_message_dialog_shown_correctly();
    }

    void test_show_message_list() {
        // When
        dispatcher->showMessageList(DialogNotificationTrait::value(), dummyMessageList);
        // Then
        verify_several_message_dialog_shown_correctly();

        // When
        StatelessNotificationDispatcher::showMessageList(DialogNotificationTrait::value(), dummyMessageList);
        // Then
        verify_several_message_dialog_shown_correctly();
    }
};

QTEST_MAIN(NotificationDispatcherTemplateIT)

#include "NotificationDispatcherTemplateIT.moc"
