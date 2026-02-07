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

#ifndef MENUENTRYCONCEPT_H
#define MENUENTRYCONCEPT_H

#include <QMenu>

#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"
#include "draupnir/utils/template_detectors.h"

namespace Draupnir::Ui
{

template<class...>
class MenuTemplate;

namespace MenuEntry
{

template<class Candidate>
concept HasSupportedEntryType =
    requires { typename Candidate::Type; } &&
    (
        std::is_same_v<QAction,typename Candidate::Type> ||
        std::is_base_of_v<QAction,typename Candidate::Type> ||
        std::is_same_v<QMenu,typename Candidate::Type> ||
        std::is_base_of_v<QMenu,typename Candidate::Type>
    );

template<class Candidate>
concept HasDisplayNameMethod = requires {
    { Candidate::displayName() } -> std::same_as<QString>;
};

template<class Candidate>
concept HasCreateElementMethod =
    requires { { Candidate::createElement() } -> std::same_as<typename Candidate::Type*>; };

template<class Candidate>
concept IsActionEntry =
    HasSupportedEntryType<Candidate> &&
    std::is_same_v<typename Candidate::Type,QAction>;

template<class Candidate>
struct IsActionEntryAdapter :
    std::bool_constant<IsActionEntry<Candidate>> {};

template<class Candidate>
concept IsSubmenuEntry =
    HasSupportedEntryType<Candidate> && (
        std::is_same_v<typename Candidate::Type,QMenu> ||
        std::is_base_of_v<QMenu,typename Candidate::Type>
    );

template<class Candidate>
struct IsSubmenuEntryAdapter :
    std::bool_constant<IsSubmenuEntry<Candidate>> {};

template<class Candidate>
concept IsMenuTemplateEntry =
    draupnir::utils::is_instantiation_of_v<typename Candidate::Type,Draupnir::Ui::MenuTemplate>;

template<class Candidate>
struct IsMenuTemplateEntryAdapter :
    std::bool_constant<IsMenuTemplateEntry<Candidate>> {};

template<class Candidate>
concept IsFinalEntry =
    HasSupportedEntryType<Candidate> && (
        IsActionEntry<Candidate> || (
            IsSubmenuEntry<Candidate> && !IsMenuTemplateEntry<Candidate>
        )
    );

}; // namespace Draupnir::Ui::MenuEntry

template<class Candidate>
concept MenuEntryConcept =
    MenuEntry::HasSupportedEntryType<Candidate> &&
    // Draupnir::Ui::SeparatorEntry has no diaplayName method, however it is a valid MenuEntryTrait
    (MenuEntry::HasDisplayNameMethod<Candidate> || std::is_same_v<Candidate,SeparatorEntry>) &&
    MenuEntry::HasCreateElementMethod<Candidate>;

}; // namespace Draupnir::Ui

#endif // MENUENTRYCONCEPT_H
