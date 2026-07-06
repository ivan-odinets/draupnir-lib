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

#ifndef ABSTRACTMESSAGECATEGORIESSELECTORMENU_H
#define ABSTRACTMESSAGECATEGORIESSELECTORMENU_H

#include <QMenu>

#include "draupnir/logging/messages/categories/MessageCategories.h"

namespace Draupnir::Logging
{

/*! @class AbstractMessageCategoriesSelectorMenu include/draupnir/logging/ui/menus/AbstractMessageCategoriesSelectorMenu.h
 *  @ingroup Logging
 *  @brief This is a class.
 * @todo Documentation: Write reasonable documentation for this class. */

class AbstractMessageCategoriesSelectorMenu : public QMenu
{
    Q_OBJECT
public:
    AbstractMessageCategoriesSelectorMenu(QWidget* parent = nullptr) :
        QMenu{parent}
    {}

    AbstractMessageCategoriesSelectorMenu(const QString& title, QWidget* parent = nullptr) :
        QMenu{title, parent}
    {}

    ~AbstractMessageCategoriesSelectorMenu() override = default;

signals:
    void flagSelectionChanged(MessageCategory category, bool isChecked);
};

}; // namespace Draupnir::Logging

#endif // ABSTRACTMESSAGECATEGORIESSELECTORMENU_H
