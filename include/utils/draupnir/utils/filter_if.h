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

#ifndef FILTER_IF_H
#define FILTER_IF_H

#include <type_traits>

namespace draupnir::utils
{

/*! @class filter_if draupnir/utils/filter_if.h
 *  @brief Compile-time filter for a variadic pack of types.
 *  @tparam Detector    Unary type trait / predicate of the form `template<class...> class Detector`. It must provide a
 *                      `Detector<T>::value` that is contextually convertible to `bool` for every `T` in @p Types....
 *  @tparam Types       Variadic list of types to be inspected.
 *
 *  @details This metafunction implements a generic "filter" over a parameter pack of types. Given a predicate @p Detector
 *           and a pack @p Types..., it produces, via the nested alias template @ref filter_if::toContainer, an instantiation
 *           of an arbitrary variadic class template @p Container that contains only those types @p T from @p Types... for
 *           which `Detector<T>::value` is `true`.
 *
 *           In other words, `filter_if<Detector, Types...>::toContainer<Container>` yields a type equivalent to: `Container<
 *           Ts... >` *  where `Ts...` is the subsequence of @p Types... that satisfies the predicate @p Detector.  */

template<template <class...> class Detector, class... Types>
class filter_if
{
private:
    /*! @brief Conditionally appends a type @p T to an accumulator @p Result.
     *  @tparam Result  Current accumulator, expected to be a class template specialization of the form `Pack<Us...>`, where
     *                  `Pack` is a variadic class template and `Us...` are the types collected so far.
     *  @tparam T       Candidate type to be appended.
     *
     *  @details The primary template is left undefined; a partial specialization (below) matches the case where @p Result
     *           is of the form `Pack<Us...>`. If `Detector<T>::value` is `true`, the resulting `type` alias is `Pack<Us...,
     *           T>`. Otherwise, the resulting `type` alias is simply `Pack<Us...>`.
     *
     *           This helper is the core mechanism that enforces the filtering logic inside @ref merge_if_true. */
    template<typename Result, typename T>
    struct push_if_true;

    /*! @brief Specialization of @ref push_if_true for accumulator types of the form `Pack<Us...>`.
     *  @tparam Pack    Variadic class template used as the accumulator container.
     *  @tparam Us      Types already stored in the accumulator.
     *  @tparam T       Candidate type to be appended if it satisfies @p Detector. */
    template<template<class...> class Pack, class... Us, class T>
    struct push_if_true<Pack<Us...>, T> {
        using type = std::conditional_t<
            (Detector<T>::value),
            Pack<Us..., T>,
            Pack<Us...>
        >;
    };

    /*! @brief Recursive fold over a variadic pack of types @p Ts..., accumulating filtered types into @p Result.
     *  @tparam Container    Variadic class template that represents the target container (e.g. `std::tuple`).
     *  @tparam Result       Current accumulator type, expected to be an instantiation of @p Container.
     *  @tparam Ts           Remaining types to be processed.
     *
     *  @details This helper metafunction implements the recursive step of the filtering algorithm:
     *           - The base case (empty @p Ts...) simply exposes @p Result as its nested `type`.
     *           - The recursive case processes the first type @p Head by feeding it into @ref push_if_true, obtaining
     *             an updated accumulator `before`. It then recurses on the remaining types @p Tail... with this updated
     *             accumulator. */
    template<template<class...> class Container, class Result, class... Ts>
    struct merge_if_true;

    /*! @brief Base case of @ref merge_if_true for an empty parameter pack.
     *  @tparam Container    Target container class template (unused in the base case).
     *  @tparam Result       Final accumulator type.
     *
     *  @details When there are no more types to process, the recursion terminates and `type` is simply aliased to @p
     *           Result. */
    template<template<class...> class Container, class Result>
    struct merge_if_true<Container,Result> {
        using type = Result;
    };

    /*! @brief Recursive case of @ref merge_if_true for a non-empty parameter pack.
     *  @tparam Container    Target container class template.
     *  @tparam Result       Current accumulator type, expected to be `Container<...>`.
     *  @tparam Head         First type in the remaining pack to be processed.
     *  @tparam Tail         Remaining types to be processed after @p Head.
     *
     *  @details This specialization processes @p Head by passing it, together with the current accumulator @p Result, to
     *           @ref push_if_true. The resulting type @p before becomes the new accumulator, and the recursion continues
     *           with @p Tail... .*/
    template<template<class...> class Container, class Result, class Head, class... Tail>
    struct merge_if_true<Container, Result,Head,Tail...> {
        using before = typename push_if_true<Result,Head>::type;
        using type = typename merge_if_true<Container,before,Tail...>::type;
    };

public:
    /*! @brief Applies the filter and collects the result into a given variadic container template.
     *  @tparam Container    Variadic class template that will hold the filtered types (e.g. `std::tuple`, `std::variant`,
     *                       or any user-defined `template<class...> class`).
     *
     *  @details This alias template exposes the final result of the filtering operation, starting from an empty instantiation
     *           of @p Container and folding over all @p Types... via @ref merge_if_true.
     *
     *           In more concrete terms: `using Result = filter_if<Detector, Types...>::toContainer<Container>;` is equivalent
     *           to: `using Result = Container< Ts... >;` only those Ts from Types... where Detector<Ts>::value is `true`. */

    template<template<class...> class Container>
    using to_container= typename merge_if_true<Container,Container<>,Types...>::type;
};

}; // namespace draupnir::utils

#endif // FILTER_IF_H
