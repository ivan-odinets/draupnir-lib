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

#include "draupnir/message_system/ui/windows/MessageSystemConfigDialog.h"

#include <QPushButton>

#include "draupnir/message_system/core/AbstractMessageUiBuilder.h"

namespace Draupnir::Messages
{

MessageSystemConfigDialog::MessageSystemConfigDialog(AbstractMessageUiBuilder* uiBuilder,QWidget* parent) :
    QDialog{parent},
    p_uiBuilder{uiBuilder},
    w_mainWidget{new CentralWidget{}},
    p_buttons{new QDialogButtonBox{QDialogButtonBox::Ok}}
{
    w_mainWidget->setWidgetForTrait<NotificationSettingsEditorTrait>(
        p_uiBuilder->createNotificationSettingsWidgetAsInterface());
    w_mainWidget->setWidgetForTrait<MessageTypesSelectorTrait>(
        p_uiBuilder->createAbstractMessageTypesSelectorWidget());

    // Setup dialog layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(w_mainWidget);
    mainLayout->addWidget(p_buttons);
    setLayout(mainLayout);

    // Connect signals from AbstractNotificationSettingsWidget
    connect(w_mainWidget->getWidget<AbstractNotificationSettingsWidget>(),&AbstractNotificationSettingsWidget::messageNotificationTypeChanged,
            this, &MessageSystemConfigDialog::messageNotificationTypeChanged);

    // Connect signals from AbstractMessageTypesSelectorWidget
    connect(w_mainWidget->getWidget<AbstractMessageTypesSelectorWidget>(), &AbstractMessageTypesSelectorWidget::messageTypeVisibilityChanged,
            this, &MessageSystemConfigDialog::messageTypeVisibilityChanged);

    // Connect signals from MessageFieldsSelectorWidget
    connect(w_mainWidget->getWidget<MessageFieldsSelectorWidget>(), &MessageFieldsSelectorWidget::messageFieldVisibilityChanged,
            this, &MessageSystemConfigDialog::messageFieldVisibilityChanged);

    // Connect signals from QDialogButtonBox
    connect(p_buttons, &QDialogButtonBox::accepted,
            this, &MessageSystemConfigDialog::accept);
}

}; // namespace Draupnir::Messages
