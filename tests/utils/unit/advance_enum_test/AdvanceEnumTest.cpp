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

#include "draupnir/utils/advance_enum.h"

using namespace draupnir::utils;

enum SimpleEnum {
    One,
    Two,
    Three,
    Four
};

using SimpleEnumValues = enum_values<SimpleEnum,
    SimpleEnum::One, SimpleEnum::Two, SimpleEnum::Three, SimpleEnum::Four
>;

inline SimpleEnum& operator++(SimpleEnum& value,int) {
    SimpleEnumValues::advance(value);
    return value;
}

/*! @class AdvanceEnumTest tests/utils/unit/advance_enum_test/AdvanceEnumTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/advance_enum.h. */

class AdvanceEnumTest final : public QObject
{
    Q_OBJECT
public:
    AdvanceEnumTest() = default;
    ~AdvanceEnumTest() final = default;

private slots:
    void test_advance_enum() {
        SimpleEnum value = SimpleEnum::One;
        value++;
        QCOMPARE(value, SimpleEnum::Two);

        value = SimpleEnum::Four;
        value++;
        QCOMPARE(value, SimpleEnum::One);
    }
};

QTEST_APPLESS_MAIN(AdvanceEnumTest)

#include "AdvanceEnumTest.moc"
