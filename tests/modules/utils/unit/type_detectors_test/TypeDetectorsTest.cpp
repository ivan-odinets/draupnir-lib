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

#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/type_detectors.h"

using namespace draupnir::utils;

/*! @class TypeDetectorsTest tests/modules/utils/unit/type_detectors_test/TypeDetectorsTest.cpp
 *  @ingroup UtilsTests
 *  @brief Unit test for entities present within @ref draupnir/utils/type_detectors.h and @ref draupnir/utils/concepts/type_concepts.h. */

class TypeDetectorsTest final : public QObject
{
    Q_OBJECT
private:
    static constexpr int arrayOfInt[] = {1,2,3};
    using StdIntArray = std::array<int,3>;

    static constexpr double arrayOfDouble[] = {0.1, 0.2, 0.3};
    using StdDoubleArray = std::array<double, 3>;

    enum MyEnum { One, Two, Three };
    enum class MyEnumClass { One, Two, Three };

private slots:
    void test_is_integer() {
        // This should be true
        QVERIFY(integer_concept<char>);
        QVERIFY(is_integer<char>::value);
        QVERIFY(is_integer_v<char>);
        QVERIFY(is_integer_v<unsigned char>);
        QVERIFY(is_integer_v<short>);
        QVERIFY(is_integer_v<unsigned short>);
        QVERIFY(is_integer_v<int>);
        QVERIFY(is_integer_v<unsigned int>);
        QVERIFY(is_integer_v<long>);
        QVERIFY(is_integer_v<unsigned long>);
        QVERIFY(is_integer_v<long long>);
        QVERIFY(is_integer_v<unsigned long long>);

        QVERIFY(is_integer_v<int8_t>);
        QVERIFY(is_integer_v<uint8_t>);
        QVERIFY(is_integer_v<int16_t>);
        QVERIFY(is_integer_v<uint16_t>);
        QVERIFY(is_integer_v<int32_t>);
        QVERIFY(is_integer_v<uint32_t>);
        QVERIFY(is_integer_v<int64_t>);
        QVERIFY(is_integer_v<uint64_t>);

        QVERIFY(is_integer_v<qint8>);
        QVERIFY(is_integer_v<quint8>);
        QVERIFY(is_integer_v<qint16>);
        QVERIFY(is_integer_v<quint16>);
        QVERIFY(is_integer_v<qint32>);
        QVERIFY(is_integer_v<quint32>);
        QVERIFY(is_integer_v<qint64>);
        QVERIFY(is_integer_v<quint64>);

        // This should be false
        QVERIFY(!is_integer_v<double>);
        QVERIFY(!is_integer_v<float>);
        QVERIFY(!is_integer_v<QString>);
    }

    void test_is_enum_or_integer() {
        // This should be true
        QVERIFY(enum_or_integer_concept<int>);
        QVERIFY(is_enum_or_integer_v<int>);
        QVERIFY(is_enum_or_integer_v<MyEnum>);
        QVERIFY(is_enum_or_integer_v<MyEnumClass>);

        // This should be false
        QVERIFY(!is_enum_or_integer_v<QString>);
        QVERIFY(!is_enum_or_integer_v<double>);
    };

    void test_is_c_array_of() {
        // This should be true
        QCOMPARE((c_array_of_concept<decltype(arrayOfInt), int>), true);
        QCOMPARE((is_c_array_of_v<decltype(arrayOfInt), int>), true);
        QCOMPARE((is_c_array_of_v<decltype(arrayOfDouble), double>), true);

        // This should be false
        QCOMPARE((is_c_array_of_v<decltype(arrayOfInt), double>), false);
        QCOMPARE((is_c_array_of_v<decltype(arrayOfDouble), int>), false);
        QCOMPARE((is_c_array_of_v<StdIntArray,int>), false);
        QCOMPARE((is_c_array_of_v<StdDoubleArray,int>), false);
    }
};

QTEST_APPLESS_MAIN(TypeDetectorsTest)

#include "TypeDetectorsTest.moc"
