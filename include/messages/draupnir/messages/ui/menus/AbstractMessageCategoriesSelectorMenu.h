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

#include "draupnir/messages/categories/MessageCategories.h"

namespace Draupnir::Messages
{

/*! @class AbstractMessageCategoriesSelectorMenu draupnir/messages/ui/menus/AbstractMessageCategoriesSelectorMenu.h
 *  @ingroup Messages
 *  @brief Base menu interface for selecting message categories.
 *
 *  @details Provides the common Qt signal emitted when the selection state of a message category changes. */

class AbstractMessageCategoriesSelectorMenu : public QMenu
{
    Q_OBJECT
public:
    /*! @brief Constructs an empty category selector menu.
     *  @param parent Optional pointer to the parent `QWidget`. */
    AbstractMessageCategoriesSelectorMenu(QWidget* parent = nullptr) :
        QMenu{parent}
    {}

    /*! @brief Constructs a category selector menu with a title.
     *  @param title Menu title.
     *  @param parent Optional pointer to the parent `QWidget`. */
    AbstractMessageCategoriesSelectorMenu(const QString& title, QWidget* parent = nullptr) :
        QMenu{title, parent}
    {}

    ~AbstractMessageCategoriesSelectorMenu() override = default;

signals:
    /*! @brief Emitted when a category selection state changes.
     *  @param category Category whose state changed.
     *  @param isChecked New selection state. */
    void flagSelectionChanged(MessageCategory category, bool isChecked);
};

} // namespace Draupnir::Messages

#endif // ABSTRACTMESSAGECATEGORIESSELECTORMENU_H
