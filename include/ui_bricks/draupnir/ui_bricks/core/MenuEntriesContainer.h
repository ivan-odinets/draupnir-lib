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

#ifndef MENUENTRIESCONTAINER_H
#define MENUENTRIESCONTAINER_H

#include <tuple>

#include <QAction>
#include <QDebug>
#include <QObject>

#include "draupnir/ui_bricks/concepts/MenuEntryConcept.h"
#include "draupnir/utils/type_presense.h"
#include "draupnir/utils/type_list.h"

class QMenu;

namespace Draupnir::Ui {

template<class...>
class MenuTemplate;

/*! @class MenuEntriesContainer draupnir/ui_bricks/core/MenuEntriesContainer.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Template-based container for GUI menu entries such as `QMenu` and `QAction`.
 *  @tparam Entries Variadic template parameter pack describing traits/classes for menu entries.
 *
 *  @details MenuEntriesContainer is a generic compile-time container that manages the creation, access, translation, and
 *           destruction of menu entry elements (QMenu/QAction or their descendants). It is designed to be used within
 *           MenuTemplate, MenuBarTemplate, or similar classes to provide DRY logic for nested menu structures.
 * @todo Improve hanling of edge-cases
 * @todo Standartize static_assert messages. */

template<MenuEntryConcept... Entries>
class MenuEntriesContainer
{
public:
    /*! @brief Compile-time list of entry trait types stored in this @ref MenuEntriesContainer.
     *  @details Convenience alias for metaprogramming over the `Entries...` pack. Equivalent to `draupnir::utils::type_list<Entries...>`. */
    using EntriesList = draupnir::utils::type_list<Entries...>;

    /*! @brief Constructor. Creates all menu elements specified in the Entries parameter pack. The elements are created
     *         via the Entry::createElement() static method. */
    MenuEntriesContainer() {
        _initUiElementsImpl<0, Entries...>();
    }

    /*! @brief Destructor. Deletes all menu elements owned by this container. */
    ~MenuEntriesContainer() {
        std::apply([](auto&... element) {
            (..., delete element);
        }, m_elements);
    }

    /*! @brief Returns the number of entry traits in this container (shallow, no recursion).
     *  @return Count of Entries... as constexpr integer. */
    static constexpr int entriesCount() { return sizeof...(Entries); }

    /*! @brief Compile-time constant: same as entriesCount(). */
    static constexpr int entriesCount_v = entriesCount();

    /*! @brief Returns the number of occurrences of the given Entry trait in this container and in all nested `MenuTemplate`-based
     *         entries.
     *  @tparam Entry Entry trait type to count.
     *  @details Traverses Entries... in order. If an entry's Type is a MenuTemplate<...> instantiation, the nested template is
     *           queried recursively. */
    template<MenuEntryConcept Entry>
    static constexpr int recursiveEntriesCount() { return _recursiveEntryCountImpl<Entry,Entries...>(); }

    /*! @brief Compile-time constant: same as recursiveEntriesCount<Entry>(). */
    template<MenuEntryConcept Entry>
    static constexpr int recursiveEntriesCount_v = recursiveEntriesCount<Entry>();

    /*! @brief Returns `true` if this container directly contains the given `Entry` trait (shallow check).
     *  @tparam Entry Entry trait type to check.
     * @note This does NOT search inside nested `MenuTemplate`-based entries. Use recursiveContains for that. */
    template<MenuEntryConcept Entry>
    static constexpr bool contains() { return draupnir::utils::is_one_of_v<Entry,Entries...>; }

    /*! @brief Compile-time constant: same as contains<Entry>(). */
    template<MenuEntryConcept Entry>
    static constexpr bool contains_v = contains<Entry>();

    /*! @brief Returns `true` if this container directly contains the instantiation of the given `EntryTemplate` trait (shallow check).
     *  @tparam EntryTemplate Template from which Entry was instantiated. */
    template<template<class...> class EntryTemplate>
    static constexpr bool containsEntryTemplate() { return draupnir::utils::is_template_instantiation_present_v<EntryTemplate,Entries...>; }

    /*! @brief Returns `true` if this container directly contains the instantiation of the given `EntryTemplate` trait (shallow check).
     *  @tparam EntryTemplate Template from which Entry was instantiated. */
    template<template<auto,class...> class EntryTemplate>
    static constexpr bool containsEntryTemplate() { return draupnir::utils::is_a1tp_instantiation_present_v<EntryTemplate,Entries...>; }

    /*! @brief Returns `true` if the given Entry trait is present either in this container or in any nested `MenuTemplate`-based
     *         entries.
     *  @tparam Entry Entry trait type to check.
     *  @details If an entry's `Type` is a `MenuTemplate<...>` instantiation, the nested template is queried recursively. */
    template<MenuEntryConcept Entry>
    static constexpr bool recursiveContains() { return _containsRecursiveImpl<Entry,Entries...>(); }

    /*! @brief Compile-time constant: same as recursiveContains<Entry>(). */
    template<MenuEntryConcept Entry>
    static constexpr bool recursiveContains_v = recursiveContains<Entry>();

    /*! @brief Returns a pointer to the UI element at the specified compile-time index.
     *  @tparam Index Zero-based index into Entries...
     *  @return Pointer to the stored UI element (QAction*, QMenu*, or a derived type pointer). */
    template<std::size_t Index>
    auto getUiElement() {
        static_assert(Index < sizeof...(Entries), "Index is out of bounds in MenuEntriesContainer.");
        return std::get<Index>(m_elements);
    }

    /*! @brief Returns a pointer to the UI element created for the specified `Entry` trait (shallow lookup).
     *  @tparam Entry Entry trait type from Entries...
     *  @return Pointer to the stored UI element of type `typename Entry::Type*`.
     *  @note This performs a shallow lookup only within this container instance. */
    template<MenuEntryConcept Entry>
    auto getUiElement() {
        static_assert(contains<Entry>(), "Entry specified is not present within this MenuEntriesContainer.");
        return std::get<_getUiElementIndex<0, Entry, Entries...>()>(m_elements);
    }

    /*! @brief Returns a pointer to the UI element created for the entry trait instantiated as `EntryTemplate<class...>` (shallow
     *         lookup).
     *  @tparam EntryTemplate EntryTemplate used to instantiate one of the Entries...
     *  @return Pointer to the stored UI element of type `typename EntryTemplate<...>::Type*`.
     *  @note This performs a shallow lookup only within this container instance. */
    template<template<class...> class EntryTemplate>
    auto getUiElement() {
        static_assert(containsEntryTemplate<EntryTemplate>());
        return _getUiElementImpl<EntryTemplate,0,Entries...>();
    }

    /*! @brief Returns a pointer to the UI element created for the entry trait instantiated as `EntryTemplate<auto,class...>` (shallow
     *         lookup).
     *  @tparam EntryTemplate EntryTemplate used to instantiate one of the Entries...
     *  @return Pointer to the stored UI element of type `typename EntryTemplate<...>::Type*`.
     *  @note This performs a shallow lookup only within this container instance. */
    template<template<auto,class...> class EntryTemplate>
    auto getUiElement() {
        static_assert(containsEntryTemplate<EntryTemplate>());
        return _getUiElementImpl<EntryTemplate,0,Entries...>();
    }

    /*! @brief Returns a pointer to the UI element corresponding to the specified Entry trait, searching recursively.
     *  @tparam Entry Entry trait type to locate.
     *  @return Pointer to the found UI element, or nullptr if not found.
     *  @details Searches Entries... in order. For nested MenuTemplate entries (`Entry::Type` is `MenuTemplate<...>`), the nested
     *           template is searched recursively.
     * @note If multiple instances exist, this returns the first match in traversal order. */
    template<MenuEntryConcept Entry>
    auto getUiElementRecursive() {
        return _getUiElementRecursive<Entry,Entries...>();
    }

    /*! @brief Returns a tuple of pointers to all UI elements for which `Condition<EntryTrait>::value == true` (shallow, no recursion).
     *  @tparam Condition A unary type-trait template providing `static constexpr bool value`.
     *  @return std::tuple<...> containing pointers to matched UI elements, in the same order as `Entries...`
     *  @details Each matched element is returned as a pointer (Entry::Type*). Non-matching entries contribute an empty tuple.
     * @note This does NOT traverse nested MenuTemplate entries. Use recursiveGetAllUiElementsIf for deep traversal. */
    template<template<class> class Condition>
    auto getAllUiElementsIf() { return std::tuple_cat(_getTupleOfMatched<Condition,Entries>()...); }

    /*! @brief Returns a tuple of pointers to all UI elements represented by the specified `Entry` trait (shallow, no recursion).
     *  @tparam Entry Entry trait type.
     *  @return std::tuple containing pointers to all matching elements in Entries... order. */
    template<MenuEntryConcept Entry>
    auto getAllUiElements() { return getAllUiElementsIf<draupnir::utils::the_same<Entry>::template as>(); }

    /*! @brief Returns a tuple of pointers to all UI elements for which `Condition<EntryTrait>::value == true`, including nested templates.
     *  @tparam Condition A unary type-trait template providing `static constexpr bool value`.
     *  @return std::tuple<...> containing pointers to matched UI elements in traversal order.
     *  @details Traversal is depth-first in Entries... order. If an entry's Type is a MenuTemplate<...> instantiation, the nested
     *           menu is queried recursively and its matches are concatenated into the result. */
    template<template<class> class Condition>
    auto recursiveGetAllUiElementsIf() { return std::tuple_cat(_recursiveGetTupleOfMatched<Condition,Entries>()...); }

    /*! @brief Returns a tuple of pointers to all UI elements represented by the specified `Entry` trait, searching recursively.
     *  @tparam Entry Entry trait type.
     *  @return std::tuple containing pointers to all matching elements in traversal order. */
    template<MenuEntryConcept Entry>
    auto recursiveGetAllUiElements() { return recursiveGetAllUiElementsIf<draupnir::utils::the_same<Entry>::template as>(); }

    /*! @brief Connects the `QAction::triggered` signal of the specified `Entry` to the provided Qt slot/functor.
     *  @tparam Entry Entry trait type to connect (must describe QAction or a QAction-derived Type).
     *  @tparam Args Arguments forwarded to QObject::connect (receiver, method, functor, connection type, ...).
     *  @return QMetaObject::Connection handle returned by QObject::connect.
     * @note Only QAction-based entries are supported. Compilation fails for non-QAction entries. */
    template<MenuEntry::IsActionEntry Entry, class... Args>
    QMetaObject::Connection on(Args&&... args) {
        return QObject::connect(getUiElement<Entry>(), &QAction::triggered, std::forward<Args>(args)...);
    }

protected:
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
    friend class MenuEntriesContainerTest;

    std::tuple<
        std::add_pointer_t<typename Entries::Type>...
    > m_elements;

    template<std::size_t Index, class First, class... Rest>
    inline void _initUiElementsImpl() {
        std::get<Index>(m_elements) = First::createElement();

        if constexpr (std::is_same_v<typename First::Type, QMenu> || std::is_base_of_v<QMenu, typename First::Type>) {
            std::get<Index>(m_elements)->setTitle(First::displayName());
        } else if constexpr (std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>) {
            if constexpr (MenuEntry::HasDisplayNameMethod<First>)
                std::get<Index>(m_elements)->setText(First::displayName());
        } else {
            static_assert(
                std::is_same_v<typename First::Type, QMenu>   || std::is_base_of_v<QMenu, typename First::Type> ||
                    std::is_same_v<typename First::Type, QAction> || std::is_base_of_v<QAction, typename First::Type>,
                "All Entries... must describe QMenu (or a derived class) or QAction (or a derived class)."
                );
        }

        if constexpr (sizeof...(Rest) > 0)
            _initUiElementsImpl<Index+1, Rest...>();
    }

    template<class Entry, class First, class... Rest>
    static constexpr int _recursiveEntryCountImpl() {
        int increment = 0;
        using EntryType = typename First::Type;
        if constexpr (draupnir::utils::is_instantiation_of_v<EntryType,Draupnir::Ui::MenuTemplate>) {
            increment += EntryType::template recursiveEntriesCount<Entry>();
        } else if constexpr (std::is_same_v<Entry,First>) {
            increment++;
        }

        if constexpr (sizeof...(Rest) > 0)
            return increment + _recursiveEntryCountImpl<Entry,Rest...>();
        else
            return increment;
    }

    template<class Entry, class First, class... Rest>
    static constexpr bool _containsRecursiveImpl() {
        if constexpr (draupnir::utils::is_instantiation_of_v<typename First::Type,Draupnir::Ui::MenuTemplate>)
            if constexpr (First::Type::template recursiveContains<Entry>()) return true;
        if constexpr (std::is_same_v<Entry,First>)
            return true;

        if constexpr (sizeof...(Rest) > 0)
            return _containsRecursiveImpl<Entry,Rest...>();
        else
            return false;
    }

    template<std::size_t Index, class Entry, class First, class... Rest>
    static constexpr std::size_t _getUiElementIndex() {
        if constexpr (std::is_same_v<Entry, First>) {
            return Index;
        } else if constexpr (sizeof...(Rest) > 0) {
            return _getUiElementIndex<Index+1, Entry, Rest...>();
        } else {
            static_assert(sizeof...(Rest) > 0, "Provided Entry is not present within the Entries... parameter pack.");
            return -1;
        }
    }

    template<template<class...> class EntryTemplate, std::size_t Index, class First, class... Rest>
    auto _getUiElementImpl() {
        if constexpr (draupnir::utils::is_instantiation_of_v<First,EntryTemplate>) {
            return getUiElement<Index>();
        } else if constexpr (sizeof...(Rest) > 0) {
            return _getUiElementImpl<EntryTemplate,Index+1,Rest...>();
        } else {
            static_assert(sizeof...(Rest) > 0, "Entries... pack does not contain EntryTemplate instantiation.");
            return nullptr;
        }
    }

    template<template<auto,class...> class EntryTemplate, std::size_t Index, class First, class... Rest>
    auto _getUiElementImpl() {
        if constexpr (draupnir::utils::is_a1tp_instantiation_of_v<First,EntryTemplate>) {
            return getUiElement<Index>();
        } else if constexpr (sizeof...(Rest) > 0) {
            return _getUiElementImpl<EntryTemplate,Index+1,Rest...>();
        } else {
            static_assert(sizeof...(Rest) > 0, "Entries... pack does not contain EntryTemplate instantiation.");
            return nullptr;
        }
    }

    template<class Entry, class First, class... Rest>
    auto _getUiElementRecursive() {
        if constexpr (draupnir::utils::is_instantiation_of_v<typename First::Type,Draupnir::Ui::MenuTemplate>) {
            if constexpr (typename First::Type::template containsRecursive<Entry>())
                return getUiElement<First>()->template getUiElementRecursive<Entry>();
        } else if constexpr (std::is_same_v<First,Entry>) {
            return getUiElement<Entry>();
        } else if constexpr (sizeof...(Rest) > 0) {
            return _getUiElementRecursive<Entry,Rest...>();
        } else {
            return nullptr;
        }
    }

    template<template<class> class Condition,class CurrentEntry>
    auto _getTupleOfMatched() {
        if constexpr (Condition<CurrentEntry>::value) {
            return std::tuple{ getUiElement<CurrentEntry>() };
        } else {
            return std::tuple{};
        }
    }

    template<template<class> class Condition,class CurrentEntry>
    auto _recursiveGetTupleOfMatched() {
        if constexpr (draupnir::utils::is_instantiation_of_v<typename CurrentEntry::Type, Draupnir::Ui::MenuTemplate>) {
            return getUiElement<CurrentEntry>()->template recursiveGetAllUiElementsIf<Condition>();
        } else if constexpr (Condition<CurrentEntry>::value) {
            return std::tuple{ getUiElement<CurrentEntry>() };
        } else {
            return std::tuple{};
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
            if constexpr (MenuEntry::HasDisplayNameMethod<First>)
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

}; // namespace Draupnir::Ui

#endif // MENUENTRIESCONTAINER_H
