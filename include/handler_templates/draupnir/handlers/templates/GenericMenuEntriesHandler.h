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

#include <tuple>

#include "GenericMenuEntryHandler.h"
#include "draupnir/SettingsBundleMerge.h"
#include "draupnir/utils/type_presense.h"

namespace Draupnir::Handlers {

/*! @class GenericMenuEntriesHandler draupnir/handlers/templates/GenericMenuEntriesHandler.h
 *  @ingroup HandlerTemplates
 *  @brief Template for handling multiple menu entries in a generic way.
 *  @tparam Implementation - CRTP implementation type (the derived handler class).
 *  @tparam HandledEntries... - Variadic list of entry trait types to be managed.
 *
 *  @details This class provides a generic mechanism for managing a set of menu entries (such as actions or menu items). It uses
 *           CRTP (`Implementation`) and a parameter pack of entry traits (`HandledEntries...`) to:
 *           - Instantiate a `GenericMenuEntryHandler<Implementation, Entry>` for each entry type.
 *           - Store all handlers in a compile-time tuple.
 *           - Connect handlers to a menu container (`connectActions()`).
 *           - Collect all setting traits required by handled entries into a single `SettingBundleClass`.
 *           - Conditionally enable settings-related methods only if the bundle is not empty.
 *
 *           The design ensures that:
 *           - Handlers are strongly typed and known at compile time.
 *           - Settings integration is optional: if no handler defines settings, settings logic is removed by SFINAE.
 *
 *  @note Handlers are stored as a tuple (`m_handlers`), each constructed with a reference to the CRTP implementation
 *        (`*static_cast<Implementation*>(this)`).
 *  @note Settings support is fully optional: if no handled entry declares settings, the related API is removed at compile time. */

template<class Implementation, class... HandledEntries>
class GenericMenuEntriesHandler
{
public:
    using SettingBundle = Settings::collect_settings_t<GenericMenuEntryHandler<Implementation,HandledEntries>...>;

    /*! @brief Constructs the handler and creates individual entry handlers for each handled entry type. */
    GenericMenuEntriesHandler() :
        m_handlers{GenericMenuEntryHandler<Implementation, HandledEntries>(*static_cast<Implementation*>(this))...}
    {}

    /*! @brief Connects each handled entry in the given menu container to its handler.
     *  @tparam MenuContainer - The container class supporting template contains<T>() and get<T>() API. For example: MenuTemplate
     *          and MenuBarTemplate classes.
     *  @param container Pointer to the menu container instance.
     * @note This method MUST be called BEFORE calling GenericMenuEntriesHandler::loadSettings method. */
    template<class MenuContainer>
    void connectActions(MenuContainer* container) {
        if constexpr (sizeof...(HandledEntries) > 0)
            _connectActionsImpl<MenuContainer,HandledEntries...>(container);
    }

    /*! @brief Loads settings, which are required by this GenericMenuEntriesHandler implementation from the provided SettingsRegistry
     *         instance.
     * @note This method MUST be called AFTER GenericMenuEntriesHandler::connectActions method.
     * @todo Add static_assert if the SettingsRegistry can not provide the required SettingsBundle. */
    template<class SettingsRegistry, class Dummy = SettingBundle>
    std::enable_if_t<!Dummy::isEmpty(), void> loadSettings(SettingsRegistry* registry) {
        m_settings = registry->template getSettingsBundle<SettingBundle>();

        _onSettingsLoaded<HandledEntries...>();
    }

    /*! @brief This method returns Value of the corresponding SettingTrait from the stored SettingsBundle. */
    template<class SettingTrait, class Dummy = SettingBundle>
    std::enable_if_t<!Dummy::isEmpty(), const typename SettingTrait::Value&> getSetting() const {
        static_assert(SettingBundle::template contains<SettingTrait>(),
                "SettingTrait provided is not available within this bundle.");

        return m_settings.template get<SettingTrait>();
    };

    /*! @brief This method sets Value of the corresponding SettingTrait from the stored SettingsBundle. */
    template<class SettingTrait, class Dummy = SettingBundle>
    std::enable_if_t<!Dummy::isEmpty(), void> setSetting(const typename SettingTrait::Value& value) {
        static_assert(SettingBundle::template contains<SettingTrait>(),
                "SettingTrait provided is not available within this bundle.");

        m_settings.template set<SettingTrait>(value);
    }

    /*! @brief Returns reference to the handler for specified menu entry. */
    template<class HandledEntry>
    auto& getHandler() {
        static_assert(draupnir::utils::is_one_of_v<HandledEntry,HandledEntries...>,
                "Specified Entry trait is not handled within this handler.");
        return std::get<GenericMenuEntryHandler<Implementation,HandledEntry> >(m_handlers);
    }

protected:
    std::tuple<
        GenericMenuEntryHandler<Implementation, HandledEntries>...
    >& handlers() { return m_handlers; }

private:
    SettingBundle m_settings;

    std::tuple<
        GenericMenuEntryHandler<Implementation, HandledEntries>...
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
            std::get<GenericMenuEntryHandler<Implementation,First> >(m_handlers)
                    .connect(container->template get<First>());
        }

        if constexpr (sizeof...(Rest) > 0)
            _connectActionsImpl<MenuContainer,Rest...>(container);
    }

    /*! @brief Checks for newFile() method with signature void newFile(). */
    template<class, class = std::void_t<>>
    struct has_onSettingsLoaded : std::false_type {};

    template<class Class>
    struct has_onSettingsLoaded<
        Class,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<Class>().onSettingsLoaded())>
        )>
    > : std::true_type {};

    template<class First,class... Rest>
    inline void _onSettingsLoaded() {
        if constexpr (has_onSettingsLoaded<GenericMenuEntryHandler<Implementation,First>>::value) {
            std::get<GenericMenuEntryHandler<Implementation,First>>(m_handlers).onSettingsLoaded();
        }

        if constexpr (sizeof...(Rest) > 0)
            _onSettingsLoaded<Rest...>();
    }
};

}

#endif // GENERICMENUENTRIESHANDLER_H
