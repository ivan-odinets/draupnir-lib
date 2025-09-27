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

#ifndef SETTINGTRAITSCONCAT_H
#define SETTINGTRAITSCONCAT_H

#include <tuple>

#include "draupnir/core/SettingTemplate.h"
#include "draupnir/SettingsBundleTemplate.h"

namespace Draupnir::Settings
{

/*! @struct Flatten draupnir/utils/SettingTraitsConcat.h
 *  @brief Primary template for Flatten, converts a single SettingTrait into a tuple of SettingTemplate.
 *  @details This struct is used to normalize all setting-related types into a uniform representation (i.e. a tuple of
 *           SettingTemplate<Trait>), whether they are standalone or bundled.
 *
 *  @tparam Trait A single setting trait.
 * @see SettingsBundleTemplate, SettingTemplate */

template<class Trait>
struct Flatten {
    using type = std::tuple<SettingTemplate<Trait>>;
};

/*! @struct Flatten draupnir/utils/SettingTraitsConcat.h
 *  @brief Specialization of Flatten for SettingsBundleTemplate.
 *  @details If the provided type is a SettingsBundleTemplate containing multiple SettingTraits, this specialization will
 *           expand them into a tuple of SettingTemplate<T> for each trait.
 *
 *  @tparam SettingTraits Variadic list of setting traits contained in the bundle.
 * @see SettingsBundleTemplate, SettingTemplate */

template<class... SettingTraits>
struct Flatten<SettingsBundleTemplate<SettingTraits...>> {
    using type = std::tuple<SettingTemplate<SettingTraits>...>;
};

/*! @struct TupleConcat draupnir/utils/SettingTraitsConcat.h
 *  @brief TupleConcat helper: base case.
 *  @details Concatenates multiple std::tuple types into a single std::tuple. This is the base case (no tuples) resulting
 *           in an empty tuple.
 *
 * @see TupleConcat<std::tuple<Ts...>> */

template<class... Tuples>
struct TupleConcat;

/*! @struct TupleConcat draupnir/utils/SettingTraitsConcat.h
 *  @brief TupleConcat specialization: empty case.
 *  @details This specialization handles the zero-argument case by returning an empty std::tuple. */

template<>
struct TupleConcat<> {
    using type = std::tuple<>;
};

/*! @struct TupleConcat draupnir/utils/SettingTraitsConcat.h
 *  @brief TupleConcat specialization: single tuple case.
 *  @details This specialization returns the same tuple if only one is passed.
 *
 *  @tparam Ts The types inside the std::tuple. */

template<class... Ts>
struct TupleConcat<std::tuple<Ts...>> {
    using type = std::tuple<Ts...>;
};

/*! @struct TupleConcat draupnir/utils/SettingTraitsConcat.h
 *  @brief TupleConcat specialization: recursive case.
 *  @details Recursively flattens and concatenates multiple std::tuple types into a single one.
 *
 *  @tparam Ts Elements from the first tuple.
 *  @tparam Us Elements from the second tuple.
 *  @tparam Rest Remaining tuple types to concatenate. */

template<class... Ts, class... Us, class... Rest>
struct TupleConcat<std::tuple<Ts...>, std::tuple<Us...>, Rest...> {
    using type = typename TupleConcat<std::tuple<Ts..., Us...>, Rest...>::type;
};

};

#endif // SETTINGTRAITSCONCAT_H
