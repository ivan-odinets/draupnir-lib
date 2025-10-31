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
 *  @tparam Entries... Variadic parameter pack of menu entry traits/classes, each describing a QMenu, QAction, or descendant type. */

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

    /*! @brief Static method which returns the number of traits within this @ref MenuBarTemplate instantiation.
     *  @return Number of elements as constexpr int. */
    static constexpr int count() { return MenuEntriesContainer<Entries...>::count(); }

    /*! @brief Static constexpr field containing the number of traits within this @ref MenuBarTemplate instantiation. */
    static constexpr int count_v = count();

    /*! @brief Runtime method which returns the number of traits within this @ref MenuBarTemplate instantiation.
     *  @return Number of elements as constexpr int (always equals count()). */
    constexpr int instanceCount() const { return count(); }

    /*! @brief Returns true if specified entry is present within this @ref MenuBarTemplate instantiation.
     *  @tparam Entry - menu entry trait to be checked. */
    template<class Entry>
    static constexpr bool contains() { return MenuEntriesContainer<Entries...>::template contains<Entry>(); }

    /*! @brief Static constexpr template variable containing `true` if this @ref MenuBarTemplate instantiation contains
     *         the specified Entry.
     *  @tparam Entry - menu entry trait to be checked. */
    template<class Entry>
    static constexpr bool contains_v = contains<Entry>();

    /*! @brief Runtime method which returns `true` if the specified Entry is present within this @ref MenuBarTemplate.
     *  @tparam Entry - menu entry trait to be checked. */
    template<class Entry>
    constexpr bool instanceContains() { return contains<Entry>(); }

    /*! @brief Provides access to the entry at the specified compile-time index.
     *  @tparam Index Compile-time index (0-based).
     *  @return Pointer to the element at the specified index.
     * @note Throws static_assert if Index is out of bounds. */
    template<std::size_t Index>
    auto get() {
        static_assert(Index < sizeof...(Entries), "Index is out of bounds in MenuBarTemplate.");
        return m_container.template get<Index>();
    }
    template<std::size_t Index>
    auto get() const {
        static_assert(Index < sizeof...(Entries), "Index is out of bounds in MenuBarTemplate.");
        return m_container.template get<Index>();
    }

    /*! @brief Provides access to the entry matching the specified Entry trait.
     *  @tparam Entry The trait class of the desired entry.
     *  @return Pointer to the element matching Entry.
     * @note Throws static_assert if Entry is not present in Entries... */
    template<class Entry>
    auto get() {
        static_assert(contains<Entry>(), "Entry specified is not present within this MenuBarTemplate.");
        return m_container.template get<Entry>();
    }
    template<class Entry>
    auto get() const {
        static_assert(contains<Entry>(), "Entry specified is not present within this MenuBarTemplate.");
        return m_container.template get<Entry>();
    }

    /*! @brief Allows connecting to the QAction-based Entry triggered signal. */
    template<class Entry, class... Args>
    QMetaObject::Connection on(Args... args) {
        static_assert(contains<Entry>(), "Entry specified is not present within this MenuBarTemplate.");
        return m_container.template on<Entry,Args...>(args...);
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
