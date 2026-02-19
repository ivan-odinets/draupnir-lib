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

/*! @class FlagsTest tests/utils/unit/flags_test/FlagsTest.cpp
 *  @brief Test class for testing @ref draupnir::utils::flags. */

class FlagsTest final : public QObject
{
    Q_OBJECT
public:
    enum MyEnum {
        One   = 0b0001,
        Two   = 0b0010,
        Three = 0b0100,
        Four  = 0b1000,
        All   = 0b1111
    };
    using MyFlags = draupnir::utils::enum_flags<MyEnum>;

private slots:
    void test_initialization() {
        QCOMPARE(MyFlags{}.value(), 0);
        QCOMPARE(MyFlags{1}.value(), 1);
        QCOMPARE(MyFlags{Three}.value(), Three);
        QVERIFY(MyFlags{Three}.testFlag(Three));

        // initializer_list ctor
        QCOMPARE((MyFlags{ {One, Three} }.value()), (One | Three));
        QVERIFY((MyFlags{ {One, Three} }.testFlag(One)));
        QVERIFY((MyFlags{ {One, Three} }.testFlag(Three)));
        QVERIFY((!MyFlags{ {One, Three} }.testFlag(Two)));

        // any/none
        QVERIFY(MyFlags{}.none());
        QVERIFY(!MyFlags{}.any());
        QVERIFY(!MyFlags{One}.none());
        QVERIFY(MyFlags{One}.any());

        // testFlag special-case for 0
        QVERIFY(MyFlags{}.testFlag(0));
        QVERIFY(!MyFlags{One}.testFlag(0));
    }

    void test_helpers() {
        MyFlags f{};
        QVERIFY(f.none());
        QVERIFY(!f.any());
        QVERIFY(f.testFlag(0));

        f.setFlag(One, true);
        QVERIFY(f.any());
        QVERIFY(!f.none());
        QVERIFY(f.testFlag(One));
        QVERIFY(!f.testFlag(Two));
        QVERIFY(!f.testFlag(All));
        QVERIFY(!f.testFlag(0));

        // setting multiple flags
        f.setFlag(Two);
        QCOMPARE(f.value(), (One | Two));
        QVERIFY(f.testFlag(One));
        QVERIFY(f.testFlag(Two));
        QVERIFY(!f.testFlag(Three));
        QVERIFY(!f.testFlag(All));

        // clearing a flag
        f.setFlag(One, false);
        QCOMPARE(f.value(), (Two));
        QVERIFY(!f.testFlag(One));
        QVERIFY(f.testFlag(Two));

        // clear all via setFlag(All,false) => AND with ~All
        f.setFlag(All, false);
        QCOMPARE(f.value(), 0);
        QVERIFY(f.none());
        QVERIFY(f.testFlag(0));
    }

    void test_or() {
        MyFlags a{One};
        MyFlags b{Two};
        MyFlags c{Three};

        QCOMPARE((a | b).value(), (One | Two));
        QCOMPARE((a | c).value(), (One | Three));

        MyFlags d{One};
        d |= b;
        QCOMPARE(d.value(), (One | Two));

        d |= Three;
        QCOMPARE(d.value(), (One | Two | Three));

        // OR-ing with empty does nothing
        MyFlags e{};
        QCOMPARE((e | a).value(), One);
        QCOMPARE((a | e).value(), One);
    }

    void test_and() {
        MyFlags all{All};
        MyFlags some{ (One | Three) };

        QCOMPARE((all & some).value(), (One | Three));
        QCOMPARE((some & all).value(), (One | Three));

        // no intersection
        MyFlags a{One};
        MyFlags b{Two};
        QCOMPARE((a & b).value(), 0);
        QVERIFY((a & b).none());

        // &= with flags
        MyFlags c{All};
        c &= some;
        QCOMPARE(c.value(), (One | Three));

        // &= with raw mask
        MyFlags d{All};
        d &= (One | Two);
        QCOMPARE(d.value(), (One | Two));
    }

    void test_xor() {
        MyFlags a{One};
        MyFlags b{Two};

        QCOMPARE((a ^ b).value(), (One | Two));
        QCOMPARE((a ^ a).value(), 0);
        QVERIFY((a ^ a).none());

        // toggle via ^= flags
        MyFlags c{One};
        c ^= b;
        QCOMPARE(c.value(), (One | Two));
        c ^= b;
        QCOMPARE(c.value(), One);

        // ^= with raw mask
        MyFlags d{0};
        d ^= One;
        QCOMPARE(d.value(), One);
        d ^= One;
        QCOMPARE(d.value(), 0);
    }
};

QTEST_APPLESS_MAIN(FlagsTest)

#include "FlagsTest.moc"
