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

#ifndef FILECLASSVALIDATOR_H
#define FILECLASSVALIDATOR_H

#include <type_traits>

#include <QFileInfo>

// namespace Draupnir::Files
// {

// class FileTemplate
// {
// public:
// };

// };

// namespace Draupnir::Files
// {

// //*           - QString errorMessage() const

// class FileClassValidator
// {
// public:
//     template<class, class = std::void_t<>>
//     struct has_fileInfo : std::false_type {};

//     template<class Class>
//     struct has_fileInfo<
//         Class,
//         std::void_t<decltype(
//             std::is_same_v<QFileInfo,decltype(std::declval<Class>().fileInfo())>
//         )>
//     > : std::true_type {};

//     template<class, class = std::void_t<>>
//     struct has_errorString : std::false_type {};

//     template<class Class>
//     struct has_errorString<
//         Class,
//         std::void_t<decltype(
//             std::is_same_v<QString,decltype(std::declval<Class>().errorString())>
//         )>
//     > : std::true_type {};

//     template<class, class = std::void_t<>>
//     struct has_open : std::false_type {};

//     template<class Class>
//     struct has_open<
//         Class,
//         std::void_t<decltype(
//             std::is_same_v<bool,decltype(std::declval<Class>().open(std::declval<const QString&>()))>
//         )>
//     > : std::true_type {};
// };

// };

#endif // FILECLASSVALIDATOR_H
