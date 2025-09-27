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

#include "draupnir/ui/menus/MessageViewConfigMenu.h"

#include <QEvent>

namespace Draupnir::Messages
{

MessageViewConfigMenu::MessageViewConfigMenu(QWidget* parent) :
    QMenu{parent},
    w_messageTypesSection{addSection(QString{})}
{
    _retranslateUi();
}

void MessageViewConfigMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        _retranslateUi();

        retranslateTypesActions();
    }
}

void MessageViewConfigMenu::addMessageTypeAction(QAction* action)
{
    addAction(action);
}

void MessageViewConfigMenu::_retranslateUi()
{
    w_messageTypesSection->setText(tr("Messages Shown:"));
}

}; // namespace Draupnir::Messages
