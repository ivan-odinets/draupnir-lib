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

#include "draupnir/logging/ui/menus/MessageLevelsSelectorMenu.h"

#include <QEvent>

namespace Draupnir::Logging
{

MessageLevelsSelectorMenu::MessageLevelsSelectorMenu(QWidget* parent) :
    QMenu{parent},
    _Base{MessageLevels::All}
{ _setupUi(); }

MessageLevelsSelectorMenu::MessageLevelsSelectorMenu(const QString& title, QWidget* parent) :
    QMenu{title, parent},
    _Base{MessageLevels::All}
{ _setupUi(); }

void MessageLevelsSelectorMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _Base::retranslateUiElements();

    QMenu::changeEvent(event);
}

void MessageLevelsSelectorMenu::_setupUi()
{
    addAction(_Base::getMaskElement<MessageLevels::All>());
    addSeparator();
    addAction(_Base::getFlagElement<MessageLevel::Debug>());
    addAction(_Base::getFlagElement<MessageLevel::Info>());
    addAction(_Base::getFlagElement<MessageLevel::Warning>());
    addAction(_Base::getFlagElement<MessageLevel::Error>());
}

};
