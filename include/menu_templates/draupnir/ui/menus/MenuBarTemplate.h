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

#ifndef MENUBARTEMPLATE_H
#define MENUBARTEMPLATE_H

#include <QMenuBar>
#include <QEvent>

#include "draupnir/core/MenuEntriesContainer.h"

namespace Draupnir::Menus {

/*! @class MenuBarTemplate draupnir/ui/menus/MenuBarTemplate.h
 *  @ingroup MenuTemplates
 *  @brief Strongly-typed, compile-time generic menu bar for Qt applications.
 *  @details MenuBarTemplate is a variadic-template class designed to automate, unify, and strongly-type the creation, translation,
 *           and access of complex menu bar structures in Qt. It aggregates menu entries (QMenu, QAction, or their descendants),
 *           manages their lifecycle, and provides convenient API for compile-time and runtime access to all entries.
 *  @tparam Entries... Variadic parameter pack of menu entry traits/classes, each describing a QMenu, QAction, or descendant type.
 *
 * @todo Add test for this class.
 * @todo Add constexpr variable versions of static constexpr methods. */

template<class... Entries>
class MenuBarTemplate final : public QMenuBar
{
public:
    /*! @brief Constructs a menu bar with the given parent and automatically populates itself with the entries described by Entries...
     *  @param parent Optional parent widget. */
    explicit MenuBarTemplate(QWidget* parent = nullptr) :
        QMenuBar{parent}
    {
        m_container.populateUiElement(this);
    }

    /*! @brief Final trivial destructor. */
    ~MenuBarTemplate() final = default;

    /*! @brief Static method which returns the number of menu entries in the menu bar.
     *  @return Number of entries as constexpr int. */
    static constexpr int staticCount() { return MenuEntriesContainer<Entries...>::staticCount(); }

    /*! @brief Runtime method which returns the runtime number of menu entries in the menu bar.
     *  @return Number of entries (always equals staticCount()). */
    constexpr int count() const { return staticCount(); }

    /*! @brief Method to check if this MenuTemplate instance contains the specified Entry.
     *  @tparam Entry - entry trait to be checked. */
    template<class Entry>
    static constexpr bool contains() { return MenuEntriesContainer<Entries...>::template contains<Entry>(); }

    /*! @brief Provides access to the entry at the specified compile-time index.
     *  @tparam Index Compile-time index (0-based).
     *  @return Pointer to the element at the specified index.
     * @note Throws static_assert if Index is out of bounds. */
    template<std::size_t Index>
    auto get() {
        return m_container.template get<Index>();
    }
    template<std::size_t Index>
    auto get() const {
        return m_container.template get<Index>();
    }

    /*! @brief Provides access to the entry matching the specified Entry trait.
     *  @tparam Entry The trait class of the desired entry.
     *  @return Pointer to the element matching Entry.
     * @note Throws static_assert if Entry is not present in Entries... */
    template<class Entry>
    auto get() {
        return m_container.template get<Entry>();
    }
    template<class Entry>
    auto get() const {
        return m_container.template get<Entry>();
    }

protected:
    MenuEntriesContainer<Entries...> m_container;

    /*! @brief Qt event handler, automatically retranslates all entry texts when language changes.
     *  @param event The event pointer. */
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange) {
            m_container.retranslateEntries();
        }
        QMenuBar::changeEvent(event);
    }
};

}; // namespace Draupnir::Menus

#endif // MENUBARTEMPLATE_H
