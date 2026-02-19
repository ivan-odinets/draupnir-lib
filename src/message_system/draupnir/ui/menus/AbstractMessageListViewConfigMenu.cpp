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

#include "draupnir/message_system/ui/menus/AbstractMessageListViewConfigMenu.h"

#include <QCheckBox>
#include <QEvent>
#include <QWidgetAction>

namespace Draupnir::Messages
{

AbstractMessageListViewConfigMenu::AbstractMessageListViewConfigMenu(QWidget* parent) :
    QMenu{parent},
    w_messageFiledsSection{nullptr},
    w_messageTypesSection{nullptr}
{
    // Create section with selectors of specific Message::Fields
    w_messageFiledsSection = addSection(QString{});
    // Add All element
    addAction(m_messageFieldsContainer.showAllUiElement());
    addSeparator();
    // Populate individual fields selector actions
    m_messageFieldsContainer.forEachUiElement([this](QAction* action){
        addAction(action);
    });
    // Set callbacks
    m_messageFieldsContainer.onElementInteracted([this](MessageField field, bool isChecked){
        emit messageFieldVisibilityChanged(field,isChecked);
    });
    // Prepeare section with selectors of specific Message types. Implemented within the MessageListViewConfigTemplate
    w_messageTypesSection = addSection(QString{});

    _retranslateUi();
}

void AbstractMessageListViewConfigMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        _retranslateUi();

        retranslateMessageTypeActions();
    }
}

void AbstractMessageListViewConfigMenu::addMessageTypeAction(QAction* action)
{
    addAction(action);
}

void AbstractMessageListViewConfigMenu::_retranslateUi()
{
    w_messageFiledsSection->setText(tr("Fields:"));
    m_messageFieldsContainer.retranslateUiElements();
    w_messageTypesSection->setText(tr("Messages Shown:"));
}

}; // namespace Draupnir::Messages
