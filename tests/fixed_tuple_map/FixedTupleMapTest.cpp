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

#include "../../include/containers/fixed_tuple_map.h"

/*! @class FixedTupleMapTest draupnir-lib/tests/fixed_map/FixedTupleMapTest.cpp
 *  @brief Test class for testing fixed_tuple_map container. */

class FixedTupleMapTest final : public QObject
{
    Q_OBJECT

public:
    FixedTupleMapTest() = default;
    ~FixedTupleMapTest() final = default;

///@name Define constexpr things for fixed_tuple_map
///@{
    /*! @brief Enum to tale values from. */
    enum Type {
        First = 0, Second = 1, Third = 4, NumberFour = 10, UnknownThing = 113,
    };

    /*! @brief Static constexpr RAW array with enum values. To be used as a template argument of fixed_map. */
    static constexpr Type enumRawArray[] = { First, Second, Third, NumberFour };

    /*! @brief Static constexpr size of FixedMapTest::enumRawArray array. */
    static constexpr std::size_t controlSizeOfRawArray = std::size(enumRawArray);

    /*! @brief Static constexpr RAW array with some other set of enum values. To be used as a template argument of fixed_map. */
    static constexpr Type enumOtherRawArray[] = { First, Second, Third };

    /*! @brief Static constexpr RAW array with integers. To be used as a template argument of fixed_map. */
    static constexpr int integerValuesArray[] = { 0, 1, 5, 10 };

    /*! @brief Static constexpr size of FixedTupleMapTest::integerValuesArray array. */
    static constexpr std::size_t integerValuesArraySize = std::size(integerValuesArray);

    /*! @brief Static constexpr RAW array with integers. To be used as a template argument of fixed_map. */
    static constexpr int integerOtherValuesArray[] = { 0, 1, 2, 3 };
///@}

///@name Instantiations to be tested
///@{

    fixed_tuple_map<enumRawArray,int,double*, QString> tupleMap;
///@}

private slots:
    /*! @brief Tests proper initialization of keys and values of fixed_tuple_map container. */
    void checkInitialization() {
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            QVERIFY(tupleMap.get<int>(enumRawArray[i]) == 0);
            QVERIFY(tupleMap.get<double*>(enumRawArray[i]) == nullptr);
            QVERIFY(tupleMap.get<QString>(enumRawArray[i]) == QString{});
        }
    }

    /*! @brief Checks if reading and writing through fixed_tuple_map::get and fixed_tuple_map::set methods works as expected. */
    void checkSettersAndGetters() {
        // Writing something via get method
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            tupleMap.get<int>(enumRawArray[i]) = i*42;
            tupleMap.get<double*>(enumRawArray[i]) = new double{i*M_PI};
            tupleMap.get<QString>(enumRawArray[i]) = QString("testing fixed_tuple_map. %1").arg(i*42);
        }

        // Checking
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            QVERIFY(tupleMap.get<int>(enumRawArray[i]) == i*42);
            QVERIFY(*tupleMap.get<double*>(enumRawArray[i]) == i*M_PI);
            delete tupleMap.get<double*>(enumRawArray[i]); // We will not use this any further
            QVERIFY(tupleMap.get<QString>(enumRawArray[i]) == QString("testing fixed_tuple_map. %1").arg(i*42));
        }

        // Writing something via set method
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            tupleMap.set(enumRawArray[i],i*42+100);
            tupleMap.set(enumRawArray[i], new double{i*M_E});
            tupleMap.set(enumRawArray[i], QString("testing fixed_tuple_map. one more time %1").arg(i*42));
        }

        // Checking
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            QVERIFY(tupleMap.get<int>(enumRawArray[i]) == (i*42 + 100));
            QVERIFY(*tupleMap.get<double*>(enumRawArray[i]) == i*M_E);
            delete tupleMap.get<double*>(enumRawArray[i]); // We will not use this any further
            QVERIFY(tupleMap.get<QString>(enumRawArray[i]) == QString("testing fixed_tuple_map. one more time %1").arg(i*42));
        }
    }
};

QTEST_APPLESS_MAIN(FixedTupleMapTest)

#include "FixedTupleMapTest.moc"
