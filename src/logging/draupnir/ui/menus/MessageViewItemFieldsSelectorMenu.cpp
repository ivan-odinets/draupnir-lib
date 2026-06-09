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

#include "draupnir/logging/ui/menus/MessageViewItemFieldsSelectorMenu.h"

#include <QEvent>

namespace Draupnir::Logging
{

MessageViewItemFieldsSelectorMenu::MessageViewItemFieldsSelectorMenu(QWidget* parent) :
    QMenu{parent},
    _Base{MessageViewItemFields::All}
{ _setupUi(); }

MessageViewItemFieldsSelectorMenu::MessageViewItemFieldsSelectorMenu(const QString& title, QWidget* parent) :
    QMenu{title, parent},
    _Base{MessageViewItemFields::All}
{ _setupUi(); }

void MessageViewItemFieldsSelectorMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _Base::retranslateUiElements();

    QMenu::event(event);
}

void MessageViewItemFieldsSelectorMenu::_setupUi()
{
    addAction(_Base::getFlagElement<MessageViewItemField::Brief>());
    addAction(_Base::getFlagElement<MessageViewItemField::What>());
    addAction(_Base::getFlagElement<MessageViewItemField::DateTime>());
    addAction(_Base::getFlagElement<MessageViewItemField::Icon>());
}

}; // namespace Draupnir::Logging
