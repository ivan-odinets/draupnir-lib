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

#include "draupnir/utils/type_presense.h"

using namespace draupnir::utils;

/*! @class TypePresenseTest tests/utils/unit/type_presense_test/TypePresenseTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/type_presense.h. */

class TypePresenseTest final : public QObject
{
    Q_OBJECT
public:
    TypePresenseTest() = default;
    ~TypePresenseTest() final = default;

    using SomeQStringAlias = QString;
    using CustomInt = int;

private slots:
    void test_is_one_of() {
        // Check if type is present
        QCOMPARE((is_one_of<int,int,double,char>::value),  true);
        QCOMPARE((is_one_of_v<int,int,double,char>),       true);

        // Check if type is absent
        QCOMPARE((is_one_of<QString,int,double,char>::value),   false);
        QCOMPARE((is_one_of_v<QString,int,double,char>),        false);

        // Check alias if present
        QCOMPARE((is_one_of<CustomInt,int,QString,int>::value), true);
        QCOMPARE((is_one_of_v<CustomInt,int,QString,int>),      true);

        // Check alias if absent
        QCOMPARE((is_one_of<SomeQStringAlias,int,QStringList,int>::value),     false);
        QCOMPARE((is_one_of_v<SomeQStringAlias,int,QStringList,int>),          false);
    }

    void test_is_type_in_tuple() {
        using MyTuple = std::tuple<int,double,QString>;

        // Check if type is present
        QCOMPARE((is_type_in_tuple<int,MyTuple>::value),   true);
        QCOMPARE((is_type_in_tuple_v<int,MyTuple>),        true);

        // Check if type is absent
        QCOMPARE((is_type_in_tuple<char,MyTuple>::value), false);
        QCOMPARE((is_type_in_tuple_v<char,MyTuple>),      false);
    }
};

QTEST_APPLESS_MAIN(TypePresenseTest)

#include "TypePresenseTest.moc"
