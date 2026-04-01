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

#ifndef MENUENTRYHANDLERCONTAINER_H
#define MENUENTRYHANDLERCONTAINER_H

#include "draupnir/utils/type_list.h"

template<class Object, class SettingsSource>
concept HasLoadSettingsFromPointer =
    requires (Object& object, SettingsSource* source) { {object.loadSettings(source) } -> std::same_as<void>; } ||
    requires (Object& object, SettingsSource* source) { {object.template loadSettings<SettingsSource>(source) } -> std::same_as<void>; };

template<class Object, class SettingsSource>
concept HasLoadSettingsFromReference =
    requires (Object& object, SettingsSource& source) { {object.loadSettings(source) } -> std::same_as<void>; } ||
    requires (Object& object, SettingsSource& source) { {object.template loadSettings<SettingsSource>(source) } -> std::same_as<void>; };

template<class Object, class SettingsSource>
concept HasLoadSettings = (
    (HasLoadSettingsFromPointer<Object,SettingsSource> ? 1 : 0) +
    (HasLoadSettingsFromReference<Object,SettingsSource> ? 1 : 0)
) == 1;

template<class Object, class SettingsSource>
struct HasLoadSettingsTemplate : std::bool_constant<
    HasLoadSettings<Object,SettingsSource>
> {};

template<class Object>
concept HasOnSettingsLoaded = requires(Object& object) {
    { object.onSettingsLoaded() } -> std::same_as<void>;
};

namespace Draupnir::Handlers
{

/*! @class MenuEntryHandlerContainer draupnir/ui_bricks/handlers/templates/MenuEntryHandlerContainer.h
 *  @ingroup UiBricks
 *  @brief Stores and initializes a compile-time list of menu entry handlers.
 *  @tparam Context Context type passed to handlers that require access to shared menu state.
 *  @tparam HandlerTemplate Handler class template of the form `HandlerTemplate<Context, Entry>`.
 *  @tparam MenuEntries List of menu entry marker types handled by this container.
 *
 *  @details `MenuEntryHandlerContainer` owns one handler instance per menu entry listed in `MenuEntries...`. The handler
 *           instances are stored as a tuple of concrete handler objects: `std::tuple<HandlerTemplate<Context, MenuEntries>...>`.
 *
 *           At compile time the container determines whether any handler requires a pointer to `Context` in its constructor.
 *           If at least one handler is constructible from `Context*` and is not default-constructible, the container requires
 *           explicit initialization with a `Context*`.
 *
 *           Otherwise the container can be initialized without a context object and all handlers are default-constructed. The
 *           container provides:
 *           - creation of all handlers as a single tuple;
 *           - lookup of a handler by menu entry type;
 *           - connection of handlers to matching UI menu entries;
 *           - notification of handlers after settings have been loaded;
 *
 *           ## Handler construction rules
 *           For every instantiated handler type `HandlerTemplate<Context, Entry>`, exactly one of the following forms is expected:
 *           - the handler is default-constructible, or
 *           - the handler is constructible from `Context*`.
 *
 *           ## Lifetime and initialization
 *           Handlers are created lazily by calling one of the `initialize(...)` overloads.
 *           All public methods that access handlers expect the container to have been initialized before use. This is checked with
 *           `Q_ASSERT`.
 *
 *           ## Ownership
 *           The container owns the tuple of handlers and deletes it in its destructor. */

template<class Context, template<class,class> class HandlerTemplate, class... MenuEntries>
class MenuEntryHandlerContainer
{
    // Lets find if individual handlers require Context object
    using _HandlerList = draupnir::utils::type_list<HandlerTemplate<Context,MenuEntries>...>;

    struct ContextRequired {
        template<class Handler>
        struct asTypeOnly : std::bool_constant<
            !std::is_constructible_v<Handler,Context*> && std::is_default_constructible_v<Handler>
        > {};

        template<class Handler>
        struct asObject : std::bool_constant<
            std::is_constructible_v<Handler,Context*> && !std::is_default_constructible_v<Handler>
        > {};
    };

    using _HandlerTuple = std::tuple<HandlerTemplate<Context,MenuEntries>...>;

public:
    /*! @brief Compile-time flag indicating whether at least one handler requires `Context*`
     *         during construction.
     *  @details If `true`, the container must be initialized with `initialize(Context*)`. If `false`, the container can be
     *           initialized with parameterless `initialize()`. */
    static inline constexpr bool contextObjectRequired_v =
        _HandlerList::template any_of_v<ContextRequired::template asObject>;

    /*! @brief Default constructor. Creates an empty container. No handlers are constructed at this stage. Call one of the
     *         `initialize(...)` overloads before using the container. */
    MenuEntryHandlerContainer() : p_handlerTuple{nullptr} {}

    /*! @brief Destructor. Deletes the owned tuple of handlers if it has been created. */
    ~MenuEntryHandlerContainer() { delete p_handlerTuple; }

    /*! @brief Initializes the container by default-constructing all handlers.
     *  @details This overload is available only when none of the handlers requires `Context*` in its constructor.
     * @pre `contextObjectRequired_v == false` and The container was not initialized before. */
    void initialize() requires(!contextObjectRequired_v) {
        Q_ASSERT(p_handlerTuple == nullptr);
        p_handlerTuple = new _HandlerTuple{_make_handler<MenuEntries>()...};
    }

    /*! @brief Initializes the container by constructing handlers with a context pointer when needed.
     *  @param context Pointer to the shared context object passed to handlers that require it.
     *  @details This overload is available only when at least one handler requires `Context*` in its constructor. Handlers
     *           that do not require context are still default-constructed.
     * @pre `contextObjectRequired_v == true` The container was not initialized before. */
    void initialize(Context* context) requires(contextObjectRequired_v) {
        Q_ASSERT(p_handlerTuple == nullptr);
        p_handlerTuple = new _HandlerTuple{_make_handler<MenuEntries>(context)...};
    }

    /*! @brief Connects all stored handlers to matching UI menu entries in the given container.
     *  @tparam MenuContainer Type of the UI menu container.
     *  @param container Pointer to the UI menu container.
     *  @details For each menu entry type in `MenuEntries...`, this method checks whether the provided `MenuContainer` contains
     *           such entries via `recursiveContains<Entry>()`. If yes, all matching UI elements are retrieved with
     *           `recursiveGetAllUiElements<Entry>()` and connected to the corresponding handler via `handler.connect(element)`.
     * @pre The container must have been initialized.
     */
    template<class MenuContainer>
    void connectActions(MenuContainer* container) {
        Q_ASSERT(p_handlerTuple != nullptr);
        if constexpr (sizeof...(MenuEntries) > 0)
            _connectActionsImpl<MenuContainer,MenuEntries...>(container);
    }

    /*! @brief Notifies all handlers that settings have already been loaded.
     *  @details Calls `onSettingsLoaded()` only on those handlers for which the `HasOnSettingsLoaded<Handler>` concept is satisfied.
     * @pre The container must have been initialized. */
    void onSettingsLoaded() {
        Q_ASSERT(p_handlerTuple != nullptr);
        _onSettingsLoadedImpl<MenuEntries...>();
    }

    /*! @brief Returns the handler responsible for the specified menu entry type.
     *  @tparam Entry Menu entry type.
     *  @return Reference to the stored handler instance of type `HandlerTemplate<Context, Entry>`.
     * @pre The container must have been initialized. */
    template<class Entry>
    auto& getHandlerForEntry() {
        Q_ASSERT(p_handlerTuple != nullptr);
        return std::get<HandlerTemplate<Context,Entry>>(*p_handlerTuple);
    }

private:
    friend class MenuEntryHandlerContainerTest;

    /*! @brief Owned tuple of all handler objects. This pointer is null until one of the `intialize(...)` overloads is
     *         called. */
    _HandlerTuple* p_handlerTuple;

    /*! @brief Creates one handler instance for the specified menu entry type.
     *  @tparam MenuEntry Menu entry type.
     *  @param context Optional shared context pointer.
     *  @return Constructed handler object.
     *  @details If the handler type is constructible from `Context*`, the passed context is used. Otherwise the handler
     *           is default-constructed. */
    template<class MenuEntry>
    static inline auto _make_handler(Context* context = nullptr) {
        using Handler = HandlerTemplate<Context,MenuEntry>;
        if constexpr (ContextRequired::template asObject<Handler>::value) {
            return Handler{context};
        } else {
            return Handler{};
        }
    };

    /*! @brief Internal recursive implementation of `connectActions()`.
     *  @tparam MenuContainer Type of the UI menu container.
     *  @tparam FirstEntry First menu entry type in the current recursion step.
     *  @tparam RestEntries Remaining menu entry types.
     *  @param container Pointer to the UI menu container. */
    template<class MenuContainer, class FirstEntry, class... RestEntries>
    inline void _connectActionsImpl(MenuContainer* container) {
        if constexpr (MenuContainer::template recursiveContains<FirstEntry>()) {
            auto entries = container->template recursiveGetAllUiElements<FirstEntry>();
            std::apply([this](auto&... element) {
                (..., getHandlerForEntry<FirstEntry>().connect(element));
            }, entries);
        }

        if constexpr (sizeof...(RestEntries) > 0)
            _connectActionsImpl<MenuContainer,RestEntries...>(container);
    }

    /*! @brief Internal recursive implementation of `onSettingsLoaded()`.
     *  @tparam FirstEntry First menu entry type in the current recursion step.
     *  @tparam RestEntries Remaining menu entry types.
     *  @details Calls `onSettingsLoaded()` only on handlers that provide such method and satisfy the concept
     *           `HasOnSettingsLoaded<Handler>`. */
    template<class FirstEntry, class... RestEntries>
    inline void _onSettingsLoadedImpl() {
        if constexpr (HasOnSettingsLoaded<HandlerTemplate<Context,FirstEntry>>)
            getHandlerForEntry<FirstEntry>().onSettingsLoaded();

        if constexpr (sizeof...(RestEntries) > 0)
            _onSettingsLoadedImpl<RestEntries...>();
    }
};

}; // namespace Draupnir::Handlers

#endif // MENUENTRYHANDLERCONTAINER_H
