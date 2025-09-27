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

#ifndef MENUTEMPLATE_H
#define MENUTEMPLATE_H

#include <QMenu>

#include <QEvent>

#include "draupnir/core/MenuEntriesContainer.h"

namespace Draupnir::Menus {

/*! @class MenuTemplate draupnir/ui/MenuTemplate.h
 *  @brief Strongly-typed, compile-time generic menu class for Qt applications.
 *  @details MenuTemplate is a variadic-template class designed to automate and unify creation, translation, and access of complex
 *           menu structures in Qt. It aggregates menu entries (QMenu, QAction, or descendants) defined by Entry traits, manages
 *           their lifecycle, and provides strongly-typed API for convenient compile-time and runtime access. Typically used in
 *           conjunction with MenuEntriesContainer and menu trait system.
 *
 * @tparam Entries... Variadic parameter pack of menu entry traits/classes, each describing a QMenu/QAction type. */

template<class... Entries>
class MenuTemplate final : public QMenu
{
public:
    /*! @brief Constructs a menu with a empty title and parent. Automatically populates itself with the entries described by Entries...
     *  @param parent Optional parent widget. */
    explicit MenuTemplate(QWidget* parent = nullptr) :
        QMenu{parent}
    {
        m_container.populateUiElement(this);
    }

    /*! @brief Constructs a menu with a given title and parent. Automatically populates itself with the entries described by Entries...
     *  @param title The menu's title.
     *  @param parent Optional parent widget. */
    explicit MenuTemplate(const QString& title, QWidget* parent = nullptr) :
        QMenu{title, parent}
    {
        m_container.populateUiElement(this);
    }

    /*! @brief Static method returning number of elements in the menu.
     *  @return Number of entries as constexpr int. */
    static constexpr int staticCount() { return MenuEntriesContainer<Entries...>::staticCount(); }

    /*! @brief Runtime method which returns the number of elements in the menu.
     *  @return Number of entries (always equals staticCount()). */
    constexpr int count() const { return staticCount(); }

    template<class Entry>
    static constexpr bool contains() { return MenuEntriesContainer<Entries...>::template contains<Entry>(); }

    /*! @brief Provides access to the entry at a specific compile-time index.
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

    template<class Entry, class... Args>
    QMetaObject::Connection on(Args... args) {
        return m_container.template on<Entry,Args...>(args...);
    }

protected:
    template<class Implementation, class Menu>
    friend class MenuHandlerTemplate;

    MenuEntriesContainer<Entries...> m_container;

    /*! @brief Qt event handler, automatically retranslates all entry texts when language changes.
     *  @param event The event pointer. */
    void changeEvent(QEvent* event) override {
        if (event->type() == QEvent::LanguageChange) {
            m_container.retranslateEntries();
        }
        QMenu::changeEvent(event);
    }

private:

};

} // namespace Draupnir::Menus

#endif // MENUTEMPLATE_H
