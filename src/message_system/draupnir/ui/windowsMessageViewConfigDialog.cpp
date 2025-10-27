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

#include "draupnir/ui/windows/MessageViewConfigDialog.h"

#include <QDialogButtonBox>
#include <QCheckBox>
#include <QVBoxLayout>

namespace Draupnir::Messages
{

MessageViewConfigDialog::MessageViewConfigDialog(QWidget* parent)  :
    QDialog{parent},
    p_typeSelectorLayout{new QVBoxLayout},
    p_buttons{new QDialogButtonBox{QDialogButtonBox::Ok|QDialogButtonBox::Close}}
{
    QVBoxLayout* p_mainLayout = new QVBoxLayout;
    p_mainLayout->addLayout(p_typeSelectorLayout);
    p_mainLayout->addWidget(p_buttons);
    setLayout(p_mainLayout);

    connect(p_buttons, &QDialogButtonBox::accepted, this, &MessageViewConfigDialog::accept);
    connect(p_buttons, &QDialogButtonBox::rejected, this, &MessageViewConfigDialog::reject);

}

void MessageViewConfigDialog::addTypeVisibilityCheckBox(QCheckBox* checkBox)
{
    p_typeSelectorLayout->addWidget(checkBox);
}

}; // namespace Draupnir::Messages
