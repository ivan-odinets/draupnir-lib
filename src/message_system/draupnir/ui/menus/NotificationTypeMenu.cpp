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

#include "draupnir/message_system/ui/menus/NotificationTypeMenu.h"

#include <QAction>
#include <QActionGroup>
#include <QEvent>

namespace Draupnir::Messages
{

NotificationTypeMenu::NotificationTypeMenu(QWidget *parent) :
    QMenu{parent},
    w_notificationActionGroup{new QActionGroup{this}},
    m_currentValue{Notification::Type::UnknownType}
{
    _setupUi();
}

NotificationTypeMenu::NotificationTypeMenu(const QString& title,QWidget *parent) :
    QMenu{title,parent},
    w_notificationActionGroup{new QActionGroup{this}},
    m_currentValue{Notification::Type::UnknownType}
{
    _setupUi();
}

void NotificationTypeMenu::setNotificationType(Notification::Type newStatus)
{
    if (newStatus == m_currentValue)
        return;

    QAction* action = getActionFor(newStatus);
    if (action  == nullptr)
        return;

    m_currentValue = newStatus;
    action->setChecked(true);
}

QAction* NotificationTypeMenu::getActionFor(Notification::Type type)
{
    for (const auto action : w_notificationActionGroup->actions()) {
        if (action->data().value<Notification::Type>() == type)
            return action;
    }

    return nullptr;
}

void NotificationTypeMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QMenu::changeEvent(event);
}

void NotificationTypeMenu::_onNotificationActionSelected(QAction* action)
{
    Q_ASSERT_X(action != nullptr, "NotificationTypeMenu::_onNotificationActionSelected",
               "Specified QAction is nullptr. This slot should be called upon emission of the QActionGroup::triggered "\
               "signal");
    Q_ASSERT_X(qobject_cast<QActionGroup*>(sender()),"NotificationTypeMenu::_onNotificationActionSelected",
               "sender() method returned something but not QActionGroup. This slot should be called upon emission of the "\
               "QActionGroup::triggered signal");

    const auto selectedValue = action->data().value<Notification::Type>();
    if (selectedValue != m_currentValue) {
        m_currentValue = selectedValue;
        emit notificationTypeChanged(action->data().value<Notification::Type>());
    }
}

void NotificationTypeMenu::_setupUi()
{
    w_notificationActionGroup->setExclusive(true);

    for (const auto type : Notification::displayedValues) {
        QAction* typeAction = QMenu::addAction(Notification::toDisplayString(type));
        typeAction->setCheckable(true);
        typeAction->setData(QVariant::fromValue<Draupnir::Messages::Notification::Type>(type));
        w_notificationActionGroup->addAction(typeAction);
    }

    connect(w_notificationActionGroup, &QActionGroup::triggered,
            this, &NotificationTypeMenu::_onNotificationActionSelected);
}

void NotificationTypeMenu::_retranslateUi()
{
    for (auto action : w_notificationActionGroup->actions()) {
        action->setText(
            Notification::toDisplayString(action->data().value<Notification::Type>())
        );
    }
}

}; // namespace Draupnir::Messages
