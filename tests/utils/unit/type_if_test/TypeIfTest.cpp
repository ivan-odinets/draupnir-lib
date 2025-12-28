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

#include <QtTest>

#include "draupnir/utils/type_if.h"

#include "draupnir-test/helpers/TypeHelpers.h"

/*! @class TypeIfTest tests/utils/unit/type_presense_test/TypeIfTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/type_if.h. */

using namespace draupnir::utils;

template<class First, class... Args>
struct AngryTemplate {
    static_assert(sizeof...(Args) == 0, "I am angry template!");
    First number = First::number(42);
    First test{"I am angry template!"};
};

struct ShouldWork
{
    using nope = int;
};

template<class T>
using BadAlias = typename T::nope;

class TypeIfTest final : public QObject
{
    Q_OBJECT
public:

private slots:
    void test_compilation_success() {
        using ShouldBeInt = type_if<false>
            ::then_template<AngryTemplate,double,int,float>
            ::else_type<
                type_if<false>
                ::then_template<BadAlias,int>
                ::else_type<int>
            ::result
        >::result;
        ShouldBeInt value{42};
        Q_UNUSED(value);
        TYPE_COMPARE(ShouldBeInt,int);
    }

    void test_if_then_else_types_behaviour() {
        using first_result = type_if<true>
            ::then_type<int>
            ::else_type<void>
        ::result;
        TYPE_COMPARE(first_result,int);

        using second_result = type_if<false>
            ::then_type<int>
            ::else_type<void>
        ::result;
        TYPE_COMPARE(second_result, void);

        using nested_result = type_if<false>
            ::then_type<double>
            ::else_type<type_if<false>
                ::then_type<float>
                ::else_type<void>
            ::result>
        ::result;
        TYPE_COMPARE(nested_result, void);
    };

    void test_templates_behavior() {
        using should_be_vector = type_if<false>
            ::then_t1_a1<std::array,int,10>
            ::else_template<std::vector,int>
        ::result;
        TYPE_COMPARE(should_be_vector, std::vector<int>);

        using should_be_array = type_if<false>
            ::then_template<std::vector,int>
            ::else_t1_a1<std::array,int,10>
        ::result;
        using array = std::array<int,10>;
        TYPE_COMPARE(should_be_array, array);
    }
};

QTEST_APPLESS_MAIN(TypeIfTest)

#include "TypeIfTest.moc"
