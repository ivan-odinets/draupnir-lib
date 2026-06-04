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
 *  @brief Checks whether a type is a supported built-in integer type.
 *
 *  @tparam T Type to check.
 *
 *  @details This trait evaluates to `true` for built-in integral arithmetic types, excluding `bool` and character types that
 *           are usually used as text/code-point types rather than numeric storage.
 *
 *           Accepted examples: `char`, `signed char`, `unsigned char`, `short`, `unsigned short`, `int`, `unsigned int`, `long`,
 *           `unsigned long`, `long long`, `unsigned long long`.
 *
 *           Rejected examples: `bool`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`, `float`, `double`, enum types, user-defined
 *           wrappers.
 *
 * @note cv-qualification and references are ignored. */

template<typename T>
struct is_integer : std::bool_constant<
    std::is_integral_v<std::remove_cvref_t<T>> &&
    !std::is_same_v<std::remove_cvref_t<T>, bool> &&
    !std::is_same_v<std::remove_cvref_t<T>, wchar_t> &&
    !std::is_same_v<std::remove_cvref_t<T>, char8_t> &&
    !std::is_same_v<std::remove_cvref_t<T>, char16_t> &&
    !std::is_same_v<std::remove_cvref_t<T>, char32_t>
> {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref is_integer.
 *  @tparam T Type to check.
 *
 *  @details Provides direct access to the result of @ref is_integer as a `bool` constant */

template<typename T>
inline constexpr bool is_integer_v = is_integer<T>::value;

/*! @ingroup Utils
 *  @brief Type trait that checks whether `Candidate` is either an integer type or an enum type.
 *  @tparam Candidate Type to check.
 *
 *  @details The check is performed on `std::remove_cvref_t<Candidate>`, so cv-qualifiers and references are ignored. The trait
 *           evaluates to `true` for integer and enum types, and to `false` otherwise. */

template<typename Candidate>
struct is_enum_or_integer : public std::bool_constant<
    is_integer_v<std::remove_cvref_t<Candidate>> || std::is_enum_v<std::remove_cvref_t<Candidate>>
> {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref is_enum_or_integer.
 *  @tparam Candidate Type to check.
 *
 *  @details Equivalent to `is_enum_or_integer<Candidate>::value`. */

template<typename T>
inline constexpr bool is_enum_or_integer_v = is_enum_or_integer<T>::value;

/*! @ingroup Utils
 *  @brief Checks whether Candidate is a C-style array with elements of type Element.
 *  @tparam Candidate Type to check.
 *  @tparam Element Expected array element type. */

template<typename Candidate, typename Element>
struct is_c_array_of : public std::bool_constant<
    std::is_array_v<std::remove_cvref_t<Candidate>> &&
    std::is_same_v<std::remove_cv_t<std::remove_extent_t<std::remove_cvref_t<Candidate>>>,Element>
> {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref draupnir::utils::is_c_array_of.
 *  @tparam Candidate Type to check.
 *  @tparam Element Expected array element type. */

template<typename Candidate, typename Element>
inline constexpr bool is_c_array_of_v = is_c_array_of<Candidate, Element>::value;

};

#endif // TYPE_DETECTORS_H
