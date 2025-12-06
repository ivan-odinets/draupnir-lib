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

#ifndef TUPLE_LIKE_MERGE_H
#define TUPLE_LIKE_MERGE_H

#include <type_traits>

namespace draupnir::utils
{

/*! @class tuple_like_merge draupnir/utils/tuple_like_merge.h
 *  @brief Collects and flattens trait-like types from a mixed pack of types into a single output container, removing
 *         duplicates.
 *  @tparam TupleLike - A tuple-like class template of the form `template<class...> class TupleLike`. All nested “tuple-like”
 *                      structures that should be recursively flattened must be instantiations of this template (directly or
 *                      indirectly).
 *  @tparam OutputContainer - A class template of the form `template<class...> class OutputContainer` used to hold the resulting
 *                            list of collected trait types. It is expected to behave as a simple variadic type container (e.g.
 *                            a type list).
 *  @tparam Types... - A parameter pack of types to be inspected. Each type in @p Types... can be either:
 *                     - a “leaf” trait type, or
 *                     - an instantiation of @p TupleLike that itself contains trait types and/or further nested @p TupleLike
 *                       instantiations.
 *
 *  @details The metafunction performs a recursive, depth-first traversal over all types in @p Types..., as well as over any
 *           nested @p TupleLike instantiations found within them. During this traversal, it:
 *           - Treats any non-@p TupleLike type as a trait “leaf” and attempts to add it to the output container.
 *           - Treats any type of the form `TupleLike<Inner...>` as a tuple-like node and recursively processes all types in
 *             `Inner...` using the same rules.
 *
 *           At each step, the metafunction maintains an accumulator of already collected trait types represented as
 *           `OutputContainer<...>`. When a new candidate trait type is encountered, it is added to the accumulator only if it
 *           is not already present there. Presence is determined by exact type equality.
 *
 *           As a result, the nested `type` alias of this class template is:
 *           - an instantiation of @p OutputContainer with a variadic type list of all distinct trait types discovered while
 *             traversing @p Types... and any nested @p TupleLike instantiations; and
 *           - the order of types in the resulting @p OutputContainer reflects the order in which distinct trait types are
 *             first encountered during the traversal.
 *
 *           The recursion terminates when all provided types (and all nested @p TupleLike content) have been processed.
 *           There is no special handling for constraints or concepts: it is assumed that all nested @p TupleLike instantiations
 *           are well formed and that @p OutputContainer can be instantiated with the final list of collected trait types. */

template<template<class...> class TupleLike, template<class...> class OutputContainer, class... Types>
class tuple_like_merge
{
private:
    /*! @brief Trait that checks whether a type @p T is already present within a variadic type container @p Pack.
     *  @tparam T    Type to search for.
     *  @tparam Pack A class template specialization of the form `Pack<Us...>`, where `Us...` is a variadic list of types.
     *  @details This helper metafunction evaluates to `std::true_type` if @p T is exactly equal to at least one of the types in
     *           `Us...`, and to `std::false_type` otherwise. It is used internally to enforce uniqueness of collected trait
     *           types in the accumulator container. */
    template<typename T, typename Pack>
    struct contains;

    template<typename T, template<class...> class Pack, class... Us>
    struct contains<T, Pack<Us...>> :
        std::bool_constant<(std::is_same_v<T, Us> || ...)> {};

    /*! @brief Appends a type @p T to a variadic type container @p Pack only if @p T is not already present in that container.
     *  @tparam Pack A class template specialization of the form `Pack<Us...>`, representing the current accumulator.
     *  @tparam T    Candidate type to be appended.
     *  @details If @p T is already present within `Pack<Us...>` according to @ref tuple_like_merge::contains, the resulting
     *           `type` alias is simply `Pack<Us...>`. Otherwise, the resulting `type` alias is `Pack<Us..., T>`. This helper
     *           is the core mechanism for ensuring that the final output contains only unique trait types. */
    template<typename Pack, typename T>
    struct push_unique;

    template<template<class...> class Pack, class... Us, class T>
    struct push_unique<Pack<Us...>, T> {
        using type = std::conditional_t<
            (contains<T, Pack<Us...>>::value),
            Pack<Us...>,
            Pack<Us..., T>
        >;
    };

    /*! @brief Processes a single type @p T and updates the accumulator @p Acc accordingly, potentially flattening nested
     *         tuple-like structures.
     *  @tparam _TupleLike   Tuple-like class template used to detect and handle nested structures.
     *  @tparam Out          Output container class template (same role as @p OutputContainer).
     *  @tparam Acc          Current accumulator type, expected to be of the form `Out<...>`.
     *  @tparam T            Type to be processed.
     *  @details The primary template handles the case where @p T is not an instantiation of @_TupleLike. In that case, @p T
     *           is treated as a trait “leaf” and is fed into @ref tuple_like_merge::push_unique to be conditionally appended
     *           to the accumulator.
     *
     *           A partial specialization (defined below) handles the case where @p T is of the form `_TupleLike<Inner...>`.
     *           In that case, the type is flattened by delegating to @ref tuple_like_merge::flatten_pack over the `Inner...`
     *           pack, thereby recursively collecting trait types from nested tuple-like structures. */
    template<template<class...> class _TupleLike, template<class...> class Out, class Acc, class T >
    struct flatten_one;

    template<template<class...> class _TupleLike, template<class...> class Out, class... Us, class T >
    struct flatten_one<_TupleLike, Out, Out<Us...>, T> {
        using type = typename push_unique<Out<Us...>, T>::type;
    };

    /*! @brief Recursively processes a variadic pack of types @p Ts... and accumulates the result into @p Acc.
     *  @tparam _TupleLike   Tuple-like class template used for detecting nested tuple-like instances.
     *  @tparam Out          Output container class template (same role as @p OutputContainer).
     *  @tparam Acc          Current accumulator type, expected to be of the form `Out<...>`.
     *  @tparam Ts...        Types to be processed.
     *  @details This helper metafunction is a variadic “fold” over @p Ts..., where each element in the pack is processed via
     *           @ref tuple_like_merge::flatten_one. The base case is an empty parameter pack @p Ts..., in which case `type` is
     *           simply @p Acc. The recursive case processes the first type @p Head, obtains an updated accumulator type, and
     *           then recurses on the remaining types @p Tail.... */
    template<template<class...> class _TupleLike, template<class...> class Out, class Acc, class... Ts >
    struct flatten_pack;

    template<template<class...> class _TupleLike, template<class...> class Out, class Acc>
    struct flatten_pack<_TupleLike, Out, Acc> {
        using type = Acc;
    };

    template<template<class...> class _TupleLike, template<class...> class Out, class Acc, class Head, class... Tail>
    struct flatten_pack<_TupleLike, Out, Acc, Head, Tail...>
    {
        using AfterHead = typename flatten_one<_TupleLike, Out, Acc, Head>::type;

        using type = typename flatten_pack<_TupleLike, Out, AfterHead, Tail...>::type;
    };

    template<template<class...> class _TupleLike, template<class...> class Out, class... Us, class... Inner>
    struct flatten_one<_TupleLike, Out, Out<Us...>, _TupleLike<Inner...>>
    {
        using type = typename flatten_pack<_TupleLike, Out, Out<Us...>, Inner...>::type;
    };

    /*! @brief Specialization of @ref tuple_like_merge::flatten_one for the case where the processed type is a tuple-like
     *         instantiation.
     *  @tparam _TupleLike   Tuple-like class template being matched.
     *  @tparam Out          Output container class template.
     *  @tparam Us...        Already accumulated trait types stored in `Out<Us...>`.
     *  @tparam Inner...     Types contained within the tuple-like instantiation
     *                      `_TupleLike<Inner...>`.
     *  @details When a type of the form `_TupleLike<Inner...>` is encountered, this specialization does not treat it as
     *           a leaf. Instead, it delegates to @ref tuple_like_merge::flatten_pack with the current accumulator and the
     *           `Inner...` types, effectively flattening nested tuple-like layers and continuing trait collection
     *           recursively. */
public:

    /*! @brief The final collected type.
     *  @details This alias represents the end result of the entire collection process. It is an instantiation of @p
     *           OutputContainer parameterized with all distinct trait types discovered while recursively traversing @p
     *           Types... and any nested @p TupleLike instantiations they may contain. */
    using type =
        typename flatten_pack<TupleLike, OutputContainer, OutputContainer<>, Types...>::type;
};

/*! @brief Convenience alias for @ref tuple_like_merge that directly exposes the nested @ref tuple_like_merge::type alias.
 *  @tparam TupleLike - Tuple-like class template used to identify and recursively flatten nested tuple-like structures.
 *  @tparam Out       - Output container class template into which all distinct trait types are collected.
 *  @tparam Ts...     - Types to be inspected, each of which may be either a trait type or an instantiation of @p
 *                      TupleLike containing further nested types.
 * @details This alias resolves to the same type as `typename tuple_like_merge<TupleLike, Out, Ts...>::type` and exists purely
 *          for syntactic convenience. */

template<template<class...> class TupleLike, template<class...> class Out, class... Ts>
using tuple_like_merge_t =
    typename tuple_like_merge<TupleLike, Out, Ts...>::type;

}; // namespace draupnir::utils

#endif // TUPLE_LIKE_MERGE_H
