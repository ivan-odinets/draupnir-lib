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

#include "NotificationTypeComboBox.h"

#include <QEvent>

NotificationTypeComboBox::NotificationTypeComboBox(QWidget* parent) :
    QComboBox{parent}
{
    for (auto notificationType : Notification::displayedValues) {
        addItem(
            Notification::toDisplayString(notificationType),
            QVariant::fromValue(notificationType)
        );
    }

    connect(this,  static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,  &NotificationTypeComboBox::_onCurrentIndexChanged);

    // The only UI strings displayed within this widget - are representation of the Notification::Type enum.
    // So we do not need to call within the consctructor a _retranslateUi() method.
}

Notification::Type NotificationTypeComboBox::selectedNotificationType() const
{
    return QComboBox::currentData().value<Notification::Type>();
}

void NotificationTypeComboBox::setNotificationType(Notification::Type type)
{
    for (int i = 0; i < count(); i++) {
        const auto notificationType = itemData(i).value<Notification::Type>();
        if (notificationType == type) {
            setCurrentIndex(i);
            return;
        }
    }

    // We should not receive here any other values than those which are part of Notification::displayedValues array
    // If we have this - probably somewhere something went wrong
    Q_ASSERT_X(false, "NotificationTypeComboBox::setNotificationType",
                      "some weird notification type. Probably Notification::displayedValues is outdated");
}

void NotificationTypeComboBox::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QComboBox::changeEvent(event);
}

void NotificationTypeComboBox::_onCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    emit notificationTypeChanged(selectedNotificationType());
}

void NotificationTypeComboBox::_retranslateUi()
{
    for (int i = 0; i < count(); i++) {
        const auto notificationType = itemData(i).value<Notification::Type>();
        setItemText(i, Notification::toDisplayString(notificationType));
    }
}
