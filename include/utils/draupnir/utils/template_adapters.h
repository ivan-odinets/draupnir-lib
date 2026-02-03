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

#ifndef TEMPLATE_ADAPTERS_H
#define TEMPLATE_ADAPTERS_H

#include "draupnir/utils/template_detectors.h"

namespace draupnir::utils
{

/*! @file draupnir/utils/template_adapters.h
 *  @ingroup Utils
 *  @brief This file contains template adapters. */

/*! @struct the_same draupnir/utils/template_adapters.h
 *  @ingroup Utils
 *  @brief Builds a unary predicate that checks for equality with a fixed type.
 *  @tparam Type Reference type to compare against.
 *  @details Given a `Type`, this helper defines a nested template `as<Other>` which is a `std::is_same<Type, Other>`. It may be
 *           used as an adapter to feed into @ref type_list::remove_if when implementing @ref type_list::remove_all_t. */
template<class Type>
struct the_same {
    template<class Other>
    struct as : std::is_same<Type,Other> {};
};

/*! @struct is_instantiation draupnir/utils/template_adapters.h
 *  @ingroup Utils
 *  @brief Builds a unary predicate that checks if type is instantiation of a fixed template.
 *  @tparam Template    Reference template to compare against. */
template<template<class...> class Template>
struct is_instantiation {
    template<class T>
    struct of : draupnir::utils::is_instantiation_of<T,Template> {};
};

/*! @struct is_auto_instantiation draupnir/utils/template_adapters.h
 *  @ingroup Utils
 *  @brief Builds a unary predicate that checks if type is instantiation of a fixed template.
 *  @tparam Template    Reference template to compare against. */

template<template<auto...> class AutoTemplate>
struct is_auto_instantiation {
    template<class T>
    struct of : draupnir::utils::is_auto_instantiation_of<T,AutoTemplate> {};
};

}; // namespace draupnir::utils

#endif // TEMPLATE_ADAPTERS_H
