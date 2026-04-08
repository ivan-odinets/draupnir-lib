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

#ifndef TYPE_QUALIFIERS_HELPERS_H
#define TYPE_QUALIFIERS_HELPERS_H

#include <type_traits>

namespace draupnir::utils
{

/*! @struct copy_const_from draupnir/utils/type_qualifiers_helpers.h
 *  @ingroup Utils
 *  @brief Copies the top-level `const` qualifier from one type to another.
 *  @tparam From Source type from which constness is copied.
 *  @tparam T    Destination type to which constness is applied.
 *
 *  @details If `From` (after removing reference) is `const`, the resulting nested `type` is `const T`. Otherwise the
 *           resulting nested `type` is `T`.
 *
 *           This trait is useful when an API needs to propagate constness from one deduced type to another one, for
 *           example from `self` to a pointee type in methods using explicit object parameters (`this auto&&`). */

template<class From, class T>
struct copy_const_from : public std::conditional<
    std::is_const_v<std::remove_reference_t<From>>,
    std::add_const_t<T>,
    std::remove_const_t<T>
> {};

/*! @ingroup Utils
 *  @brief Convenience alias for @ref copy_const_from.
 *  @tparam From Source type from which constness is copied.
 *  @tparam T    Destination type to which constness is applied. */

template<class From, class T>
using copy_const_from_t = copy_const_from<From,T>::type;

}; // namespace draupnir::utils

#endif // TYPE_QUALIFIERS_HELPERS_H
