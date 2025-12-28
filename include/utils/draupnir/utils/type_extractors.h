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

/*! @struct get_template_instantiation draupnir/utils/type_extractors.h
 *  @ingroup Utils
 *  @brief Metafunction that extracts the first type from a parameter pack that is an instantiation of a given class template.
 *  @tparam Template    Class template to search for. Must be of the form `template<typename...> class Template`.
 *  @tparam Ts...       Variadic list of types to be inspected from left to right.
 *
 *  @details This metafunction scans the parameter pack @p Ts... in order and selects the first type that is an instantiation of
 *           @p Template. If such a type is found, the nested alias `type` resolves to that type.
 *
 *           If none of the types in @p Ts... is an instantiation of @p Template, a `static_assert` in the base-case specialization
 *           is triggered, producing a clear compilation error.
 *
 *           Detection of whether a given type is an instantiation of @p Template is delegated to @ref draupnir::utils::is_instantiation_of. */

template<template<typename...> class Template, typename... Ts>
struct get_template_instantiation;

/*! @struct get_template_instantiation draupnir/utils/type_extractors.h
 *  @brief Specialization for the case where the first type in the pack is already an instantiation of @p Template and there are
 *         additional types following.
 *  @tparam Template    Class template being searched for.
 *  @tparam Inner...    Template arguments of the first type, which is of the form `Template<Inner...>`.
 *  @tparam Tail...     Remaining types in the parameter pack.
 *
 *  @details When the first type in the parameter pack matches the pattern `Template<Inner...>`, this specialization is selected and
 *           the search terminates immediately. The nested alias `type` is defined as that first type, i.e. `Template<Inner...>`. The
 *           remaining types @p Tail... are not inspected. */

template<template<typename...> class Template, typename... Inner, typename... Tail>
struct get_template_instantiation<Template, Template<Inner...>, Tail...>
{
    using type = Template<Inner...>;
};

/*! @struct get_template_instantiation draupnir/utils/type_extractors.h
 *  @brief Recursive specialization for the case where the first type is not an instantiation of @p Template, but there are more types
 *         to inspect.
 *  @tparam Template     Class template being searched for.
 *  @tparam Head         First type in the parameter pack, which is not of the form `Template<...>`.
 *  @tparam Tail...      Remaining types in the parameter pack.
 *
 *  @details This specialization skips @p Head and continues the search in the remaining types @p Tail... by delegating to
 *           `get_template_instantiation<Template, Tail...>`. The resulting `type` alias is whatever the recursive instantiation resolves
 *           to. */

template<template<typename...> class Template, typename Head, typename... Tail>
struct get_template_instantiation<Template, Head, Tail...>
{
    using type = typename get_template_instantiation<Template, Tail...>::type;
};

/*! @struct get_template_instantiation draupnir/utils/type_extractors.h
 *  @brief Base-case specialization for a single type that is an instantiation of @p Template.
 *  @tparam Template    Class template being searched for.
 *  @tparam Inner...    Template arguments of the single type, which is of the form `Template<Inner...>`.
 *
 *  @details When the parameter pack has been reduced to a single type and this type matches the pattern `Template<Inner...>`, this
 *           specialization is selected. The nested alias `type` is defined as that type, i.e. `Template<Inner...>`. */

template<template<typename...> class Template, typename... Inner>
struct get_template_instantiation<Template, Template<Inner...>>
{
    using type = Template<Inner...>;
};

/*! @struct get_template_instantiation draupnir/utils/type_extractors.h
 *  @brief Base-case specialization for a single type that is not guaranteed to be an instantiation of @p Template.
 *  @tparam Template    Class template being searched for.
 *  @tparam Head        The only remaining type in the parameter pack.
 *
 *  @details This specialization is instantiated when the recursive search has exhausted all types except @p Head. It performs a
 *           final check:
 *           - If `is_instantiation_of_v<Head, Template>` is `true`, then @p Head is indeed an instantiation of @p Template and the
 *             nested alias `type` is defined as @p Head.
 *           - Otherwise, the `static_assert` fails and the compilation stops with a descriptive error message stating that no type
 *             in the parameter pack is an instantiation of the given template.
 *
 *           This ensures that `get_template_instantiation` either returns a valid instantiation of @p Template or fails to compile,
 *           making missing matches explicit at compile time. */

template<template<typename...> class Template, typename Head>
struct get_template_instantiation<Template, Head>
{
    static_assert(
        is_instantiation_of_v<Head, Template>,
        "get_template_instantiation: no type in the parameter pack is an "
        "instantiation of the given template"
    );

    using type = Head;
};

/*! @ingroup Utils
 *  @brief Convenience alias for @ref get_template_instantiation.
 *  @tparam Template    Class template being searched for.
 *  @tparam Ts...       Variadic list of types to be inspected.
 *
 *  @details This alias resolves directly to the nested `type` of @ref get_template_instantiation for the given @p Template and
 *           parameter pack @p Ts....
 *
 *           If none of the types in @p Ts... is an instantiation of @p Template, the underlying `static_assert` in @ref
 *           get_template_instantiation will trigger and produce a compilation error. */

template<template<typename...> class Template, typename... Ts>
using get_template_instantiation_t = typename get_template_instantiation<Template,Ts...>::type;

template<template<typename...> class TemplateBase, class Derived>
struct get_base_template_instantiation_or_void
{
private:
    template<class... Args>
    static TemplateBase<Args...> getBase(TemplateBase<Args...>*);
    static void getBase(...);
public:
    using type = decltype(getBase(std::declval<Derived*>()));
};

template<template<typename...> class TemplateBase, class Derived>
using get_base_template_instantiation_or_void_t = typename get_base_template_instantiation_or_void<TemplateBase,Derived>::type;

}; // namespace draupnir::utils

#endif // TYPE_EXTRACTORS_H
