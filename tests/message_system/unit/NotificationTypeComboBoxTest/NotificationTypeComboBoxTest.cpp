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

#include "draupnir/ui/widgets/NotificationTypeComboBox.h"

namespace Draupnir::Messages
{

/*! @class NotificationTypeComboBoxTest tests/message_system/unit/NotificationTypeComboBoxTest.cpp
 *  @brief This test class tests NotificationTypeComboBox class. */

class NotificationTypeComboBoxTest final : public QObject
{
    Q_OBJECT
public:
    NotificationTypeComboBox* widget = nullptr;

private slots:
    void init() { widget = new NotificationTypeComboBox; }
    void cleanup() { delete widget; }

    void test_initialization() {
        QCOMPARE(widget->notificationType(), Notification::None);
    }

    void test_setting_selection() {
        QSignalSpy notificationTypeChangedSpy( widget, &NotificationTypeComboBox::notificationTypeSelected );

        widget->setNotificationType(Notification::MessageBoxType);
        widget->setNotificationType(Notification::MessageBoxType);

        QCOMPARE(widget->notificationType(), Notification::MessageBoxType);
        QTRY_COMPARE(notificationTypeChangedSpy.count(), 0);
    }

    void test_user_setting_selection() {
        QSignalSpy notificationTypeChangedSpy( widget, &NotificationTypeComboBox::notificationTypeSelected );

        QVERIFY(widget->notificationType() != Notification::MessageBoxType);

        // Select something
        widget->setNotificationTypeSelected(Notification::MessageBoxType);
        QCOMPARE(widget->notificationType(), Notification::MessageBoxType);
        // We have signal
        QTRY_COMPARE(notificationTypeChangedSpy.count(), 1);
        // With propper args
        auto args = notificationTypeChangedSpy.takeAt(0);
        QCOMPARE(args.count(),1);
        QCOMPARE(args[0].value<Notification::Type>(), Notification::MessageBoxType);

        // Select the same stuff again
        widget->setNotificationTypeSelected(Notification::MessageBoxType);
        QCOMPARE(notificationTypeChangedSpy.count(),0);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::NotificationTypeComboBoxTest)

#include "NotificationTypeComboBoxTest.moc"
