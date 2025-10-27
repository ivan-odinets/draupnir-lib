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

#ifndef CUSTOMMENUENTRYTRAIT_H
#define CUSTOMMENUENTRYTRAIT_H

#include <QString>

class QWidget;

namespace Draupnir::Menus {

/*! @class CustomMenuEntryTrait draupnir/traits/entries/CustomMenuEntryTrait.h
 *  @ingroup MenuTemplates
 *  @brief Trait for describing a menu entry that uses a custom menu class. Provides static creation of the custom menu,
 *         passing displayName() and parent as arguments.
 *  @tparam displayNameImpl A callable returning a QString (used for the menu's title).
 *  @tparam CustomMenu The custom menu class type (should have a constructor taking QString). */

template<auto displayNameImpl, class CustomMenu>
class CustomMenuEntryTrait
{
public:
    using Type = CustomMenu;

    /*! @brief Creates a new instance of the custom menu with the display name.
     *  @param parent Optional parent widget.
     *  @return Pointer to newly created CustomMenu. */
    static CustomMenu* createElement(QWidget* parent = nullptr) {
        return new CustomMenu{displayName(), parent};
    }

    /*! @brief Returns the display name for this entry.
     *  @return Localized QString. */
    static QString displayName() {
        return displayNameImpl();
    }
};

}; // namespace Draupnir::Menus

#endif // CUSTOMMENUENTRYTRAIT_H
