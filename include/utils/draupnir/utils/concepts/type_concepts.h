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

#ifndef TYPE_CONCEPTS_H
#define TYPE_CONCEPTS_H

#include "draupnir/utils/type_detectors.h"
#include "draupnir/utils/template_detectors.h"

/*! @file draupnir/utils/concepts/type_concepts.h
 *  @ingroup Utils
 *  @brief This file contains concepts used to constrain templates based on fundamental type categories. */

namespace draupnir::utils
{

/*! @ingroup Utils
 *  @brief Matches types recognised by @ref draupnir::utils::is_integer.
 *  @tparam Candidate Type to check.
 *
 *  @details This concept is satisfied only for the explicitly supported built-in integer types, which are accepted by
 *           @ref draupnir::utils::is_integer. */

template<class Candidate>
concept integer_concept = is_integer<Candidate>::value;

template<integer_concept Integer, class Derived>
class integer_wrapper;

/*! @ingroup Utils
 *  @brief Matches types recognised by @ref draupnir::utils::is_integer and for types derived from @ref
 *         draupnir::utils::integer_wrapper.
 *  @tparam Candidate Type to check.
 *
 *  @details This concept is satisfied for the explicitly supported built-in integer types, which are accepted by @ref
 *           draupnir::utils::is_integer and for types derived from @ref draupnir::utils::integer_wrapper. */

template<class Candidate>
concept integer_like_concept =
    is_integer<Candidate>::value ||
    draupnir::utils::is_template_base_of<integer_wrapper,Candidate>::value;

/*! @ingroup Utils
 *  @brief Matches enumeration types.
 *  @tparam Candidate Type to check.
 *
 *  @details This concept is satisfied for any type for which `std::is_enum_v` is `true`. */

template<class Candidate>
concept enum_concept = std::is_enum_v<Candidate>;

}; // namespace draupnir::utils

#endif // TYPE_CONCEPTS_H
