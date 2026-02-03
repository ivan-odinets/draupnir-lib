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

#ifndef MENUHANDLERTEMPLATE_H
#define MENUHANDLERTEMPLATE_H

#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/settings_registry/utils/OptionalSettingsBundle.h"
#include "draupnir/utils/type_list.h"
#include "draupnir/utils/type_if.h"

template<class Object>
concept RequireUpdateUponSettingsLoading = requires(Object obj) {
    obj.onSettingsLoaded();
};

template<class Object>
concept HasNestedSettings =
    requires { typename Object::SettingsBundle; } &&
    draupnir::utils::is_instantiation_of_v<typename Object::SettingsBundle,Draupnir::Settings::SettingsBundleTemplate>;

namespace Draupnir::Handlers
{

/*! @class MenuHandlerTemplate draupnir/ui_bricks/handlers/templates/MenuHandlerTemplate.h
 *  @brief This is a class. */

template<class UserContext, template<class,class> class HandlerTemplate, class... MenuEntries>
class MenuHandlerTemplate
{
    // Lets define who is responsible for handling settings
    static inline constexpr bool userContextHasSettings =
        draupnir::utils::is_t1a1_template_base_of_v<Settings::OptionalSettingsBundle,UserContext> ||
        draupnir::utils::is_template_base_of_v<Draupnir::Settings::NonEmptySettingsBundle,UserContext> ||
        HasNestedSettings<UserContext>;

    class _Empty {};

    // Ok and now lets form the _RealContext, which will be passed down to the individual handlers
    class _RealContext :
        public draupnir::utils::type_if<userContextHasSettings>
            ::template then_type<_Empty>
            ::template else_type<
                Draupnir::Settings::NonEmptySettingsBundle<
                    typename Settings::SettingsTraitsConcatenator<HandlerTemplate<class Empty,MenuEntries>...>::toSettingsBundle
                >
            >
        ::result,
        public UserContext
    {};

    // Lets find if individual handlers require Context object
    using _HandlerList = draupnir::utils::type_list<HandlerTemplate<_RealContext,MenuEntries>...>;

    template<class Context>
    struct ContextRequired {
        template<class Handler>
        struct asTypeOnly : std::bool_constant<
            !std::is_constructible_v<Handler,Context&> && std::is_default_constructible_v<Handler>
        > {};

        template<class Handler>
        struct asObject : std::bool_constant<
            std::is_constructible_v<Handler,Context&> && !std::is_default_constructible_v<Handler>
        > {};
    };

    static inline constexpr bool someHandlersRequireContextObject =
        _HandlerList::template any_of_v<ContextRequired<_RealContext>::template asObject>;

    // Ok. We have Settings, Context. Lets deduct the fields of this Handler
    // Lets create fields struct with optional context
    template<bool enableContextObject, class Unused = void>
    struct _Fields;

    // Case when we don't need the Context object
    template<class Unused>
    struct _Fields<false,Unused> {
        std::tuple<HandlerTemplate<_RealContext,MenuEntries>...> handlers =
            std::tuple<HandlerTemplate<_RealContext,MenuEntries>...>{HandlerTemplate<_RealContext,MenuEntries>()...};
    };

    // Case when we need the context object
    template<class Unused>
    struct _Fields<true,Unused> {
        _Fields() :
            context{},
            handlers{_make_handler<MenuEntries>()...}
        {};

        _RealContext context;
        std::tuple<HandlerTemplate<_RealContext,MenuEntries>...> handlers;

    private:
        template<class MenuEntry>
        auto _make_handler() {
            using Handler = HandlerTemplate<_RealContext,MenuEntry>;
            if constexpr (ContextRequired<_RealContext>::template asObject<Handler>::value) {
                return Handler{context};
            } else {
                return Handler{};
            }
        }
    };

    // Finally - lets proceed with public interface and member definitions
public:
    auto& context() requires (someHandlersRequireContextObject) { return m_filels.context; }

    template<class MenuContainer>
    void connectActions(MenuContainer* container) {
        if constexpr (sizeof...(MenuEntries) > 0)
            _connectActionsImpl<MenuContainer,MenuEntries...>(container);
    }

    template<class SettingsSource>
    void loadSettings(SettingsSource source) {
        m_filels.context.loadSettings(source);

        _onSettingsLoadedImpl<MenuEntries...>();
    }

    template<class Entry>
    auto& getHandlerForEntry() {
        return std::get<HandlerTemplate<_RealContext,Entry>>(m_filels.handlers);
    }

private:
    _Fields<someHandlersRequireContextObject> m_filels;

    template<class MenuContainer, class FirstEntry, class... RestEntries>
    void _connectActionsImpl(MenuContainer* container) {
        if constexpr (MenuContainer::template contains<FirstEntry>()) {
            getHandlerForEntry<FirstEntry>().connect(container->template get<FirstEntry>());
        }

        if constexpr (sizeof...(RestEntries) > 0)
            _connectActionsImpl<MenuContainer,RestEntries...>(container);
    }

    template<class FirstEntry, class... RestEntries>
    void _onSettingsLoadedImpl() {
        using FirstHandler = HandlerTemplate<_RealContext,FirstEntry>;

        if constexpr (RequireUpdateUponSettingsLoading<FirstHandler>) {
            getHandlerForEntry<FirstEntry>().onSettingsLoaded();
        }

        if constexpr (sizeof...(RestEntries) > 0)
            _onSettingsLoadedImpl<RestEntries...>();
    }
};

}; // namespace Draupnir::Handlers

#endif // MENUHANDLERTEMPLATE_H
