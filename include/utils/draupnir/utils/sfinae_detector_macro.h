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

#ifndef SFINAE_DETECTOR_MACRO_H
#define SFINAE_DETECTOR_MACRO_H

#include <type_traits>
#include <utility>

/*! @file draupnir/utils/sfinae_detector_macro.h
 *  @ingroup Utils
 *  @brief Preprocessor helpers for generating simple SFINAE-based detectors. */

/*! @def DEFINE_HAS_NESTED_TYPE(NestedType)
 *  @ingroup Utils
 *  @brief Generates a trait that detects presence of a nested type `T::NestedType`.
 *
 *  @details Expands to:
 *           - `template<class T, class = std::void_t<>> struct has_NestedType : std::false_type {};`
 *           - `template<class T> struct has_NestedType<T, std::void_t<typename T::NestedType>> : std::true_type {};`
 *           - `template<class T> inline constexpr bool has_NestedType_v = has_NestedType<T>::value;`
 *
 *           Usage example:
 *           `DEFINE_HAS_NESTED_TYPE(SettingsBundle);`
 *           gives you `has_SettingsBundle<T>` / `has_SettingsBundle_v<T>`. */

#define DEFINE_HAS_NESTED_TYPE(NestedType)                                               \
    template<class T, class = std::void_t<>>                                             \
    struct has_##NestedType : std::false_type {};                                        \
                                                                                         \
    template<class T>                                                                    \
    struct has_##NestedType<T, std::void_t<typename T::NestedType>> : std::true_type {}; \
                                                                                         \
    template<class T>                                                                    \
    static inline constexpr bool has_##NestedType##_v = has_##NestedType<T>::value;

/*! @def DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(NestedType)
 *  @brief Generates a metafunction that extracts `T::NestedType` or yields `void` if absent.
 *
 *  @details Expands to:
 *           - `template<class T, class = std::void_t<>> struct extract_NestedType_or_void { using type = void; };`
 *           - `template<class T> struct extract_NestedType_or_void<T, std::void_t<typename T::NestedType>> { using type
 *              = typename T::NestedType; };`
 *           - `template<class T> using extract_NestedType_or_void_t = typename extract_NestedType_or_void<T>::type;`
 *           Useful when you want to read a nested type if it exists, but remain SFINAE-friendly otherwise. */

#define DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(NestedType)                                  \
    template<class T, class = std::void_t<>>                                            \
    struct extract_##NestedType##_or_void { using type = void; };                       \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##NestedType##_or_void<T,std::void_t<typename T::NestedType>> {      \
        using type = typename T::NestedType; };                                         \
                                                                                        \
    template<class T>                                                                   \
    using extract_##NestedType##_or_void_t =                                            \
        typename extract_##NestedType##_or_void<T>::type;

#endif // SFINAE_DETECTOR_MACRO_H
