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

#include "NotificationTypeMenu.h"

#include <QAction>
#include <QActionGroup>
#include <QEvent>

NotificationTypeMenu::NotificationTypeMenu(QWidget *parent) :
    QMenu{parent},
    w_notificationActionGroup{new QActionGroup{this}}
{
    _setupUi();
}

NotificationTypeMenu::NotificationTypeMenu(const QString& title,QWidget *parent) :
    QMenu{title,parent},
    w_notificationActionGroup{new QActionGroup{this}}
{
    _setupUi();
}

void NotificationTypeMenu::setNotificationType(Notification::Type newStatus)
{
    for (const auto action : w_notificationActionGroup->actions()) {
        if (action->data().value<Notification::Type>() == newStatus) {
            action->setChecked(true);
            return;
        }
    }
}

Notification::Type NotificationTypeMenu::selectedNotificationType() const
{
    const QAction* const selectedAction = w_notificationActionGroup->checkedAction();
    return (selectedAction != nullptr) ?
                selectedAction->data().value<Notification::Type>() :
                Notification::UnknownType;

}

void NotificationTypeMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QMenu::changeEvent(event);
}

void NotificationTypeMenu::_notificationActionSelected(QAction* action)
{
    Q_ASSERT(action != nullptr);

    emit notificationTypeChanged(action->data().value<Notification::Type>());
}

void NotificationTypeMenu::_setupUi()
{
    w_notificationActionGroup->setExclusive(true);

    for (const auto type : Notification::displayedValues) {
        QAction* typeAction = QMenu::addAction(Notification::toDisplayString(type));
        typeAction->setCheckable(true);
        typeAction->setData(QVariant::fromValue<Notification::Type>(type));
        w_notificationActionGroup->addAction(typeAction);
    }

    connect(w_notificationActionGroup,&QActionGroup::triggered,this,&NotificationTypeMenu::_notificationActionSelected);
}


void NotificationTypeMenu::_retranslateUi()
{
    for (auto action : w_notificationActionGroup->actions()) {
        action->setText(
            Notification::toDisplayString(action->data().value<Notification::Type>())
        );
    }
}
