/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/message_system/ui/menus/NotificationTypeMenu.h"

namespace Draupnir::Messages
{

/*! @class NotificationTypeComboBoxTest tests/message_system/unit/NotificationTypeMenuTest.cpp
 *  @brief This test class tests NotificationTypeMenu class. */

class NotificationTypeMenuTest final : public QObject
{
    Q_OBJECT
public:
    NotificationTypeMenu* menu = new NotificationTypeMenu;

private slots:
    void init() { menu = new NotificationTypeMenu; }
    void cleanup() { delete menu; }

    void test_initialization() {
        QCOMPARE(menu->notificationType(), Notification::UnknownType);
    }

    void test_setting_selection() {
        QSignalSpy notificationTypeChangedSpy( menu, &NotificationTypeMenu::notificationTypeChanged );

        menu->setNotificationType(Notification::MessageBoxType);
        menu->setNotificationType(Notification::MessageBoxType);

        QCOMPARE(menu->notificationType(), Notification::MessageBoxType);
        QTRY_COMPARE(notificationTypeChangedSpy.count(), 0);
    }

    void test_user_setting_selection() {
        QSignalSpy notificationTypeChangedSpy( menu, &NotificationTypeMenu::notificationTypeChanged );

        QVERIFY(menu->notificationType() != Notification::MessageBoxType);

        // Select something
        menu->getActionFor(Notification::MessageBoxType)->trigger();
        QCOMPARE(menu->notificationType(), Notification::MessageBoxType);
        // We have signal
        QTRY_COMPARE(notificationTypeChangedSpy.count(), 1);
        // With propper args
        auto args = notificationTypeChangedSpy.takeAt(0);
        QCOMPARE(args.count(),1);
        QCOMPARE(args[0].value<Notification::Type>(), Notification::MessageBoxType);

        // Select the same stuff again
        menu->getActionFor(Notification::MessageBoxType)->trigger();
        QCOMPARE(notificationTypeChangedSpy.count(),0);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::NotificationTypeMenuTest)

#include "NotificationTypeMenuTest.moc"
