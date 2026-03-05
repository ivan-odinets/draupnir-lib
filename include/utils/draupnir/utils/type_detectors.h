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

#ifndef TYPE_DETECTORS_H
#define TYPE_DETECTORS_H

#include <type_traits>

namespace draupnir::utils
{

/*! @file draupnir/utils/type_detectors.h
 *  @ingroup Utils
 *  @brief This is a file.
 * @todo Document entities within this file. */

template<typename T>
struct is_integer : std::bool_constant<
    std::is_same_v<T,char> || std::is_same_v<T,unsigned char> ||
    std::is_same_v<T,short> || std::is_same_v<T,unsigned short> ||
    std::is_same_v<T,int> || std::is_same_v<T,unsigned int> ||
    std::is_same_v<T,long> || std::is_same_v<T,unsigned long> ||
    std::is_same_v<T,long long> || std::is_same_v<T,unsigned long long>
> {};

template<class Candidate>
concept integer_concept = is_integer<Candidate>::value;

template<class Candidate>
concept enum_concept = std::is_enum_v<Candidate>;

};

#endif // TYPE_DETECTORS_H
