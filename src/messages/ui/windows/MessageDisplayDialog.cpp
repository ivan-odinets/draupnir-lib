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

#include "ui/windows/MessageDisplayDialog.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QEvent>
#include <QScrollArea>
#include <QVBoxLayout>

#include "ui/widgets/MessageDisplayWidget.h"

namespace Draupnir::Messages
{

MessageDisplayDialog::MessageDisplayDialog(QWidget* parent) :
    QDialog{parent},
    w_containerWidget{new QWidget},
    w_scrollableLayout{new QVBoxLayout},
    w_scrollArea{new QScrollArea},
    w_buttons{new QDialogButtonBox{QDialogButtonBox::Ok}}
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    w_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    w_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    w_scrollArea->setWidgetResizable(true);
    w_scrollableLayout->addStretch();
    w_containerWidget->setLayout(w_scrollableLayout);
    w_scrollArea->setWidget(w_containerWidget);
    mainLayout->addWidget(w_scrollArea);

    mainLayout->addWidget(w_buttons);

    setLayout(mainLayout);

    w_scrollArea->setFrameShape(QFrame::NoFrame);
    w_scrollArea->setStyleSheet("QScrollBar:vertical { width: 12px; }");

    connect(w_buttons,  &QDialogButtonBox::accepted,
            this,       &MessageDisplayDialog::accept);

}

MessageDisplayDialog::~MessageDisplayDialog()
{}

void MessageDisplayDialog::addMessage(Message* message)
{
    Q_ASSERT_X(message, "void MessageDisplayDialog::addMessage",
               "Provided Message* pointer is nullptr.");
    MessageDisplayWidget* widget = new MessageDisplayWidget;
    widget->showMessage(message);

    w_scrollableLayout->insertWidget(w_scrollableLayout->count()-1,widget); // w_scrollableLayout has stretch in the end.
                                                                            // So we need insert widget at one before last position
}

void MessageDisplayDialog::addMessageList(const QList<Message*>& messages)
{
    for (const auto message : messages)
        addMessage(message);
}

void MessageDisplayDialog::clear()
{
    QLayoutItem* item = nullptr;
    while ((item = w_scrollableLayout->takeAt(0))) {
        QWidget* widget = item->widget();
        if (widget)
            widget->deleteLater();
        delete item;
    }
    w_scrollableLayout->addStretch();
}

}; // namespace Draupnir::Messages
