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

/*! @struct is_pair_of_ptr draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait to detect if provided `std::pair` instance contains both pointers, general case. Returns false. */

template<typename... Ts>
struct is_pair_of_ptr : std::false_type {};

/*! @struct is_pair_of_ptr draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait to detect if provided `std::pair` instance contains both pointers.
 *
 *  @details This trait inherits from `std::true_type` if the given type `T` is a specialization of `std::pair<First,Second>`,
 *           and both First and Second are pointers. Inherits from `std::false_type` otherwise. */

template<typename First,typename Second>
struct is_pair_of_ptr<std::pair<First,Second>> :
    std::conditional_t<
        std::is_pointer_v<First> && std::is_pointer_v<Second>,
        std::true_type,
        std::false_type
    > {};

/*! @ingroup Utils
 *  @brief Convenience constexpr variable for @ref draupnir::utils::is_pair_of_ptr. Evaluates to `true` if `T` is a
 *         `std::pair`, which consists only from pointers. `false` otherwise. */

template<typename T>
inline constexpr bool is_pair_of_ptr_v = is_pair_of_ptr<T>::value;

/*! @struct is_tuple_ptr_only draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait to detect if provided `std::tuple` instance contains only pointers, general case. Returns false. */

template<typename... Ts>
struct is_tuple_ptr_only : std::false_type {};

/*! @struct is_tuple_ptr_only draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait to detect if provided std::tuple instance contains only pointers.
 *
 *  @details This trait inherits from `std::true_type` if the given type `T` is a specialization of `std::tuple<Args...>`,
 *           and all of the provided Args... are pointers. Inherits from `std::false_type` otherwise. */

template<typename... Ts>
struct is_tuple_ptr_only<std::tuple<Ts...>> :
    std::conditional_t<
        std::conjunction_v<std::is_pointer<Ts>...>,
        std::true_type,
        std::false_type
    > {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref draupnir::utils::is_tuple_ptr_only.
 *  @details Evaluates to `true` if `T` is a specialization of `std::tuple` whose element types are all pointer types. Evaluates
 *           to `false` for any other type, including non-`std::tuple` types and tuples that contain at least one non-pointer
 *           element. */

template<typename T>
inline constexpr bool is_tuple_ptr_only_v = is_tuple_ptr_only<T>::value;

/*! @struct is_tuple_like_pointees_default_constructible draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait that checks whether a “tuple-like” class-template instantiation is parameterized only with pointer types
 *         whose pointees are default-constructible.
 *  @tparam Container Type to be inspected.
 *
 *  @details Primary template: yields `std::false_type` for all types. A partial specialization is provided for class-template
 *           instantiations of the form `ContainerTemplate<Ts...>`, which is intended to be used with tuple-like types such as
 *           `std::tuple<...>`, `std::pair<...>`, `QPair<...>` and similar.
 *
 *           The specialization evaluates to `std::true_type` if and only if:
 *           - each type in `Ts...` is a pointer type, and
 *           - for each pointer type `P` in `Ts...`, the pointee type `std::remove_pointer_t<P>` is `std::is_default_constructible_v`.
 *
 *           In all other cases this trait evaluates to `std::false_type`. */

template<class Container>
struct is_tuple_like_pointees_default_constructible : std::false_type {};

/*! @struct is_tuple_like_pointees_default_constructible draupnir/utils/template_detectors.h
 *          `QPair`, or a custom tuple-like type).
 *  @ingroup Utils
 *  @brief Partial specialization of @ref draupnir::utils::is_tuple_like_pointees_default_constructible for class-template
 *         instantiations.
 *  @tparam ContainerTemplate Class template that accepts one or more type parameters (for example `std::tuple`, `std::pair`,
 *  @tparam Ts... The template argument types used to instantiate @p ContainerTemplate. */

template<template <class...> class ContainerTemplate,class... Ts>
struct is_tuple_like_pointees_default_constructible<ContainerTemplate<Ts...>> :
    std::bool_constant<
        ((std::is_pointer_v<Ts> && std::is_default_constructible_v<std::remove_pointer_t<Ts>>) && ...)
    > {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref draupnir::utils::is_tuple_like_pointees_default_constructible.
 *  @details Evaluates to `true` if @p Container is a class-template instantiation that satisfies the pointee default-constructibility
 *           requirements described in @ref draupnir::utils::is_tuple_like_pointees_default_constructible. `false` otherwise. */

template<typename Container>
inline constexpr bool is_tuple_like_pointees_default_constructible_v = is_tuple_like_pointees_default_constructible<Container>::value;

/*! @struct is_container_pointees_default_constructible draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait that checks whether a “container-like” class-template instantiation has a pointer value type whose pointee is
 *         default-constructible.
 *  @tparam Container Type to be inspected.
 *
 *  @details Primary template: yields `std::false_type` for all types. A partial specialization is provided for class-template
 *           instantiations of the form `ContainerTemplate<First, Ts...>`. It is intended to be used with standard container
 *           types such as `std::vector<T*>`, `std::list<T*>`, `std::set<T*>`, etc., where the first template parameter models
 *           the “value type” of the container. */

template<class Container>
struct is_container_pointees_default_constructible : std::false_type {};

/*! @struct is_container_pointees_default_constructible draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Partial specialization of @ref draupnir::utils::is_container_pointees_default_constructible for class-template instantiations
 *         where the first template parameter represents the element/value type.
 *  @tparam ContainerTemplate Class template that accepts one or more type parameters (for example `std::vector`, `std::list`, `std::set`,
 *          or a similar container).
 *  @tparam First The first template argument of @p ContainerTemplate, interpreted as the container’s element/value type.
 *  @tparam Ts... Remaining template arguments (allocators, comparators, traits, etc.).
 *
 *  @details This specialization evaluates to: `std::true_type` if and only if:
 *           - `First` is a pointer type, and the pointee type `std::remove_pointer_t<First>` is default-constructible (i.e.
 *             `std::is_default_constructible_v<std::remove_pointer_t<First>>` is `true`).
 *           `std::false_type` in all other cases.
 *
 *  @note Note that the additional template parameters in `Ts...` are intentionally ignored. Only the first template parameter
 *        (the “value type”) participates in this check. */

template<template <class...> class ContainerTemplate,class First, class... Ts>
struct is_container_pointees_default_constructible<ContainerTemplate<First,Ts...>> :
    std::bool_constant<
        (std::is_pointer_v<First> && std::is_default_constructible_v<std::remove_pointer_t<First>>)
      > {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref draupnir::utils::is_container_pointees_default_constructible.
 *  @details Evaluates to `true` if @p Container satisfies the value-type pointee default-constructibility requirements described
 *           in @ref draupnir::utils::is_container_pointees_default_constructible. `false` otherwise. */

template<class Container>
inline constexpr bool is_container_pointees_default_constructible_v =
    is_container_pointees_default_constructible<Container>::value;

/*! @struct is_collection_pointees_default_constructible draupnir/utils/template_detectors.h
 *  @ingroup Utils
 *  @brief Type trait that combines tuple-like and container-like pointee checks.
 *  @tparam Container Type to be inspected.
 *
 *  @details Primary template: yields `std::false_type` for all types. A partial specialization is provided for class-template
 *           instantiations of the form `ContainerTemplate<Ts...>`. It evaluates to `std::true_type` if either:
 *           - @ref draupnir::utils::is_tuple_like_pointees_default_constructible reports `true` for the given instantiation,
 *             or
 *           - @ref draupnir::utils::is_container_pointees_default_constructible reports `true` for the same instantiation.
 *
 *           In all other cases this trait evaluates to `std::false_type`. Conceptually, this answers the question: “Does this
 *           collection-like type use pointer types (either as tuple-like elements or as container value type) whose pointees
 *           are default-constructible?” */

template<class Container>
struct is_collection_pointees_default_constructible : std::false_type {};

/*! @struct is_collection_pointees_default_constructible draupnir/utils/template_detectors.h
 *  @brief Partial specialization of @ref draupnir::utils::is_collection_pointees_default_constructible for class-template
 *         instantiations.
 *  @tparam ContainerTemplate Class template that accepts one or more type parameters.
 *  @tparam Ts... Template argument types used to instantiate @p ContainerTemplate. */

template<template <class...> class ContainerTemplate,class... Ts>
struct is_collection_pointees_default_constructible<ContainerTemplate<Ts...>> :
    std::conditional_t<
        (is_tuple_like_pointees_default_constructible<ContainerTemplate<Ts...>>::value
         || is_container_pointees_default_constructible<ContainerTemplate<Ts...>>::value),
        std::true_type,
        std::false_type
    > {};

/*! @ingroup Utils
 *  @brief Convenience variable template for @ref draupnir::utils::is_collection_pointees_default_constructible.
 *  @details Evaluates to `true` if @p Container satisfies either the tuple-like or container-like pointee default-constructibility
 *           requirements, as described in @ref draupnir::utils::is_collection_pointees_default_constructible. `false` otherwise. */

template<class Container>
inline constexpr bool is_collection_pointees_default_constructible_v =
    is_collection_pointees_default_constructible<Container>::value;

}; // namespace draupnir::utils

#endif // TEMPLATE_DETECTORS_H
