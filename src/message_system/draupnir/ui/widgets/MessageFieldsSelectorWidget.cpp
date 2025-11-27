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

#include "draupnir/ui/widgets/MessageFieldsSelectorWidget.h"

#include <QEvent>
#include <QFrame>
#include <QVBoxLayout>

namespace Draupnir::Messages
{

MessageFieldsSelectorWidget::MessageFieldsSelectorWidget(QWidget* parent) :
    QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    // Add All QCheckBox
    mainLayout->addWidget(m_messageFieldsContainer.showAllUiElement());
    QFrame* w_separator = new QFrame;
    w_separator->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(w_separator);
    // Add other QCheckBoxes
    m_messageFieldsContainer.forEachUiElement([mainLayout](QCheckBox* checkbox){
        mainLayout->addWidget(checkbox);
    });
    //mainLayout->addStretch();
    setLayout(mainLayout);

    // Setup UI-strings
    m_messageFieldsContainer.retranslateUiElements();

    // Set callback
    m_messageFieldsContainer.onElementInteracted([this](Message::Fields field,bool isChecked){
        emit messageFieldVisibilityChanged(field,isChecked);
    });
}

void MessageFieldsSelectorWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        m_messageFieldsContainer.retranslateUiElements();

    QWidget::changeEvent(event);
}

}; // namespace Draupnir::Messages
