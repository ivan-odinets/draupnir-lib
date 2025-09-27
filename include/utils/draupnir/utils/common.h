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

#ifndef COMMON_H
#define COMMON_H

/*! @file draupnir/utils/common.h
 *  @brief Type trait utilities for working with templates and type detection.
 *  @details This file provides helper templates and constexpr values for detecting tuples, pairs, and type membership in
 *           parameter packs. */

#include <tuple>
#include <type_traits>

/*! @brief Checks if a type is present in a variadic list of types.
 *  @details Provides a compile-time boolean indicating whether T matches any of Args...
 *  @tparam T - type to check.
 *  @tparam Args... - types to test against. */
template<typename T, typename... Args>
struct is_one_of : std::disjunction<std::is_same<T, Args>...> {};

/*! @brief Convenience constexpr value for is_one_of.
 *  @details True if T is present in Args..., false otherwise. */
template<typename T, typename... Args>
constexpr bool is_one_of_v = is_one_of<T, Args...>::value;

/*! @brief Type trait for detecting std::pair types.
 *  @details Inherits from std::true_type if T is a std::pair, otherwise std::false_type.
 *  @tparam T - type to test. */
template<typename T>
struct is_pair : std::false_type {};

/*! @brief Type trait for detecting std::pair types.
 *  @details Inherits from std::true_type if T is a std::pair, otherwise std::false_type.
 *  @tparam T - type to test. */
template<typename First, typename Second>
struct is_pair<std::pair<First,Second> > : std::true_type {};

/*! @brief Convenience constexpr value for is_pair.
 *  @details True - if T is a std::pair, false otherwise. */
template<typename T>
constexpr bool is_pair_v = is_pair<T>::value;

/*! @brief Type trait for detecting std::tuple types.
 *  @details Inherits from std::true_type if T is a std::tuple, otherwise std::false_type.
 *  @tparam T - type to test. */
template<typename T>
struct is_tuple : std::false_type {};

/*! @brief Type trait for detecting std::tuple types.
 *  @details Inherits from std::true_type if T is a std::tuple, otherwise std::false_type.
 *  @tparam T - type to test. */
template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/*! @brief Convenience constexpr value for is_tuple.
 *  @details True if T is a std::tuple, false otherwise. */
template<typename T>
constexpr bool is_tuple_v = is_tuple<T>::value;

/*! @brief This is a struct.
 * @todo DocumentMe. */
template<typename T, typename Tuple>
struct is_type_in_tuple : std::false_type {};

/*! @brief This is a struct.
 * @todo DocumentMe. */
template<typename T, typename... Ts>
struct is_type_in_tuple<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...> { };

/*! @brief This is a var.
 * @todo DocumentMe. */
template<typename T, typename Tuple>
inline constexpr bool is_type_in_tuple_v = is_type_in_tuple<T, Tuple>::value;

#endif // COMMON_H
