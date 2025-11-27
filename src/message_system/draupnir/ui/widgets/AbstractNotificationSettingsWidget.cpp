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

#include "draupnir/ui/widgets/AbstractNotificationSettingsWidget.h"

#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "draupnir/core/AbstractMessageHandler.h"

namespace Draupnir::Messages
{

AbstractNotificationSettingsWidget::AbstractNotificationSettingsWidget(QWidget *parent) :
    QWidget{parent},
    w_testLabel{new QLabel},
    w_testNotiticationSelector{new NotificationTypeComboBox},
    w_testButton{new QPushButton},
    p_notificationTypesLayout{new QFormLayout}
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* testLayout = new QHBoxLayout;
    testLayout->addWidget(w_testLabel);
    testLayout->addWidget(w_testNotiticationSelector);
    testLayout->addWidget(w_testButton);
    mainLayout->addLayout(testLayout);

    QFrame* w_separator = new QFrame;
    w_separator->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(w_separator);

    mainLayout->addLayout(p_notificationTypesLayout);
    setLayout(mainLayout);

    _retranslateUi();

    connect(w_testButton, &QPushButton::clicked,
            this, &AbstractNotificationSettingsWidget::_onShowDummyClicked);
}

void AbstractNotificationSettingsWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        _retranslateUi();

        _retranslateTypeLabels();
    }

    QWidget::changeEvent(event);
}

void AbstractNotificationSettingsWidget::addRow(QLabel* label, NotificationTypeComboBox* selector)
{
    p_notificationTypesLayout->addRow(label,selector);
}

void AbstractNotificationSettingsWidget::_onShowDummyClicked()
{
    handler()->showDummy(w_testNotiticationSelector->notificationType());
}

void AbstractNotificationSettingsWidget::_retranslateUi()
{
    w_testLabel->setText(tr("Test"));
    w_testButton->setText(tr("Show"));
}

}; // namespace Draupnir::Messages
