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

#ifndef VARIADIC_TEMPLATE_FROM_ARRAY_H
#define VARIADIC_TEMPLATE_FROM_ARRAY_H

#include <iterator>
#include <tuple>

namespace draupnir::utils
{

/*! @class variadic_template_from_static_array draupnir/utils/variadic_template_from_array.h
 *  @ingroup Utils
 *  @brief Builds a variadic template instantiation from values stored in a static array.
 *  @tparam Array Reference to a compile-time accessible static array.
 *  @tparam Wrapper Unary template used to wrap each array value.
 *  @tparam Variadic Variadic template to instantiate with wrapped array values.
 *
 *  @details For an array containing values `{A, B, C}`, this metafunction produces:
 *           @code
 *           Variadic<Wrapper<A>, Wrapper<B>, Wrapper<C>>
 *           @endcode
 *
 *           This may be useful when compile-time arrays are used as metadata and need to be converted into type-level variadic
 *           representations such as `std::tuple`.
 * @todo Tests: Add test for this class. */

template<auto const& Array, template<auto> class Wrapper, template<class...> class Variadic>
class variadic_template_from_static_array
{
private:
    template<class Seq>
    struct variadic_template_from_static_array_impl;

    template<std::size_t... I>
    struct variadic_template_from_static_array_impl<std::index_sequence<I...>> {
        using type = Variadic<Wrapper<Array[I]>...>;
    };

public:
    using type = typename variadic_template_from_static_array_impl<std::make_index_sequence<std::size(Array)>>::type;
};

template<auto const& Array, template<auto> class Wrapper, template<class...> class Variadic>
using variadic_template_from_static_array_t = variadic_template_from_static_array<Array, Wrapper, Variadic>::type;

/*! @ingroup Utils
 *  @brief Convenience alias that builds `std::tuple<Wrapper<Array[I]>...>` from a static array.
 *  @tparam Array Reference to a compile-time accessible static array.
 *  @tparam Wrapper Unary template used to wrap each array value. */

template<auto const& Array, template<auto> class Wrapper>
using tuple_from_static_array_t = variadic_template_from_static_array<Array,Wrapper,std::tuple>::type;

};

#endif // VARIADIC_TEMPLATE_FROM_ARRAY_H
