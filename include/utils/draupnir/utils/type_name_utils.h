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

#ifndef TYPE_NAME_UTILS_H
#define TYPE_NAME_UTILS_H

#include <vector>
#include <list>
#include <optional>
#include <string>
#include <tuple>
#include <typeinfo>
#include <variant>

#if defined(QT_CORE_LIB)
    #include <QObject>
#endif // QT_CORE_LIB

// Draupnir-utils-specific
#include "draupnir/utils/type_list.h"

/*! @file draupnir/utils/type_name_utils.h
 *  @ingroup Utils
 *  @brief Utilities for producing human-readable type names at run time.
 *
 *  @details This header provides a small customization framework for pretty-printing C++ types. It is intended primarily for
 *           debugging, logging, unit tests and diagnostic output (e.g. printing @ref draupnir::utils::type_list instantiations).
 *
 *           The core abstraction is the @ref draupnir::utils::type_pretty_name_impl template, which can be specialized for
 *           particular types or class templates. A convenience function @ref draupnir::utils::type_pretty_name is provided as
 *           the main user-facing entry point.
 *
 *           Out of the box, the following are supported:
 *           - A fallback implementation that uses `typeid(T).name()` for arbitrary `T`.
 *           - Specializations for a set of fundamental types and pointers to them (e.g. `int`, `unsigned long long*`, `std::string`).
 *           - Specializations for common standard templates (e.g. `std::pair`, `std::tuple`, `std::vector`, `std::list`,
 *             `std::optional`) that recursively pretty-print their template arguments.
 *           - Basic handling of top-level cv-qualifiers and references, producing forms such as `"const int"`, `"double&"`, `"const
 *             std::vector<int>&"`, etc.
 *
 *           Customization is done via the helper macros:
 *           - DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(TypeName)
 *           - DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(TemplateName)
 *
 *           See their documentation below for details.
 *
 * @todo Extend the list of supported things and try writing some simple test. */

namespace draupnir::utils
{

/*! @struct type_pretty_name_impl
 *  @brief Primary implementation of a human-readable name provider for a type @p T.
 *  @tparam T Type whose name should be obtained.
 *
 *  @details The primary template provides a very generic fallback: it simply returns `typeid(T).name()`, which is implementation-defined
 *           and often mangled. This is sufficient for debugging and for types that do not have a dedicated specialization.
 *
 *           More user-friendly names are provided via explicit specializations, typically introduced with convenience macros:
 *           - DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(TypeName)
 *           - DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(TemplateName)
 *
 *           Additional partial specializations are defined below to handle cv-qualifiers and references (e.g. `const T`, `T&`, `const
 *           T&`) by decorating the base name. */

template<class T>
struct type_pretty_name_impl {
    /*! @brief Returns a human-readable name for `T`.
     *  @details For the primary template this simply forwards to `typeid(T).name()`. Specializations may override this behaviour to
     *           provide a more stable and prettier representation. */
    static std::string get() {
#if defined(QT_CORE_LIB)
        if constexpr (std::is_base_of_v<QObject,T>) {
            return std::string{T::staticMetaObject.className()};
        } else {
            return typeid(T).name();
        }
#else
        return typeid(T).name();
#endif
    }
};

/*! @brief Convenience wrapper that returns a pretty name for `Type`.
 *  @tparam Type Type whose name should be obtained.
 *  @return Human-readable representation of `Type`.
 *
 *  @details This helper forwards to @ref type_pretty_name_impl<Type>::get(), and is the primary function intended for direct use in
 *           client code. */
template<class Type>
static inline auto type_pretty_name() { return type_pretty_name_impl<Type>::get(); }

/*! @brief Pretty name for `const-qualified` types.
 *
 *  @details For a type of the form `const T`, this specialization prefixes the base pretty name of `T` with `"const "`. The base
 *           name is obtained via @ref type_pretty_name_impl<T>::get().
 *
 *           Examples (assuming suitable specializations exist for the underlying types):
 *           - `type_pretty_name_impl<const int>::get()` -> `"const int"`
 *           - `type_pretty_name_impl<const std::vector<int>>::get()` -> `"const std::vector<int>"`. */
template<class T>
struct type_pretty_name_impl<const T> {
    static std::string get() {
        return std::string{"const "} + type_pretty_name_impl<T>::get();
    }
};

/*! @brief Pretty name for `volatile`-qualified types. */
template<class T>
struct type_pretty_name_impl<volatile T> {
    static std::string get() {
        return std::string{"volatile "} + type_pretty_name_impl<T>::get();
    }
};

/*! @brief Pretty name for `const volatile`-qualified types. */
template<class T>
struct type_pretty_name_impl<const volatile T> {
    static std::string get() {
        return std::string{"const volatile "} + type_pretty_name_impl<T>::get();
    }
};

/*! @brief Pretty name for lvalue references `T&`.
 *  @details Appends `"&"` to the base pretty name of `T`. If `T` itself is cv-qualified, that qualification is already reflected
 *           in the base name. For example:
 *           - `type_pretty_name_impl<int&>::get()` -> `"int&"`
 *           - `type_pretty_name_impl<const int&>::get()` -> `"const int&"`. */
template<class T>
struct type_pretty_name_impl<T&> {
    static std::string get() {
        return type_pretty_name_impl<T>::get() + "&";
    }
};

/*! @brief Pretty name for pointers `T*`.
 *  @details Appends `"*"` to the base pretty name of `T`. If `T` itself is cv-qualified, that qualification is already reflected
 *           in the base name. For example:
 *           - `type_pretty_name_impl<int*>::get()` -> `"int*"`
 *           - `type_pretty_name_impl<const int*>::get()` -> `"const int*"`. */
template<class T>
struct type_pretty_name_impl<T*> {
    static std::string get() {
        return type_pretty_name_impl<T>::get() + "*";
    }
};

/*! @brief Pretty name for `const T* const`. */
template<class T>
struct type_pretty_name_impl<const T* const> {
    static std::string get() {
        return "const " + type_pretty_name_impl<T>::get() + "* const";
    }
};

/*! @brief Pretty name for rvalue references `T&&`. */
template<class T>
struct type_pretty_name_impl<T&&> {
    static std::string get() {
        return type_pretty_name_impl<T>::get() + "&&";
    }
};

}; // namespace draupnir::utils

/*! @def DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(TypeName)
 *  @brief Declares a specialization of @ref type_pretty_name_impl for a concrete (non-template) type.
 *  @param TypeName A fully specified type name (e.g. `int`, `bool*`, `std::string`).
 *
 *  @details This macro generates a specialization of @ref type_pretty_name_impl that returns the textual form of `TypeName` as a
 *           string literal. For example:
 *
 *           @code
 *           DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(int);
 *
 *           // later:
 *           auto name = draupnir::utils::type_pretty_name<int>();  // "int"
 *           @endcode */

#define DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(TypeName)                       \
    template<> struct draupnir::utils::type_pretty_name_impl<TypeName> {       \
        static std::string get() { return #TypeName; }                         \
    };

/*! @def DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(TemplateName)
 *  @brief Declares a specialization of @ref type_pretty_name_impl for all instantiations of
 *         a given class template.
 *  @param TemplateName Name of a class template (e.g. @c std::vector, @c std::tuple).
 *
 *  @details This macro creates a partial specialization of @ref type_pretty_name_impl for types of the form `TemplateName<Ts...>`.
 *           The resulting name is built as: `"TemplateName<pretty(T0), pretty(T1), ...>"` where `pretty(Ti)` is obtained recursively
 *           via @ref type_pretty_name_impl<Ti>::get().
 *
 *           Example:
 *           @code
 *           DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(std::vector);
 *
 *           using V = std::vector<int>;
 *           auto name = draupnir::utils::type_pretty_name<V>(); // "std::vector<int>"
 *           @endcode */

#define DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(TemplateName)             \
    template<class... Ts>                                                      \
    struct draupnir::utils::type_pretty_name_impl<TemplateName<Ts...>> {       \
        static std::string get() {                                             \
            std::string result{#TemplateName};                                 \
            result += '<';                                                     \
            bool first = true;                                                 \
            ((                                                                 \
                result += (first ? "" : ", "),                                 \
                first = false,                                                 \
                result += draupnir::utils::type_pretty_name_impl<Ts>::get()    \
            ), ...);                                                           \
            result += '>';                                                     \
            return result;                                                     \
        }                                                                      \
    };


/**********************************************************************************************************************
 *  Built-in and common library types
 *********************************************************************************************************************/

// void
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(void);

// bool
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(bool);

// integer types
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(short);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(unsigned short);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(int);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(unsigned int);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(long);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(unsigned long);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(long long);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(unsigned long long);

// floating point types
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(float);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(double);

// characters and std::string
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(char);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(unsigned char);
DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(std::string);

// some of the STL templates
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(std::pair);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(std::tuple);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(std::vector);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(std::list);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(std::optional);

template<class Type, std::size_t N>
struct draupnir::utils::type_pretty_name_impl<std::array<Type,N>> {
    static std::string get() {
        std::string result{"std::array<"};
        result += draupnir::utils::type_pretty_name_impl<Type>::get();
        result += ',';
        result += std::to_string(N);
        result += '>';
        return result;
    }
};

#if defined(QT_CORE_LIB)

#include <QHash>
#include <QList>
#include <QMap>
#include <QPair>
#include <QString>
#include <QVector>

DEFINE_PRETTY_SIMPLE_TYPE_NAME_PRINTER(QString);

DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(QHash);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(QList);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(QMap);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(QPair);
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(QVector);

#endif // QT_CORE_LIB

// Draupnir-Lib things
DEFINE_PRETTY_TEMPLATE_INSTANTIATION_PRINTER(draupnir::utils::type_list);

#endif // TYPE_NAME_UTILS_H
