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

#ifndef CONDITIONAL_TYPE_EXTRACTORS_H
#define CONDITIONAL_TYPE_EXTRACTORS_H

namespace draupnir::utils
{

template<bool getOrNot, class Type>
struct type_or_void;

template<class Type>
struct type_or_void<true,Type> { using result = Type; };

template<class Type>
struct type_or_void<false,Type> { using result = void; };

template<bool getOrNot,class Type>
using type_or_void_t = type_or_void<getOrNot,Type>::result;

template<class TypeList,template<class...> class Template,
    bool getOrNot = TypeList::template contains_template_instantiation_v<Template>>
struct get_template_instantiation_or_void;

template<class TypeList,template<class...> class Template>
struct get_template_instantiation_or_void<TypeList,Template,true>
{
    using result = int;
};

template<class TypeList,template<class...> class Template>
struct get_template_instantiation_or_void<TypeList,Template,false>
{
    using result = void;
};

}; // namespace draupnir::utils

#endif // CONDITIONAL_TYPE_EXTRACTORS_H
