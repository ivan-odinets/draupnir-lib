/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "ButtonsWidget.h"

#include <QHBoxLayout>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QPushButton>

#include "draupnir/Logger.h"

#include "traits/messages/MyMessageTrait.h"

ButtonsWidget::ButtonsWidget(QWidget *parent) :
    QWidget{parent},
    p_mainLayout{new QVBoxLayout}
{
    QHBoxLayout* logButtonsLayout = new QHBoxLayout;
    QPushButton* debugButton = new QPushButton{tr("Debug")};
    connect(debugButton, &QPushButton::clicked,[](){
        logger.logDebug(tr("Debug Brief"),tr("I am the Debug message."));
    });
    logButtonsLayout->addWidget(debugButton);
    QPushButton* infoButton = new QPushButton{tr("Info")};
    connect(infoButton, &QPushButton::clicked, [](){
        logger.logInfo(tr("Info Brief"),tr("I am the Info message."));
    });
    logButtonsLayout->addWidget(infoButton);
    QPushButton* warningButton = new QPushButton{tr("Warning")};
    connect(warningButton, &QPushButton::clicked, [](){
        logger.logWarning(tr("Warning Brief"),tr("I am the Warning message."));
    });
    logButtonsLayout->addWidget(warningButton);
    QPushButton* errorButton = new QPushButton{tr("Error")};
    connect(errorButton, &QPushButton::clicked, [](){
        logger.logError(tr("Error Brief"),tr("I am the Error message."));
    });
    logButtonsLayout->addWidget(errorButton);
    QPushButton* customButton = new QPushButton{tr("Custom")};
    connect(customButton, &QPushButton::clicked, [](){
        logger.logMessage<MyMessageTrait>(tr("Debug Custom"),tr("I am the Custom message."));
    });
    logButtonsLayout->addWidget(customButton);
    p_mainLayout->addLayout(logButtonsLayout);
    p_mainLayout->addStretch();
    setLayout(p_mainLayout);
}
