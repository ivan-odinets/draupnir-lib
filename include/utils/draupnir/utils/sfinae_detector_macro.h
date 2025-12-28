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

#ifndef SFINAE_DETECTOR_MACRO_H
#define SFINAE_DETECTOR_MACRO_H

#include <type_traits>

/*! @file draupnir/utils/sfinae_detector_macro.h
 *  @ingroup Utils
 *  @brief Preprocessor helpers for generating small SFINAE-based detectors.
 *  @details This header provides a set of macros that expand into simple traits for probing nested members of a type in a
 *           SFINAE-friendly way. They cover:
 *           - **Nested types**
 *             - DEFINE_HAS_NESTED_TYPE
 *             - DEFINE_EXTRACT_NESTED_TYPE_OR_VOID
 *             - DEFINE_EXTRACT_NESTED_TYPE_OR_ASSERT
 *           - **Nested template type aliases**
 *             - DEFINE_HAS_NESTED_TEMPLATE_TYPE
 *             - DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_VOID
 *             - DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_ASSERT
 *           - **Nested static boolean flags**
 *             - DEFINE_HAS_NESTED_BOOL
 *             - DEFINE_EXTRACT_NESTED_BOOL_OR_FALSE
 *             - DEFINE_EXTRACT_NESTED_BOOL_OR_ASSERT
 *           - **Nested template boolean flags**
 *             - DEFINE_HAS_NESTED_TEMPLATE_BOOL
 *             - DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_FALSE
 *             - DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_ASSERT
 *
 *           Typical usage pattern:
 *           @code
 *           struct Foo {
 *               using  value_type = int;
 *
 *               template<class U>
 *               using Rebind = Foo;
 *
 *               static constexpr bool enabled = true;
 *
 *               template<class U>
 *               static constexpr bool enabled_for = sizeof(U) > 1;
 *           };
 *
 *           // 1. Nested type
 *           DEFINE_HAS_NESTED_TYPE(value_type);
 *           DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(value_type);
 *           DEFINE_EXTRACT_NESTED_TYPE_OR_ASSERT(value_type);
 *
 *           static_assert(has_value_type_v<Foo>);
 *           using VT1 = extract_value_type_or_void_t<Foo>;    // Foo::value_type
 *           using VT2 = extract_value_type_or_void_t<int>;    // void
 *           using VT3 = extract_value_type_or_assert_t<Foo>;  // Foo::value_type, hard error if missing
 *
 *           // 2. Nested template type
 *           DEFINE_HAS_NESTED_TEMPLATE_TYPE(Rebind, int);
 *           DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_VOID(Rebind, int);
 *           DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_ASSERT(Rebind, int);
 *
 *           static_assert(has_Rebind_v<Foo>);
 *           using R1 = extract_Rebind_or_void_t<Foo>;    // Foo::Rebind<int>
 *           using R2 = extract_Rebind_or_void_t<int>;    // void
 *
 *           // 3. Nested bool
 *           DEFINE_HAS_NESTED_BOOL(enabled);
 *           DEFINE_EXTRACT_NESTED_BOOL_OR_FALSE(enabled);
 *           DEFINE_EXTRACT_NESTED_BOOL_OR_ASSERT(enabled);
 *
 *           static_assert(has_enabled_v<Foo>);
 *           static_assert(extract_enabled_or_false_v<Foo>);    // true
 *
 *           // 4. Nested template bool
 *           DEFINE_HAS_NESTED_TEMPLATE_BOOL(enabled_for, int);
 *           DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_FALSE(enabled_for, int);
 *           DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_ASSERT(enabled_for, int);
 *
 *           static_assert(has_enabled_for_v<Foo>);
 *           static_assert(extract_enabled_for_or_false_v<Foo>); // Foo::enabled_for<int>
 *           @endcode */

/*! @def DEFINE_HAS_NESTED_TYPE(NestedType)
 *  @ingroup Utils
 *  @brief Generates `has_NestedType<T>` and `has_NestedType_v<T>` to detect presence of `T::NestedType`.
 *  @details Expands into:
 *           - primary trait `has_NestedType<T>` inheriting from `std::false_type` by default;
 *           - partial specialization that inherits from `std::true_type` when `T::NestedType` is well-formed;
 *           - convenience variable template `has_NestedType_v<T>`. */

#define DEFINE_HAS_NESTED_TYPE(NestedType)                                               \
    template<class T, class = std::void_t<>>                                             \
    struct has_##NestedType : std::false_type {};                                        \
                                                                                         \
    template<class T>                                                                    \
    struct has_##NestedType<T, std::void_t<typename T::NestedType>> : std::true_type {}; \
                                                                                         \
    template<class T>                                                                    \
    static inline constexpr bool has_##NestedType##_v = has_##NestedType<T>::value;

/*! @def DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(NestedType)
 *  @ingroup Utils
 *  @brief Generates `extract_NestedType_or_void<T>` and `extract_NestedType_or_void_t<T>` that yield `T::NestedType` if it exists,
 *         or `void` otherwise.
 *
 * @details The primary template aliases `type = void`. A partial specialization is enabled when `T::NestedType` is a valid type and
 *          aliases `type = T::NestedType`. */

#define DEFINE_EXTRACT_NESTED_TYPE_OR_VOID(NestedType)                                  \
    template<class T, class = std::void_t<>>                                            \
    struct extract_##NestedType##_or_void { using type = void; };                       \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##NestedType##_or_void<T,std::void_t<typename T::NestedType>> {      \
        using type = typename T::NestedType; };                                         \
                                                                                        \
    template<class T>                                                                   \
    using extract_##NestedType##_or_void_t =                                            \
        typename extract_##NestedType##_or_void<T>::type;

/*! @def DEFINE_EXTRACT_NESTED_TYPE_OR_ASSERT(NestedType)
 *  @ingroup Utils
 *  @brief Generates a trait that yields `T::NestedType` or produces a hard compile-time error.
 *  @details The primary template always triggers a `static_assert`, while the partial specialization is enabled when `T::NestedType`
 *           exists and defines `type = T::NestedType`. */

#define DEFINE_EXTRACT_NESTED_TYPE_OR_ASSERT(NestedType)                                \
    template<class T, class = std::void_t<>>                                            \
    struct extract_##NestedType##_or_assert {                                           \
        static_assert(sizeof(T) == 0,                                                   \
            "Specified class does not have nested " #NestedType);                       \
    };                                                                                  \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##NestedType##_or_assert<T, std::void_t<typename T::NestedType>> {   \
        using type = typename T::NestedType;                                            \
    };                                                                                  \
                                                                                        \
    template<class T>                                                                   \
    using extract_##NestedType##_or_assert_t =                                          \
        typename extract_##NestedType##_or_assert<T>::type;


/* **********************************************************************************************
 *  Nested template type: T::template NestedTemplateType<Args...>
 * ******************************************************************************************* */

/*! @def DEFINE_HAS_NESTED_TEMPLATE_TYPE(NestedTemplateType, ...)
 *  @ingroup Utils
 *  @brief Generates `has_NestedTemplateType<T>` and `has_NestedTemplateType_v<T>` to detect presence of `T::template
 *         NestedTemplateType<...>`.
 *
 * @details Uses a probe `T::NestedTemplateType<__VA_ARGS__>` inside `std::void_t` to decide whether the nested template
 *          alias is well-formed for the given arguments. */

#define DEFINE_HAS_NESTED_TEMPLATE_TYPE(NestedTemplateType, ...)                        \
    template<class T, class = std::void_t<>>                                            \
    struct has_##NestedTemplateType : std::false_type {};                               \
                                                                                        \
    template<class T>                                                                   \
    struct has_##NestedTemplateType<                                                    \
        T, std::void_t<typename T::NestedTemplateType<__VA_ARGS__>>                     \
    > : std::true_type {};                                                              \
                                                                                        \
    template<class T>                                                                   \
    static inline constexpr bool has_##NestedTemplateType##_v =                         \
        has_##NestedTemplateType<T>::value;

/*! @def DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_VOID(NestedTemplateType, ...)
 *  @ingroup Utils
 *  @brief Generates an SFINAE-friendly trait that extracts `T::NestedTemplateType<...>` or
 *         falls back to `void` when it is not available.
 *
 *  @details The primary template aliases `type = void`. The partial specialization is enabled
 *           when `T::NestedTemplateType<__VA_ARGS__>` is a valid type and aliases `type` to it. */

#define DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_VOID(Alias,NestedTemplateType, ...)            \
    template<class T, class = std::void_t<>>                                            \
    struct extract_##Alias##_or_void { using type = void; };               \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##Alias##_or_void<                                      \
        T, std::void_t<typename T::template NestedTemplateType<__VA_ARGS__>>                     \
    > { using type = typename T::template NestedTemplateType<__VA_ARGS__>; };                    \
                                                                                        \
    template<class T>                                                                   \
    using extract_##Alias##_or_void_t =                                    \
        typename extract_##Alias##_or_void<T>::type;

/*! @def DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_ASSERT(NestedTemplateType, ...)
 *  @ingroup Utils
 *  @brief Generates a trait that extracts `T::NestedTemplateType<...>` or produces a hard compile-time error when it does not
 *         exist.
 *
 * @details The primary template unconditionally triggers a `static_assert`. The partial specialization is enabled when
 *          `T::NestedTemplateType<__VA_ARGS__>` is well-formed and aliases `type` to that nested template instantiation. */

#define DEFINE_EXTRACT_NESTED_TEMPLATE_TYPE_OR_ASSERT(NestedTemplateType, ...)          \
    template<class T, class = std::void_t<>>                                            \
    struct extract_##NestedTemplateType##_or_assert {                                   \
        static_assert(sizeof(T) == 0,                                                   \
            "Specified class does not have nested template " #NestedTemplateType);      \
    };                                                                                  \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##NestedTemplateType##_or_assert<                                    \
        T, std::void_t<typename T::NestedTemplateType<__VA_ARGS__>>                     \
    > { using type = typename T::NestedTemplateType<__VA_ARGS__>; };                    \
                                                                                        \
    template<class T>                                                                   \
    using extract_##NestedTemplateType##_or_assert_t =                                  \
        typename extract_##NestedTemplateType##_or_assert<T>::type;


/* **********************************************************************************************
 *  Nested bool: static constexpr bool T::NestedBoolName
 * ******************************************************************************************* */

/*! @def DEFINE_HAS_NESTED_BOOL(NestedBoolName)
 *  @ingroup Utils
 *  @brief Generates `has_NestedBoolName<T>` and `has_NestedBoolName_v<T>` to detect presence of a `bool`-typed static data member
 *         `T::NestedBoolName`.
 *
 *  @details The detection succeeds only if `T::NestedBoolName` exists and its type is exactly `bool` (ignoring cv-qualifiers).
 *           Otherwise the trait yields `false`. */

#define DEFINE_HAS_NESTED_BOOL(NestedBoolName)                                          \
    template<class T, class = void>                                                     \
    struct has_##NestedBoolName : std::false_type {};                                   \
                                                                                        \
    template<class T>                                                                   \
    struct has_##NestedBoolName<                                                        \
        T, std::void_t<decltype(T::NestedBoolName)>                                     \
    > : std::bool_constant<                                                             \
            std::is_same_v<                                                             \
                std::remove_cv_t<decltype(T::NestedBoolName)>, bool                     \
            >                                                                           \
        > {};                                                                           \
                                                                                        \
    template<class T>                                                                   \
    inline static constexpr bool has_##NestedBoolName##_v =                             \
        has_##NestedBoolName<T>::value;

/*! @def DEFINE_EXTRACT_NESTED_BOOL_OR_FALSE(NestedBoolName)
 *  @ingroup Utils
 *  @brief Generates a trait that reads `T::NestedBoolName` when it is a `bool`, or yields `false` otherwise.
 *
 *  @details The primary template inherits from `std::false_type`. The partial specialization is enabled when `T::NestedBoolName`
 *           exists and is of type `bool`, and then it inherits from `std::bool_constant<T::NestedBoolName>`. A variable template
 *           `<NestedBoolName>_or_false_v<T>` is also provided. */

#define DEFINE_EXTRACT_NESTED_BOOL_OR_FALSE(NestedBoolName)                             \
    template<class T, class = void>                                                     \
    struct extract_##NestedBoolName##_or_false : std::false_type {};                    \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##NestedBoolName##_or_false<                                         \
        T, std::void_t<decltype(T::NestedBoolName)>>                                    \
        : std::bool_constant<                                                           \
            std::is_same_v<                                                             \
                std::remove_cv_t<decltype(T::NestedBoolName)>, bool                     \
            > && T::NestedBoolName                                                      \
        > {};                                                                           \
                                                                                        \
    template<class T>                                                                   \
    inline static constexpr bool extract_##NestedBoolName##_or_false_v =                \
        extract_##NestedBoolName##_or_false<T>::value;

/*! @def DEFINE_EXTRACT_NESTED_BOOL_OR_ASSERT(NestedBoolName)
 *  @ingroup Utils
 *  @brief Generates a trait that reads `T::NestedBoolName` as a `bool` or produces a compile-time error if it is missing or of the
 *         wrong type.
 *
 *  @details The primary template always triggers a `static_assert`. The partial specialization is enabled when `T::NestedBoolName`
 *           exists and has type `bool`, and provides a `value` static member plus `extract_NestedBoolName_or_assert_v<T>`. */

#define DEFINE_EXTRACT_NESTED_BOOL_OR_ASSERT(NestedBoolName)                            \
    template<class T, class = void>                                                     \
    struct extract_##NestedBoolName##_or_assert {                                       \
        static_assert(sizeof(T) == 0,                                                   \
            "Specified class does not have nested boolean " #NestedBoolName);           \
    };                                                                                  \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##NestedBoolName##_or_assert<                                        \
        T, std::void_t<decltype(T::NestedBoolName)>                                     \
    > {                                                                                 \
        static_assert(                                                                  \
            std::is_same_v<                                                             \
                std::remove_cv_t<decltype(T::NestedBoolName)>, bool                     \
            >,                                                                          \
            "Nested boolean " #NestedBoolName " must be of type bool"                   \
        );                                                                              \
        static constexpr bool value = T::NestedBoolName;                                \
    };                                                                                  \
                                                                                        \
    template<class T>                                                                   \
    inline static constexpr bool extract_##NestedBoolName##_or_assert_v =               \
        extract_##NestedBoolName##_or_assert<T>::value;


/* **********************************************************************************************
 *  Nested template bool: static constexpr bool T::template NestedTemplateBoolName<Args...>
 * ******************************************************************************************* */

/*! @def DEFINE_HAS_NESTED_TEMPLATE_BOOL(Alias, NestedTemplateBoolName, ...)
 *  @ingroup Utils
 *  @brief Generates a detector `has_<Alias><T>` and boolean shortcut `has_<Alias>_v<T>` that check for a nested
 *        `bool`-typed **static template value** `T::template NestedTemplateBoolName<...>`.
 *
 *  @param Alias                 Suffix used in generated identifiers (e.g. `Alias=foo` -> `has_foo<T>`, `has_foo_v<T>`).
 *  @param NestedTemplateBoolName Name of the nested template value to detect (e.g. `enabled`, `hasState`, etc.).
 *  @param ...                   Template arguments passed to `NestedTemplateBoolName<...>`.
 *
 * @details The detection succeeds only if:
 *          - The expression `T::template NestedTemplateBoolName<__VA_ARGS__>` is well-formed, and
 *          - Its type is exactly `bool` (ignoring cv-qualifiers).
 *
 *          This macro is useful when you have feature/trait types that optionally expose a compile-time boolean flag as a
 *          **variable template**.
 *
 *          @code
 *          struct A {
 *              template<class> static constexpr bool enabled = true;
 *          };
 *          struct B { };
 *
 *          DEFINE_HAS_NESTED_TEMPLATE_BOOL(enabled_flag, enabled, int)
 *
 *          static_assert( has_enabled_flag_v<A> );
 *          static_assert( !has_enabled_flag_v<B> );
 *          @endcode */

#define DEFINE_HAS_NESTED_TEMPLATE_BOOL(Alias, NestedTemplateBoolName, ...)             \
    template<class T, class = void>                                                     \
    struct has_##Alias : std::false_type {};                                            \
                                                                                        \
    template<class T>                                                                   \
    struct has_##Alias<                                                                 \
        T, std::void_t<decltype(T::template NestedTemplateBoolName<__VA_ARGS__>)>       \
    > : std::bool_constant<                                                             \
            std::is_same_v<                                                             \
                std::remove_cv_t<decltype(T::template NestedTemplateBoolName<           \
                    __VA_ARGS__>)>,                                                     \
                bool                                                                    \
            >                                                                           \
        > {};                                                                           \
                                                                                        \
    template<class T>                                                                   \
    inline static constexpr bool has_##Alias##_v = has_##Alias<T>::value;


/*! @def DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_FALSE(Alias, NestedTemplateBoolName, ...)
 *  @ingroup Utils
 *  @brief Generates a trait `extract_<Alias>_or_false<T>` that reads `T::template NestedTemplateBoolName<...>` as `bool`,
 *         or yields `false` when unavailable (or when the member exists but is not `bool`-typed).
 *  @param Alias                  Suffix used in generated identifiers (e.g. `Alias=foo` -> `extract_foo_or_false<T>`,
 *                                `extract_foo_or_false_v<T>`).
 *  @param NestedTemplateBoolName Name of the nested template value to read.
 *  @param ...                    Template arguments passed to `NestedTemplateBoolName<...>`.
 *
 *  @details - The primary template inherits from `std::false_type`.
 *           - The specialization is enabled only when the nested expression exists **and** is of type `bool` (ignoring cv).
 *           In that case it inherits from `std::bool_constant< value >`.
 *
 *           This behavior matches the "or_false" intent: anything missing or malformed-as-bool simply becomes `false`.
 *
 *           @code
 *           struct A { template<class> static constexpr bool enabled = true; };
 *           struct B { template<class> static constexpr int  enabled = 123; }; // not bool -> treated as false
 *           struct C { };
 *
 *           DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_FALSE(enabled_flag, enabled, int)
 *
 *           static_assert( extract_enabled_flag_or_false_v<A> == true  );
 *           static_assert( extract_enabled_flag_or_false_v<B> == false );
 *           static_assert( extract_enabled_flag_or_false_v<C> == false );
 *           @endcode */

#define DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_FALSE(Alias, NestedTemplateBoolName, ...)\
    template<class T, class = void>                                                     \
    struct extract_##Alias##_or_false : std::false_type {};                             \
                                                                                        \
    template<class T>                                                                   \
    struct extract_##Alias##_or_false<                                                  \
        T, std::void_t<                                                                 \
            std::enable_if_t<                                                           \
                std::is_same_v<                                                         \
                    std::remove_cv_t<decltype(T::template NestedTemplateBoolName<       \
                        __VA_ARGS__>)>,                                                 \
                    bool                                                                \
                >                                                                       \
            >,                                                                          \
            decltype(T::template NestedTemplateBoolName<__VA_ARGS__>)                   \
        >                                                                               \
    > : std::bool_constant<                                                             \
            T::template NestedTemplateBoolName<__VA_ARGS__>                             \
        > {};                                                                           \
                                                                                        \
    template<class T>                                                                   \
    inline static constexpr bool extract_##Alias##_or_false_v =                         \
        extract_##Alias##_or_false<T>::value;


/*! @def DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_ASSERT(Alias, NestedTemplateBoolName, ...)
 *  @ingroup Utils
 *  @brief Generates a trait `extract_<Alias>_or_assert<T>` that reads `T::template NestedTemplateBoolName<...>` as `bool`,
 *         or produces a compile-time error if it does not exist or is not `bool`-typed.
 *  @param Alias                  Suffix used in generated identifiers (e.g. `Alias=foo` -> `extract_foo_or_assert<T>`,
 *                                `extract_foo_or_assert_v<T>`).
 *  @param NestedTemplateBoolName Name of the nested template value to read.
 *  @param ...                    Template arguments passed to `NestedTemplateBoolName<...>`.
 *
 * @details - The primary template always triggers a `static_assert` when instantiated (i.e. when you try to read it).
 *          - The specialization is enabled when the nested expression exists; it then asserts that the expression type is `bool`
 *            (ignoring cv) and exposes `static constexpr bool value`.
 *
 *          @code
 *          struct A { template<class> static constexpr bool enabled = true; };
 *          struct B { };
 *
 *          DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_ASSERT(enabled_flag, enabled, int)
 *
 *          static_assert( extract_enabled_flag_or_assert_v<A> == true );
 *          // extract_enabled_flag_or_assert_v<B> -> hard error with a clear message
 *          @endcode */

#define DEFINE_EXTRACT_NESTED_TEMPLATE_BOOL_OR_ASSERT(Alias, NestedTemplateBoolName, ...)\
    template<class T, class = void>                                                      \
    struct extract_##Alias##_or_assert {                                                 \
        static_assert(sizeof(T) == 0,                                                    \
            "Specified class does not have nested template boolean "                     \
            #NestedTemplateBoolName);                                                    \
    };                                                                                   \
                                                                                         \
    template<class T>                                                                    \
    struct extract_##Alias##_or_assert<                                                  \
        T, std::void_t<decltype(T::template NestedTemplateBoolName<__VA_ARGS__>)>        \
    > {                                                                                  \
        static_assert(                                                                   \
            std::is_same_v<                                                              \
                std::remove_cv_t<decltype(T::template NestedTemplateBoolName<            \
                    __VA_ARGS__>)>,                                                      \
                bool                                                                     \
            >,                                                                           \
            "Nested template boolean " #NestedTemplateBoolName " must be of type bool"   \
        );                                                                               \
        static constexpr bool value =                                                    \
            T::template NestedTemplateBoolName<__VA_ARGS__>;                             \
    };                                                                                   \
                                                                                         \
    template<class T>                                                                    \
    inline static constexpr bool extract_##Alias##_or_assert_v =                         \
        extract_##Alias##_or_assert<T>::value;


/*! @def DEFINE_HAS_TEMPLATE_STATIC_BOOL_CALL(Alias, StaticMethodName, ...)
 *  @ingroup Utils
 *  @brief Generates `has_<Alias><T>` and `has_<Alias>_v<T>` that detect a well-formed call
 *         `T::template StaticMethodName<__VA_ARGS__>()` with exact return type `bool` (ignoring cv).
 *
 *  @details The trait is `true` only if:
 *           - the call expression is well-formed, and
 *           - `decltype(...)` is `bool` (after cv removal).
 *
 *           @code
 *           struct X { template<class> static constexpr bool ok() { return true; } };
 *           struct Y { template<class> static constexpr int  ok() { return 1; } };
 *           struct Z { };
 *
 *           DEFINE_HAS_TEMPLATE_STATIC_BOOL_CALL(ok_call, ok, int)
 *
 *           static_assert(has_ok_call_v<X>);
 *           static_assert(!has_ok_call_v<Y>);
 *           static_assert(!has_ok_call_v<Z>);
 *           @endcode */

#define DEFINE_HAS_TEMPLATE_STATIC_BOOL_CALL(Alias, StaticMethodName, ...)               \
    template<class T, class = void>                                                      \
    struct has_##Alias : std::false_type {};                                             \
                                                                                         \
    template<class T>                                                                    \
    struct has_##Alias<                                                                  \
        T, std::void_t<decltype(T::template StaticMethodName<__VA_ARGS__>())>            \
    > : std::bool_constant<                                                              \
            std::is_same_v<                                                              \
                bool,                                                                    \
                std::remove_cv_t<decltype(T::template StaticMethodName<__VA_ARGS__>())>  \
            >                                                                            \
        > {};                                                                            \
                                                                                         \
    template<class T>                                                                    \
    inline static constexpr bool has_##Alias##_v = has_##Alias<T>::value;


/*! @def DEFINE_EXTRACT_TEMPLATE_STATIC_BOOL_CALL_OR_FALSE(Alias, StaticMethodName, ...)
 *  @ingroup Utils
 *  @brief Generates `extract_<Alias>_or_false<T>` and `extract_<Alias>_or_false_v<T>` that evaluate
 *         `T::template StaticMethodName<__VA_ARGS__>()` if it exists and returns `bool`, otherwise yield `false`.
 *
 *  @details The specialization is enabled only when the call exists and returns `bool` (ignoring cv), so this macro
 *           is safe even if the method returns `void`/`int`/etc. In those cases it simply falls back to `false`.
 *
 *           @code
 *           struct X { template<class> static constexpr bool ok() { return true; } };
 *           struct Y { template<class> static constexpr void ok() {} };
 *           struct Z { };
 *
 *           DEFINE_EXTRACT_TEMPLATE_STATIC_BOOL_CALL_OR_FALSE(ok_value, ok, int)
 *           static_assert(extract_ok_value_or_false_v<X> == true);
 *           static_assert(extract_ok_value_or_false_v<Y> == false);
 *           static_assert(extract_ok_value_or_false_v<Z> == false);
 *           @endcode */

#define DEFINE_EXTRACT_TEMPLATE_STATIC_BOOL_CALL_OR_FALSE(Alias, StaticMethodName, ...)  \
    template<class T, class = void>                                                      \
    struct extract_##Alias##_or_false : std::false_type {};                              \
                                                                                         \
    template<class T>                                                                    \
    struct extract_##Alias##_or_false<                                                   \
        T, std::void_t<                                                                  \
            std::enable_if_t<                                                            \
                std::is_same_v<                                                          \
                    bool,                                                                \
                    std::remove_cv_t<decltype(                                           \
                        T::template StaticMethodName<__VA_ARGS__>()                      \
                    )>                                                                   \
                >                                                                        \
            >                                                                            \
        >                                                                                \
    > : std::bool_constant<                                                              \
            T::template StaticMethodName<__VA_ARGS__>()                                  \
        > {};                                                                            \
                                                                                          \
    template<class T>                                                                    \
    inline static constexpr bool extract_##Alias##_or_false_v =                          \
        extract_##Alias##_or_false<T>::value;

#endif // SFINAE_DETECTOR_MACRO_H
