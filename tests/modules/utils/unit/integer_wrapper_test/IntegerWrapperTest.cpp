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

#include <QtTest>

#include "draupnir/utils/integer_wrapper.h"

#include "draupnir-test/helpers/TypeHelpers.h"

DEFINE_WRAPPED_INTEGER(FirstChild,int)
DEFINE_WRAPPED_INTEGER(SecondChild,int)
DEFINE_WRAPPED_INTEGER(ThirdChild,short)
DEFINE_WRAPPED_INTEGER(FourthChild,long)

/*! @class IntegerWrapperTest tests/modules/utils/unit/flags_test/IntegerWrapperTest.cpp
 *  @brief Unit test for @ref draupnir::utils::integer_wrapper. */

class IntegerWrapperTest final : public QObject
{
    Q_OBJECT
private:
    static constexpr int firstArgument  = 0b0001'0001;
    static constexpr int secondArgument = 0b1000'0011;
    static constexpr int firstAndSecond = firstArgument & secondArgument;
    static constexpr int firstOrSecond  = firstArgument | secondArgument;
    static constexpr int firstXorSecond = firstArgument ^ secondArgument;

    static constexpr int firstArgumentShiftedLeft  = firstArgument << 1;
    static constexpr int firstArgumentShiftedRight = firstArgument >> 1;

private slots:
    void test_compile_time_properties() {
        // Underlying type for wrapper children
        TYPE_COMPARE(FirstChild::underlying_type,int);
        TYPE_COMPARE(ThirdChild::underlying_type,short);
        TYPE_COMPARE(FourthChild::underlying_type,long);

        // Constructors for wrapper children
        QVERIFY(std::is_default_constructible_v<FirstChild>);
        QVERIFY((std::is_constructible_v<FirstChild, short>));
        QVERIFY((std::is_constructible_v<FirstChild, int>));
        QVERIFY((std::is_constructible_v<FirstChild, FirstChild>));
        QVERIFY((!std::is_constructible_v<FirstChild, SecondChild>));
        QVERIFY((!std::is_constructible_v<SecondChild, FirstChild>));

        // Assignment for wrapper children
        QVERIFY((std::is_assignable_v<FirstChild&, short>));
        QVERIFY((std::is_assignable_v<FirstChild&, int>));
        QVERIFY((std::is_assignable_v<FirstChild&, FirstChild>));
        //QVERIFY((!std::is_assignable_v<FirstChild&,long>));
        QVERIFY((!std::is_assignable_v<FirstChild&, SecondChild>));
        QVERIFY((!std::is_assignable_v<SecondChild&, FirstChild>));

        // Type identity, children
        QVERIFY((std::is_same_v<FirstChild, FirstChild>));
        QVERIFY(!(std::is_same_v<FirstChild, int>));
        QVERIFY(!(std::is_same_v<FirstChild, SecondChild>));
    }

    void test_constructor() {
        // Constructor w/o arguments
        FirstChild noArguments{};
        QCOMPARE(noArguments.value(), 0);

        // Int literal
        FirstChild intLiteral{42};
        QCOMPARE(intLiteral.value(), 42);

        // FirstChild from another firstChild
        FirstChild subchild{intLiteral};
        QCOMPARE(subchild.value(), 42);

        // FirstChild from SecondChild - should fail
        // FirstChild fail{SecondChild{42}};
    }

    void test_assignment() {
        // Assigning numeric literal
        FirstChild child{};
        child = 42;
        QCOMPARE(child.value(), 42);

        // Assigning another first child
        FirstChild assignedChild{44};
        assignedChild = child;
        QCOMPARE(assignedChild.value(), 42);

        // FirstChild from SecondChild - should fail
        // FirstChild fail{};
        // fail = SecondChild{42};
    }

    void test_comparing() {
        FirstChild one{42};
        FirstChild two{92};
        // Comparing with integer literal
        QVERIFY(one == 42);
        QVERIFY(one != 92);
        QVERIFY(42 == one);
        QVERIFY(92 != one);

        // Comparing with integer variable
        const int v = 50;
        QVERIFY(one < v);
        QVERIFY(v > one);
        QVERIFY(two > v);
        QVERIFY(v < two);

        // Comparing together
        QVERIFY(one == FirstChild{one.value()});
        QVERIFY(one != two);
        QVERIFY(one < two);
        QVERIFY(two > one);
    }

    void test_assigning_bitwise_operators_with_integers() {
        FirstChild child{};

        // Test assigning bitwise AND operator for wrapper
        child = firstArgument;
        child &= secondArgument;
        QCOMPARE(child, firstAndSecond);
        // child &= SecondChild{42};

        // Test assigning bitwise OR operator for wrapper
        child = firstArgument;
        child |= secondArgument;
        QCOMPARE(child, firstOrSecond);
        // child |= SecondChild{42};

        // Test assigning bitwise XOR operator for wrapper
        child = firstArgument;
        child ^= secondArgument;
        QCOMPARE(child, firstXorSecond);
        // child ^= SecondChild{42};
    }

    void test_non_assigning_bitwise_operators_with_integers() {
        FirstChild child{};

        // Test non-assigning bitwise AND operator for wrapper
        child = firstArgument;
        QCOMPARE(child &            secondArgument,  firstAndSecond);
        QCOMPARE(child & FirstChild{secondArgument}, firstAndSecond);
        // child & SecondChild{42};

        // Test non-assigning bitwise OR operator for wrapper
        child = firstArgument;
        QCOMPARE(child |            secondArgument,  firstOrSecond);
        QCOMPARE(child | FirstChild{secondArgument}, firstOrSecond);
        // child | SecondChild{42};

        // Test non-assigning bitwise XOR operator for wrapper
        child = firstArgument;
        QCOMPARE(child ^            secondArgument,  firstXorSecond);
        QCOMPARE(child ^ FirstChild{secondArgument}, firstXorSecond);
        // child & SecondChild{42};
    }

    void test_assigning_bit_shift_operators() {
        FirstChild child{firstArgument};
        child <<= 1;
        QCOMPARE(child.value(), firstArgumentShiftedLeft);

        child = firstArgument;
        child >>= 1;
        QCOMPARE(child.value(), firstArgumentShiftedRight);
    }

    void test_non_assigning_bit_shift_operators() {
        FirstChild child{firstArgument};
        QCOMPARE(child << 1, firstArgumentShiftedLeft);

        child = firstArgument;
        QCOMPARE(child >> 1, firstArgumentShiftedRight);
    }
};

QTEST_APPLESS_MAIN(IntegerWrapperTest)

#include "IntegerWrapperTest.moc"
