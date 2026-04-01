/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MENUBARHANDLERSHELPER_H
#define MENUBARHANDLERSHELPER_H

#include "draupnir/ui_bricks/handlers/file_menu/FileMenuEntriesHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/HelpMenuEntriesHandler.h"
#include "draupnir/ui_bricks/handlers/settings_menu/SettingsMenuHandlerTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/FileMenuTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/HelpMenuTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/SettingsMenuTemplate.h"
#include "draupnir/ui_bricks/ui/menus/MenuBarTemplate.h"
#include "draupnir/ui_bricks/utils/MenuEntriesConcatenator.h"

namespace Draupnir::Application::Utils
{

/*! @class MenuBarHandlersHelper draupnir/application_template/utils/MenuBarHandlersHelper.h
 *  @brief Compile-time helper that extracts known menus from a MenuBar and produces menu handler types.
 *  @tparam MenuBar A menu bar configuration type. If not a recognized menu bar type, the default template yields `void` handlers.
 *
 *  @details This helper is designed to work with `Draupnir::Ui::MenuBarTemplate<...>` configuration. It looks for specific
 *           menu template instantiations (currently Settings and Help menus) among `MenuBarEntries...` and, if found, derives
 *           a corresponding handler type.
 *
 *           The resulting handler type is built by:
 *           1) Extracting the menu type from the menu bar (or yielding `void` if absent).
 *           2) Expanding the menu into its (unique) entry list via `MenuEntriesConcatenator<Menu>::UniqueEntries`.
 *           3) Filtering out entries that should not be handled as "action items" (e.g. separators, section headers, and
 *              submenu template entries).
 *           4) Converting the final entry list into a `Draupnir::Handlers::MenuHandlerTemplate<Context, EntryHandlerTemplate,
 *              Entries...>`.
 *
 *           If a target menu does not exist in the menu bar, the corresponding handler type is `void`. */

template<class MenuBar>
class MenuBarHandlersHelper
{
public:    
    /*! @brief Settings menu handler type (default case).
     *  @tparam SettingsRegistry A settings registry type.
     *  @details For unsupported MenuBar types the handler is always `void`. */
    template<class>
    using SettingsMenuHandler = void;

    /*! @brief Help menu handler type (default case).
     *  @tparam HelpSource A help source type used by the help handler.
     *  @details For unsupported MenuBar types the handler is always `void`. */
    template<class>
    using HelpMenuHandler = void;
};

/*! @brief Specialization for `Draupnir::Ui::MenuBarTemplate<...>`.
 *  @tparam MenuBarEntries Variadic list of entries present in the menu bar.
 *  @details Enables menu extraction and handler deduction based on the menu bar entry list. */

template<class... MenuBarEntries>
class MenuBarHandlersHelper<Draupnir::Ui::MenuBarTemplate<MenuBarEntries...>>
{
private:
    /*! @brief Extracts a menu template instantiation from `MenuBarEntries...`, or yields `void`.
     *  @tparam MenuTemplate A menu template (template template parameter), e.g. `SettingsMenuTemplate` or `HelpMenuTemplate`.
     *  @details Uses `draupnir::utils::is_template_instantiation_present_v` and `draupnir::utils::get_template_instantiation_t`
     *           to locate an instantiation of `MenuTemplate<...>` among the menu bar entries.
     *
     *           Implemented as a constrained partial specialization:
     *           - If present: `result` is the extracted instantiation type;
     *           - If absent:  `result` is `void`; */
    template<template<class...> class MenuTemplate>
    struct SubMenuExtractor;

    template<template<class...> class MenuTemplate>
        requires(draupnir::utils::is_template_instantiation_present_v<MenuTemplate,MenuBarEntries...>)
    struct SubMenuExtractor<MenuTemplate> {
        using result = draupnir::utils::get_template_instantiation_t<MenuTemplate,MenuBarEntries...>;
    };

    template<template<class...> class MenuTemplate>
        requires(!draupnir::utils::is_template_instantiation_present_v<MenuTemplate,MenuBarEntries...>)
    struct SubMenuExtractor<MenuTemplate> {
        using result = void;
    };

    template<template<class...> class HandlerTemplate, class... HandlerTemplateArgs>
    struct HandlerAdapter {
        template<class... Entries>
        using ResultingHandler = HandlerTemplate<HandlerTemplateArgs...,Entries...>;
    };

    /*! @brief Main case: build a handler type from a concrete menu type.
     *  @details The menu is expanded into `UniqueEntries` and then filtered:
     *           - separators are removed (`SeparatorEntry`);
     *           - section headers are removed (`SectionEntry<...>`);
     *           - submenu template entries are removed (entries that represent `MenuTemplateEntry` / submenu adapters),
     *             and finally converted to a `MenuHandlerTemplate<Context, EntryHandlerTemplate, ...>`.
     * @note The exact meaning of "submenu template entry" depends on `Draupnir::Ui::MenuEntry::IsMenuTemplateEntryAdapter`. */
    template<class Menu, template<class...> class HandlerTemplate, class... HandlerTemplateArgs>
    struct HandlerDeductor {
        using result = Draupnir::Ui::MenuEntriesConcatenator<Menu>::UniqueEntries
            ::template remove_all_t<Draupnir::Ui::SeparatorEntry>
            ::template remove_if_t<draupnir::utils::is_auto_instantiation<Draupnir::Ui::SectionEntry>::of>
            ::template remove_if_t<Draupnir::Ui::MenuEntry::IsMenuTemplateEntryAdapter>
            ::template convert_to_t<
                HandlerAdapter<HandlerTemplate,HandlerTemplateArgs...>::template ResultingHandler
            >;
    };

    template<template<class...> class HandlerTemplate, class... HandlerTemplateArgs>
    struct HandlerDeductor<void,HandlerTemplate,HandlerTemplateArgs...> {
        using result = void;
    };

public:
    /*! @brief Extracted File menu type from the menu bar, or `void` if not present. */
    using FileMenu_or_void        = SubMenuExtractor<Draupnir::Ui::FileMenuTemplate>::result;

    /*! @brief Extracted Settings menu type from the menu bar, or `void` if not present. */
    using SettingsMenu_or_void    = SubMenuExtractor<Draupnir::Ui::SettingsMenuTemplate>::result;

    /*! @brief Extracted Help menu type from the menu bar, or `void` if not present. */
    using HelpMenu_or_void        = SubMenuExtractor<Draupnir::Ui::HelpMenuTemplate>::result;

    /*! @brief Derived handler type for the File menu.
     *  @tparam FileManager A file manager type used to construct `FileContext`.
     *  @details If `FileMenu_or_void` is `void`, this alias resolves to `void`. Otherwise it resolves to a
     *           `Draupnir::Handlers::MenuHandlerTemplate<...>` specialization. */
    template<class FileManager, class UiHelper = void>
    using FileMenuHandler = HandlerDeductor<
        FileMenu_or_void,
        Handlers::FileMenuHandler,
        FileManager,
        UiHelper
    >::result;

    /*! @brief Derived handler type for the Settings menu.
     *  @tparam SettingsRegistry A settings registry type used to construct `SettingsContext`.
     *  @details If `SettingsMenu_or_void` is `void`, this alias resolves to `void`. Otherwise it resolves to a
     *           `Draupnir::Handlers::MenuHandlerTemplate<...>` specialization. */
    template<class SettingsRegistry>
    using SettingsMenuHandler = HandlerDeductor<
        SettingsMenu_or_void,
        Handlers::SettingsMenuHandler,
        SettingsRegistry
    >::result;

    /*! @brief Derived handler type for the Help menu.
     *  @tparam HelpSource A type representing help content/source used by the help entry handlers.
     *  @details If `HelpMenu_or_void` is `void`, this alias resolves to `void`. Otherwise it resolves to a
     *           `Draupnir::Handlers::MenuHandlerTemplate<...>` specialization. */
    template<class HelpSource>
    using HelpMenuHandler = HandlerDeductor<
        HelpMenu_or_void,
        Handlers::HelpMenuEntriesHandler,
        HelpSource
    >::result;
};

}; // namespace Draupnir::Application::Utils

#endif // MENUBARHANDLERSHELPER_H
