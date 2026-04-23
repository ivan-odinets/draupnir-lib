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

#ifndef CONSTRUCTIBILITY_CONCEPTS_H
#define CONSTRUCTIBILITY_CONCEPTS_H

/*! @file draupnir/utils/concepts/constructibility_concepts.h
 *  @ingroup Utils
 *  @brief This file contains concepts used to constrain templates based on constructibility objects. */

namespace draupnir::utils
{

/*! @ingroup Utils
 *  @brief Checks whether `To` can be brace-initialized from `From`.
 *  @tparam To Target type to construct.
 *  @tparam From Source type used for construction.
 *
 *  @details This concept is satisfied if an expression of the form `To{value}` is well-formed for a `const From& value`.
 *           It is useful for detecting support for brace initialization, including cases where direct initialization
 *           with parentheses would behave differently. */

template<class To, class From>
concept brace_constructible_from =
    requires(const From& x) { To{x}; };

}; // namespace draupnir::utils

#endif // CONSTRUCTIBILITY_CONCEPTS_H
