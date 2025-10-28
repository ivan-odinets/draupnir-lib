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

/*! @file draupnir/utils/template_constructors.h
 *  @ingroup Utils
 *  @brief Utilities for generic, zero-initializing and dynamic construction of objects and tuples.
 *
 *  @details Provides templates to recursively zero-initialize pairs, tuples, pointers, and arithmetic types, as well as
 *           utilities to dynamically allocate tuples of objects by type. */

#include <type_traits>
#include "template_detectors.h"

namespace draupnir::utils
{

/*! @brief Constructs a "zero" value for the given type `T`.
 *  @ingroup Utils
 *  @tparam T The type to initialize.
 *  @return A "zero" value of type `T`, constructed as described above.
 *
 *  @details This utility returns a compile‑time–generated default/zero value appropriate for type `T`. The returned value
 *           depends on the type category:
 *           - **Pointer types** -> `nullptr`;
 *           - **Arithmetic types** -> `T{0}`;
 *           - **std::pair** -> both `.first` and `.second` initialized with `make_zero_value<>()`;
 *           - **std::tuple** -> all elements initialized recursively with `make_zero_value<>()`;
 *           - **Other types** → default‑constructed via `T{}`; */

template<typename T>
inline constexpr T make_zero_value() {
    if constexpr (std::is_pointer_v<T>) {
        return nullptr;
    } else if constexpr (std::is_arithmetic_v<T>) {
        return T{0};
    } else if constexpr (is_pair_v<T>) {
        return []<typename First, typename Second>(std::__type_identity<std::pair<First, Second>>) {
            return std::pair{make_zero_value<First>(), make_zero_value<Second>()};
        }(std::__type_identity<T>{});
    } else if constexpr (is_tuple_v<T>) {
        return []<typename... Ts>(std::__type_identity<std::tuple<Ts...>>) {
            return std::tuple{make_zero_value<Ts>()...};
        }(std::__type_identity<T>{});
    } else {
        return T{};
    }
}

/*! @brief Creates a tuple of dynamically allocated default-initialized objects.
 *  @ingroup Utils
 *  @tparam Tuple A tuple type whose elements are all pointer types.
 *  @return A tuple of dynamically allocated, default-initialized objects.
 *
 *  @details Given a tuple type where each element is a pointer type (e.g., `std::tuple<int*, double*>`), this function
 *           returns a tuple of pointers, where each pointer points to a new default-constructed instance of the pointed-to
 *           type.
 *
 *           Memory is allocated using `new`, and it is the caller's responsibility to delete the objects afterwards.
 *
 * @warning static_assert if `Tuple` is not a `std::tuple`.
 *
 * @todo static_assert when Tuple contains non-pointer types. */

template<typename Tuple>
inline Tuple create_tuple_new() {
    static_assert(is_tuple_v<Tuple>, "Provided type is not a tuple!");

    static constexpr auto tupleSize = std::tuple_size_v<Tuple>;
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::tuple{
            new std::remove_pointer_t<
                std::tuple_element_t<I, Tuple>
            >()...
        };
    }(std::make_index_sequence<tupleSize>{});
}

}; // draupnir::utils

#endif // TEMPLATE_CONSTRUCTORS_H
