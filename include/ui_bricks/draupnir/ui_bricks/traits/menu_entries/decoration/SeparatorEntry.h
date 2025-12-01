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

#ifndef SEPARATORENTRY_H
#define SEPARATORENTRY_H

#include <QAction>

namespace Draupnir::Ui {

/*! @class SeparatorEntry draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for the separator menu entry. Provides a `QAction` marked as a separator. */

class SeparatorEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a separator `QAction`.
     *  @param parent Optional parent object.
     *  @return Newly created `QAction` pointer with separator property set. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{parent};
        result->setSeparator(true);
        return result;
    }
};

}; // namespace Draupnir::Ui

#endif // SEPARATORENTRY_H
