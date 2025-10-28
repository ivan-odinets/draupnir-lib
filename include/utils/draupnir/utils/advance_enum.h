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

#ifndef ADVANCE_ENUM_H
#define ADVANCE_ENUM_H

/*! @file draupnir/utils/advance_enum.h
 *  @ingroup Utils
 *  @brief Compile-time helpers for implementing cyclic operator++ on enums.
 *
 *  @details Provides templates for generic enum value iteration and wrap-around logic, intended for use in operator++ overloads. */

#include <type_traits>

namespace draupnir::utils
{

/*! @brief Advances enum value v to the next in the parameter pack; wraps around to first at the end (base case).
 *  @ingroup Utils
 *  @tparam E - enum type.
 *  @tparam first - first enum value (wrap-around target).
 *  @tparam head - current enum value being compared.
 *  @param v - vnum value to advance. */

template<typename E, E first, E head>
void advance_enum(E& v)
{
    static_assert(std::is_enum_v<E>,
            "Provided E key is not an enum type.");
    if (v == head)
        v = first;
}

/*! @brief Advances enum value v to the next in the parameter pack; recursive case.
 *  @ingroup Utils
 *  @tparam E - enum type.
 *  @tparam first - first enum value (wrap-around target).
 *  @tparam head - current enum value being compared.
 *  @tparam next - next enum value in the sequence.
 *  @tparam tail - remaining enum values.
 *  @param v - enum value to advance.
 *
 *  @details Compares v to head; if equal, assigns next; otherwise, recurses. */

template<typename E, E first, E head, E next, E... tail>
void advance_enum(E& v)
{
    static_assert(std::is_enum_v<E>,
            "Provided E key is not an enum type.");
    if (v == head)
        v = next;
    else
        advance_enum<E,first,next,tail...>(v);
}

/*! @brief Utility for iterating over enum values with cyclic increment.
 *  @ingroup Utils
 *  @tparam E - enum type.
 *  @tparam first - first enum value (used for wrap-around).
 *  @tparam values... - remaining enum values in the desired iteration order.
 *
 *  @details Provides a static method advance() to cycle an enum variable v through a specified list of enum values. */

template<typename E, E first, E... values>
struct enum_values
{
    static void advance(E& v) {
        static_assert(std::is_enum_v<E>,
                "Provided E key is not an enum type.");
        advance_enum<E, first, first, values...>(v);
    }
};

}; // draupnir::utils

#endif // ADVANCE_ENUM_H
