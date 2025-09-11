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

#ifndef GENERICMENUENTRIESHANDLER_H
#define GENERICMENUENTRIESHANDLER_H

#include <QDebug>

#include <tuple>

#include "AppSettings.h"
#include "AbstractHandlers.h"

#include "../../utils/common.h"

namespace Draupnir::Menus {

/*! @class GenericMenuEntriesHandler draupnir-lib/include/templateMenus/handlers/GenericMenuEntriesHandler.h
 *  @brief Template for handling multiple menu entries in a generic way.
 *  @details This class provides a generic handler for a set of menu entries (e.g., actions or menu items) using a provided
 *           Context and a parameter pack of handled entry types.
 *
 *           It automatically instantiates a handler for each entry type via GenericMenuEntryHandler, stores them in a tuple,
 *           and provides methods for connecting actions and delegating settings loading.
 *
 *           For each entry in HandledEntries..., an instance of GenericMenuEntryHandler<Context, Entry> is created and stored
 *           in m_handlers.
 *           Provides SFINAE-enabled loadSettings() method which is only available if Context defines a member function
 *           loadSettings(AppSettings*).
 *
 *           connectActions() traverses all handled entries and connects each to its corresponding handler using the
 *           MenuContainer API.
 *
 *  @tparam Context - The context type providing data or logic required by the menu entries.
 *  @tparam HandledEntries... - The parameter pack of all handled menu entry traits/types.
 *
 * @note Context is stored as a value member (m_context), and is forwarded to each entry handler by reference.
 * @note Handlers are stored as a tuple, and actions are connected recursively at compile time using _connectActionsImpl.
 *
 * @see GenericMenuEntryHandler */

template<class Context,class... HandledEntries>
class GenericMenuEntriesHandler
{
private:
    /*! @struct has_loadSettings
     *  @brief Trait to check if Context defines a member function loadSettings(AppSettings*). */
    template<class, class = std::void_t<>>
    struct has_loadSettings : std::false_type {};

    template<class Class>
    struct has_loadSettings<
        Class,
        std::void_t<decltype(std::declval<Class>().loadSettings(std::declval<AppSettings*>()))>
    > : std::true_type {};

public:   
    /*! @brief Constructs the handler and creates individual entry handlers for each handled entry type. */
    GenericMenuEntriesHandler() :
        m_context{},
        m_handlers{GenericMenuEntryHandler<Context, HandledEntries>(m_context)...}
    {}

    /*! @brief Delegates loading settings to Context if it supports loadSettings(AppSettings*). Only enabled if Context has a
     *         loadSettings method (checked via has_loadSettings trait).
     * @param settings Pointer to the settings object (must not be nullptr). */
    typename std::enable_if<has_loadSettings<Context>::value,void>
    loadSettings(AppSettings* settings) {
        Q_ASSERT_X(settings,"GenericMenuEntriesHandler::loadSettings",
                   "Provided AppSettings pointer is nullptr");
        m_context.loadSettings(settings);
    }

    /*! @brief Connects each handled entry in the given menu container to its handler.
     *  @tparam MenuContainer - The container class supporting template contains<T>() and get<T>() API. For example: MenuTemplate
     *          and MenuBarTemplate classes.
     *  @param container Pointer to the menu container instance. */
    template<class MenuContainer>
    void connectActions(MenuContainer* container) {
        if constexpr (sizeof...(HandledEntries) > 0)
            _connectActionsImpl<MenuContainer,HandledEntries...>(container);
    }

    /*! @brief Returns reference to the handler for specified menu entry. */
    template<class HandledEntry>
    auto& getHandler() {
        static_assert(is_one_of_v<HandledEntry,HandledEntries...>,
                "Specified Entry trait is not handled within this handler.");
        return std::get<GenericMenuEntryHandler<Context,HandledEntry> >(m_handlers);
    }

protected:
    /*! @brief Accessor for the context instance.
     *  @return Reference to the internal context. */
    Context& context() { return m_context; }

    std::tuple<
        GenericMenuEntryHandler<Context, HandledEntries>...
    >& handlers() { return m_handlers; }

private:
    Context m_context;
    std::tuple<
        GenericMenuEntryHandler<Context, HandledEntries>...
    > m_handlers;

    /*! @brief Helper to recursively connect each entry handler to its QAction. Only connects if MenuContainer contains the
     *         given entry type.
     *  @tparam MenuContainer - The menu container type.
     *  @tparam First - The current entry type.
     *  @tparam Rest - Remaining entry types.
     *  @param container - Pointer to the menu container. */
    template<class MenuContainer, class First, class... Rest>
    void _connectActionsImpl(MenuContainer* container) {
        if constexpr (MenuContainer::template contains<First>()) {
            std::get<GenericMenuEntryHandler<Context,First> >(m_handlers)
                    .connect(container->template get<First>());
        }

        if constexpr (sizeof...(Rest) > 0)
            _connectActionsImpl<MenuContainer,Rest...>(container);
    }
};

}

#endif // GENERICMENUENTRIESHANDLER_H
