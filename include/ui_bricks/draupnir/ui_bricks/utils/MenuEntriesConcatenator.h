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

#ifndef MENUENTRIESCONCATENATOR_H
#define MENUENTRIESCONCATENATOR_H

#include "draupnir/ui_bricks/concepts/MenuEntryConcept.h"
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"
#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"
#include "draupnir/ui_bricks/ui/menus/MenuBarTemplate.h"
#include "draupnir/utils/type_list.h"

namespace Draupnir::Ui
{

template<auto> class SectionEntry;

/*! @class MenuEntriesConcatenator draupnir/ui_bricks/utils/MenuEntriesConcatenator.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Compile-time utility that expands and flattens a heterogeneous pack of "menu things" into a single list of menu entry
 *         traits.
 *  @tparam Things...   A variadic pack of types describing menu content. Each element of the pack may be:
 *                      - a menu entry trait type (e.g. `FileNewEntry`, `SeparatorEntry`, ...), i.e. a type that exposes:
 *                        - `using Type = ...;` (typically `QAction`, `QMenu` or `MenuTemplate<...>`)
 *                        - `static QString displayName();`
 *                      - a `MenuTemplate<Entries...>` type (Qt menu container template) to be recursively expanded
 *                      - a `MenuBarTemplate<Entries...>` type (menu bar container template) to be recursively expanded
 *
 *  @details This meta-utility is meant for situations where a menu definition can contain both "leaf" entries (actions / separators
 *           / sections / etc.), and nested menu containers (templates) which themselves contain entries.
 *
 *           The concatenator recursively traverses `MenuTemplate` / `MenuBarTemplate` instantiations and produces a flattened
 *           `draupnir::utils::type_list<...>` of *entry trait types* in the same order as they appear in `Things...`.
 *
 *           Internally it performs three conceptual steps:
 *           1) **Entry handling** (`_HandledEntry<Entry, Type>`)
 *             - Default: any entry trait produces a list with that entry only.
 *             - Special case: if `Entry::Type` is `MenuTemplate<SubEntries...>`, the entry is treated as a nested menu wrapper and
 *               its subentries are recursively expanded into the final list.
 *           2) **Thing handling** (`_HandledThing<Thing, ...>`)
 *             - `MenuTemplate<...>` and `MenuBarTemplate<...>` are expanded directly by recursively concatenating their entries.
 *             - Any other `Thing` is expected to be an entry trait. A compile-time validation is performed via `MenuEntryTraitValidator`
 *               (requires `Thing::Type` and `Thing::displayName()`). If validation fails, compilation stops with a `static_assert`.
 *           3) **Pack processing** (`_ThingsProcessor<...>`)
 *             - Folds the `Things...` pack into a single `type_list` by prepending each handled result. The base case returns an
 *               empty list for an empty pack.
 *
 *            **Result aliases:**
 *            - `AllEntries`: A flattened `draupnir::utils::type_list<...>` of all entry trait types produced from `Things...` (
 *               including decoration entries).
 *            - `UniqueEntries`: Same as `AllEntries`, but with duplicate types removed (`unique_types_t`). Useful when you want
 *               to compute a "set" of entries involved in a menu definition.
 *            - `RemovedDecorationEntries`: Same as `AllEntries` but without decoration-only entries (`SeparatorEntry` and instantiations
 *              of the `SectionEntry`).
 *
 *            @code
 *            // Example: simple flattening
 *            using L1 = MenuEntriesConcatenator<FileNewEntry, SeparatorEntry, FileOpenEntry>::AllEntries;
 *            // L1 == type_list<FileNewEntry, SeparatorEntry, FileOpenEntry>
 *
 *            // Example: flattening with recursion through wrapper traits
 *            using L2 = MenuEntriesConcatenator<
 *                FileNewEntry,
 *                SettingsMenuTemplate<FileOpenEntry, FileSaveAsEntry>,
 *                SeparatorEntry
 *            >::AllEntries;
 *            // L2 == type_list<FileNewEntry, FileOpenEntry, FileSaveAsEntry, SeparatorEntry>
 *
 *            // Example: remove decorations
 *            using L3 = MenuEntriesConcatenator<FileNewEntry, SeparatorEntry, FileOpenEntry>::RemovedDecorationEntries;
 *            // L3 == type_list<FileNewEntry, FileOpenEntry>
 *            @endcode */

template<class... Things>
class MenuEntriesConcatenator
{
private:
    /*! @brief Maps a single entry trait to a type_list of entries.
     *  @tparam Entry Entry trait type.
     *  @tparam Type  `Entry::Type`.
     *  @details Default behavior: treat `Entry` as a leaf and return `type_list<Entry>`. Specialized behavior: if `Type` is
     *           `MenuTemplate<SubEntries...>`, expand the subentries recursively and return the flattened list of those
     *           subentries (the wrapper entry itself is not included). */
    template<class Entry, class Type>
    struct _HandledEntry {
        using result = draupnir::utils::type_list<Entry>;
    };

    /*! @brief Specialization for entry traits whose `Entry::Type` is a `MenuTemplate<SubEntries...>`.
     *  @details Interprets this entry as a nested menu wrapper and expands its `SubEntries...` recursively. */
    template<class Entry, class... SubEntries>
    struct _HandledEntry<Entry,MenuTemplate<SubEntries...>> {
        using result = typename MenuEntriesConcatenator<SubEntries...>::AllEntries;
    };

    /*! @brief Normalizes a "thing" from `Things...` into a type_list of entry traits.
     *  @tparam Thing Candidate input type from `Things...`.
     *  @details Primary template validates that `Thing` looks like a menu entry trait (has `Thing::Type` and
     *           `Thing::displayName()`). This is a fallback used only when no other specialization matches.
     *  @note If validation fails, compilation stops with a static_assert. */
    template<class Thing, class = void>
    struct _HandledThing {
        static inline constexpr bool isValidThing =
            MenuEntry::HasEntryType<Thing> &&
            MenuEntry::HasDisplayNameMethod<Thing>;

        static_assert(isValidThing);

        using result = void;
    };

    /*! @brief Specialization: when a thing is a `MenuTemplate<SubEntries...>`, expand its entries recursively. */
    template<class... SubEntries>
    struct _HandledThing<MenuTemplate<SubEntries...>,void> {
        using result = typename MenuEntriesConcatenator<SubEntries...>::AllEntries;
    };

    /*! @brief Specialization: when a thing is a `MenuBarTemplate<SubEntries...>`, expand its entries recursively. */
    template<class... SubEntries>
    struct _HandledThing<MenuBarTemplate<SubEntries...>,void> {
        using result = typename MenuEntriesConcatenator<SubEntries...>::AllEntries;
    };

    /*! @brief Specialization: if `Thing` provides nested `Thing::Type`, treat it as an entry trait.
     *  @details Delegates handling to `_HandledEntry<Thing, Thing::Type>` to support both leaf entries and nested menu wrappers. */
    template<class Thing>
    struct _HandledThing<Thing,std::void_t<typename Thing::Type>> {
        using result = typename _HandledEntry<Thing,typename Thing::Type>::result;
    };

    /*! @brief Processes the `Things...` pack into a single flattened type_list.
     *  @details Base case: empty pack yields an empty list. */
    template<class... Args>
    struct _ThingsProcessor {
        using result = draupnir::utils::type_list<>;
    };

    /*! @brief Recursive case: prepend handled head to the processed tail. */
    template<class First,class... Rest>
    struct _ThingsProcessor<First,Rest...> {
        using result = typename _ThingsProcessor<Rest...>::result
            ::template prepend_t<typename _HandledThing<First>::result>;
    };

public:
    /*! @brief Flattened list of all entry trait types produced from `Things...` (including decoration entries). */
    using AllEntries = typename _ThingsProcessor<Things...>::result;

    /*! @brief A `type_list` containing unique entry trait types from @ref AllEntries. */
    using UniqueEntries = typename AllEntries::unique_types_t;

    /*! @brief Entries list with decoration-only entries removed. Removes `SeparatorEntry` and all instantiations of `SectionEntry` */
    using RemovedDecorationEntries = typename AllEntries
        ::template remove_all_t<SeparatorEntry>
        ::template remove_if_t<draupnir::utils::is_auto_instantiation<SectionEntry>::of>;
};

}; // namespace Draupnir::Ui

#endif // MENUENTRIESCONCATENATOR_H
