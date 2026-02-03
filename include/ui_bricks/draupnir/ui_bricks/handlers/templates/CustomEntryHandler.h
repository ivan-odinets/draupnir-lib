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

#ifndef CUSTOMENTRYHANDLER_H
#define CUSTOMENTRYHANDLER_H

namespace Draupnir::Handlers {

/*! @class CustomEntryHandler draupnir/ui_bricks/handlers/templates/CustomEntryHandler.h
 *  @ingroup UiBricks
 *  @brief CRTP base class for handling custom menu entries (MenuEntry::Type is not `QAction` but a custom widget/menu).
 *  @tparam Implementation The derived class implementing `connect(T*)`.
 *  @tparam MenuEntry The menu entry trait with `using Type = ... `(actual menu/widget type).
 *
 *  @details Stores a pointer to an instance of MenuEntry::Type and delegates connection/initialization to
 *           Implementation::connect(typename MenuEntry::Type*). */

template<class Implementation, class MenuEntry>
class CustomEntryHandler
{
public:
    /*! @brief Constructor. Initializes internal p_menu pointer with nullptr. */
    CustomEntryHandler() :
        p_menu{nullptr}
    {}

    /*! @brief Stores the menu/widget pointer and calls \c Implementation::connect(...).
     *  @param menu Object of type \c MenuEntry::Type for handling/connection. */
    void connect(typename MenuEntry::Type* menu) {
        p_menu = menu;

        static_cast<Implementation*>(this)->connectImplementation(menu);
    }

protected:
    /*! @brief Access the connected menu/widget.
     *  @return Stored pointer (may be nullptr before connect()). */
    typename MenuEntry::Type* menu() { return p_menu; }

private:
    typename MenuEntry::Type* p_menu;
};

}; // namespace Draupnir::Handlers

#endif // CUSTOMENTRYHANDLER_H
