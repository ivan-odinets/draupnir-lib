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

#ifndef CLASS_MARCOS_H
#define CLASS_MARCOS_H

/*! @file draupnir/utils/class_macros.h
 *  @ingroup Utils
 *  @brief Convenience macros for constraining class usage at compile time.
 *
 *  @details This header currently provides a single helper macro, @ref DEFINE_COMPILE_TIME, which turns a class into a purely
 *           compile-time “type container” by deleting all constructors, destructor and copy/move operations. This is useful for
 *           metafunction holders, traits and other types that are never meant to be instantiated. */

/*! @def DEFINE_COMPILE_TIME(Class)
 *  @brief Disable all runtime construction and assignment for `Class`.
 *  @param Class Name of the class for which all constructors, destructor and copy/move operations should be deleted.
 *
 *  @details Expands inside the class definition and:
 *           - deletes the default constructor;
 *           - deletes the destructor;
 *           - deletes the copy constructor;
 *           - deletes the move constructor;
 *           - deletes the copy-assignment operator.
 *
 *           As a result, `Class` can only be used as a compile-time type container (e.g. for nested aliases and `constexpr`
 *           values) and cannot be instantiated or assigned.
 *
 *           Example usage:
 *           @code
 *           struct MyMeta {
 *               DEFINE_COMPILE_TIME(MyMeta)
 *               using type = int;
 *           };
 *           @endcode */

#define DEFINE_COMPILE_TIME(Class) \
    Class() = delete;              \
    ~Class() = delete;             \
    Class(const Class&) = delete;  \
    Class(Class&&) = delete;       \
    Class& operator=(const Class&) = delete;


#endif // CLASS_MARCOS_H
