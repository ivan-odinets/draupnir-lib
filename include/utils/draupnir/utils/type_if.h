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

#ifndef TYPE_IF_H
#define TYPE_IF_H

#include <type_traits>

#include "draupnir/utils/class_marcos.h"

namespace draupnir::utils
{

/*! @class type_if draupnir/utils/type_if.h
 *  @brief Compile-time `if / then / else` selection for types and template instantiations.
 *  @tparam Cond Compile-time boolean condition. If `true`, the `then_*` branch is chosen; otherwise the `else_*` branch.
 *  @details `type_if<Cond>` is a small template-based wrapper that allows you to express conditional type selection in a
 *           readable, chainable form:
 *           - `then_*` selects the "then" branch (stored as a *carrier* type).
 *           - `else_*` selects the "else" branch and produces the final `::result` type.
 *           - The chain is finalized by accessing `::result` of the chosen `else_*` expression.
 *
 *           Internally, this class uses *carriers* (small wrappers that expose a nested `::type`) and performs the selection
 *           via `std::conditional_t` **between carriers**. This keeps the unselected branch inert (i.e. not directly referenced
 *           as `Template<Args...>` in the conditional), which helps preserve lazy behavior and avoids many cases where an
 *           ill-formed "else" branch would otherwise be instantiated eagerly.
 *
 *           @code
 *           // Example 1: simple type selection
 *           using R1 = type_if<true>
 *               ::then_type<int>
 *               ::else_type<double>
 *           ::result; // int
 *
 *           using R2 = type_if<false>
 *              ::then_type<int>
 *              ::else_type<double>
 *           ::result; // double
 *
 *           // Example 2: select between template instantiations
 *           #include <vector>
 *           #include <array>
 *
 *           using VecOrArray = type_if<false>
 *              ::then_template<std::vector, int>
 *              ::else_t1_a1<std::array, int, 10>
 *           ::result; // std::array<int, 10>
 *           @endcode
 *
 * @todo Maybe try generalizing this structure with macros? */

template<bool Cond>
class type_if {
private:
    DEFINE_COMPILE_TIME(type_if);

    /*! @brief Implementation detail: picks one of two carrier types using `std::conditional_t`. */
    template<bool _Cond, class IfTrue, class IfFalse>
    using _comparator_t = std::conditional_t<_Cond,IfTrue,IfFalse>;

    /*! @brief Carrier for a plain type `T` (exposes `T` as `::type`). */
    template<class T>
    struct _type      { using type = T; };

    /*! @brief Carrier for instantiating a `template<class...>` with arguments `Args...`. */
    template<template<class...> class Template, class... Args>
    struct _tp { using type = Template<Args...>; };

    /*! @brief Carrier for instantiating a `template<class, auto>` with `(Arg, Value)`. */
    template<template<class,auto> class Template, class Arg, auto Value>
    struct _t1_a1 { using type = Template<Arg,Value>; };

    /*! @brief Carrier for instantiating a `template<auto, class...>` with `(Value, Args...)`. */
    template<template<auto,class...> class Template, auto Value, class... Args>
    struct _a1_tp { using type = Template<Value,Args...>; };

    /*! @brief "Then-node" of the `type_if`: stores the chosen `ThenCarrier` and exposes `else_*` endpoints.
     *  @tparam ThenCarrier Carrier type that exposes the "then" branch as `ThenCarrier::type`.
     *  @details A node represents the "then" branch. Any `else_*` endpoint combines the stored `ThenCarrier` with the
     *           corresponding "else carrier" and yields the final `::result` type. */
    template<class ThenCarrier>
    struct _then_node {
    private:
        DEFINE_COMPILE_TIME(_then_node);

        /*! @brief Implementation detail: selects between `ThenCarrier` and `ElseCarrier` and returns the chosen `::type`.
         *  @tparam ElseCarrier Carrier for the else-branch (must expose `ElseCarrier::type`). */
        template<class ElseCarrier>
        struct else_carrier { using result = typename _comparator_t<Cond, ThenCarrier, ElseCarrier>::type; };

    public:
        /*! @brief Else-branch as a plain type.
         *  @tparam ElseType Type to use if `Cond == false`.
         *  @return Selected type as `::result`. */
        template<class ElseType>
        using else_type = else_carrier<_type<ElseType>>;

        /*! @brief Else-branch as `template<class...>` instantiated with `ElseArgs...`.
         *  @tparam ElseTemplate Template of form `template<class...>`.
         *  @tparam ElseArgs Template arguments to instantiate `ElseTemplate`.
         *  @return Selected type as `::result`. */
        template<template<class...> class ElseTemplate, class... ElseArgs>
        using else_tp = else_carrier<_tp<ElseTemplate,ElseArgs...>>;

        /*! @brief Alias for @ref else_tp (readability sugar).
         *  @tparam ElseTemplate Template of form `template<class...>`.
         *  @tparam ElseArgs Template arguments to instantiate `ElseTemplate`.
         *  @return Selected type as `::result`. */
        template<template<class...> class ElseTemplate, class... ElseArgs>
        using else_template = else_tp<ElseTemplate,ElseArgs...>;

        /*! @brief Else-branch as `template<class, auto>` instantiated with `(ElseArg, ElseValue)`.
         *  @tparam ElseTemplate Template of form `template<class, auto>`.
         *  @tparam ElseArg First (type) argument.
         *  @tparam ElseValue Second (non-type) argument.
         *  @return Selected type as `::result`. */
        template<template<class,auto> class ElseTemplate, class ElseArg, auto ElseValue>
        using else_t1_a1 = else_carrier<_t1_a1<ElseTemplate,ElseArg,ElseValue>>;

        /*! @brief Else-branch as `template<auto, class...>` instantiated with `(ElseValue, ElseArgs...)`.
         *  @tparam ElseTemplate Template of form `template<auto, class...>`.
         *  @tparam ElseValue First (non-type) argument.
         *  @tparam ElseArgs Remaining (type) arguments.
         *  @return Selected type as `::result`. */
        template<template<auto,class...> class ElseTemplate, auto ElseValue, class... ElseArgs>
        using else_a1_tp = else_carrier<_a1_tp<ElseTemplate,ElseValue,ElseArgs...>>;
    };

public:
    /*! @brief Start the chain with a plain type `Type` as the then-branch.
     *  @tparam Type Type to use if `Cond == true`. */
    template<class Type>
    using then_type = _then_node<_type<Type>>;

    /*! @brief Start the chain with a `template<class...>` instantiation as the then-branch.
     *  @tparam ThenTemplate Template of form `template<class...>`.
     *  @tparam ThenArgs Template arguments to instantiate `ThenTemplate`. */
    template<template<class...> class ThenTemplate, class... ThenArgs>
    using then_tp = _then_node<_tp<ThenTemplate, ThenArgs...>>;

    /*! @brief Alias for @ref then_tp (readability sugar).
     *  @tparam ThenTemplate Template of form `template<class...>`.
     *  @tparam ThenArgs Template arguments to instantiate `ThenTemplate`. */
    template<template<class...> class ThenTemplate, class... ThenArgs>
    using then_template = _then_node<_tp<ThenTemplate, ThenArgs...>>;

    /*! @brief Start the chain with a `template<class, auto>` instantiation as the then-branch.
     *  @tparam ThenTemplate Template of form `template<class, auto>`.
     *  @tparam ThenArg First (type) argument.
     *  @tparam ThenValue Second (non-type) argument. */
    template<template<class,auto> class ThenTemplate, class ThenArg, auto ThenValue>
    using then_t1_a1 = _then_node<_t1_a1<ThenTemplate,ThenArg,ThenValue>>;

    /*! @brief Start the chain with a `template<auto, class...>` instantiation as the then-branch.
     *  @tparam ThenTemplate Template of form `template<auto, class...>`.
     *  @tparam ThenValue First (non-type) argument.
     *  @tparam ThenArgs Remaining (type) arguments. */
    template<template<auto,class...> class ThenTemplate, auto ThenValue, class... ThenArgs>
    using then_a1_tp = _then_node<_a1_tp<ThenTemplate,ThenValue,ThenArgs...>>;
};

};

#endif // TYPE_IF_H
