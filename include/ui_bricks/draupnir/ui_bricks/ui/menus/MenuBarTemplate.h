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

#ifndef MENUBARTEMPLATE_H
#define MENUBARTEMPLATE_H

#include <QMenuBar>

#include <QEvent>

#include "draupnir/ui_bricks/core/MenuEntriesContainer.h"

namespace Draupnir::Ui {

/*! @class MenuBarTemplate draupnir/ui_bricks/ui/menus/MenuBarTemplate.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Strongly-typed, compile-time generic menu bar for Qt applications.
 *  @details MenuBarTemplate is a variadic-template class designed to automate, unify, and strongly-type the creation, translation,
 *           and access of complex menu bar structures in Qt. It aggregates menu entries (QMenu, QAction, or their descendants),
 *           manages their lifecycle, and provides convenient API for compile-time and runtime access to all entries.
 *  @tparam Entries... Variadic parameter pack of menu entry traits/classes, each describing a QMenu, QAction, or descendant type. */

template<class... Entries>
class MenuBarTemplate final :
    public QMenuBar,
    public MenuEntriesContainer<Entries...>
{
public:
    using FirstSubmenuEntries = typename MenuEntriesContainer<Entries...>::EntriesList::template filter_if_t<MenuEntry::IsMenuTemplateEntryAdapter>;

    using FirstActionsEntries = typename MenuEntriesContainer<Entries...>::EntriesList::template filter_if_t<MenuEntry::IsActionEntryAdapter>;

    /*! @brief Constructs a menu bar with the given parent and automatically populates itself with the entries described by Entries...
     *  @param parent Optional parent widget. */
    explicit MenuBarTemplate(QWidget* parent = nullptr) :
        QMenuBar{parent},
        MenuEntriesContainer<Entries...>{}
    {
        MenuEntriesContainer<Entries...>::populateUiElement(this);
    }

    /*! @brief Trivial final destructor. */
    ~MenuBarTemplate() final = default;

    /*! @brief Returns a tuple of pointers to all UI elements for which exposed `Type` is `QAction` (shallow, no recursion).
     *  @return std::tuple<...> containing pointers to matched UI elements, in the same order as `Entries...`
     *  @details Each matched element is returned as a pointer. Non-matching entries contribute an empty tuple.
     * @note This does NOT traverse nested MenuTemplate entries.*/
    auto getFirstActions() { return MenuEntriesContainer<Entries...>::template getAllUiElementsIf<MenuEntry::IsActionEntryAdapter>(); }

    /*! @brief Returns a tuple of pointers to all UI elements for which exposed `Type` is @ref Draupnir::Ui::MenuTemplate instantiation
     *         (shallow, no recursion).
     *  @return std::tuple<...> containing pointers to matched UI elements, in the same order as `Entries...`
     *  @details Each matched element is returned as a pointer. Non-matching entries contribute an empty tuple.
     * @note This does NOT traverse nested MenuTemplate entries.*/
    auto getFirstSubmenus() { return MenuEntriesContainer<Entries...>::template getAllUiElementsIf<MenuEntry::IsMenuTemplateEntryAdapter>(); }

protected:
    /*! @brief Qt event handler, automatically retranslates all entry texts when language changes.
     *  @param event The event pointer. */
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange)
            MenuEntriesContainer<Entries...>::retranslateEntries();

        QMenuBar::changeEvent(event);
    }
};

}; // namespace Draupnir::Ui

#endif // MENUBARTEMPLATE_H
