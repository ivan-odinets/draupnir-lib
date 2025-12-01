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

#ifndef CHECKABLEENTRY_H
#define CHECKABLEENTRY_H

#include <QAction>

namespace Draupnir::Ui {

/*! @class CheckableEntry draupnir/ui_bricks/traits/menu_entries/templates/CheckableEntry.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for the checkable `QAction` entry. Provides a `QAction` with setCheckable(true).
 *  @tparam displayNameImpl - a callable returning `QString`. */

template<auto displayNameImpl>
class CheckableEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a checkable `QAction`.
     *  @param parent Optional parent object.
     *  @return Newly created `QAction` pointer with checkable property set. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(),parent};
        result->setCheckable(true);
        return result;
    }

    /*! @brief Returns display name for this `QAction` (from displayNameImpl).
     *  @return `QAction` title (`QString`). */
    static QString displayName() {
        return displayNameImpl();
    }
};

}; // namespace Draupnir::Ui

#endif // CHECKABLEENTRY_H
