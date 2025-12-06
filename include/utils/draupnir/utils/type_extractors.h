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

#ifndef TYPE_EXTRACTORS_H
#define TYPE_EXTRACTORS_H

#include "draupnir/utils/template_detectors.h"

namespace draupnir::utils
{

template<template<typename...> class Template, typename... Ts>
struct get_template_instantiation;

template<template<typename...> class Template, typename... Inner, typename... Tail>
struct get_template_instantiation<Template, Template<Inner...>, Tail...>
{
    using type = Template<Inner...>;
};

template<template<typename...> class Template, typename Head, typename... Tail>
struct get_template_instantiation<Template, Head, Tail...>
{
    using type = typename get_template_instantiation<Template, Tail...>::type;
};

template<template<typename...> class Template, typename... Inner>
struct get_template_instantiation<Template, Template<Inner...>>
{
    using type = Template<Inner...>;
};

template<template<typename...> class Template, typename Head>
struct get_template_instantiation<Template, Head>
{
    static_assert(
        is_instantiation_of_v<Head, Template>,
        "get_template_instantiation: no type in the parameter pack is an "
        "instantiation of the given template"
    );

    using type = Head; // Сюди дійдемо тільки якщо static_assert пройшов (тобто Head таки Template<...>)
};
}; // namespace draupnir::utils

#endif // TYPE_EXTRACTORS_H
