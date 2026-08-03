/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/messages/ui/widgets/MessageDisplayWidget.h"

#include <QEvent>
#include <QFormLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QStyle>

#include "draupnir/messages/core/MessageViewItem.h"

static QSize defaultIconSize{64,64};

namespace Draupnir::Messages
{

MessageDisplayWidget::MessageDisplayWidget(QWidget *parent) :
    QWidget{parent},
    p_message{nullptr},
    m_iconSize{defaultIconSize},
    w_messageBriefNameLabel{      new QLabel},
    w_messageBriefLabel{          new QLabel},
    w_messageWhatNameLabel{       new QLabel},
    w_messageWhatLabel{           new QLabel},
    w_messageDateTimeNameLabel{   new QLabel},
    w_messageDateTimeLabel{       new QLabel},
    w_messageIconLabel{           new QLabel}
{
    QHBoxLayout* mainLayout = new QHBoxLayout;

    mainLayout->addWidget(w_messageIconLabel);

    QFormLayout* detailsLayout = new QFormLayout;
    detailsLayout->addRow(w_messageBriefNameLabel, w_messageBriefLabel);
    detailsLayout->addRow(w_messageWhatNameLabel, w_messageWhatLabel);
    detailsLayout->addRow(w_messageDateTimeNameLabel, w_messageDateTimeLabel);
    mainLayout->addLayout(detailsLayout);

    setLayout(mainLayout);

    QFont font = w_messageBriefLabel->font();
    font.setBold(true);
    w_messageBriefLabel->setFont(font);
    w_messageBriefLabel->setAlignment(Qt::AlignTop);

    w_messageWhatLabel->setWordWrap(true);
    w_messageWhatLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    w_messageWhatLabel->setAlignment(Qt::AlignTop);

    w_messageDateTimeLabel->setAlignment(Qt::AlignTop);

    _retranslateUi();
}

void MessageDisplayWidget::clear()
{
    w_messageBriefLabel->clear();
    w_messageWhatLabel->clear();
    w_messageDateTimeLabel->clear();

    w_messageIconLabel->setPixmap(QPixmap(m_iconSize));

    p_message = nullptr;
}

void MessageDisplayWidget::showMessage(const MessagePtr& message)
{
    p_message = message;
    _updateView();
}

void MessageDisplayWidget::showMessage(MessagePtr&& message)
{
    p_message = std::move(message);
    _updateView();
}

void MessageDisplayWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        _retranslateUi();
    }

    QWidget::changeEvent(event);
}

void MessageDisplayWidget::_updateView()
{
    Q_ASSERT(p_message);
    w_messageBriefLabel->setText(p_message->brief());
    w_messageWhatLabel->setText(p_message->what());
    w_messageDateTimeLabel->setText(p_message->dateTime().toString(Qt::TextDate));

    w_messageIconLabel->setPixmap(MessageViewItem::iconFor(p_message->type()).pixmap(m_iconSize));
}

void MessageDisplayWidget::_retranslateUi()
{
    w_messageBriefNameLabel->setText(tr("Brief:"));
    w_messageWhatNameLabel->setText(tr("What:"));
    w_messageDateTimeNameLabel->setText(tr("Date:"));
}

} // namespace Draupnir::Messages
