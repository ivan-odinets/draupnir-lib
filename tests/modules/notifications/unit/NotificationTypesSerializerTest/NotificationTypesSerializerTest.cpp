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

#include "draupnir/notifications/core/NotificationTypesSerializerTemplate.h"
#include "draupnir/notifications/traits/notifications/DialogNotificationTrait.h"
#include "draupnir/notifications/traits/notifications/TrayNotificationTrait.h"

using namespace Draupnir::Notifications;

/*! @class NotificationTypesSerializerTest tests/modules/message_system/unit/NotificationTypesSerializerTest.cpp
 *  @ingroup NotificationsTests */

class NotificationTypesSerializerTest final : public QObject
{
    Q_OBJECT
private:
    using NotificationTypesSerializer = NotificationTypesSerializerTemplate<
        DialogNotificationTrait,
        TrayNotificationTrait
    >;
    NotificationTypesSerializer* serializer = nullptr;

    static inline constexpr NotificationTypes trayAndDialogValue =
        NotificationType::TrayNotification | NotificationType::DialogNotification;

private slots:
    void initTestCase() { serializer = new NotificationTypesSerializer; }
    void cleanupTestCase() { delete serializer; serializer = nullptr; }

    void test_from_config_string() {
        // Single values
        QCOMPARE(serializer->fromConfigString("none"), NotificationType::NoNotification);
        QCOMPARE(serializer->fromConfigString("tray"), NotificationType::TrayNotification);

        // Combinations
        QCOMPARE(serializer->fromConfigString("tray,dialog"), trayAndDialogValue);
    }

    void test_to_config_string() {
        // Single values
        QCOMPARE(serializer->toConfigString(NotificationType::NoNotification), "none");
        QCOMPARE(serializer->toConfigString(NotificationType::TrayNotification), "tray");
        QCOMPARE(serializer->toConfigString(NotificationType::DialogNotification), "dialog");

        // Combinations
        const QStringList resultParts = serializer->toConfigString(trayAndDialogValue).split(',');
        QCOMPARE(resultParts.count(), 2);
        QVERIFY(resultParts.contains("tray"));
        QVERIFY(resultParts.contains("dialog"));
    }
};

QTEST_MAIN(NotificationTypesSerializerTest)

#include "NotificationTypesSerializerTest.moc"
