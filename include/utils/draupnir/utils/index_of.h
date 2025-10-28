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

#ifndef INDEX_OF_H
#define INDEX_OF_H

#include <type_traits>

namespace draupnir::utils
{

/*! @struct index_of draupnir/utils/index_of.h
 *  @ingroup Utils
 *  @brief Compile‑time index lookup of a type within a variadic type list.
 *  @tparam T  The type whose index is to be determined.
 *  @tparam Ts...  The variadic list of types to search through.
 *
 *  @details `index_of<T, Ts...>` yields, at compile time, the zero‑based index of type `T` within the parameter pack `Ts...`.
 *           The result is provided as a `std::integral_constant<std::size_t, N>` with a nested `value` member equal to the
 *           index.
 *
 *           This can be used in template metaprogramming to determine the position of a type in a pack or to access the
 *           corresponding element in a tuple‑like type.
 *
 * @todo Add support for the case when T is not present within Ts.. pack. */

template<typename T, typename... Ts>
struct index_of;

/*! @struct index_of draupnir/utils/index_of.h
 *  @ingroup Utils
 *  @brief Specialization for the case when the head of the list matches `T`.
 *
 *  @details Yields `std::integral_constant<std::size_t, 0>`, i.e. the index is 0. */

template<typename T, typename... Ts>
struct index_of<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

/*! @struct index_of draupnir/utils/index_of.h
 *  @ingroup Utils
 *  @brief Recursive specialization for the case when the head of the list does not match `T`.
 *
 *  @details Increments the index by one and recurses on the remaining types. */

template<typename T, typename U, typename... Ts>
struct index_of<T, U, Ts...>
    : std::integral_constant<std::size_t, 1 + index_of<T, Ts...>::value> {};

/*! @ingroup Utils
 *  @brief Helper template variable to access results of @ref draupnir::utils::index_of evaluation. */

template<typename T, typename... Ts>
inline constexpr std::size_t index_of_v = index_of<T,Ts...>::value;

};

#endif // INDEX_OF_H
