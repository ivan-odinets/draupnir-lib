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

#ifndef TYPE_PRESENSE_H
#define TYPE_PRESENSE_H

/*! @file draupnir/utils/type_presense.h
 *  @brief This is a file.
 *
 * @todo Document usage of entities within this file. Add some examples to the documentation.
 * @todo Add some test to check if this works as expected. */

#include <tuple>
#include <type_traits>

namespace draupnir::utils
{

/*! @struct is_one_of draupnir/utils/type_presense.h
 *  @ingroup Utils
 *  @brief Compile‑time check whether a type appears in a variadic type list.
 *  @tparam T      The type to test.
 *  @tparam Args...  The types to check against.
 *
 *  @details `is_one_of<T, Args...>` inherits from `std::true_type` if `T` matches any of the types in `Args...`, and from
 *           `std::false_type` otherwise. This is useful for SFINAE, static_asserts and other compile‑time constraints. */

template<typename T, typename... Args>
struct is_one_of : std::disjunction<std::is_same<T, Args>...> {};

/*! @var is_one_of_v draupnir/utils/type_presense.h
 *  @ingroup Utils
 *  @brief Convenience constexpr variable template for @ref is_one_of.
 *
 *  @details Evaluates to `true` if `T` is present in `Args...`, `false` otherwise. */

template<typename T, typename... Args>
constexpr bool is_one_of_v = is_one_of<T, Args...>::value;

/*! @struct is_one_of draupnir/utils/type_presense.h
 *  @ingroup Utils
 *  @tparam T      The type to test.
 *  @tparam Tuple  A `std::tuple<...>` type.
 *  @brief Compile‑time check whether a type appears in a `std::tuple` type.
 *
 *  @details Primary template defaults to `std::false_type`. The partial specialization below matches `std::tuple<Ts...>` and
 *           evaluates to `std::true_type` if `T` matches any of the element types `Ts...`. */

template<typename T, typename Tuple>
struct is_type_in_tuple : std::false_type {};

/*! @struct is_one_of draupnir/utils/type_presense.h
 *  @ingroup Utils
 *  @brief Specialization for the case where `Tuple` is a `std::tuple`.
 *
 *  @details Inherits from `std::true_type` if `T` matches any element type inside the tuple, and from `std::false_type`
 *           otherwise. */

template<typename T, typename... Ts>
struct is_type_in_tuple<T, std::tuple<Ts...>>
    : std::disjunction<std::is_same<T, Ts>...> { };

/*! @var is_one_of_v draupnir/utils/type_presense.h
 *  @ingroup Utils
 *  @brief Convenience constexpr variable template for @ref is_type_in_tuple.
 *
 *  @details Evaluates to `true` if `T` is present in the element types of `Tuple`, `false` otherwise. */

template<typename T, typename Tuple>
inline constexpr bool is_type_in_tuple_v = is_type_in_tuple<T, Tuple>::value;

}; // draupnir::utils

#endif // TYPE_PRESENSE_H
