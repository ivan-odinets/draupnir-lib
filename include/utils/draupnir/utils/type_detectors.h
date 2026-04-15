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

#ifndef TYPE_DETECTORS_H
#define TYPE_DETECTORS_H

#include <type_traits>

/*! @file draupnir/utils/type_detectors.h
 *  @ingroup Utils
 *  @brief This file contains helper type traits and variable templates used to classify C++ types at compile time. */

namespace draupnir::utils
{

/*! @struct is_integer draupnir/utils/type_detectors.h
 *  @ingroup Utils
 *  @brief Checks whether a type is one of the supported built-in integer types.
 *  @tparam T Type to check.
 *
 *  @details This trait evaluates to `true` only for the explicitly listed built-in integer types: `char`, `unsigned char`,
 *           `short`, `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long` `unsigned long long`.
 *
 *           Types such as `bool`, wide/UTF character types, floating-point types, enumerations, and user-defined integer-like
 *           wrappers are not considered integers by this trait. */

template<typename T>
struct is_integer : std::bool_constant<
    std::is_same_v<T,char> || std::is_same_v<T,unsigned char> ||
    std::is_same_v<T,short> || std::is_same_v<T,unsigned short> ||
    std::is_same_v<T,int> || std::is_same_v<T,unsigned int> ||
    std::is_same_v<T,long> || std::is_same_v<T,unsigned long> ||
    std::is_same_v<T,long long> || std::is_same_v<T,unsigned long long>
> {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref is_integer.
 *  @tparam T Type to check.
 *
 *  @details Provides direct access to the result of @ref is_integer as a `bool` constant */

template<typename T>
inline constexpr bool is_integer_v = is_integer<T>::value;


};

#endif // TYPE_DETECTORS_H
