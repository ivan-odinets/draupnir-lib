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

#ifndef FLAGS_CONCEPTS_H
#define FLAGS_CONCEPTS_H

#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/template_detectors.h"

namespace draupnir::utils
{

template<integer_like_concept>
class flags;

template<enum_concept>
class enum_flags;

/*! @brief Concept satisfied by instantiations of @ref draupnir::utils::flags.
 *  @tparam Candidate Type to test. */

template<class Candidate>
concept flags_concept = draupnir::utils::is_instantiation_of_v<Candidate,flags>;

/*! @brief This is a concept.
 * @todo Important: Extend this concept, so it will require proper using type = ... aliases, constructors / assignments for flags derivatives
 * @todo Documentation: Write reasonable docs for this concept. */

template<class Candidate>
concept flags_like_concept =
    draupnir::utils::is_template_base_of_v<enum_flags, Candidate> ||
    draupnir::utils::is_template_base_of_v<flags, Candidate>;

/*! @brief Concept satisfied by instantiations of @ref draupnir::utils::enum_flags.
 *  @tparam Candidate Type to test. */
template<class Candidate>
concept enum_flags_concept =
    draupnir::utils::is_instantiation_of_v<Candidate, enum_flags>;

/*! @brief Concept satisfied by derivatives of @ref draupnir::utils::enum_flags, but not by instantiations of the @ref draupnir::utils::enum_flags.
 *         itself.
 *  @tparam Candidate Type to test. */
template<class Candidate>
concept enum_flags_derived_concept =
    (draupnir::utils::is_instantiation_of_v<Candidate,enum_flags> == false) &&
    draupnir::utils::is_template_base_of_v<enum_flags,Candidate>;

/*! @brief Concept satisfied by derivatives of @ref draupnir::utils::enum_flags and by instantiations of the @ref draupnir::utils::enum_flags.
 *         itself.
 *  @tparam Candidate Type to test. */
template<class Candidate>
concept enum_flags_like_concept =
    draupnir::utils::is_instantiation_of_v<Candidate,enum_flags> ||
    draupnir::utils::is_template_base_of_v<enum_flags,Candidate>;

/*! @brief Checks whether `Flags::test_flag(Value)` is a valid expression returning `bool`.
 *  @tparam Flags Flags-like type to test.
 *  @tparam Value Value type to pass into `test_flag`.
 *
 *  @details This concept is intended primarily for compile-time tests of the public API. It evaluates to `true` when the
 *           expression `std::declval<const Flags&>().test_flag(std::declval<Value>())` is well-formed and returns `bool`. */

template<class Flags, class Value>
concept test_flag_callable = requires(const Flags& flags, Value value) {
    { flags.test_flag(value) } -> std::same_as<bool>;
};

/*! @brief Checks whether `Flags::set_flag(Value, bool)` is a valid expression returning `Flags&`.
 *  @tparam Flags Flags-like type to test.
 *  @tparam Value Value type to pass into `set_flag`.
 *
 *  @details This concept is intended primarily for compile-time tests of the public API. It evaluates to `true` when the
 *           expression `std::declval<Flags&>().set_flag(std::declval<Value>(), true)` is well-formed and returns `Flags&`. */
template<class Flags, class Value>
concept set_flag_callable = requires(Flags& flags, Value value, bool on) {
    { flags.set_flag(value, on) } -> std::same_as<Flags&>;
};

};

#endif // FLAGS_CONCEPTS_H
