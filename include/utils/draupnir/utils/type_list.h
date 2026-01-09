/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef TYPE_LIST_H
#define TYPE_LIST_H

#include <iostream>

#include "draupnir/utils/class_marcos.h"
#include "draupnir/utils/index_of.h"
#include "draupnir/utils/template_adapters.h"
#include "draupnir/utils/type_if.h"
#include "draupnir/utils/type_presense.h"

namespace draupnir::utils
{

/*! @class type_list draupnir/utils/type_list.h
 *  @ingroup Utils
 *  @brief Compile-time container representing an ordered list of types.
 *  @tparam Ts...  Parameter pack of types stored in the list.
 *
 *  @details @ref type_list is a purely compile-time abstraction that models a simple sequence (list) of types. It provides a set
 *           of meta-functions and convenience aliases for querying, transforming and filtering the contained types, similarly to
 *           how one would use algorithms on a runtime container.
 *
 *           The class is non-instantiable and non-copyable: it is intended to be used only as a type-level "handle" with nested
 *           aliases and constexpr values. Example usage patterns include:
 *           - membership and index queries (`contains_v`, `index_of_v`);
 *           - element access by index (`get_t`);
 *           - structural modifications such as appending or prepending types (`push_back_t`, `push_front_t`);
 *           - removal and filtering of types based on a predicate (`remove_if_t`, `remove_all_t`, `filter_if_t`);
 *           - transformation of all elements via a meta-function (`transform_t`);
 *           - conversion to other variadic class templates (`convert_to_t`);
 *           - deduplication of the list (`unique_types_t`).
 *
 *           Internally, several helper metafunctions (such as `get`, `remove_if`, `filter_if` and `unique_types`) are used to
 *           implement the public interface. These helpers are considered implementation details and are not intended to be used
 *           directly.
 *
 *  @note All operations are performed entirely at compile time and introduce no runtime overhead.
 *  @note The public interface follows the common `_t` / `_v` naming convention, mirroring the style of the standard library’s
 *        type traits (e.g. `std::is_same_v`, `std::remove_cv_t`). */

template<class... Ts>
class type_list
{
    DEFINE_COMPILE_TIME(type_list);

protected:
    /*! @brief Grant access to other @ref type_list instantiations for internal helpers. */
    template<class... Us>
    friend class type_list;

    /*! @struct contains
     *  @brief Implementation detail: wraps @ref draupnir::utils::is_one_of for the current parameter pack.
     *  @tparam Type Type to test for membership.
     *  @details This helper adapts the global @ref draupnir::utils::is_one_of trait to the context of the current @ref type_list
     *           instantiation. For a given `Type` it exposes a `value` member that is `true` if `Type` is one of `Ts...`,  and
     *           `false` otherwise. */
    template<class Type>
    struct contains : is_one_of<Type,Ts...> {};

    /*! @struct index_of_impl
     *  @brief Thin wrapper around @ref draupnir::utils::index_of_v , but providing static_assert within @ref type_list if there
     *         is no elements of type `Type` within `Ts...` pack. */
    template<class Type, class... Args>
    struct index_of_impl {
        static_assert(draupnir::utils::is_one_of_v<Type,Ts...>,
            "type_list<class... Ts>::index_of_impl - There is no Type within the Ts... pack.");

        static constexpr std::size_t value = draupnir::utils::index_of_v<Type,Args...>;
    };

    /*! @struct index_of_first_if
     *  @brief Implementation detail: finds the index of the first type satisfying a unary predicate.
     *  @tparam Condition Unary trait template of the form `template<class> class Condition`.
     *  @tparam Current   Current index in the recursion.
     *  @tparam Args...   Remaining types to be inspected.
     *  @details This metafunction performs a linear scan over a parameter pack, starting from the logical index `Current`. For
     *           each type `T` in `Args...` it evaluates `Condition<T>::value`:
     *           - if `true`, the current index is reported via the static constexpr member `value`;
     *           - otherwise, it recurses with `Current + 1` on the tail of the pack.
     *
     *           A specialization for an empty pack provides the recursion terminator and defines the fallback index (currently
     *           `0`). The public alias @ref type_list::index_of_first_if_v is built on top of this helper and always starts
     *           with `Current == 0`. */
    template<template<class> class Condition, std::size_t Current, class... Args>
    struct index_of_first_if;

    /*! @brief Base specialization of @ref index_of_first_if for an empty pack.
     *  @details When no types remain to be checked, `static_assert` happens. */
    template<template<class> class Condition, std::size_t Current>
    struct index_of_first_if<Condition,Current> {
        static_assert((Condition<Ts>::value || ...),
            "type_list<class... Ts>::index_of_first_if - None of the elements satisfies provided Condition.");

        static inline constexpr std::size_t value = 0;
    };

    /*! @brief Recursive specialization of @ref index_of_first_if for a non-empty pack.
     *  @details If `Condition<First>::value` is `true`, the search stops and `value` is equal to `Current`. Otherwise, the search
     *           continues with `Rest...` and `Current + 1`. */
    template<template<class> class Condition, std::size_t Current, class First, class... Rest>
    struct index_of_first_if<Condition,Current,First,Rest...> {
        static inline constexpr std::size_t value = (Condition<First>::value) ?
            Current : index_of_first_if<Condition,Current+1,Rest...>::value;
    };

    /*! @struct count_if
     *  @brief Implementation detail: counts amount of elements for which Condition<Element>::value is evaluated to `true`. */
    template<template <class> class Condition, class... Args>
    struct count_if {
        static inline constexpr int value = 0;
    };

    /*! @struct count_if
     *  @brief Implementation detail: counts amount of elements for which Condition<Element>::value is evaluated to `true`. */
    template<template <class> class Condition, class First, class... Rest>
    struct count_if<Condition,First,Rest...> {
    private:
        static inline constexpr int _rest_value = count_if<Condition,Rest...>::value;
    public:
        static inline constexpr int value = (Condition<First>::value) ?
                _rest_value + 1 : _rest_value;
    };

    /*! @struct get
     *  @brief Implementation detail: recursively retrieves the type at the given index.
     *  @tparam Index   Target zero-based index.
     *  @tparam Current Current index in the recursion.
     *  @tparam Pack    Remaining types to traverse.
     *  @details The primary template and its specializations form a recursive metafunction that walks the parameter pack until
     *           `Current` equals `Index`, at which point the corresponding type is exposed via the nested alias `result`. If
     *           the recursion reaches the end of the pack without finding the requested index, a `static_assert` is triggered. */
    template<std::size_t Index, std::size_t Current, class... Pack>
    struct get;

    /*! @brief Specialization of @ref type_list::get when `Current` equals `Index`.
     *  @details When the desired position is reached, `result` is defined as the current head type. */
    template<std::size_t Index, class First, class... Rest>
    struct get<Index,Index,First,Rest...> { using result = First; };

    /*! @brief Recursive specialization of @ref type_list::get for the general case.
     *  @details Increments `Current` and recurses on the remaining types until the desired index is found. */
    template<std::size_t Index, std::size_t Current, class First, class... Rest>
    struct get<Index,Current,First,Rest...> {
        using result = typename get<Index,Current+1,Rest...>::result;
    };

    /*! @brief Terminal specialization of @ref type_list::get for an exhausted pack.
     *  @details Triggers a `static_assert` if the requested `Index` is out of range with respect to the number of types in this
     *           @ref type_list instantiation. */
    template<std::size_t Index, std::size_t Current>
    struct get<Index,Current> {
        static_assert(sizeof...(Ts) != 0,
            "type_list<class... Ts>::get - This is not supported for empty type_list.");
        static_assert(Index < sizeof...(Ts),
            "type_list<class... Ts>::get - Index out of range.");

        using result = void;
    };

    /*! @struct append
     *  @brief Implementation detail: appends a single type or another @ref type_list to the end of this list.
     *  @tparam Type Type to be appended.
     *  @details The primary template appends a single type `Type` to the existing `Ts...`, forming a new @ref type_list. A partial
     *           specialization is provided for the case where `Type` itself is a @ref type_list, in which case its elements are
     *           expanded and appended individually. */
    template<class Type>
    struct append { using result = type_list<Ts...,Type>; };

    /*! @brief Partial specialization of @ref type_list::append for appending another @ref type_list.
     *  @tparam Us... Types contained in the appended @ref type_list.
     *  @details Expands `Us...` and appends them to the current sequence `Ts...`. */
    template<class... Us>
    struct append<type_list<Us...>> { using result = type_list<Ts...,Us...>; };

    /*! @struct prepend
     *  @brief Implementation detail: prepends a single type or another @ref type_list to the front of this list.
     *  @tparam Type Type to be prepended.
     *  @details The primary template prepends a single type `Type` before the existing `Ts...`, forming a new @ref type_list. A
     *           partial specialization is provided for the case where `Type` is itself a @ref type_list, in which case its elements
     *           are expanded and placed before `Ts...`. */
    template<class Type>
    struct prepend { using result = type_list<Type,Ts...>; };

    /*! @brief Partial specialization of @ref type_list::prepend for prepending another type_list.
     *  @tparam Us... Types contained in the prepended @ref type_list.
     *  @details Expands `Us...  and places them in front of the current sequence `Ts...`. */
    template<class... Us>
    struct prepend<type_list<Us...>> { using result = type_list<Us...,Ts...>; };

    /*! @struct insert_before
     *  @brief Implementation detail: inserts a type `Type` into an accumulating output list before the element at a given index.
     *  @tparam Index   Zero-based index in the original @ref type_list at which `Type` should be inserted.
     *  @tparam Output  Accumulator list built so far (always an instantiation of @ref type_list).
     *  @tparam Type    Type to be inserted.
     *  @tparam Us...   Remaining types from the original parameter pack that have not yet been processed.
     *  @details This metafunction is used by the public alias @ref type_list::insert_before_t. Conceptually, it walks through the
     *           original sequence `Ts...`, copying elements into `Output` one by one until `Output::size_v == Index`. At that point,
     *           `Type` is placed in front of the remaining unprocessed elements.
     *
     *           - If the index is reached while there are still elements left to process, the resulting type is: `Output::append_t<
     *             type_list<Type, Remaining...> >`.
     *           - If the recursion reaches the end of the input sequence without ever seeing `Output::size_v == Index`, the base
     *             specialization triggers a `static_assert` (see below), signaling an out-of-range index.
     *
     *           Note that the index is interpreted relative to the original `Ts...` pack, and is required to be strictly less than
     *           the length of that pack (i.e. insertion at the logical end is not permitted). */
    template<std::size_t Index, class Output, class Type, class... Us>
    struct insert_before;

    /*! @brief Base specialization of @ref type_list::insert_before for an exhausted input pack.
     *  @details This specialization is instantiated only if the recursion has consumed all remaining input types without encountering
     *           a position where `Output::size_v` equals the requested `Index`.
     *
     *           In this case, the `static_assert` enforces that `Index` must be strictly less than the length of the original sequence,
     *           effectively rejecting out-of-range indices (including insertion "at the end"). */
    template<std::size_t Index, class Output, class Type>
    struct insert_before<Index,Output,Type> {
        static_assert((Index == 0 && sizeof...(Ts) == 0) || Index < sizeof...(Ts),
            "type_list<class... Ts>::insert_before - trying to insert element outside of list boundaries.");

        using result = type_list<Type>;
    };

    /*! @brief Recursive specialization of @ref type_list::insert_before.
     *  @details At each step:
     *           - If `Output::size_v == Index`, the metafunction stops consuming further
     *             input types and returns `Output::append_t< type_list<Type, First, Rest...> >`, which corresponds to inserting `Type`
     *             before the element that originally resided at position `Index`.
     *           - Otherwise, `First` is appended to `Output` and recursion continues with the remainder `Rest...`.
     *
     *           The recursion ultimately terminates either when the index is matched, or when the input pack is exhausted and the base
     *           specialization is reached. */
    template<std::size_t Index, class Output, class Type, class First, class... Rest>
    struct insert_before<Index,Output,Type,First,Rest...> {
        using result = std::conditional_t<
            Output::size_v == Index,
            typename Output::template append_t<type_list<Type,First,Rest...>>,
            typename insert_before<Index, typename Output::template push_back_t<First>, Type, Rest...>::result
        >;
    };

    /*! @struct remove_at
     *  @brief Implementation detail: removes the type at a given index from the list.
     *  @tparam Index   Zero-based index of the element to remove.
     *  @tparam Current Current index in the recursion.
     *  @tparam Us...   Remaining types to traverse.
     *  @details This metafunction walks through the sequence `Ts...` while tracking the current position `Current`. Once `Current ==
     *           Index`, the element at that position is skipped, and the remaining tail is preserved as-is. All other elements are
     *           copied into a new @ref type_list.
     *
     *           If the recursion reaches the end of the input pack without encountering `Index`, the base specialization triggers a
     *           `static_assert`, indicating that the index was out of range. */
    template<std::size_t Index, std::size_t Current, class... Us>
    struct remove_at;

    /*! @brief Base specialization of @ref type_list::remove_at for an exhausted pack.
     *  @details Instantiated only if the recursion has consumed all elements without matching `Index`, in which case `Index` is
     *           necessarily out of bounds. */
    template<std::size_t Index, std::size_t Current>
    struct remove_at<Index,Current> {
        static_assert(Index < sizeof...(Ts),"type_list<class... Ts>::remove_at - Index out of bounds.");

        using result = void;
    };

    /*! @brief Recursive specialization of @ref type_list::remove_at.
     *  @details At each step:
     *           - If `Index == Current`, the element `First` is removed by returning `type_list<Rest...>`, effectively dropping
     *             it from the resulting list.
     *           - Otherwise, `First` is preserved and prepended in front of the result produced by recursing on `Rest...` with
     *             `Current + 1`.
     *
     *             The recursion thus yields a new @ref type_list identical to the original, except that the element at position
     *             `Index` is omitted. */
    template<std::size_t Index,std::size_t Current, class First, class... Rest>
    struct remove_at<Index,Current,First,Rest...> {
        using result = std::conditional_t<
            Index == Current,
            type_list<Rest...>,
            typename type_list<First>::template append_t<typename remove_at<Index,Current+1,Rest...>::result>
        >;
    };

    /*! @struct remove_if
     *  @brief Implementation detail: removes all types that satisfy a unary type trait `Condition`.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition` that exposes a `value` convertible
     *          to `bool`.
     *  @tparam Pack         Types to be processed recursively.
     *  @details The metafunction traverses `Pack` and constructs a new @ref type_list by skipping all types `T` for which
     *           `Condition<T>::value` is `true`, while preserving those for which it is @c false. The result is exposed via the
     *           nested alias `result`. */
    template<template<class> class Condition, class... Pack>
    struct remove_if;

    /*! @brief Base specialization of @ref type_list::remove_if for an empty pack.
     *  @details When no types remain to be processed, the resulting list is the empty @ref type_list. */
    template<template<class> class Condition>
    struct remove_if<Condition> { using result = type_list<>; };

    /*! @brief Recursive specialization of @ref type_list::remove_if.
     *  @details If `First` satisfies `Condition`, it is discarded and recursion continues with `Rest...`. Otherwise, `First` is
     *           preserved and appended in front of the recursively processed remainder. */
    template<template<class> class Condition,class First, class... Rest>
    struct remove_if<Condition, First, Rest...> {
        using result = std::conditional_t<
            Condition<First>::value,
            typename remove_if<Condition, Rest...>::result,
            typename type_list<First>::template append_t<typename remove_if<Condition, Rest...>::result>
        >;
    };

    /*! @struct filter_if
     *  @brief Implementation detail: retains only those types that satisfy a unary type trait `Condition`.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition`.
     *  @tparam Pack         Types to be processed recursively.
     *  @details This metafunction is conceptually dual to @ref type_list::remove_if: it constructs a new @ref type_list consisting
     *           exclusively of types for which `Condition` evaluates to `true`. The resulting list is exposed via the nested alias
     *           `result`. */
    template<template<class> class Condition, class... Pack>
    struct filter_if;

    /*! @brief Base specialization of @ref type_list::filter_if for an empty pack. */
    template<template<class> class Condition>
    struct filter_if<Condition> { using result = type_list<>; };

    /*! @brief Recursive specialization of @ref type_list::filter_if.
     *  @details If `First` satisfies `Condition`, it is appended to the result and recursion continues on `Rest...`. Otherwise
     *           `First is skipped. */
    template<template<class> class Condition,class First, class... Rest>
    struct filter_if<Condition, First, Rest...> {
        using result = std::conditional_t<
            Condition<First>::value,
            typename type_list<First>::template append_t<typename filter_if<Condition, Rest...>::result>,
            typename filter_if<Condition, Rest...>::result
        >;
    };

    /*! @struct slice
     *  @brief Implementation detail: extracts a contiguous subrange of types as a @ref type_list.
     *  @tparam First    Zero-based index of the first element to keep (inclusive).
     *  @tparam Last     Zero-based index of the last  element to keep (inclusive).
     *  @tparam Current  Current index in the recursion while walking the parameter pack.
     *  @tparam Args...  Remaining types to traverse.
     *  @details This helper is used by the public alias @ref type_list::slice_t. Conceptually, it walks the pack `Args...` while
     *           maintaining a running index `Current`. For each element:
     *           - if `Current` is in the range [`First`, `Last`], that element is included into the resulting @ref type_list;
     *           - otherwise the element is skipped.
     *
     *           When the input pack is exhausted, the base specialization yields an empty @ref type_list, and the recursion folds
     *           back, building the final subrange in order. */
    template<std::size_t First, std::size_t Last, std::size_t Current, class... Args>
    struct slice;

    /*! @brief Base specialization of @ref slice for an exhausted input pack.
     *  @details When no types remain to be processed, the resulting subrange is the empty @ref type_list. */
    template<std::size_t First, std::size_t Last, std::size_t Current>
    struct slice<First,Last,Current> {
        static_assert(First <= sizeof...(Ts) && First <= Last && (Last == 0 || Last < sizeof...(Ts)),
            "type_list<class... Ts>::slice - [First,Last] should be within range of type_list indexes.");

        using result = type_list<>;
    };

    /*! @brief Recursive specialization of @ref slice that processes one element at a time.
     *  @tparam From     Lower bound of the slice (inclusive).
     *  @tparam To       Upper bound of the slice (inclusive).
     *  @tparam Current  Index of the current element being inspected.
     *  @tparam First    Current head type of the remaining input pack.
     *  @tparam Rest...  Tail of the remaining input pack.
     *  @details The recursion first computes the sliced tail for `Rest...` at `Current + 1`, and then:
     *           - if `Current` is within [`From`, `To`], it prepends `First` to the processed tail;
     *           - otherwise, it simply returns the processed tail unchanged.
     *
     *           This produces a @ref type_list containing exactly the types in positions [`From`, `To`] of the original pack, in
     *           their original order. */
    template<std::size_t From, std::size_t To, std::size_t Current, class First, class... Rest>
    struct slice<From,To,Current,First,Rest...> {
        using processed_tail = typename slice<From,To,Current+1,Rest...>::result;

        using result = typename type_if<Current >= From && Current <= To>
            ::template then_type<typename processed_tail::template push_front_t<First>>
            ::template else_type<processed_tail>
        ::result;
    };

    /*! @struct unique_types
     *  @brief Implementation detail: accumulates a list of unique types, preserving first occurrence order.
     *  @tparam Output  Current accumulator list.
     *  @tparam Us...   Remaining types to be processed.
     *  @details The metafunction iterates over `Us...` and appends a type `First` to the accumulator `Output` only if `Output`
     *           does not already contain `First`. The final result is exposed via the nested alias `result`. */
    template<class Output, class... Us>
    struct unique_types;

    /*! @brief Base specialization of @ref type_list::unique_types for an empty input pack.
     *  @details When no types remain to be processed, the accumulator `Output` itself is the result. */
    template<class Output>
    struct unique_types<Output> { using result = Output; };

    /*! @brief Recursive specialization of @ref type_list::unique_types.
     *  @details If `First` is already present in the accumulator `Output`, it is skipped and recursion continues with `Rest...`.
     *           Otherwise, `First` is appended to `Output` and the recursion proceeds with the updated accumulator. */
    template<class Output, class First, class... Rest>
    struct unique_types<Output, First,Rest...> {
        using result = std::conditional_t<
            Output::template contains_v<First>,
            typename unique_types<Output,Rest...>::result,
            typename unique_types<typename Output::template push_back_t<First>,Rest...>::result
        >;
    };

    /*! @struct is_subset_of
     *  @brief Implementation detail: checks whether this type sequence is a subset of another @ref type_list.
     *  @tparam Other     Candidate superset type. Must be an instantiation of @ref type_list.
     *  @tparam ThisArgs  Types from the “current” list being tested for subset relation.
     *  @details The primary template acts as a guard: it triggers a `static_assert` unless `Other` is an instantiation of @ref
     *           type_list. This enforces correct usage of the metafunction.
     *
     *           A partial specialization is provided for the case when `Other` is @ref type_List instantiation. In this case,
     *           the metafunction inherits from `std::bool_constant` whose `value` is `true` when every type in `ThisArgs...`
     *           is present in `OtherTs...` (treating the “current” @ref type_list as a subset of @p Other). For an empty
     *           `ThisArgs...` pack this condition evaluates to `true` as well, which matches the usual set-theoretic convention
     *           that the empty set is a subset of any set. */
    template<class Other, class... ThisArgs>
    struct is_subset_of : std::false_type {
        static_assert(is_instantiation_of_v<Other,type_list>,
            "type_list<class... Ts>::is_subset_of - Other must be instantiation of type_list<class... Ts> template.");
    };

    /*! @brief Partial specialization of @ref is_subset_of for `Other` being a @ref type_list.
     *  @tparam OtherTs   Types stored in the `Other` list.
     *  @tparam ThisArgs  Types from the “current” @ref type_list being tested.
     *  @details The metafunction inherits from `std::bool_constant` whose `value` equals to `type_list<OtherTs...>::template
     *           contains<ThisArgs>::value && ... )` i.e. all types in @p ThisArgs... must be contained in @p OtherTs.... */
    template<class... OtherTs,class... ThisArgs>
    struct is_subset_of<type_list<OtherTs...>,ThisArgs...> : std::bool_constant<
        (type_list<OtherTs...>::template contains<ThisArgs>::value && ...)
    > {};

    /*! @struct is_superset_of
     *  @brief Implementation detail: checks whether this type sequence is a superset of another @ref type_list.
     *  @tparam Other     Candidate subset type. Must be an instantiation of @ref type_list.
     *  @tparam ThisArgs  Types from the “curren"" list, i.e. the one being tested as a potential superset.
     *  @details The primary template acts as a guard: it triggers a `static_assert` unless `Other` is an instantiation of @ref
     *           type_list. This enforces correct usage of the metafunction.
     *
     *           A partial specialization is provided for the case when `Other` is a @ref type_list instantiation. In this case,
     *           the metafunction inherits from `std::bool_constant` whose `value` is `true` when every type in `OtherTs...` is
     *           present in `ThisArgs...` (treating the "current" @ref type_list as a superset of `Other`). In case when the
     *           `OtherTs...` pack is empty this condition evaluates to `true` as well, which matches the usual set-theoretic
     *           convention that the empty set is a subset of any set (and thus any list is its superset). */
    template<class Other, class... ThisArgs>
    struct is_superset_of : std::false_type {
        static_assert(is_instantiation_of_v<Other,type_list>,
            "type_list<class... Ts>::is_superset_of - Other must be instantiation of type_list<class... Ts> template.");
    };

    /*! @brief Partial specialization of @ref is_superset_of for `Other` being a @ref type_list.
     *  @tparam OtherTs   Types stored in the `Other` list (candidate subset).
     *  @tparam ThisArgs  Types from the “current” @ref type_list being tested as a superset.
     *  @details The metafunction inherits from `std::bool_constant` whose `value` equals `( type_list<ThisArgs...>::template
     *           contains<OtherTs>::value && ... )`, i.e. all types in `OtherTs...` must be contained in `ThisArgs...`. */
    template<class... OtherTs,class... ThisArgs>
    struct is_superset_of<type_list<OtherTs...>,ThisArgs...> : std::bool_constant<
        (type_list<ThisArgs...>::template contains<OtherTs>::value && ...)
    > {};

    /*! @struct reverse
     *  @brief Implementation detail: reverses the order of a pack of types into an accumulator.
     *  @tparam Us...  Types to be processed.
     *  @details This helper is parameterized by an accumulator type `Output` (always an instantiation of @ref type_list) followed
     *           by the remaining types to be reversed. The public alias @ref type_list::reverse_t uses it with an initially empty
     *           accumulator `type_list<>`.
     *
     *           Conceptually, the recursion proceeds from left to right over the input pack, while the reversed suffix is built
     *           via a combination of @ref type_list::push_back_t and @ref type_list::prepend_t. */
    template<class... Us>
    struct reverse;

    /*! @brief Base specialization of @ref type_list::reverse for an empty input.
     *  @tparam Output Accumulator list built so far.
     *  @details When no types remain to be processed, the accumulator `Output` is taken as the final reversed sequence. */
    template<class Output>
    struct reverse<Output> { using result = Output; };

    /*! @brief Recursive specialization of @ref type_list::reverse.
     *  @tparam Output  Accumulator list.
     *  @tparam First   Current head type in the remaining input.
     *  @tparam Rest... Remaining tail of the input.
     *  @details Each step constructs a new intermediate list by:
     *           - appending `First` to `Output`; and
     *           - prepending the recursively reversed tail `Rest...` (computed with the same accumulator `Output`).
     *
     *           When instantiated as `reverse<type_list<>, Ts...>`, this results in a list where the original sequence `Ts...`
     *           is fully reversed. */
    template<class Output, class First, class... Rest>
    struct reverse<Output, First, Rest...> {
        using result = typename Output::template push_back_t<First>::template prepend_t<typename reverse<Output,Rest...>::result>;
    };

    /*! @struct union_impl
     *  @brief Implementation detail: computes the set-theoretic union of two @ref type_list sequences.
     *  @tparam Other   Another type sequence, expected to be an instantiation of @ref type_list.
     *  @tparam ThisTs  Types from the “current” list participating in the union.
     *  @details The primary template is left undefined and is only specialized for the case when `Other` is a @ref type_list. The
     *           specialization takes all types from the “current” list (`ThisTs...`) and from `Other` (`OtherTs...`), concatenates
     *           them into a single pack, and then passes that pack into @ref unique_types with an initially empty accumulator.
     *
     *           The resulting type thus contains each distinct type from `ThisTs...` and `OtherTs...` at most once, preserving
     *           the order of their first appearance. Conceptually, this corresponds to the set-theoretic union of two type sets. */
    template<class Other,class... ThisTs>
    struct union_impl {
        static_assert(is_instantiation_of_v<Other,type_list>,
            "type_list<class... Ts>::union_impl - Other must be instantiation of type_list<class... Ts> template.");
    };

    /*! @brief Partial specialization of @ref union_impl for `Other` being a @ref type_list.
     *  @tparam OtherTs Types contained in the `Other` list.
     *  @tparam ThisTs  Types from the “current” list.
     *  @details Inherits from @ref unique_types instantiated with an empty accumulator and the concatenated parameter pack `ThisTs...,
     *           OtherTs...`. */
    template<class... OtherTs, class... ThisTs>
    struct union_impl<type_list<OtherTs...>,ThisTs...> : unique_types<type_list<>,ThisTs...,OtherTs...> {};

    /*! @struct intersection
     *  @brief Implementation detail: computes the set-theoretic intersection with another @ref type_list.
     *  @tparam Other   Candidate type sequence to intersect with. Must be an instantiation of @ref type_list.
     *  @tparam ThisTs  Types from the "current" list participating in the intersection.
     *  @details The primary template serves as a guard and is only valid when `Other` is a @ref type_list. Any other use triggers a
     *           `static_assert`, making misuse of the metafunction immediately visible. */
    template<class Other,class... ThisTs>
    struct intersection {
        static_assert(is_instantiation_of_v<Other,type_list>,
            "type_list<class... Ts>::intersection - Other must be instantiation of type_list<class... Ts> template.");
    };

    /*! @brief Base specialization of @ref intersection for an empty "current" list.
     *  @tparam OtherTs Types contained in the `Other` list.
     *  @details When there are no `ThisTs...` to process, the intersection is empty by definition, so the nested alias `result` is
     *           an empty @ref type_list. */
    template<class... OtherTs>
    struct intersection<type_list<OtherTs...>> {
        using result = type_list<>;
    };

    /*! @brief Recursive specialization of @ref intersection for non-empty "current" list.
     *  @tparam OtherTs       Types contained in the `Other` list.
     *  @tparam FirstThisTs   First type of the "current" list being inspected.
     *  @tparam RestThisTs... Remaining types of the "current" list.
     *  @details The recursion proceeds left-to-right over `FirstThisTs, RestThisTs...`. For each `FirstThisTs`:
     *           - it first computes the intersection of `RestThisTs...` with `type_list<OtherTs...>` into `tail`;
     *           - then, if `FirstThisTs` is contained in `OtherTs...` *and* is not yet present in `tail`, it is
     *             prepended to `tail`. Otherwise, `tail` is propagated unchanged.
     *
     *             This yields a deduplicated intersection that preserves the order of first appearance from the "current" list
     *             `ThisTs...`. */
    template<class... OtherTs,class FirstThisTs, class... RestThisTs>
    struct intersection<type_list<OtherTs...>,FirstThisTs,RestThisTs...> {
    private:
        using tail = typename intersection<type_list<OtherTs...>,RestThisTs...>::result;
    public:
        using result = typename type_if<
                type_list<OtherTs...>::template contains<FirstThisTs>::value && !tail::template contains<FirstThisTs>::value
            >::template then_type<typename tail::template push_front_t<FirstThisTs>>
             ::template else_type<tail>
        ::result;
    };

public:
    /*! @brief Number of types stored in this @ref type_list. */
    inline static constexpr std::size_t size_v = sizeof...(Ts);

    /*! @brief Indicates whether this @ref type_list is empty. */
    inline static constexpr bool is_empty_v = (sizeof...(Ts) == 0);

    /*! @brief Compile-time check for membership of a given type.
     *  @tparam Type Type to test for presence in the list.
     *  @details Evaluates to `true` if `Type` is exactly equal to at least one of `Ts...`, and to `false` otherwise. Internally,
     *           this relies on @ref draupnir::utils::is_one_of_v. */
    template<class Type>
    inline static constexpr bool contains_v = contains<Type>::value;

    /*! @brief Compile-time check for membership of a instantiation of a given template.
     *  @tparam Template     Template to test for presence in the list.
     *  @details Evaluates to `true` if there is present at least one instantiation of a given `Template` and to `false` otherwise.
     *           Internally, this relies on @ref draupnir::utils::is_template_instantiation_present_v. */
    template<template<class...> class Template>
    inline static constexpr bool contains_template_instantiation_v = is_template_instantiation_present_v<Template,Ts...>;

    /*! @brief Logical "any_of" over the list with respect to a unary type trait.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition`.
     *  @details Evaluates to `true` if `Condition<T>::value` is `true` for at least one type `T` among `Ts...`, and `false`
     *           otherwise. Internally uses `std::disjunction`. */
    template<template <class> class Condition>
    inline static constexpr bool any_of_v = std::disjunction<Condition<Ts>...>::value;

    /*! @brief Logical "all_of" over the list with respect to a unary type trait.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition`.
     *  @details Evaluates to `true` if `Condition<T>::value` is `true` for every type `T` in `Ts...`, and `false` otherwise.
     *           Internally uses `std::conjunction`. */
    template<template <class> class Condition>
    inline static constexpr bool all_of_v = std::conjunction<Condition<Ts>...>::value;

    /*! @brief Logical "none_of" over the list with respect to a unary type trait.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition`.
     *  @details Evaluates to `true` if `Condition<T>::value` is `false` for all types `T` in `Ts...`, and `false` otherwise.
     *           Conceptually equivalent to `!any_of_v<Condition>`. */
    template<template <class> class Condition>
    inline static constexpr bool none_of_v = !any_of_v<Condition>;

    /*! @brief Compile-time index lookup of a type within the list.
     *  @tparam Type    Type whose index is to be determined.
     *  @details Produces the zero-based index of `Type` within `Ts...`, assuming it is present. The actual computation is delegated
     *           to @ref draupnir::utils::index_of_v (Through internal structure @ref index_of_impl). If `Type` is not present in
     *           `Ts...` - `static_assert` will happen. */
    template<class Type>
    inline static constexpr std::size_t index_of_v = index_of_impl<Type,Ts...>::value;

    /*! @brief Index of the first element satisfying a unary predicate.
     *  @tparam Condition Unary type trait of the form `template<class> class Condition`.
     *  @details Computes the zero-based index of the first type `T` in `Ts...` for which `Condition<T>::value` is `true`, by
     *           delegating to the internal @ref index_of_first_if metafunction with an initial `Current` index of `0`.
     *
     *           If no type in the list satisfies `Condition`, `static_assert` happens. */
    template<template<class> class Condition>
    inline static constexpr std::size_t index_of_first_if_v = index_of_first_if<Condition,0,Ts...>::value;

    /*! @brief Counts amount of elements for which Predicate<Element>::value is evaluated to `true`. */
    template<template<class> class Condition>
    inline static constexpr int count_if_v = count_if<Condition,Ts...>::value;

    /*! @brief Counts amount of occurrences of a specific type.
     *  @tparam Type Type to be counted. */
    template<class Type>
    inline static constexpr int count_v = count_if_v<the_same<Type>::template as>;

    /*! @brief Retrieves the type at a given index in the list.
     *  @tparam Index Zero-based index of the element to access.
     *  @details Alias to the nested `result` of the internal @ref type_list::get metafunction. If `Index` is out of range, a
     *           `static_assert` is triggered during instantiation. */
    template<std::size_t Index>
    using get_t = typename get<Index,0,Ts...>::result;

    /*! @brief Appends a type or another type_list to the end of this list.
     *  @tparam Type Either a single type or an instantiation of @ref type_list.
     *  @details If `Type` is a plain type, the result is a new @ref type_list with `Type` appended after `Ts...`. If `Type` is itself
     *           a @ref type_list, its elements are expanded and appended to the end of this list. */
    template<class Type>
    using append_t = typename append<Type>::result;

    /*! @brief Appends a type to the end of this list.
     *  @tparam Type Single type.
     *  @details The result is a new @ref type_list with `Type` appended after `Ts...`. Compared to @ref type_list::append_t - if
     *           `Type` is itself a @ref type_list, it is added as single element. */
    template<class Type>
    using push_back_t = type_list<Ts...,Type>;

    /*! @brief Prepends a type or another type_list to the front of this list.
     *  @tparam Type Either a single type or an instantiation of @ref type_list.
     *  @details If `Type` is a plain type, the result is a new @ref type_list with `Type` placed before `Ts...`. If `Type` is itself
     *           a @ref type_list, its elements are expanded and placed in front of this list. */
    template<class Type>
    using prepend_t = typename prepend<Type>::result;

    /*! @brief Prepends a type to the end of this list.
     *  @tparam Type Single type.
     *  @details The result is a new @ref type_list with `Type` prepended before `Ts...`. Compared to @ref type_list::prepend_t - if
     *           `Type` is itself a @ref type_list, it is added as single element. */
    template<class Type>
    using push_front_t = type_list<Type,Ts...>;

    /*! @brief Inserts a type before the element at a given index.
     *  @tparam Index Zero-based index at which @p Type should be inserted.
     *  @tparam Type  Type to insert.
     *  @details Starting from an empty accumulator, this alias invokes the internal metafunction @ref type_list::insert_before to
     *           conceptually walk the list `Ts...` and insert `Type` just before the element at position `Index`.
     *
     *           The index must satisfy `Index < size_v`; attempting to insert at or beyond the logical end of the list results in
     *           a `static_assert` during template instantiation. */
    template<std::size_t Index, class Type>
    using insert_before_t = typename insert_before<Index,type_list<>,Type,Ts...>::result;

    /*! @brief Removes the element at a given index from the list.
     *  @tparam Index Zero-based index of the element to remove.
     *  @details Delegates to the internal @ref type_list::remove_at metafunction, starting the recursion with `Current == 0`. If
     *           `Index` is not less than @ref type_list::size_v, a `static_assert` is triggered. Otherwise, the resulting type
     *           is a @ref type_list formed by all elements `Ts...` except the one at position `Index`. */
    template<std::size_t Index>
    using remove_at_t = typename remove_at<Index,0,Ts...>::result;

    /*! @brief Removes all types that satisfy a given unary predicate.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition`.
     *  @details Produces a new @ref type_list containing only those types `T` from `Ts...` for which `Condition<T>::value` is
     *           `false`. */
    template<template<class> class Condition>
    using remove_if_t = typename remove_if<Condition,Ts...>::result;

    /*! @brief Removes all occurrences of a specific type from the list.
     *  @tparam Type Type to be removed.
     *  @details This is a convenience alias built on top of @ref type_list::remove_if_t. It constructs a predicate that is `true`
     *           exactly for `Type` and applies @ref type_list::remove_if_t with that predicate. The resulting list contains all
     *           types from `Ts...` except `Type`. */
    template<class Type>
    using remove_all_t = remove_if_t<the_same<Type>::template as>;

    /*! @brief Retains only types that satisfy a given unary predicate.
     *  @tparam Condition    Unary trait template of the form `template<class> class Condition`.
     *  @details Produces a new @ref type_list containing exactly those types `T` from `Ts...` for which `Condition<T>::value` is
     *           `true`. Conceptually dual to @ref type_list::remove_if_t. */
    template<template<class> class Condition>
    using filter_if_t = typename filter_if<Condition,Ts...>::result;

    /*! @brief Extracts a contiguous subrange of types as a new @ref type_list.
     *  @tparam First Zero-based index of the first element to include (inclusive).
     *  @tparam Last  Zero-based index of the last element to include (inclusive).
     *  @details Delegates to the internal @ref slice metafunction, starting with `Current == 0`, and produces a @ref type_list
     *           consisting of all types `Ts...` whose indices satisfy `First <= index <= Last`.
     *
     *           If `First` and `Last` are outside the bounds of the list, the behavior is defined by the underlying @ref slice
     *           implementation - `static_assert`. */
    template<std::size_t First,std::size_t Last>
    using slice_t = typename slice<First,Last,0,Ts...>::result;

    /*! @brief Converts the stored types into another variadic class template.
     *  @tparam Container    Variadic class template of the form `template<class...> class Container`.
     *  @details Instantiates `Container` with the types `Ts...`, effectively reusing the type sequence in a different container
     *           representation (e.g. `std::tuple`, `std::variant`, a custom type list, etc.). */
    template<template<class...> class Container>
    using convert_to_t = Container<Ts...>;

    /*! @brief Applies a unary type transformer to each element of the list.
     *  @tparam Template     Class template of the form `template<class...> class Template` that is intended to be instantiated with
     *                       a single type.
     *  @details Produces a new @ref type_list where each element is the result of instantiating `Template` with the corresponding
     *           type from `Ts...`. In other words, the result is `type_list<Template<Ts>...>`. */
    template<template<class...> class Template>
    using transform_t = type_list<Template<Ts>...>;

    /*! @brief Deduplicated version of this list, preserving first occurrence order.
     *  @details Alias to the nested `result` of @ref type_list::unique_types with an initially empty accumulator. The resulting
     *           @ref type_list contains each distinct type from `Ts...` at most once, in the order of their first appearance. */
    using unique_types_t = typename unique_types<type_list<>,Ts...>::result;

    /*! @brief Compile-time check whether this list is a subset of another @ref type_list.
     *  @tparam Other Candidate superset. Must be an instantiation of @ref type_list.
     *  @details Equivalent to @ref is_subset_of<Other,Ts...>::value. Evaluates to `true` when every type in this @ref type_list
     *           appears in `Other` (treating this list as a subset of `Other`), and `false` otherwise. */
    template<class Other>
    static inline constexpr bool is_subset_of_v = is_subset_of<Other,Ts...>::value;

    /*! @brief Compile-time check whether this list is a superset of another @ref type_list.
     *  @tparam Other Candidate subset. Must be an instantiation of @ref type_list.
     *  @details Equivalent to @ref is_superset_of<Other,Ts...>::value. Evaluates to `true` when every type in `Other` appears in
     *           this @ref type_list (treating this list as a superset of `Other`), and `false` otherwise. */
    template<class Other>
    static inline constexpr bool is_superset_of_v = is_superset_of<Other,Ts...>::value;

    /*! @brief Reversed version of this @ref type_list.
     *  @details Alias to the nested `result` of the internal @ref type_list::reverse metafunction with an initially empty accumulator.
     *           The resulting type is a @ref type_list where the order of `Ts...` is reversed. */
    using reverse_t = typename reverse<type_list<>,Ts...>::result;

    /*! @brief Set-theoretic union of this @ref type_list with another.
     *  @tparam Other Other list to be combined with. Must be an instantiation of @ref type_list.
     *  @details Delegates to the internal @ref union_impl metafunction, producing a @ref type_list that contains all types from this
     *           list and `Other`, with duplicates removed according to @ref unique_types. */
    template<class Other>
    using union_t = typename union_impl<Other,Ts...>::result;

    /*! @brief Set-theoretic intersection of this @ref type_list with another.
     *  @tparam Other Other list to intersect with. Must be an instantiation of @ref type_list.
     *  @details Delegates to the internal @ref intersection metafunction, producing a @ref type_list that contains exactly those types
     *           that appear both in this list and in `Other`. Duplicates are removed; order is defined by the @ref intersection
     *           implementation (typically driven by the order of this list). */
    template<class Other>
    using intersection_t = typename intersection<Other,Ts...>::result;
};

/*! @struct type_list_from_template_instantiation draupnir/utils/type_list.h
 *  @brief Extracts template type arguments from a class template instantiation into a @ref type_list.
 *  @tparam Type Type to be inspected.
 *  @details The primary template is a fallback that yields an empty @ref type_list for arbitrary `Type`. A partial specialization
 *           is provided for types of the form `SourceTemplate<Args...>`, in which case the nested alias `result` is defined as
 *           `type_list<Args...>`. */
template<class Type>
struct type_list_from_template_instantiation {
    static_assert(!std::is_same_v<Type,Type>,
        "type_list_from_template_instantiation<class Type> - Provided Type should be instantiation of some template in a form "\
        "template<class... Args> Template.");

    using result = type_list<>;
};

/*! @brief Partial specialization of @ref type_list_from_template_instantiation for class template instantiations.
 *  @tparam SourceTemplate Class template being inspected.
 *  @tparam Args...        Template arguments used to instantiate `SourceTemplate`.
 *  @details When `Type` is of the form `SourceTemplate<Args...>`, this specialization exposes the pack `Args...` as
 *           `type_list<Args...>` via the nested alias `result`. */
template<template <class...> class SourceTemplate, class... Args>
struct type_list_from_template_instantiation<SourceTemplate<Args...>> {
    using result = type_list<Args...>;
};

/*! @ingroup Utils
 *  @brief Convenience alias for @ref type_list_from_template_instantiation.
 *  @tparam Type Type to be inspected.
 *  @details Equivalent to `typename from_template_instantiation<Type>::result`. This metafunction is useful when you have an instantiation
 *           of some variadic class template and want to re-use its template arguments as a @ref type_list for further compile-time processing.
 *
 *           Example:
 *           @code
 *           using Tuple = std::tuple<int, double, char>;
 *           using Args  = typename from_template_instantiation<Tuple>::result;
 *           // Args == type_list<int, double, char>
 *           @endcode */
template<class Type>
using type_list_from_template_instantiation_t = typename type_list_from_template_instantiation<Type>::result;

/*! @struct type_list_merge draupnir/utils/type_list.h
 *  @ingroup Utils
 *  @brief Compile-time union-like merge of two @ref type_list instantiations.
 *  @tparam First   First operand. Must be an instantiation of @ref type_list.
 *  @tparam Second  Second operand. Must be an instantiation of @ref type_list.
 *  @details The primary template serves as a guard and enforces correct usage via `static_assert`s: both `First` and `Second` are
 *           required to be instantiations of @ref type_list. If either is not, a clear diagnostic message is emitted at compile time.
 *
 *           A partial specialization is provided for the case where both operands are of the form `type_list<...>`. In that case the
 *           nested alias `result` is defined as: `type_list<FirstTs...>::union_t< type_list<SecondTs...> >` i.e. it computes the
 *           set-theoretic union of the two lists, as defined by @ref type_list::union_t (with duplicates removed according to @ref
 *           type_list::unique_types_t). */
template<class First,class Second>
struct type_list_merge {
    static_assert(is_instantiation_of_v<First,type_list>,
        "Both arguments should be type_list<...> instantiations. First is not.");
    static_assert(is_instantiation_of_v<Second,type_list>,
        "Both arguments should be type_list<...> instantiations. Second is not.");
};

/*! @brief Partial specialization of @ref type_list_merge for two @ref type_list operands.
 *  @tparam FirstTs...   Types in the first list.
 *  @tparam SecondTs...  Types in the second list.
 *  @details The nested alias `result` is defined as the union of `type_list<FirstTs...>` and `type_list<SecondTs...>` via @ref
 *           type_list::union_t. */
template<class... FirstTs,class... SecondTs>
struct type_list_merge<type_list<FirstTs...>,type_list<SecondTs...>>
{
    using result = typename type_list<FirstTs...>::template union_t<type_list<SecondTs...>>;
};

/*! @ingroup Utils
 *  @brief Convenience alias for @ref type_list_merge.
 *  @tparam First   First @ref type_list operand.
 *  @tparam Second  Second @ref type_list operand.
 *  @details Equivalent to `typename type_list_merge<First,Second>::result`. */
template<class First, class Second>
using type_list_merge_t = typename type_list_merge<First,Second>::result;

}; // namespace draupnir::utils

#endif // TYPE_LIST_H
