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

#include "draupnir/notifications/ui/menus/NotificationTypesSelectorMenuTemplate.h"
#include "draupnir/notifications/traits/DialogNotificationTrait.h"
#include "draupnir/notifications/traits/TrayNotificationTrait.h"

using namespace Draupnir::Notifications;

/*! @class NotificationTypesSelectorMenuTest tests/modules/message_system/unit/NotificationTypesSelectorMenuTest.cpp
 *  @ingroup NotificationsTests */

class NotificationTypesSelectorMenuTest final : public QObject
{
    Q_OBJECT
private:
    using NotificationTypesSelectorMenu = NotificationTypesSelectorMenuTemplate<
        TrayNotificationTrait,
        DialogNotification
    >;
    NotificationTypesSelectorMenu* menu = nullptr;

    static inline constexpr NotificationTypes trayAndDialogValue =
        NotificationType::TrayNotification | NotificationType::DialogNotification;

private slots:
    void init() { menu = new NotificationTypesSelectorMenu; }
    void cleanup() { delete menu; menu = nullptr; }

    void test_initial_state() {
        QCOMPARE(menu->selectedMask(), NotificationType::NoNotification);
    }
};

QTEST_MAIN(NotificationTypesSelectorMenuTest)

#include "NotificationTypesSelectorMenuTest.moc"
