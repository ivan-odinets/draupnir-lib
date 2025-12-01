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

#ifndef MENUTEMPLATEENTRY_H
#define MENUTEMPLATEENTRY_H

#include <QMenu>

namespace Draupnir::Ui {

template<class... Entries>
class MenuTemplate;

/*! @class MenuTemplateEntry draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for a nested sub-menu described by a MenuTemplate with compile-time entries. Provides static creation of
 *         the menu template (as a QMenu subclass) with the given entries.
 *  @tparam displayNameFunction Callable returning a QString (used as menu title).
 *  @tparam Entries...  Variadic trait parameter pack describing menu entries.
 *
 * @todo Write reasonable documentation and example of tha usage of this template.
 * @todo Add a test to check this functionality. */

template<auto displayNameFunction, class... Entries>
class MenuTemplateEntry
{
public:
    using Type = MenuTemplate<Entries...>;

    /*! @brief Creates a new MenuTemplate instance (as `QMenu`) with the given entries and title.
     *  @param parent Optional parent widget.
     *  @return Pointer to the newly created MenuTemplate<Entries...>. */
    static MenuTemplate<Entries...>* createElement(QWidget* parent = nullptr) {
        return new MenuTemplate<Entries...>{displayName(),parent};
    }

    /*! @brief Returns the display name for the menu template.
     *  @return Localized `QString`. */
    static QString displayName() {
        return displayNameFunction();
    }
};

}; // namespace Draupnir::Ui

#endif // MENUTEMPLATEENTRY_H
