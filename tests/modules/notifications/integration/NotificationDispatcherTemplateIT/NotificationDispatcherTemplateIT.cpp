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
#include <QDebug>

#include "draupnir/messages/core/Message.h"
#include "draupnir/messages/core/MessageTypeIconProvider.h"
#include "draupnir/messages/core/MessageViewItem.h"
#include "draupnir/notifications/channels/DialogNotificationChannel.h"
#include "draupnir/notifications/channels/TrayNotificationChannel.h"
#include "draupnir/notifications/core/NotificationDispatcherTemplate.h"

#include "draupnir-test/helpers/MessageDisplayDialogHelpers.h"

using namespace Draupnir::Messages;
using namespace Draupnir::Notifications;

/*! @class NotificationDispatcherTemplateIT tests/modules/notifications/integration/NotificationDispatcherTemplateIT.cpp
 *  @ingroup NotificationsTests
 *  @brief Integration test for the @ref Draupnir::Notifications::NotificationDispatcherTemplate. */

class NotificationDispatcherTemplateIT final : public QObject
{
    Q_OBJECT
private:
    using NotificationDispatcher = NotificationDispatcherTemplate<
        DialogNotificationChannel,
        TrayNotificationChannel
    >;

    using StatelessNotificationDispatcher = NotificationDispatcherTemplate<
        DialogNotificationChannel
    >;

    NotificationDispatcher* dispatcher = nullptr;

    MessagePtr dummyMessage =
        Message::create("Very Important Message", MessageLevel::Info);
    MessageList dummyMessageList = MessageList{
        Message::create("One",   MessageLevel::Debug),
        Message::create("two",   MessageLevel::Info),
        Message::create("Three", MessageLevel::Warning),
        Message::create("Four",  MessageLevel::Error)
    };

private slots:
    void initTestCase() {
        MessageViewItem::registerIconProvider(new MessageTypeIconProvider);
    }

    void init() { dispatcher = new NotificationDispatcher; }
    void cleanup() { delete dispatcher; dispatcher = nullptr; }

    void test_initial_state() {
        QVERIFY(dispatcher->getNotificationHandler<TrayNotificationChannel>().trayIcon() == nullptr);
        QVERIFY(NotificationDispatcher::hasInstanceChannels_v == true);
        QVERIFY(StatelessNotificationDispatcher::hasInstanceChannels_v == false);

        QCOMPARE(sizeof(StatelessNotificationDispatcher), 1);
        QVERIFY(sizeof(NotificationDispatcher) > sizeof(StatelessNotificationDispatcher));
    }

    void test_show_message() {
        std::expected<void,QString> result;
        // When
        dispatcher->showMessage(DialogNotificationChannel::Trait::value(), dummyMessage);
        // Than
        result = MessageDisplayDialogHelpers::dialogWasDisplayedWith(MessageList{dummyMessage});
        if (!result) QFAIL(result.error().toLatin1().constData());

        // When
        StatelessNotificationDispatcher::showMessage(DialogNotificationChannel::Trait::value(), dummyMessage);
        // Than
        result = MessageDisplayDialogHelpers::dialogWasDisplayedWith(MessageList{dummyMessage});
        if (!result) QFAIL(result.error().toLatin1().constData());
    }

    void test_show_message_list() {
        std::expected<void,QString> result;
        // When
        dispatcher->showMessageList(DialogNotificationChannel::Trait::value(), dummyMessageList);
        // Then
        result = MessageDisplayDialogHelpers::dialogWasDisplayedWith(dummyMessageList);
        if (!result) QFAIL(result.error().toLatin1().constData());

        // When
        StatelessNotificationDispatcher::showMessageList(DialogNotificationChannel::Trait::value(), dummyMessageList);
        // Then
        result = MessageDisplayDialogHelpers::dialogWasDisplayedWith(dummyMessageList);
        if (!result) QFAIL(result.error().toLatin1().constData());
    }
};

QTEST_MAIN(NotificationDispatcherTemplateIT)

#include "NotificationDispatcherTemplateIT.moc"
