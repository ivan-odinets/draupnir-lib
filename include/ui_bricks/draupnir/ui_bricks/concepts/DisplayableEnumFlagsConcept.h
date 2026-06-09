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

#ifndef DISPLAYABLEENUMFLAGSCONCEPT_H
#define DISPLAYABLEENUMFLAGSCONCEPT_H

#include <QString>

#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Ui
{

template<class Candidate>
concept DisplayableEnumFlagsConcept =
    draupnir::utils::enum_flags_derived_concept<Candidate> &&
    requires(Candidate flags) {
        typename Candidate::enum_type;
        typename Candidate::integer;

        requires draupnir::utils::c_array_of_concept<decltype(Candidate::displayedFlags), typename Candidate::enum_type>;

        { Candidate::toDisplayString(flags) } -> std::same_as<QString>;
    };

template<class Candidate>
concept DisplayableEnumFlagsWithMasks =
    DisplayableEnumFlagsConcept<Candidate> &&
    draupnir::utils::c_array_of_concept<decltype(Candidate::displayedMasks),typename Candidate::integer>;

}; // namespace Draupnir::Ui

#endif // DISPLAYABLEENUMFLAGSCONCEPT_H
