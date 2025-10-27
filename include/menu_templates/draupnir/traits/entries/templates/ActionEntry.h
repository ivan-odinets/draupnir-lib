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

#ifndef ACTIONENTRY_H
#define ACTIONENTRY_H

#include <QAction>

namespace Draupnir::Menus {

/*! @class ActionEntry draupnir/traits/entries/templates/ActionEntry.h
 *  @ingroup MenuTemplates
 *  @brief Trait for the simple QAction entry. Provides a QAction with display name set.
 *  @tparam displayNameImpl - a callable returning QString. */

template<auto displayNameImpl>
class ActionEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction.
     *  @param parent Optional parent object.
     *  @return Newly created QAction pointer. */
    static QAction* createElement(QWidget* parent = nullptr) {
        return new QAction{displayName(), parent};
    }

    /*! @brief Returns display name for this QAction (from displayNameImpl).
     *  @return QAction title (QString). */
    static QString displayName() {
        return displayNameImpl();
    }
};

}; // namespace Draupnir::Menus

#endif // ACTIONENTRY_H
