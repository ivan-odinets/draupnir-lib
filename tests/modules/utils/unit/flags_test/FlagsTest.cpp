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

#include "draupnir/utils/flags.h"
#include "draupnir/utils/integer_wrapper.h"
#include "draupnir/utils/concepts/operator_concepts.h"

DEFINE_WRAPPED_INTEGER(WrappedInt,int)
DEFINE_WRAPPED_INTEGER(WrappedOtherInt,int)

using namespace draupnir::utils;

/*! @class FlagsTest tests/modules/utils/unit/flags_test/FlagsTest.cpp
 *  @brief Test class for testing @ref draupnir::utils::flags. */

class FlagsTest final : public QObject
{
    Q_OBJECT
private:
    static constexpr int firstArgument  = 0b0001'0001;
    static constexpr int secondArgument = 0b1000'0011;
    static constexpr int firstAndSecond = firstArgument & secondArgument;
    static constexpr int firstOrSecond  = firstArgument | secondArgument;
    static constexpr int firstXorSecond = firstArgument ^ secondArgument;

    enum MyEnum { One   = 0b0001, Two   = 0b0010, Three = 0b0100, Four  = 0b1000, All   = 0b1111 };
    enum MyOtherEnum { Blah = 0x01, OtherBlah = 0x02 };

    using MyFlags      = draupnir::utils::flags<int>;
    using MyOtherFlags = draupnir::utils::flags<long>;

    using MyEnumFlags      = draupnir::utils::enum_flags<MyEnum>;
    using MyOtherEnumFlags = draupnir::utils::enum_flags<MyOtherEnum>;

    using MyWrappedFlags      = draupnir::utils::flags<WrappedInt>;
    using MyOtherWrappedFlags = draupnir::utils::flags<WrappedOtherInt>;

private slots:
    void test_construction() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer can be constructed from built-in integer
        QCOMPARE((std::is_constructible_v<MyFlags, int>), true);
        // flags based on built-in integer can be constructed from any enum
        QCOMPARE((std::is_constructible_v<MyFlags, MyEnum>), true);
        QCOMPARE((std::is_constructible_v<MyFlags, MyOtherEnum>), true);
        // flags based on built-in integer can not be constructed from bool
        QCOMPARE((std::is_constructible_v<MyFlags, bool>), false);
        // flags based on built-in integer can not be constructed from any integer wrapper
        QCOMPARE((std::is_constructible_v<MyFlags, WrappedInt>), false);
        QCOMPARE((std::is_constructible_v<MyFlags, WrappedOtherInt>), false);
        // flags based on built-in integer support copy construction from the same flags type
        QCOMPARE((std::is_constructible_v<MyFlags, MyFlags>), true);
        // flags based on built-in integer can not be constructed from flags-based objects of other types
        QCOMPARE((std::is_constructible_v<MyFlags, MyEnumFlags>), false);
        QCOMPARE((std::is_constructible_v<MyFlags, MyOtherFlags>), false);
        QCOMPARE((std::is_constructible_v<MyFlags, MyWrappedFlags>), false);

        // Let's verify runtime construction behavior
        QCOMPARE(MyFlags().value(), 0);
        QCOMPARE(MyFlags(42).value(), 42);
        QCOMPARE(MyFlags(MyEnum::All).value(), MyEnum::All);
        QCOMPARE(MyFlags(MyFlags(42)).value(), 42);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer can be constructed from built-in integer
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, int>), true);
        // flags based on wrapped integer can be constructed from any enum
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyOtherEnum>), true);
        // flags based on wrapped integer can not be constructed from bool
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, bool>), false);
        // flags based on wrapped integer can be constructed from matching wrapped integer
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, WrappedInt>), true);
        // flags based on wrapped integer can not be constructed from non-matching wrapped integer
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, WrappedOtherInt>), false);
        // flags based on wrapped integer support copy construction from the same flags type
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be constructed from flags-based objects of other types
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyFlags>), false);
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyOtherFlags>), false);
        QCOMPARE((std::is_constructible_v<MyWrappedFlags, MyOtherWrappedFlags>), false);

        // Let's verify runtime construction behavior
        QCOMPARE(MyWrappedFlags().value(), 0);
        QCOMPARE(MyWrappedFlags(42).value(), 42);
        QCOMPARE(MyWrappedFlags(WrappedInt(42)).value(), 42);
        QCOMPARE(MyWrappedFlags(MyEnum::All).value(), MyEnum::All);
        QCOMPARE(MyWrappedFlags(MyWrappedFlags(42)).value(), 42);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags can be constructed from built-in integer
        QCOMPARE((std::is_constructible_v<MyEnumFlags, int>), true);
        // enum_flags can be constructed from matching enum
        QCOMPARE((std::is_constructible_v<MyEnumFlags, MyEnum>), true);
        // enum_flags can not be constructed from non-matching enum
        QCOMPARE((std::is_constructible_v<MyEnumFlags, MyOtherEnum>), false);
        // enum_flags can not be constructed from bool
        QCOMPARE((std::is_constructible_v<MyEnumFlags, bool>), false);
        // enum_flags can not be constructed from any integer wrapper
        QCOMPARE((std::is_constructible_v<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((std::is_constructible_v<MyEnumFlags, WrappedOtherInt>), false);
        // enum_flags support copy construction from the same enum_flags type
        QCOMPARE((std::is_constructible_v<MyEnumFlags, MyEnumFlags>), true);
        // enum_flags can not be constructed from flags-based objects of other types
        QCOMPARE((std::is_constructible_v<MyEnumFlags, MyFlags>), false);
        QCOMPARE((std::is_constructible_v<MyEnumFlags, MyOtherEnumFlags>), false);
        QCOMPARE((std::is_constructible_v<MyEnumFlags, MyWrappedFlags>), false);

        // Let's verify runtime construction behavior
        QCOMPARE(MyEnumFlags().value(), 0);
        QCOMPARE(MyEnumFlags(42).value(), 42);
        QCOMPARE(MyEnumFlags(MyEnum::All).value(), MyEnum::All);
        QCOMPARE(MyEnumFlags(MyEnumFlags(42)).value(), 42);
    }

    void test_conversion() {
        // Testing draupnir::utils::flags<int>
        // built-in integer can be implicitly converted to flags based on built-in integer
        QCOMPARE((std::convertible_to<int, MyFlags>), true);
        // flags based on built-in integer can not be converted back to built-in integer
        QCOMPARE((std::convertible_to<MyFlags, int>), false);
        // any enum can be implicitly converted to flags based on built-in integer
        QCOMPARE((std::convertible_to<MyEnum, MyFlags>), true);
        QCOMPARE((std::convertible_to<MyOtherEnum, MyFlags>), true);
        // flags based on built-in integer can not be converted to any enum
        QCOMPARE((std::convertible_to<MyFlags, MyEnum>), false);
        QCOMPARE((std::convertible_to<MyFlags, MyOtherEnum>), false);
        // bool can not be implicitly converted to flags based on built-in integer
        QCOMPARE((std::convertible_to<bool, MyFlags>), false);
        // flags based on built-in integer can not be implicitly converted to bool
        QCOMPARE((std::convertible_to<MyFlags, bool>), false);
        // integer wrappers can not be implicitly converted to flags based on built-in integer
        QCOMPARE((std::convertible_to<WrappedInt, MyFlags>), false);
        QCOMPARE((std::convertible_to<WrappedOtherInt, MyFlags>), false);
        // flags based on built-in integer can not be implicitly converted to any integer wrapper
        QCOMPARE((std::convertible_to<MyFlags, WrappedInt>), false);
        QCOMPARE((std::convertible_to<MyFlags, WrappedOtherInt>), false);
        // same-type implicit conversion is available
        QCOMPARE((std::convertible_to<MyFlags, MyFlags>), true);
        // no other flags-based type can be implicitly converted to flags based on built-in integer
        QCOMPARE((std::convertible_to<MyOtherFlags, MyFlags>), false);
        QCOMPARE((std::convertible_to<MyEnumFlags, MyFlags>), false);
        QCOMPARE((std::convertible_to<MyWrappedFlags, MyFlags>), false);


        // Testing draupnir::utils::flags<WrappedInt>
        // built-in integer can be implicitly converted to flags based on wrapped integer
        QCOMPARE((std::convertible_to<int, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be converted back to built-in integer
        QCOMPARE((std::convertible_to<MyWrappedFlags, int>), false);
        // any enum can be implicitly converted to flags based on wrapped integer
        QCOMPARE((std::convertible_to<MyEnum, MyWrappedFlags>), true);
        QCOMPARE((std::convertible_to<MyOtherEnum, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be converted to any enum
        QCOMPARE((std::convertible_to<MyWrappedFlags, MyEnum>), false);
        QCOMPARE((std::convertible_to<MyWrappedFlags, MyOtherEnum>), false);
        // bool can not be implicitly converted to flags based on wrapped integer
        QCOMPARE((std::convertible_to<bool, MyWrappedFlags>), false);
        // flags based on wrapped integer can not be implicitly converted to bool
        QCOMPARE((std::convertible_to<MyWrappedFlags, bool>), false);
        // matching integer wrapper can be implicitly converted to flags based on wrapped integer
        QCOMPARE((std::convertible_to<WrappedInt, MyWrappedFlags>), true);
        // non-matching integer wrapper can not be implicitly converted to flags based on wrapped integer
        QCOMPARE((std::convertible_to<WrappedOtherInt, MyWrappedFlags>), false);
        // flags based on wrapped integer can not be converted to any integer wrapper
        QCOMPARE((std::convertible_to<MyWrappedFlags, WrappedInt>), false);
        QCOMPARE((std::convertible_to<MyWrappedFlags, WrappedOtherInt>), false);
        // same-type implicit conversion is available
        QCOMPARE((std::convertible_to<MyWrappedFlags, MyWrappedFlags>), true);
        // no other flags-based type can be implicitly converted to flags based on wrapped integer
        QCOMPARE((std::convertible_to<MyFlags, MyWrappedFlags>), false);
        QCOMPARE((std::convertible_to<MyOtherWrappedFlags, MyWrappedFlags>), false);
        QCOMPARE((std::convertible_to<MyEnumFlags, MyWrappedFlags>), false);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // built-in integer can be implicitly converted to enum_flags
        QCOMPARE((std::convertible_to<int, MyEnumFlags>), true);
        // enum_flags can not be converted back to built-in integer
        QCOMPARE((std::convertible_to<MyEnumFlags, int>), false);
        // matching enum can be implicitly converted to enum_flags
        QCOMPARE((std::convertible_to<MyEnum, MyEnumFlags>), true);
        // non-matching enum can not be implicitly converted to enum_flags
        QCOMPARE((std::convertible_to<MyOtherEnum, MyEnumFlags>), false);
        // enum_flags can not be converted to any enum
        QCOMPARE((std::convertible_to<MyEnumFlags, MyEnum>), false);
        QCOMPARE((std::convertible_to<MyEnumFlags, MyOtherEnum>), false);
        // bool can not be implicitly converted to enum_flags
        QCOMPARE((std::convertible_to<bool, MyEnumFlags>), false);
        // enum_flags can not be implicitly converted to bool
        QCOMPARE((std::convertible_to<MyEnumFlags, bool>), false);
        // integer wrappers can not be implicitly converted to enum_flags
        QCOMPARE((std::convertible_to<WrappedInt, MyEnumFlags>), false);
        QCOMPARE((std::convertible_to<WrappedOtherInt, MyEnumFlags>), false);
        // enum_flags can not be converted to any integer wrapper
        QCOMPARE((std::convertible_to<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((std::convertible_to<MyEnumFlags, WrappedOtherInt>), false);
        // same-type implicit conversion is available
        QCOMPARE((std::convertible_to<MyEnumFlags, MyEnumFlags>), true);
        // no other flags-based type can be implicitly converted to enum_flags
        QCOMPARE((std::convertible_to<MyFlags, MyEnumFlags>), false);
        QCOMPARE((std::convertible_to<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((std::convertible_to<MyOtherEnumFlags, MyEnumFlags>), false);
    }

    void test_assignment() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer can be assigned from built-in integer
        QCOMPARE((std::is_assignable_v<MyFlags&, int>), true);
        // flags based on built-in integer can not be assigned from bool
        QCOMPARE((std::is_assignable_v<MyFlags&, bool>), false);
        // flags based on built-in integer can be assigned from any enum
        QCOMPARE((std::is_assignable_v<MyFlags&, MyEnum>), true);
        QCOMPARE((std::is_assignable_v<MyFlags&, MyOtherEnum>), true);
        // flags based on built-in integer can not be assigned from any integer wrapper
        QCOMPARE((std::is_assignable_v<MyFlags&, WrappedInt>), false);
        QCOMPARE((std::is_assignable_v<MyFlags&, WrappedOtherInt>), false);
        // flags based on built-in integer support assignment from the same flags type
        QCOMPARE((std::is_assignable_v<MyFlags&, MyFlags>), true);
        // flags based on built-in integer can not be assigned from flags-based objects of other types
        QCOMPARE((std::is_assignable_v<MyFlags&, MyOtherFlags>), false);
        QCOMPARE((std::is_assignable_v<MyFlags&, MyEnumFlags>), false);
        QCOMPARE((std::is_assignable_v<MyFlags&, MyWrappedFlags>), false);

        // Let's verify runtime assignment behavior
        MyFlags flags;
        flags = 42;
        QCOMPARE(flags.value(), 42);
        flags = MyEnum::All;
        QCOMPARE(flags.value(), MyEnum::All);
        flags = MyFlags(24);
        QCOMPARE(flags.value(), 24);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer can be assigned from built-in integer
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, int>), true);
        // flags based on wrapped integer can not be assigned from bool
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, bool>), false);
        // flags based on wrapped integer can be assigned from any enum
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, MyEnum>), true);
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, MyOtherEnum>), true);
        // flags based on wrapped integer can be assigned from matching integer wrapper
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, WrappedInt>), true);
        // flags based on wrapped integer can not be assigned from non-matching integer wrapper
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, WrappedOtherInt>), false);
        // flags based on wrapped integer support assignment from the same flags type
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be assigned from flags-based objects of other types
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, MyOtherFlags>), false);
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, MyEnumFlags>), false);
        QCOMPARE((std::is_assignable_v<MyWrappedFlags&, MyOtherWrappedFlags>), false);

        // Let's verify runtime assignment behavior
        MyWrappedFlags wrappedFlags;
        wrappedFlags = 42;
        QCOMPARE(wrappedFlags.value(), 42);
        wrappedFlags = MyEnum::One;
        QCOMPARE(wrappedFlags.value(), MyEnum::One);
        wrappedFlags = WrappedInt{92};
        QCOMPARE(wrappedFlags.value(), 92);
        wrappedFlags = MyWrappedFlags(125);
        QCOMPARE(wrappedFlags.value(), 125);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags can be assigned from built-in integer
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, int>), true);
        // enum_flags can not be assigned from bool
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, bool>), false);
        // enum_flags can be assigned from matching enum
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, MyEnum>), true);
        // enum_flags can not be assigned from non-matching enum
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, MyOtherEnum>), false);
        // enum_flags can not be assigned from any integer wrapper
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, WrappedInt>), false);
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, WrappedOtherInt>), false);
        // enum_flags support assignment from the same enum_flags type
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, MyEnumFlags>), true);
        // enum_flags can not be assigned from flags-based objects of other types
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, MyFlags>), false);
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, MyOtherEnumFlags>), false);
        QCOMPARE((std::is_assignable_v<MyEnumFlags&, MyWrappedFlags>), false);

        // Let's verify runtime assignment behavior
        MyEnumFlags myEnumFlags;
        myEnumFlags = 42;
        QCOMPARE(myEnumFlags.value(), 42);
        myEnumFlags = MyEnum::Three;
        QCOMPARE(myEnumFlags.value(), MyEnum::Three);
    }

    void test_equality() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer can be compared with built-in integer
        QCOMPARE((equal_comparable<MyFlags, int>), true);
        // flags based on built-in integer can be compared with the same flags type
        QCOMPARE((equal_comparable<MyFlags, MyFlags>), true);
        // flags based on built-in integer can be compared with any enum
        QCOMPARE((equal_comparable<MyFlags, MyEnum>), true);
        QCOMPARE((equal_comparable<MyFlags, MyOtherEnum>), true);
        // flags based on built-in integer can not be compared with any integer wrapper
        QCOMPARE((equal_comparable<MyFlags, WrappedInt>), false);
        QCOMPARE((equal_comparable<MyFlags, WrappedOtherInt>), false);
        // flags based on built-in integer can not be compared with flags-based objects of other types
        QCOMPARE((equal_comparable<MyFlags, MyWrappedFlags>), false);
        QCOMPARE((equal_comparable<MyFlags, MyEnumFlags>), false);

        // Let's verify runtime equality behavior
        QVERIFY(MyFlags(42) == 42);
        QVERIFY(MyFlags(42) != 92);
        QVERIFY(MyFlags(MyEnum::All) == MyEnum::All);
        QVERIFY(MyFlags(MyEnum::All) != MyEnum::One);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer can be compared with matching integer wrapper
        QCOMPARE((equal_comparable<MyWrappedFlags, WrappedInt>), true);
        // flags based on wrapped integer can be compared with the same flags type
        QCOMPARE((equal_comparable<MyWrappedFlags, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be compared with non-matching integer wrapper
        QCOMPARE((equal_comparable<MyWrappedFlags, WrappedOtherInt>), false);
        // flags based on wrapped integer can be compared with any enum
        QCOMPARE((equal_comparable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((equal_comparable<MyWrappedFlags, MyOtherEnum>), true);
        // flags based on wrapped integer can not be compared with flags-based objects of other types
        QCOMPARE((equal_comparable<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((equal_comparable<MyWrappedFlags, MyFlags>), false);
        QCOMPARE((equal_comparable<MyWrappedFlags, MyOtherWrappedFlags>), false);

        // Let's verify runtime equality behavior
        QVERIFY(MyWrappedFlags(42) == 42);
        QVERIFY(MyWrappedFlags(42) != 92);
        QVERIFY(MyWrappedFlags(42) == WrappedInt(42));
        QVERIFY(MyWrappedFlags(42) != WrappedInt(92));
        QVERIFY(MyWrappedFlags(MyEnum::All) == MyEnum::All);
        QVERIFY(MyWrappedFlags(MyEnum::All) != MyEnum::One);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags can be compared with built-in integer
        QCOMPARE((equal_comparable<MyEnumFlags, int>), true);
        // enum_flags can be compared with the same enum_flags type
        QCOMPARE((equal_comparable<MyEnumFlags, MyEnumFlags>), true);
        // enum_flags can be compared with matching enum
        QCOMPARE((equal_comparable<MyEnumFlags, MyEnum>), true);
        // enum_flags can not be compared with non-matching enum
        QCOMPARE((equal_comparable<MyEnumFlags, MyOtherEnum>), false);
        // enum_flags can not be compared with flags-based objects of other types
        QCOMPARE((equal_comparable<MyEnumFlags, MyFlags>), false);
        QCOMPARE((equal_comparable<MyEnumFlags, MyWrappedFlags>), false);
        QCOMPARE((equal_comparable<MyEnumFlags, MyOtherEnumFlags>), false);

        // Let's verify runtime equality behavior
        QVERIFY(MyEnumFlags(42) == 42);
        QVERIFY(MyEnumFlags(42) != 92);
        QVERIFY(MyEnumFlags(MyEnum::Two) == MyEnum::Two);
        QVERIFY(MyEnumFlags(MyEnum::Two) != MyEnum::All);
        QVERIFY(MyEnumFlags(MyEnum::Two) == MyEnumFlags(MyEnum::Two));
        QVERIFY(MyEnumFlags(MyEnum::Two) != MyEnumFlags(MyEnum::All));
    }

    void test_three_way_comparison() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer can be three-way compared with built-in integer
        QCOMPARE((three_way_comparable<MyFlags, int>), true);
        // flags based on built-in integer can be three-way compared with the same flags type
        QCOMPARE((three_way_comparable<MyFlags, MyFlags>), true);
        // flags based on built-in integer can be three-way compared with any enum
        QCOMPARE((three_way_comparable<MyFlags, MyEnum>), true);
        QCOMPARE((three_way_comparable<MyFlags, MyOtherEnum>), true);
        // flags based on built-in integer can not be three-way compared with any integer wrapper
        QCOMPARE((three_way_comparable<MyFlags, WrappedInt>), false);
        QCOMPARE((three_way_comparable<MyFlags, WrappedOtherInt>), false);
        // flags based on built-in integer can not be three-way compared with flags-based objects of other types
        QCOMPARE((three_way_comparable<MyFlags, MyWrappedFlags>), false);
        QCOMPARE((three_way_comparable<MyFlags, MyEnumFlags>), false);

        // Let's verify runtime three-way comparison behavior
        QVERIFY(MyFlags(42) > 12);
        QVERIFY(MyFlags(42) < 92);
        QVERIFY(MyFlags(MyEnum::All) <= MyEnum::All);
        QVERIFY(MyFlags(MyEnum::All) >= MyEnum::One);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer can be three-way compared with matching integer wrapper
        QCOMPARE((three_way_comparable<MyWrappedFlags, WrappedInt>), true);
        // flags based on wrapped integer can be three-way compared with the same flags type
        QCOMPARE((three_way_comparable<MyWrappedFlags, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be three-way compared with non-matching integer wrapper
        QCOMPARE((three_way_comparable<MyWrappedFlags, WrappedOtherInt>), false);
        // flags based on wrapped integer can be three-way compared with any enum
        QCOMPARE((three_way_comparable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((three_way_comparable<MyWrappedFlags, MyOtherEnum>), true);
        // flags based on wrapped integer can not be three-way compared with flags-based objects of other types
        QCOMPARE((three_way_comparable<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((three_way_comparable<MyWrappedFlags, MyFlags>), false);
        QCOMPARE((three_way_comparable<MyWrappedFlags, MyOtherWrappedFlags>), false);

        // Let's verify runtime three-way comparison behavior
        QVERIFY(MyWrappedFlags(42) > 24);
        QVERIFY(MyWrappedFlags(42) < 92);
        QVERIFY(MyWrappedFlags(42) >= WrappedInt(42));
        QVERIFY(MyWrappedFlags(42) <= WrappedInt(92));
        QVERIFY(MyWrappedFlags(MyEnum::All) <= MyEnum::All);
        QVERIFY(MyWrappedFlags(MyEnum::All) >= MyEnum::One);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags can be three-way compared with built-in integer
        QCOMPARE((three_way_comparable<MyEnumFlags, int>), true);
        // enum_flags can be three-way compared with the same enum_flags type
        QCOMPARE((three_way_comparable<MyEnumFlags, MyEnumFlags>), true);
        // enum_flags can be three-way compared with matching enum
        QCOMPARE((three_way_comparable<MyEnumFlags, MyEnum>), true);
        // enum_flags can not be three-way compared with non-matching enum
        QCOMPARE((three_way_comparable<MyEnumFlags, MyOtherEnum>), false);
        // enum_flags can not be three-way compared with flags-based objects of other types
        QCOMPARE((three_way_comparable<MyEnumFlags, MyFlags>), false);
        QCOMPARE((three_way_comparable<MyEnumFlags, MyWrappedFlags>), false);
        QCOMPARE((three_way_comparable<MyEnumFlags, MyOtherEnumFlags>), false);

        // Let's verify runtime three-way comparison behavior
        QVERIFY(MyEnumFlags(42) > 24);
        QVERIFY(MyEnumFlags(42) < 92);
        QVERIFY(MyEnumFlags(MyEnum::Two) >= MyEnum::Two);
        QVERIFY(MyEnumFlags(MyEnum::Two) <= MyEnum::All);
        QVERIFY(MyEnumFlags(MyEnum::Two) >= MyEnumFlags(MyEnum::Two));
        QVERIFY(MyEnumFlags(MyEnum::Two) <= MyEnumFlags(MyEnum::All));
    }

    void test_helpers() {
        // Compile-time checks for helper API

        // MyFlags
        QCOMPARE((test_flag_callable<MyFlags, int>), true);
        QCOMPARE((test_flag_callable<MyFlags, MyEnum>), true);
        QCOMPARE((test_flag_callable<MyFlags, MyOtherEnum>), true);
        QCOMPARE((test_flag_callable<MyFlags, bool>), false);
        QCOMPARE((test_flag_callable<MyFlags, WrappedInt>), false);
        QCOMPARE((test_flag_callable<MyFlags, WrappedOtherInt>), false);

        QCOMPARE((set_flag_callable<MyFlags, int>), true);
        QCOMPARE((set_flag_callable<MyFlags, MyEnum>), true);
        QCOMPARE((set_flag_callable<MyFlags, MyOtherEnum>), true);
        QCOMPARE((set_flag_callable<MyFlags, bool>), false);
        QCOMPARE((set_flag_callable<MyFlags, WrappedInt>), false);
        QCOMPARE((set_flag_callable<MyFlags, WrappedOtherInt>), false);

        // MyWrappedFlags
        QCOMPARE((test_flag_callable<MyWrappedFlags, int>), true);
        QCOMPARE((test_flag_callable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((test_flag_callable<MyWrappedFlags, MyOtherEnum>), true);
        QCOMPARE((test_flag_callable<MyWrappedFlags, WrappedInt>), true);
        QCOMPARE((test_flag_callable<MyWrappedFlags, WrappedOtherInt>), false);
        QCOMPARE((test_flag_callable<MyWrappedFlags, bool>), false);

        QCOMPARE((set_flag_callable<MyWrappedFlags, int>), true);
        QCOMPARE((set_flag_callable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((set_flag_callable<MyWrappedFlags, MyOtherEnum>), true);
        QCOMPARE((set_flag_callable<MyWrappedFlags, WrappedInt>), true);
        QCOMPARE((set_flag_callable<MyWrappedFlags, WrappedOtherInt>), false);
        QCOMPARE((set_flag_callable<MyWrappedFlags, bool>), false);

        // MyEnumFlags
        QCOMPARE((test_flag_callable<MyEnumFlags, int>), true);
        QCOMPARE((test_flag_callable<MyEnumFlags, MyEnum>), true);
        QCOMPARE((test_flag_callable<MyEnumFlags, MyOtherEnum>), false);
        QCOMPARE((test_flag_callable<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((test_flag_callable<MyEnumFlags, WrappedOtherInt>), false);
        QCOMPARE((test_flag_callable<MyEnumFlags, bool>), false);

        QCOMPARE((set_flag_callable<MyEnumFlags, int>), true);
        QCOMPARE((set_flag_callable<MyEnumFlags, MyEnum>), true);
        QCOMPARE((set_flag_callable<MyEnumFlags, MyOtherEnum>), false);
        QCOMPARE((set_flag_callable<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((set_flag_callable<MyEnumFlags, WrappedOtherInt>), false);
        QCOMPARE((set_flag_callable<MyEnumFlags, bool>), false);


        // Runtime checks for MyFlags
        MyFlags f{};
        QVERIFY(f.none());
        QVERIFY(!f.any());
        QVERIFY(f.test_flag(0));

        f.set_flag(One, true);
        QVERIFY(f.any());
        QVERIFY(!f.none());
        QVERIFY(f.test_flag(One));
        QVERIFY(!f.test_flag(Two));
        QVERIFY(!f.test_flag(All));
        QVERIFY(!f.test_flag(0));

        f.set_flag(Two);
        QCOMPARE(f.value(), (One | Two));
        QVERIFY(f.test_flag(One));
        QVERIFY(f.test_flag(Two));
        QVERIFY(!f.test_flag(Three));
        QVERIFY(!f.test_flag(All));

        f.set_flag(One, false);
        QCOMPARE(f.value(), Two);
        QVERIFY(!f.test_flag(One));
        QVERIFY(f.test_flag(Two));

        f.set_flag(All, false);
        QCOMPARE(f.value(), 0);
        QVERIFY(f.none());
        QVERIFY(f.test_flag(0));

        f.set_flag(All, true);
        QCOMPARE(f.value(), All);
        QVERIFY(f.test_flag(One));
        f.set_flag(One, false);
        QVERIFY(!f.test_flag(One));


        // Runtime checks for MyWrappedFlags
        MyWrappedFlags wrapped{};
        QVERIFY(wrapped.none());
        QVERIFY(!wrapped.any());
        QVERIFY(wrapped.test_flag(0));
        QVERIFY(wrapped.test_flag(WrappedInt{0}));

        wrapped.set_flag(WrappedInt{One});
        QVERIFY(wrapped.any());
        QVERIFY(wrapped.test_flag(WrappedInt{One}));
        QVERIFY(!wrapped.test_flag(WrappedInt{Two}));

        wrapped.set_flag(MyEnum::Two);
        QCOMPARE(wrapped.value(), (One | Two));
        QVERIFY(wrapped.test_flag(WrappedInt{One}));
        QVERIFY(wrapped.test_flag(MyEnum::Two));

        wrapped.set_flag(WrappedInt{One}, false);
        QCOMPARE(wrapped.value(), Two);
        QVERIFY(!wrapped.test_flag(WrappedInt{One}));
        QVERIFY(wrapped.test_flag(WrappedInt{Two}));


        // Runtime checks for MyEnumFlags
        MyEnumFlags enumFlags{};
        QVERIFY(enumFlags.none());
        QVERIFY(!enumFlags.any());
        QVERIFY(enumFlags.test_flag(0));

        enumFlags.set_flag(MyEnum::One);
        QVERIFY(enumFlags.any());
        QVERIFY(enumFlags.test_flag(MyEnum::One));
        QVERIFY(!enumFlags.test_flag(MyEnum::Two));

        enumFlags.set_flag(MyEnum::Two);
        QCOMPARE(enumFlags.value(), (One | Two));
        QVERIFY(enumFlags.test_flag(MyEnum::One));
        QVERIFY(enumFlags.test_flag(MyEnum::Two));
        QVERIFY(!enumFlags.test_flag(MyEnum::Three));

        enumFlags.set_flag(MyEnum::One, false);
        QCOMPARE(enumFlags.value(), Two);
        QVERIFY(!enumFlags.test_flag(MyEnum::One));
        QVERIFY(enumFlags.test_flag(MyEnum::Two));

        enumFlags.set_flag(All, false);
        QCOMPARE(enumFlags.value(), 0);
        QVERIFY(enumFlags.none());
        QVERIFY(enumFlags.test_flag(0));
    }

    void test_assigning_bitwise_and() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer support bitwise AND-assignment from built-in integer
        QCOMPARE((bitwise_and_assignable<MyFlags&, int>), true);
        // flags based on built-in integer can not be bitwise AND-assigned from bool
        QCOMPARE((bitwise_and_assignable<MyFlags&, bool>), false);
        // flags based on built-in integer support bitwise AND-assignment from any enum
        QCOMPARE((bitwise_and_assignable<MyFlags&, MyEnum>), true);
        QCOMPARE((bitwise_and_assignable<MyFlags&, MyOtherEnum>), true);
        // flags based on built-in integer can not be bitwise AND-assigned from any integer wrapper
        QCOMPARE((bitwise_and_assignable<MyFlags&, WrappedInt>), false);
        QCOMPARE((bitwise_and_assignable<MyFlags&, WrappedOtherInt>), false);
        // flags based on built-in integer support bitwise AND-assignment from the same flags type
        QCOMPARE((bitwise_and_assignable<MyFlags&, MyFlags>), true);
        // flags based on built-in integer can not be bitwise AND-assigned from flags-based objects of other types
        QCOMPARE((bitwise_and_assignable<MyFlags&, MyOtherFlags>), false);
        QCOMPARE((bitwise_and_assignable<MyFlags&, MyEnumFlags>), false);
        QCOMPARE((bitwise_and_assignable<MyFlags&, MyWrappedFlags>), false);

        // Let's verify runtime bitwise AND-assignment behavior
        MyFlags flags(firstArgument);
        flags &= secondArgument;
        QCOMPARE(flags.value(), firstAndSecond);
        flags = MyEnum::All;
        flags &= MyEnum::One;
        QCOMPARE(flags.value(), MyEnum::One);
        flags = MyFlags(firstArgument);
        flags &= MyFlags(secondArgument);
        QCOMPARE(flags.value(), firstAndSecond);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer support bitwise AND-assignment from built-in integer
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, int>), true);
        // flags based on wrapped integer can not be bitwise AND-assigned from bool
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, bool>), false);
        // flags based on wrapped integer support bitwise AND-assignment from any enum
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, MyEnum>), true);
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, MyOtherEnum>), true);
        // flags based on wrapped integer support bitwise AND-assignment from matching integer wrapper
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, WrappedInt>), true);
        // flags based on wrapped integer can not be bitwise AND-assigned from non-matching integer wrapper
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, WrappedOtherInt>), false);
        // flags based on wrapped integer support bitwise AND-assignment from the same flags type
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be bitwise AND-assigned from flags-based objects of other types
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, MyOtherFlags>), false);
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, MyEnumFlags>), false);
        QCOMPARE((bitwise_and_assignable<MyWrappedFlags&, MyOtherWrappedFlags>), false);

        // Let's verify runtime bitwise AND-assignment behavior
        MyWrappedFlags wrappedFlags(firstArgument);
        wrappedFlags &= secondArgument;
        QCOMPARE(wrappedFlags.value(), firstAndSecond);
        wrappedFlags = MyEnum::All;
        wrappedFlags &= MyEnum::Two;
        QCOMPARE(wrappedFlags.value(), MyEnum::Two);
        wrappedFlags = WrappedInt(firstArgument);
        wrappedFlags &= WrappedInt(secondArgument);
        QCOMPARE(wrappedFlags.value(), firstAndSecond);
        wrappedFlags = MyWrappedFlags(firstArgument);
        wrappedFlags &= MyWrappedFlags(secondArgument);
        QCOMPARE(wrappedFlags.value(), firstAndSecond);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags support bitwise AND-assignment from built-in integer
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, int>), true);
        // enum_flags can not be bitwise AND-assigned from bool
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, bool>), false);
        // enum_flags support bitwise AND-assignment from matching enum
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, MyEnum>), true);
        // enum_flags can not be bitwise AND-assigned from non-matching enum
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, MyOtherEnum>), false);
        // enum_flags can not be bitwise AND-assigned from any integer wrapper
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, WrappedInt>), false);
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, WrappedOtherInt>), false);
        // enum_flags support bitwise AND-assignment from the same enum_flags type
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, MyEnumFlags>), true);
        // enum_flags can not be bitwise AND-assigned from flags-based objects of other types
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, MyFlags>), false);
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, MyOtherEnumFlags>), false);
        QCOMPARE((bitwise_and_assignable<MyEnumFlags&, MyWrappedFlags>), false);

        // Let's verify runtime bitwise AND-assignment behavior
        MyEnumFlags myEnumFlags;
        myEnumFlags = MyEnum::All;
        myEnumFlags &= MyEnum::Three;
        QCOMPARE(myEnumFlags.value(), MyEnum::Three);
        myEnumFlags = firstArgument;
        myEnumFlags &= secondArgument;
        QCOMPARE(myEnumFlags.value(), firstAndSecond);
        myEnumFlags = MyEnumFlags(firstArgument);
        myEnumFlags &= MyEnumFlags(secondArgument);
        QCOMPARE(myEnumFlags.value(), firstAndSecond);
    }

    void test_assigning_bitwise_or() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer support bitwise OR-assignment from built-in integer
        QCOMPARE((bitwise_or_assignable<MyFlags&, int>), true);
        // flags based on built-in integer can not be bitwise OR-assigned from bool
        QCOMPARE((bitwise_or_assignable<MyFlags&, bool>), false);
        // flags based on built-in integer support bitwise OR-assignment from any enum
        QCOMPARE((bitwise_or_assignable<MyFlags&, MyEnum>), true);
        QCOMPARE((bitwise_or_assignable<MyFlags&, MyOtherEnum>), true);
        // flags based on built-in integer can not be bitwise OR-assigned from any integer wrapper
        QCOMPARE((bitwise_or_assignable<MyFlags&, WrappedInt>), false);
        QCOMPARE((bitwise_or_assignable<MyFlags&, WrappedOtherInt>), false);
        // flags based on built-in integer support bitwise OR-assignment from the same flags type
        QCOMPARE((bitwise_or_assignable<MyFlags&, MyFlags>), true);
        // flags based on built-in integer can not be bitwise OR-assigned from flags-based objects of other types
        QCOMPARE((bitwise_or_assignable<MyFlags&, MyOtherFlags>), false);
        QCOMPARE((bitwise_or_assignable<MyFlags&, MyEnumFlags>), false);
        QCOMPARE((bitwise_or_assignable<MyFlags&, MyWrappedFlags>), false);

        // Let's verify runtime bitwise OR-assignment behavior
        MyFlags flags(firstArgument);
        flags |= secondArgument;
        QCOMPARE(flags.value(), firstOrSecond);
        flags = MyEnum::Three;
        flags |= MyEnum::One;
        QCOMPARE(flags.value(), MyEnum::One | MyEnum::Three);
        flags = MyFlags(firstArgument);
        flags |= MyFlags(secondArgument);
        QCOMPARE(flags.value(), firstOrSecond);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer support bitwise OR-assignment from built-in integer
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, int>), true);
        // flags based on wrapped integer can not be bitwise OR-assigned from bool
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, bool>), false);
        // flags based on wrapped integer support bitwise OR-assignment from any enum
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, MyEnum>), true);
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, MyOtherEnum>), true);
        // flags based on wrapped integer support bitwise OR-assignment from matching integer wrapper
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, WrappedInt>), true);
        // flags based on wrapped integer can not be bitwise OR-assigned from non-matching integer wrapper
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, WrappedOtherInt>), false);
        // flags based on wrapped integer support bitwise OR-assignment from the same flags type
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be bitwise OR-assigned from flags-based objects of other types
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, MyOtherFlags>), false);
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, MyEnumFlags>), false);
        QCOMPARE((bitwise_or_assignable<MyWrappedFlags&, MyOtherWrappedFlags>), false);

        // Let's verify runtime bitwise OR-assignment behavior
        MyWrappedFlags wrappedFlags(firstArgument);
        wrappedFlags |= secondArgument;
        QCOMPARE(wrappedFlags.value(), firstOrSecond);
        wrappedFlags = MyEnum::Four;
        wrappedFlags |= MyEnum::Two;
        QCOMPARE(wrappedFlags.value(), MyEnum::Four | MyEnum::Two);
        wrappedFlags = WrappedInt(firstArgument);
        wrappedFlags |= WrappedInt(secondArgument);
        QCOMPARE(wrappedFlags.value(), firstOrSecond);
        wrappedFlags = MyWrappedFlags(firstArgument);
        wrappedFlags |= MyWrappedFlags(secondArgument);
        QCOMPARE(wrappedFlags.value(), firstOrSecond);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags support bitwise OR-assignment from built-in integer
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, int>), true);
        // enum_flags can not be bitwise OR-assigned from bool
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, bool>), false);
        // enum_flags support bitwise OR-assignment from matching enum
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, MyEnum>), true);
        // enum_flags can not be bitwise OR-assigned from non-matching enum
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, MyOtherEnum>), false);
        // enum_flags can not be bitwise OR-assigned from any integer wrapper
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, WrappedInt>), false);
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, WrappedOtherInt>), false);
        // enum_flags support bitwise OR-assignment from the same enum_flags type
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, MyEnumFlags>), true);
        // enum_flags can not be bitwise OR-assigned from flags-based objects of other types
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, MyFlags>), false);
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, MyOtherEnumFlags>), false);
        QCOMPARE((bitwise_or_assignable<MyEnumFlags&, MyWrappedFlags>), false);

        // Let's verify runtime bitwise OR-assignment behavior
        MyEnumFlags myEnumFlags;
        myEnumFlags = MyEnum::One;
        myEnumFlags |= MyEnum::Three;
        QCOMPARE(myEnumFlags.value(), MyEnum::Three | MyEnum::One);
        myEnumFlags = firstArgument;
        myEnumFlags |= secondArgument;
        QCOMPARE(myEnumFlags.value(), firstOrSecond);
        myEnumFlags = MyEnumFlags(firstArgument);
        myEnumFlags |= MyEnumFlags(secondArgument);
        QCOMPARE(myEnumFlags.value(), firstOrSecond);
    }

    void test_assigning_bitwise_xor() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer support bitwise XOR-assignment from built-in integer
        QCOMPARE((bitwise_xor_assignable<MyFlags&, int>), true);
        // flags based on built-in integer can not be bitwise XOR-assigned from bool
        QCOMPARE((bitwise_xor_assignable<MyFlags&, bool>), false);
        // flags based on built-in integer support bitwise XOR-assignment from any enum
        QCOMPARE((bitwise_xor_assignable<MyFlags&, MyEnum>), true);
        QCOMPARE((bitwise_xor_assignable<MyFlags&, MyOtherEnum>), true);
        // flags based on built-in integer can not be bitwise XOR-assigned from any integer wrapper
        QCOMPARE((bitwise_xor_assignable<MyFlags&, WrappedInt>), false);
        QCOMPARE((bitwise_xor_assignable<MyFlags&, WrappedOtherInt>), false);
        // flags based on built-in integer support bitwise XOR-assignment from the same flags type
        QCOMPARE((bitwise_xor_assignable<MyFlags&, MyFlags>), true);
        // flags based on built-in integer can not be bitwise XOR-assigned from flags-based objects of other types
        QCOMPARE((bitwise_xor_assignable<MyFlags&, MyOtherFlags>), false);
        QCOMPARE((bitwise_xor_assignable<MyFlags&, MyEnumFlags>), false);
        QCOMPARE((bitwise_xor_assignable<MyFlags&, MyWrappedFlags>), false);

        // Let's verify runtime bitwise XOR-assignment behavior
        MyFlags flags(firstArgument);
        flags ^= secondArgument;
        QCOMPARE(flags.value(), firstXorSecond);
        flags = MyEnum::Three;
        flags ^= MyEnum::One;
        QCOMPARE(flags.value(), MyEnum::One ^ MyEnum::Three);
        flags = MyFlags(firstArgument);
        flags ^= MyFlags(secondArgument);
        QCOMPARE(flags.value(), firstXorSecond);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer support bitwise XOR-assignment from built-in integer
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, int>), true);
        // flags based on wrapped integer can not be bitwise XOR-assigned from bool
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, bool>), false);
        // flags based on wrapped integer support bitwise XOR-assignment from any enum
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, MyEnum>), true);
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, MyOtherEnum>), true);
        // flags based on wrapped integer support bitwise XOR-assignment from matching integer wrapper
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, WrappedInt>), true);
        // flags based on wrapped integer can not be bitwise XOR-assigned from non-matching integer wrapper
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, WrappedOtherInt>), false);
        // flags based on wrapped integer support bitwise XOR-assignment from the same flags type
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, MyWrappedFlags>), true);
        // flags based on wrapped integer can not be bitwise XOR-assigned from flags-based objects of other types
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, MyOtherFlags>), false);
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, MyEnumFlags>), false);
        QCOMPARE((bitwise_xor_assignable<MyWrappedFlags&, MyOtherWrappedFlags>), false);

        // Let's verify runtime bitwise XOR-assignment behavior
        MyWrappedFlags wrappedFlags(firstArgument);
        wrappedFlags ^= secondArgument;
        QCOMPARE(wrappedFlags.value(), firstXorSecond);
        wrappedFlags = MyEnum::Four;
        wrappedFlags ^= MyEnum::Two;
        QCOMPARE(wrappedFlags.value(), MyEnum::Four ^ MyEnum::Two);
        wrappedFlags = WrappedInt(firstArgument);
        wrappedFlags ^= WrappedInt(secondArgument);
        QCOMPARE(wrappedFlags.value(), firstXorSecond);
        wrappedFlags = MyWrappedFlags(firstArgument);
        wrappedFlags ^= MyWrappedFlags(secondArgument);
        QCOMPARE(wrappedFlags.value(), firstXorSecond);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags support bitwise XOR-assignment from built-in integer
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, int>), true);
        // enum_flags can not be bitwise XOR-assigned from bool
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, bool>), false);
        // enum_flags support bitwise XOR-assignment from matching enum
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, MyEnum>), true);
        // enum_flags can not be bitwise XOR-assigned from non-matching enum
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, MyOtherEnum>), false);
        // enum_flags can not be bitwise XOR-assigned from any integer wrapper
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, WrappedInt>), false);
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, WrappedOtherInt>), false);
        // enum_flags support bitwise XOR-assignment from the same enum_flags type
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, MyEnumFlags>), true);
        // enum_flags can not be bitwise XOR-assigned from flags-based objects of other types
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, MyFlags>), false);
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, MyOtherEnumFlags>), false);
        QCOMPARE((bitwise_xor_assignable<MyEnumFlags&, MyWrappedFlags>), false);

        // Let's verify runtime bitwise XOR-assignment behavior
        MyEnumFlags myEnumFlags;
        myEnumFlags = MyEnum::One;
        myEnumFlags ^= MyEnum::Three;
        QCOMPARE(myEnumFlags.value(), MyEnum::Three ^ MyEnum::One);
        myEnumFlags = firstArgument;
        myEnumFlags ^= secondArgument;
        QCOMPARE(myEnumFlags.value(), firstXorSecond);
        myEnumFlags = MyEnumFlags(firstArgument);
        myEnumFlags ^= MyEnumFlags(secondArgument);
        QCOMPARE(myEnumFlags.value(), firstXorSecond);
    }

    void test_non_assigning_and() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer support non-assigning bitwise AND with built-in integer
        QCOMPARE((bitwise_and_appliable<MyFlags, int>), true);
        // flags based on built-in integer do not support non-assigning bitwise AND with bool
        QCOMPARE((bitwise_and_appliable<MyFlags, bool>), false);
        // flags based on built-in integer support non-assigning bitwise AND with any enum
        QCOMPARE((bitwise_and_appliable<MyFlags, MyEnum>), true);
        QCOMPARE((bitwise_and_appliable<MyFlags, MyOtherEnum>), true);
        // flags based on built-in integer do not support non-assigning bitwise AND with any integer wrapper
        QCOMPARE((bitwise_and_appliable<MyFlags, WrappedInt>), false);
        QCOMPARE((bitwise_and_appliable<MyFlags, WrappedOtherInt>), false);
        // flags based on built-in integer support non-assigning bitwise AND with the same flags type
        QCOMPARE((bitwise_and_appliable<MyFlags, MyFlags>), true);
        // flags based on built-in integer do not support non-assigning bitwise AND with flags-based objects of other types
        QCOMPARE((bitwise_and_appliable<MyFlags, MyOtherFlags>), false);
        QCOMPARE((bitwise_and_appliable<MyFlags, MyEnumFlags>), false);
        QCOMPARE((bitwise_and_appliable<MyFlags, MyWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise AND behavior
        MyFlags flags(firstArgument);
        QCOMPARE(flags & secondArgument, firstAndSecond);
        flags = MyEnum::All;
        QCOMPARE(flags & MyEnum::One, MyEnum::One);
        flags = MyFlags(firstArgument);
        QCOMPARE(flags & MyFlags(secondArgument), firstAndSecond);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer support non-assigning bitwise AND with built-in integer
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, int>), true);
        // flags based on wrapped integer do not support non-assigning bitwise AND with bool
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, bool>), false);
        // flags based on wrapped integer support non-assigning bitwise AND with any enum
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, MyOtherEnum>), true);
        // flags based on wrapped integer support non-assigning bitwise AND with matching integer wrapper
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, WrappedInt>), true);
        // flags based on wrapped integer do not support non-assigning bitwise AND with non-matching integer wrapper
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, WrappedOtherInt>), false);
        // flags based on wrapped integer support non-assigning bitwise AND with the same flags type
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, MyWrappedFlags>), true);
        // flags based on wrapped integer do not support non-assigning bitwise AND with flags-based objects of other types
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, MyOtherFlags>), false);
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((bitwise_and_appliable<MyWrappedFlags, MyOtherWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise AND behavior
        MyWrappedFlags wrappedFlags(firstArgument);
        QCOMPARE(wrappedFlags & secondArgument, firstAndSecond);
        wrappedFlags = MyEnum::All;
        QCOMPARE(wrappedFlags & MyEnum::Two, MyEnum::Two);
        wrappedFlags = WrappedInt(firstArgument);
        QCOMPARE(wrappedFlags & WrappedInt(secondArgument), firstAndSecond);
        wrappedFlags = MyWrappedFlags(firstArgument);
        QCOMPARE(wrappedFlags & MyWrappedFlags(secondArgument), firstAndSecond);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags support non-assigning bitwise AND with built-in integer
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, int>), true);
        // enum_flags do not support non-assigning bitwise AND with bool
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, bool>), false);
        // enum_flags support non-assigning bitwise AND with matching enum
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, MyEnum>), true);
        // enum_flags do not support non-assigning bitwise AND with non-matching enum
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, MyOtherEnum>), false);
        // enum_flags do not support non-assigning bitwise AND with any integer wrapper
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, WrappedOtherInt>), false);
        // enum_flags support non-assigning bitwise AND with the same enum_flags type
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, MyEnumFlags>), true);
        // enum_flags do not support non-assigning bitwise AND with flags-based objects of other types
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, MyFlags>), false);
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, MyOtherEnumFlags>), false);
        QCOMPARE((bitwise_and_appliable<MyEnumFlags, MyWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise AND behavior
        MyEnumFlags myEnumFlags = MyEnum::All;
        QCOMPARE(myEnumFlags & MyEnum::Three, MyEnum::Three);
        myEnumFlags = firstArgument;
        QCOMPARE(myEnumFlags & secondArgument, firstAndSecond);
        myEnumFlags = MyEnumFlags(firstArgument);
        QCOMPARE(myEnumFlags & MyEnumFlags(secondArgument), firstAndSecond);
    }

    void test_non_assigning_or() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer support non-assigning bitwise OR with built-in integer
        QCOMPARE((bitwise_or_appliable<MyFlags, int>), true);
        // flags based on built-in integer do not support non-assigning bitwise OR with bool
        QCOMPARE((bitwise_or_appliable<MyFlags, bool>), false);
        // flags based on built-in integer support non-assigning bitwise OR with any enum
        QCOMPARE((bitwise_or_appliable<MyFlags, MyEnum>), true);
        QCOMPARE((bitwise_or_appliable<MyFlags, MyOtherEnum>), true);
        // flags based on built-in integer do not support non-assigning bitwise OR with any integer wrapper
        QCOMPARE((bitwise_or_appliable<MyFlags, WrappedInt>), false);
        QCOMPARE((bitwise_or_appliable<MyFlags, WrappedOtherInt>), false);
        // flags based on built-in integer support non-assigning bitwise OR with the same flags type
        QCOMPARE((bitwise_or_appliable<MyFlags, MyFlags>), true);
        // flags based on built-in integer do not support non-assigning bitwise OR with flags-based objects of other types
        QCOMPARE((bitwise_or_appliable<MyFlags, MyOtherFlags>), false);
        QCOMPARE((bitwise_or_appliable<MyFlags, MyEnumFlags>), false);
        QCOMPARE((bitwise_or_appliable<MyFlags, MyWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise OR behavior
        MyFlags flags(firstArgument);
        QCOMPARE(flags | secondArgument, firstOrSecond);
        flags = MyEnum::One;
        QCOMPARE(flags | MyEnum::Three, MyEnum::One | MyEnum::Three);
        flags = MyFlags(firstArgument);
        QCOMPARE(flags | MyFlags(secondArgument), firstOrSecond);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer support non-assigning bitwise OR with built-in integer
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, int>), true);
        // flags based on wrapped integer do not support non-assigning bitwise OR with bool
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, bool>), false);
        // flags based on wrapped integer support non-assigning bitwise OR with any enum
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, MyOtherEnum>), true);
        // flags based on wrapped integer support non-assigning bitwise OR with matching integer wrapper
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, WrappedInt>), true);
        // flags based on wrapped integer do not support non-assigning bitwise OR with non-matching integer wrapper
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, WrappedOtherInt>), false);
        // flags based on wrapped integer support non-assigning bitwise OR with the same flags type
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, MyWrappedFlags>), true);
        // flags based on wrapped integer do not support non-assigning bitwise OR with flags-based objects of other types
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, MyOtherFlags>), false);
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((bitwise_or_appliable<MyWrappedFlags, MyOtherWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise OR behavior
        MyWrappedFlags wrappedFlags(firstArgument);
        QCOMPARE(wrappedFlags | secondArgument, firstOrSecond);
        wrappedFlags = MyEnum::Four;
        QCOMPARE(wrappedFlags | MyEnum::Two, MyEnum::Two | MyEnum::Four);
        wrappedFlags = WrappedInt(firstArgument);
        QCOMPARE(wrappedFlags | WrappedInt(secondArgument), firstOrSecond);
        wrappedFlags = MyWrappedFlags(firstArgument);
        QCOMPARE(wrappedFlags | MyWrappedFlags(secondArgument), firstOrSecond);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags support non-assigning bitwise OR with built-in integer
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, int>), true);
        // enum_flags do not support non-assigning bitwise OR with bool
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, bool>), false);
        // enum_flags support non-assigning bitwise OR with matching enum
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, MyEnum>), true);
        // enum_flags do not support non-assigning bitwise OR with non-matching enum
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, MyOtherEnum>), false);
        // enum_flags do not support non-assigning bitwise OR with any integer wrapper
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, WrappedOtherInt>), false);
        // enum_flags support non-assigning bitwise OR with the same enum_flags type
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, MyEnumFlags>), true);
        // enum_flags do not support non-assigning bitwise OR with flags-based objects of other types
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, MyFlags>), false);
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, MyOtherEnumFlags>), false);
        QCOMPARE((bitwise_or_appliable<MyEnumFlags, MyWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise OR behavior
        MyEnumFlags myEnumFlags = MyEnum::One;
        QCOMPARE(myEnumFlags | MyEnum::Three, MyEnum::One | MyEnum::Three);
        myEnumFlags = firstArgument;
        QCOMPARE(myEnumFlags | secondArgument, firstOrSecond);
        myEnumFlags = MyEnumFlags(firstArgument);
        QCOMPARE(myEnumFlags | MyEnumFlags(secondArgument), firstOrSecond);
    }

    void test_non_assigning_xor() {
        // Testing draupnir::utils::flags<int>
        // flags based on built-in integer support non-assigning bitwise XOR with built-in integer
        QCOMPARE((bitwise_xor_appliable<MyFlags, int>), true);
        // flags based on built-in integer do not support non-assigning bitwise XOR with bool
        QCOMPARE((bitwise_xor_appliable<MyFlags, bool>), false);
        // flags based on built-in integer support non-assigning bitwise XOR with any enum
        QCOMPARE((bitwise_xor_appliable<MyFlags, MyEnum>), true);
        QCOMPARE((bitwise_xor_appliable<MyFlags, MyOtherEnum>), true);
        // flags based on built-in integer do not support non-assigning bitwise XOR with any integer wrapper
        QCOMPARE((bitwise_xor_appliable<MyFlags, WrappedInt>), false);
        QCOMPARE((bitwise_xor_appliable<MyFlags, WrappedOtherInt>), false);
        // flags based on built-in integer support non-assigning bitwise XOR with the same flags type
        QCOMPARE((bitwise_xor_appliable<MyFlags, MyFlags>), true);
        // flags based on built-in integer do not support non-assigning bitwise XOR with flags-based objects of other types
        QCOMPARE((bitwise_xor_appliable<MyFlags, MyOtherFlags>), false);
        QCOMPARE((bitwise_xor_appliable<MyFlags, MyEnumFlags>), false);
        QCOMPARE((bitwise_xor_appliable<MyFlags, MyWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise XOR behavior
        MyFlags flags(firstArgument);
        QCOMPARE(flags ^ secondArgument, firstXorSecond);
        flags = MyEnum::One;
        QCOMPARE(flags ^ MyEnum::Three, MyEnum::One ^ MyEnum::Three);
        flags = MyFlags(firstArgument);
        QCOMPARE(flags ^ MyFlags(secondArgument), firstXorSecond);


        // Testing draupnir::utils::flags<WrappedInt>
        // flags based on wrapped integer support non-assigning bitwise XOR with built-in integer
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, int>), true);
        // flags based on wrapped integer do not support non-assigning bitwise XOR with bool
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, bool>), false);
        // flags based on wrapped integer support non-assigning bitwise XOR with any enum
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, MyEnum>), true);
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, MyOtherEnum>), true);
        // flags based on wrapped integer support non-assigning bitwise XOR with matching integer wrapper
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, WrappedInt>), true);
        // flags based on wrapped integer do not support non-assigning bitwise XOR with non-matching integer wrapper
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, WrappedOtherInt>), false);
        // flags based on wrapped integer support non-assigning bitwise XOR with the same flags type
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, MyWrappedFlags>), true);
        // flags based on wrapped integer do not support non-assigning bitwise XOR with flags-based objects of other types
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, MyOtherFlags>), false);
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, MyEnumFlags>), false);
        QCOMPARE((bitwise_xor_appliable<MyWrappedFlags, MyOtherWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise XOR behavior
        MyWrappedFlags wrappedFlags(firstArgument);
        QCOMPARE(wrappedFlags ^ secondArgument, firstXorSecond);
        wrappedFlags = MyEnum::Four;
        QCOMPARE(wrappedFlags ^ MyEnum::Two, MyEnum::Two ^ MyEnum::Four);
        wrappedFlags = WrappedInt(firstArgument);
        QCOMPARE(wrappedFlags ^ WrappedInt(secondArgument), firstXorSecond);
        wrappedFlags = MyWrappedFlags(firstArgument);
        QCOMPARE(wrappedFlags ^ MyWrappedFlags(secondArgument), firstXorSecond);


        // Testing draupnir::utils::enum_flags<MyEnum>
        // enum_flags support non-assigning bitwise XOR with built-in integer
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, int>), true);
        // enum_flags do not support non-assigning bitwise XOR with bool
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, bool>), false);
        // enum_flags support non-assigning bitwise XOR with matching enum
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, MyEnum>), true);
        // enum_flags do not support non-assigning bitwise XOR with non-matching enum
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, MyOtherEnum>), false);
        // enum_flags do not support non-assigning bitwise XOR with any integer wrapper
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, WrappedInt>), false);
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, WrappedOtherInt>), false);
        // enum_flags support non-assigning bitwise XOR with the same enum_flags type
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, MyEnumFlags>), true);
        // enum_flags do not support non-assigning bitwise XOR with flags-based objects of other types
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, MyFlags>), false);
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, MyOtherEnumFlags>), false);
        QCOMPARE((bitwise_xor_appliable<MyEnumFlags, MyWrappedFlags>), false);

        // Let's verify runtime non-assigning bitwise XOR behavior
        MyEnumFlags myEnumFlags = MyEnum::One;
        QCOMPARE(myEnumFlags ^ MyEnum::Three, MyEnum::One ^ MyEnum::Three);
        myEnumFlags = firstArgument;
        QCOMPARE(myEnumFlags ^ secondArgument, firstXorSecond);
        myEnumFlags = MyEnumFlags(firstArgument);
        QCOMPARE(myEnumFlags ^ MyEnumFlags(secondArgument), firstXorSecond);
    }
};

QTEST_APPLESS_MAIN(FlagsTest)

#include "FlagsTest.moc"
