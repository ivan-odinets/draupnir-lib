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

#ifndef INTEGER_NORMALIZATION_H
#define INTEGER_NORMALIZATION_H

/*! @file draupnir/utils/integer_normalization.h
 *  @ingroup Utils
 *  @brief Utilities for normalizing enum and integer values to a target integer type. */

#include <utility>

#include "draupnir/utils/concepts/constructibility_concepts.h"
#include "draupnir/utils/concepts/type_concepts.h"

namespace draupnir::utils
{

/*! @class normalizer draupnir/utils/integer_normalization.h
 *  @ingroup Utils
 *  @brief Helper for normalizing integer-like and enum values to a target integer-like type.
 *  @tparam Into Target integer-like type into which source values should be normalized.
 *
 *  @details `normalizer<Into>` provides compile-time utilities for:
 *           - checking whether a source integer-like or enum type can be normalized into `Into` using direct initialization
 *             (`Into(...)`);
 *           - checking whether such a source type can be normalized into `Into` using brace initialization (`Into{...}`);
 *           - converting such values into `Into`.
 *
 *           Integer-like source types are normalized directly. Enum types are first converted to their underlying integer
 *           type and then normalized into `Into`. */

template<enum_or_integer_like_concept Into>
class normalizer
{
private:
    /*! @brief Maps a source integer-like or enum type to an intermediate integer type used for normalization.
     *  @tparam C Source type. */
    template<class C>
    struct _normalized_integer { using result = void; };

    /*! @brief Identity mapping for integer-like source types.
     *  @tparam C Source integer-like type. */
    template<integer_like_concept C>
    struct _normalized_integer<C> { using result = C; };

    /*! @brief Maps enum types to their underlying integer type.
     *  @tparam C Source enum type. */
    template<enum_concept C>
    struct _normalized_integer<C> { using result = std::underlying_type_t<C>; };

    /*! @brief Alias for the intermediate integer type used during normalization.
     *  @tparam C Source type. */
    template<class C>
    using _normalized_integer_t = typename _normalized_integer<std::remove_cvref_t<C>>::result;

public:
    /*! @brief Indicates whether values of type `From` can be normalized into `Into` using direct initialization.
     *  @tparam From Source type.
     *  @details The value is `true` if `Into` can be directly initialized from the normalized integer representation of `From`,
     *           and `false` otherwise. */
    template<class From>
    static inline constexpr bool can_be_normalized_v = std::is_constructible_v<Into,_normalized_integer_t<From>>;

    /*! @brief Indicates whether values of type `From` can be brace-normalized into `Into`.
     *  @tparam From Source type.
     *  @details The value is `true` if `Into` can be brace-initialized from the normalized integer representation of `From`,
     *           and `false` otherwise. */
    template<class From>
    static inline constexpr bool can_be_brace_normalized_v = brace_constructible_from<Into,_normalized_integer_t<From>>;

    /*! @brief Normalizes an integer-like or enum value into `Into` using direct initialization.
     *  @tparam From Source type. Must satisfy @ref can_be_normalized_v.
     *  @param value Source value to normalize.
     *  @return Value converted to `Into`.
     *  @details Integer-like values are normalized directly. Enum values are first converted to their underlying integer type
     *           and then directly initialized into `Into`. */
    template<class From>
    static constexpr Into normalize(From value) requires (can_be_normalized_v<From> ){
        if constexpr (enum_concept<From>)
            return Into(std::to_underlying(value));
        else
            return Into(value);
    }

    /*! @brief Normalizes an integer-like or enum value into `Into` using brace initialization.
     *  @tparam From Source type. Must satisfy @ref can_be_brace_normalized_v.
     *  @param value Source value to normalize.
     *  @return Value converted to `Into`.
     *  @details Integer-like values are normalized directly. Enum values are first converted to their underlying integer type
     *           and then brace-initialized into `Into`. */
    template<class From>
    static constexpr Into brace_normalize(From value) requires (can_be_brace_normalized_v<From> ){
        if constexpr (enum_concept<From>)
            return Into{std::to_underlying(value)};
        else
            return Into{value};
    }
};

}; // namespace draupnir::utils

#endif // INTEGER_NORMALIZATION_H
