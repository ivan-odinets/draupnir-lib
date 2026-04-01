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

#ifndef CONDITIONAL_TYPE_EXTRACTORS_H
#define CONDITIONAL_TYPE_EXTRACTORS_H

namespace draupnir::utils
{

/*! @file draupnir/utils/conditional_type_extractors.h
 *  @ingroup Utils
 *  @brief Small compile-time helpers for conditional type selection.
 *
 *  @details This file provides utility templates that return either a requested type or `void` depending on a compile-time
 *           condition. These helpers are intended for metaprogramming and SFINAE-style type deduction. */


/*! @struct type_or_void draupnir/utils/conditional_type_extractors.h
 *  @brief Returns `Type` if the boolean condition is `true`, otherwise returns `void`. The selected type is exposed through
 *         the nested alias `result`
 *  @tparam getOrNot Compile-time boolean controlling the resulting type.
 *  @tparam Type Type to return when `getOrNot == true`. */
template<bool getOrNot, class Type>
struct type_or_void;

/*! @brief Specialization of `type_or_void` returning `Type`. */
template<class Type>
struct type_or_void<true,Type> { using result = Type; };

/*! @brief Specialization of `type_or_void` returning `void`. */
template<class Type>
struct type_or_void<false,Type> { using result = void; };

/*! @brief Convenience alias for `type_or_void::result`.
 *  @tparam getOrNot Compile-time boolean controlling the resulting type.
 *  @tparam Type Type to return when `getOrNot == true`. */
template<bool getOrNot,class Type>
using type_or_void_t = type_or_void<getOrNot,Type>::result;

}; // namespace draupnir::utils

#endif // CONDITIONAL_TYPE_EXTRACTORS_H
