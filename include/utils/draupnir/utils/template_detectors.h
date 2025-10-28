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

#ifndef TEMPLATE_DETECTORS_H
#define TEMPLATE_DETECTORS_H

/*! @file draupnir/utils/template_detectors.h
 *  @brief This file contains utilities to check if specified type is one of template instanciations. */

#include <tuple>
#include <type_traits>

namespace draupnir::utils
{

/*! @struct is_pair draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait to detect whether a type is a `std::pair`.
 *  @tparam T  The type to check.
 *
 *  @details This trait inherits from `std::true_type` if the given type `T` is a specialization of `std::pair<First, Second>`,
 *           and from `std::false_type` otherwise. */

template<typename T>
struct is_pair : std::false_type {};

/*! @struct is_pair draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Specialization for `std::pair`.
 *
 *  @details Matches any `std::pair<First, Second>` and inherits from `std::true_type`. */

template<typename First, typename Second>
struct is_pair<std::pair<First, Second>> : std::true_type {};

/*! @ingroup Utils
 *  @brief Convenience constexpr variable for @ref draupnir::utils::is_pair. Evaluates to `true` if `T` is a `std::pair`,
 *         `false` otherwise. */

template<typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

/*! @struct is_tuple draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait to detect whether a type is a `std::tuple`.
 *  @tparam T  The type to check.
 *
 *  @details This trait inherits from `std::true_type` if the given type `T` is a specialization of `std::tuple<Args...>`,
 *           and from `std::false_type` otherwise.  */

template<typename T>
struct is_tuple : std::false_type {};

/*! @struct is_tuple draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Specialization for `std::tuple`.
 *
 *  @details Matches any `std::tuple<Args...>` and inherits from `std::true_type`. */

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/*! @ingroup Utils
 *  @brief Convenience constexpr variable for @ref draupnir::utils::is_tuple. Evaluates to `true` if `T` is a `std::tuple`,
 *         `false` otherwise. */

template<typename T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

}; // namespace draupnir::utils

#endif // TEMPLATE_DETECTORS_H
