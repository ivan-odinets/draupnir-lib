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

#ifndef MENUENTRIESCONTAINER_H
#define MENUENTRIESCONTAINER_H

#include <tuple>

#include <QAction>
#include <QDebug>
#include <QObject>

#include "draupnir/utils/type_presense.h"

class QMenu;

namespace Draupnir::Menus {

/*! @class MenuEntriesContainer draupnir/core/MenuEntriesContainer.h
 *  @ingroup MenuTemplates
 *  @brief Template-based container for GUI menu entries such as QMenu and QAction.
 *  @tparam Entries Variadic template parameter pack describing traits/classes for menu entries.
 *
 *  @details MenuEntriesContainer is a generic compile-time container that manages the creation, access, translation, and
 *           destruction of menu entry elements (QMenu/QAction or their descendants). It is designed to be used within
 *           MenuTemplate, MenuBarTemplate, or similar classes to provide DRY logic for nested menu structures. */

template<class... Entries>
class MenuEntriesContainer
{
    /*! @brief Checks that static QString Entry::displayName() is well-formed and returns QString. */
    template<class, class = std::void_t<>>
    struct has_displayName : std::false_type {};

    template<class Entry>
    struct has_displayName<
        Entry,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(Entry::displayName())>
        )>
    > : std::true_type {};

public:
    /*! @brief Constructor. Creates all menu elements specified in the Entries parameter pack. The elements are created
     *         via the Entry::createElement() static method. */
    MenuEntriesContainer() {
        _initEntriesImpl<0, Entries...>();
    }

    /*! @brief Destructor. Deletes all menu elements owned by this container. */
    ~MenuEntriesContainer() {
        std::apply([](auto&... element) {
                (..., delete element);
        }, m_elements);
    }

    /*! @brief Static method which returns the number of traits within this @ref MenuEntriesContainer instantiation.
     *  @return Number of elements as constexpr int. */
    static constexpr int count() { return sizeof...(Entries); }

    /*! @brief Static constexpr field containing the number of traits within this @ref MenuEntriesContainer instantiation. */
    static constexpr int count_v = count();

    /*! @brief Runtime method which returns the number of traits within this @ref MenuEntriesContainer instantiation.
     *  @return Number of elements as constexpr int (always equals count()). */
    constexpr int instanceCount() const { return count(); }

    /*! @brief Returns true if specified entry is present within this @ref MenuEntriesContainer instantiation.
     *  @tparam Entry - menu entry trait to be checked. */
    template<class Entry>
    static constexpr bool contains() { return draupnir::utils::is_one_of_v<Entry,Entries...>; }

    /*! @brief Static constexpr template variable containing `true` if this @ref MenuEntriesContainer instantiation contains
     *         the specified Entry.
     *  @tparam Entry - menu entry trait to be checked. */
    template<class Entry>
    static constexpr bool contains_v = contains<Entry>();

    /*! @brief Runtime method which returns `true` if the specified Entry is present within this @ref MenuEntriesContainer.
     *  @tparam Entry - menu entry trait to be checked. */
    template<class Entry>
    constexpr bool instanceContains() { return contains<Entry>(); }

    /*! @brief Returns a pointer to the element at the specified compile-time index.
     *  @tparam Index Index of the element to access (0-based).
     *  @return Pointer to the stored element. */
    template<std::size_t Index>
    auto get() {
        static_assert(Index < sizeof...(Entries), "Index is out of bounds in MenuEntriesContainer.");
        return std::get<Index>(m_elements);
    }

    /*! @brief Returns a pointer to the element matching the provided Entry trait.
     *  @tparam Entry Either the trait class describing the desired entry, or the type of actual element stored.
     *  @return Pointer to the stored element. */
    template<class Entry>
    auto get() {
        static_assert(contains<Entry>(), "Entry specified is not present within this MenuEntriesContainer.");
        return std::get<_getEntryIndex<0, Entry, Entries...>()>(m_elements);
    }

    /*! @brief Allows connecting to the QAction-based Entry triggered signal. */
    template<class Entry, class... Args>
    QMetaObject::Connection on(Args... args) {
        auto entry = get<Entry>();

        if constexpr (std::is_base_of_v<QAction, typename Entry::Type> || std::is_same_v<QAction, typename Entry::Type>) {
            return QObject::connect(entry, &QAction::triggered, args...);
        } else {
            static_assert(std::is_base_of_v<QAction, typename Entry::Type> || std::is_same_v<QAction, typename Entry::Type>,
                "Only QAction-based things are allowed within this method.");
            return QMetaObject::Connection{};
        }
    }

    /*! @brief Adds all elements stored in this container to the given Qt UI container (QMenu, QMenuBar, etc.).
     *         Correct addMenu/addAction is automatically chosen based on element type.
     *  @tparam UiElement Type of Qt container (e.g., QMenu, QMenuBar).
     *  @param dest Pointer to the UI container. */
    template<class UiElement>
    void populateUiElement(UiElement* dest) {
        _populateUiElementImpl<UiElement, 0, Entries...>(dest);
    }

    /*! @brief Retranslates (updates) display texts/titles for all elements in the container. Calls Entry::displayName()
     *         for each entry and sets appropriate title/text. */
    void retranslateEntries() {
        _retranslateEntriesImpl<0, Entries...>();
    }

private:
    std::tuple<
        std::add_pointer_t<typename Entries::Type>...
    > m_elements;

    template<std::size_t Index, class First, class... Rest>
    inline void _initEntriesImpl() {
        std::get<Index>(m_elements) = First::createElement();

        if constexpr (std::is_same_v<typename First::Type, QMenu> || std::is_base_of_v<QMenu, typename First::Type>) {
            std::get<Index>(m_elements)->setTitle(First::displayName());
        } else if constexpr (std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>) {
            if constexpr (has_displayName<First>::value)
                std::get<Index>(m_elements)->setText(First::displayName());
        } else {
            static_assert(
                std::is_same_v<typename First::Type, QMenu>   || std::is_base_of_v<QMenu, typename First::Type> ||
                std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>,
                "All Entries... must describe QMenu (or a derived class) or QAction (or a derived class)."
            );
        }

        if constexpr (sizeof...(Rest) > 0)
            _initEntriesImpl<Index+1, Rest...>();
    }

    template<std::size_t Index, class Entry, class First, class... Rest>
    static constexpr std::size_t _getEntryIndex() {
        if constexpr (std::is_same_v<Entry, First> || std::is_same_v<Entry, typename First::Type>) {
            return Index;
        } else if constexpr (sizeof...(Rest) > 0) {
            return _getEntryIndex<Index+1, Entry, Rest...>();
        } else {
            static_assert(sizeof...(Rest) > 0, "Provided Entry is not present within the Entries... parameter pack.");
            return -1;
        }
    }

    template<class UiElement, std::size_t Index, class First, class... Rest>
    inline void _populateUiElementImpl(UiElement* dest) {
        auto element = std::get<Index>(m_elements);

        if constexpr (std::is_same_v<typename First::Type, QMenu> || std::is_base_of_v<QMenu, typename First::Type>) {
            dest->addMenu(element);
        } else if constexpr (std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>) {
            dest->addAction(element);
        } else {
            static_assert(
                std::is_same_v<typename First::Type, QMenu>   || std::is_base_of_v<QMenu, typename First::Type> ||
                std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>,
                "All Entries... must describe QMenu (or a derived class) or QAction (or a derived class)."
            );
        }

        if constexpr (sizeof...(Rest) > 0)
            _populateUiElementImpl<UiElement, Index+1, Rest...>(dest);
    }

    template<std::size_t Index, class First, class... Rest>
    inline void _retranslateEntriesImpl() {
        if constexpr (std::is_same_v<typename First::Type, QMenu> || std::is_base_of_v<QMenu, typename First::Type>) {
            std::get<Index>(m_elements)->setTitle(First::displayName());
        } else if constexpr (std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>) {
            if constexpr (has_displayName<First>::value)
                std::get<Index>(m_elements)->setText(First::displayName());
        } else {
            static_assert(
                std::is_same_v<typename First::Type, QMenu>   || std::is_base_of_v<QMenu, typename First::Type> ||
                std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>,
                "All Entries... must describe QMenu (or a derived class) or QAction (or a derived class)."
            );
        }

        if constexpr (sizeof...(Rest) > 0)
            _retranslateEntriesImpl<Index+1, Rest...>();
    }
};

}; // namespace Draupnir::Menus

#endif // MENUENTRIESCONTAINER_H
