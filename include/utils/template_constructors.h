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

#ifndef TEMPLATE_CONSTRUCTORS_H
#define TEMPLATE_CONSTRUCTORS_H

/*! @file template_constructors.h draupnir-lib/include/utils/template_constructors.h
 *  @brief Utilities for generic, zero-initializing and dynamic construction of objects and tuples.
 *  @details Provides templates to recursively zero-initialize pairs, tuples, pointers, and arithmetic types, as well as
 *           utilities to dynamically allocate tuples of objects by type. */

#include <type_traits>

#include "common.h"

/*! @brief Returns a "zero" value for the given type T.
 *  @details This is done in the following way:
 *           - If T == pointer -> nullptr;
 *           - If T == arithmetic type -> 0;
 *           - If T == std::pair - initizlizes both fields of pair with make_zero_value<T>();
 *           - If T == std::tuple - initializes each element of the tuple with make_zero_value<T>();
 *         - If T == other object -> default constructor is called WITHOUT any arguments;
 *  @tparam T - the type to initialize.
 *  @return A "zero" value for the specified type.*/
template<class T>
constexpr T make_zero_value() {
    if constexpr (std::is_pointer_v<T>) {
        return nullptr;
    } else if constexpr (std::is_arithmetic_v<T>) {
        return T{0};
    } else if constexpr (is_pair_v<T>) {
        return []<typename First,typename Second>(std::__type_identity<std::pair<First,Second>>){
            return std::pair{make_zero_value<First>(),make_zero_value<Second>()};
        }(std::__type_identity<T>{});
    } else if constexpr (is_tuple_v<T>) {
        return []<typename... Ts>(std::__type_identity<std::tuple<Ts...>>){
                    return std::tuple{make_zero_value<Ts>()...};
                }(std::__type_identity<T>{});
    } else {
        return T{};
    }
}

/*! @brief Creates and returns a tuple of dynamically allocated objects.
 *  @details For a tuple type of pointers (e.g., std::tuple<int*, double*>), this function uses `new` to allocate
 *           a default-initialized object for each tuple element and returns a tuple of pointers to these objects.
 *  @tparam Tuple - a std::tuple type where each element is a pointer type.
 *  @return Tuple of pointers, each pointing to a newly allocated, default-constructed object of the corresponding type.
 * @note The caller is responsible for deleting the allocated objects to avoid memory leaks.
 * @throws static_assert if Tuple is not a std::tuple type. */
template<class Tuple>
static Tuple create_tuple_new() {
    static_assert(is_tuple_v<Tuple>,"Provided type is not a tuple!");

    static constexpr auto tupleSize = std::tuple_size_v<Tuple>;
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::tuple{
            new std::remove_pointer_t<
                std::tuple_element_t<I, Tuple>
            >()...
        };
    }(std::make_index_sequence<tupleSize>{});
};

#endif // TEMPLATE_CONSTRUCTORS_H
