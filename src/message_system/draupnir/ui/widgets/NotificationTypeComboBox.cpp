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

#include "draupnir/ui/widgets/NotificationTypeComboBox.h"

#include <QDebug>
#include <QEvent>

namespace Draupnir::Messages
{

NotificationTypeComboBox::NotificationTypeComboBox(QWidget* parent) :
    QComboBox{parent},
    m_currentValue{Notification::Type::None}
{
    for (auto notificationType : Notification::displayedValues) {
        addItem(
            Notification::toDisplayString(notificationType),
            QVariant::fromValue(notificationType)
        );
    }

    connect(this,  static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this,  &NotificationTypeComboBox::_onActivated);

    // The only UI strings displayed within this widget - are representation of the Notification::Type enum.
    // So we do not need to call within the consctructor a _retranslateUi() method.
}

void NotificationTypeComboBox::setNotificationTypeSelected(Notification::Type type)
{
    const auto index = _indexOf(type);
    if (index < 0) {
        // We should not receive here any other values than those which are part of Notification::displayedValues array
        // If we have this - probably somewhere something went wrong
        Q_ASSERT_X(false, "NotificationTypeComboBox::setNotificationTypeSelected",
                   "Some weird notification type. Probably Notification::displayedValues is outdated");
        return;
    }

    setCurrentIndex(index);
    emit activated(index);
}

void NotificationTypeComboBox::setNotificationType(Notification::Type type)
{
    const auto index = _indexOf(type);
    if (index < 0) {
        // We should not receive here any other values than those which are part of Notification::displayedValues array
        // If we have this - probably somewhere something went wrong
        Q_ASSERT_X(false, "NotificationTypeComboBox::setNotificationType",
                   "some weird notification type. Probably Notification::displayedValues is outdated");
        return;
    }

    if (m_currentValue == type)
        return;

    m_currentValue = type;
    setCurrentIndex(index);
}

void NotificationTypeComboBox::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QComboBox::changeEvent(event);
}

void NotificationTypeComboBox::_onActivated(int index)
{
    const auto activatedType = itemData(index).value<Notification::Type>();
    if (m_currentValue == activatedType)
        return;

    m_currentValue = activatedType;;
    emit notificationTypeSelected(m_currentValue);
}

void NotificationTypeComboBox::_retranslateUi()
{
    for (int i = 0; i < count(); i++) {
        const auto notificationType = itemData(i).value<Notification::Type>();
        setItemText(i, Notification::toDisplayString(notificationType));
    }
}

int NotificationTypeComboBox::_indexOf(Notification::Type type)
{
    for (auto i = 0; i < static_cast<int>(sizeof(Notification::displayedValues)); i++) {
        if (Notification::displayedValues[i] == type)
            return i;
    }

    return -1;
}

}; // namespace Draupnir::Messages
