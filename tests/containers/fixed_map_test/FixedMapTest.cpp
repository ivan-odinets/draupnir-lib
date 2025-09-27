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

#include <chrono>
#include <map>
#include <unordered_map>

#include "draupnir/containers/fixed_map.h"

/*! @class FixedMapTest tests/containers/fixed_map_test/FixedMapTest.cpp
 *  @brief Test class for testing fixed_map container. */

class FixedMapTest final : public QObject
{
    Q_OBJECT

public:
    FixedMapTest() = default;
    ~FixedMapTest() final =default;

///@name Define constexpr things for fixed_map
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

    static constexpr std::size_t integerValuesArraySize = std::size(integerValuesArray);

    /*! @brief Static constexpr RAW array with integers. To be used as a template argument of fixed_map. */
    static constexpr int integerOtherValuesArray[] = { 0, 1, 2, 3 };
///@}

///@name Instantiations to be tested
///@{
    fixed_map<enumRawArray,int>     enumIntegerMap;        ///< @brief Mapping of integers to values from enum
    fixed_map<enumRawArray,double*> enumDoublePointerMap;  ///< @brief Mapping of double pointers to values from enum
    fixed_map<enumRawArray,QString> enumQStringMap;        ///< @brief Mapping of QString to values from enum

    fixed_map<integerValuesArray, int>   integerIntegerMap;     ///< @brief Maping of integers to another integers
///@}

private slots:
    /*! @brief Tests proper initialization of keys and values of fixed_map container. */
    void checkInitialization() {
        // Check size
        QVERIFY(decltype(enumIntegerMap)::keys_size          == controlSizeOfRawArray);
        QVERIFY(decltype(enumDoublePointerMap)::keys_size    == controlSizeOfRawArray);
        QVERIFY(decltype(integerIntegerMap)::keys_size       == integerValuesArraySize);

        // Check first and last keys
        QVERIFY(enumIntegerMap.first_key()      == enumRawArray[0]);
        QVERIFY(enumIntegerMap.last_key()       == enumRawArray[controlSizeOfRawArray-1]);
        QVERIFY(enumQStringMap.first_key()      == enumRawArray[0]);
        QVERIFY(enumQStringMap.last_key()       == enumRawArray[controlSizeOfRawArray-1]);
        QVERIFY(integerIntegerMap.first_key()   == integerValuesArray[0]);
        QVERIFY(integerIntegerMap.last_key()    == integerValuesArray[integerValuesArraySize-1]);


        // Check iterators & default values of the enum-keyed map
        auto enumIntegerMapIterator        = enumIntegerMap.begin();
        auto enumDoublePointerMapIterator  = enumDoublePointerMap.begin();
        auto enumQStringMapIterator        = enumQStringMap.begin();

        for (int counter = 0; counter < static_cast<int>(controlSizeOfRawArray); counter++) {
            // Check content of maps
            QVERIFY(enumIntegerMapIterator->first         == enumRawArray[counter]);
            QVERIFY(enumIntegerMapIterator->second        == 0);

            QVERIFY(enumDoublePointerMapIterator->first   == enumRawArray[counter]);
            QVERIFY(enumDoublePointerMapIterator->second  == nullptr);

            QVERIFY(enumQStringMapIterator->first         == enumRawArray[counter]);
            QVERIFY(enumQStringMapIterator->second        == QString());

            // Increment iterators
            enumIntegerMapIterator++;
            enumDoublePointerMapIterator++;
            enumQStringMapIterator++;
        }

        // Are iterators at the end?
        QVERIFY(enumIntegerMapIterator          == enumIntegerMap.end());
        QVERIFY(enumDoublePointerMapIterator    == enumDoublePointerMap.end());
        QVERIFY(enumQStringMapIterator          == enumQStringMap.end());

        // Iterate over fixed_map<int[],int>#
        auto integerIntegerMapIterator     = integerIntegerMap.begin();

        for (int counter = 0; counter < static_cast<int>(controlSizeOfRawArray); counter++) {
            // Check content of maps
            QVERIFY(integerIntegerMapIterator->first      == integerValuesArray[counter]);
            QVERIFY(integerIntegerMapIterator->second     == 0);

            // Increment iterators
            integerIntegerMapIterator++;
        }

        // Are iterators at the end?
        QVERIFY(integerIntegerMapIterator  == integerIntegerMap.end());
    }

    /*! @brief Checks if reading and writing through fixed_map::get method works as expected. */
    void checkGetterAccess() {
        // Writing using getter of the maps
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            enumIntegerMap.get(enumRawArray[i]) = i;

            // Double pointer
            enumDoublePointerMap.get(enumRawArray[i]) = new double{i*M_PI};

            // QString
            enumQStringMap.get(enumRawArray[i]) = QString("TESTING ENUM MAP! %1").arg(i);
        }

        // Checking what was written
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            QVERIFY(enumIntegerMap.get(enumRawArray[i]) == i);

            // Double pointer
            QVERIFY(enumDoublePointerMap.get(enumRawArray[i]) != nullptr);
            QVERIFY(*enumDoublePointerMap.get(enumRawArray[i]) == double{i*M_PI});

            // QString
            QVERIFY(enumQStringMap.get(enumRawArray[i]) == QString("TESTING ENUM MAP! %1").arg(i));
        }

        // Reseting maps to a default state
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            enumIntegerMap.get(enumRawArray[i]) = 0;

            // Double pointer
            delete enumDoublePointerMap.get(enumRawArray[i]);
            enumDoublePointerMap.get(enumRawArray[i]) = nullptr;

            // QString
            enumQStringMap.get(enumRawArray[i]) = QString();
        }
    }

    /*! @brief Checks if reading and writing through EnumMap::operator[] works as expected. */
    void checkOperatorAccess() {
        // Writing using operator[] of the maps
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            enumIntegerMap[enumRawArray[i]] = i*i;

            // Double pointer
            enumDoublePointerMap[enumRawArray[i]] = new double{i*M_E};

            // QString
            enumQStringMap[enumRawArray[i]] = QString("TESTING ENUM MAP in another way! %1").arg(i*3);
        }

        // Checking what was written
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            QVERIFY(enumIntegerMap[enumRawArray[i]] == i*i);

            // Double pointer
            QVERIFY(enumDoublePointerMap[enumRawArray[i]] != nullptr);
            QVERIFY(*enumDoublePointerMap[enumRawArray[i]] == double{i*M_E});

            // QString
            QVERIFY(enumQStringMap.get(enumRawArray[i]) == QString("TESTING ENUM MAP in another way! %1").arg(i*3));
        }

        // Reseting maps to a default state
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            enumIntegerMap[enumRawArray[i]] = 0;

            // Double pointer
            delete enumDoublePointerMap[enumRawArray[i]];
            enumDoublePointerMap[enumRawArray[i]] = nullptr;

            // QString
            enumQStringMap.get(enumRawArray[i]) = QString();
        }
    }

    /*! @brief Checks if EnumMap::clear method works as expected. */
    void checkClear() {
        // Writing using getter of the maps
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            enumIntegerMap.get(enumRawArray[i]) = i;

            // Double pointer
            enumDoublePointerMap.get(enumRawArray[i]) = new double{i*M_PI};

            // QString
            enumQStringMap.get(enumRawArray[i]) = QString("TESTING ENUM MAP! %1").arg(i);
        }

        // Clearing
        enumIntegerMap.clear();
        enumDoublePointerMap.for_each_value([](double* pointer){ delete pointer; });
        enumDoublePointerMap.clear();
        enumQStringMap.clear();

        // Checking emptiness of the map
        for (int i = 0; i < static_cast<int>(controlSizeOfRawArray); i++) {
            // Integer
            QVERIFY(enumIntegerMap[enumRawArray[i]] == 0);

            // Double pointer
            QVERIFY(enumDoublePointerMap[enumRawArray[i]] == nullptr);

            // QString
            QVERIFY(enumQStringMap.get(enumRawArray[i]) == QString());
        }
    }

    /*! @brief Checks EnumMap::hasTheSameKeys method. */
    void checkTheSameKeys() {
        // Checking static version
        QVERIFY(decltype(enumIntegerMap)::same_keys<decltype(enumDoublePointerMap)>());

        bool shouldBeFalse = decltype(enumIntegerMap)::same_keys<decltype(integerIntegerMap)>();
        QVERIFY(shouldBeFalse == false);

        // Checking non-static version
        QVERIFY(enumIntegerMap.same_keys(enumQStringMap) == true);
        QVERIFY(enumIntegerMap.same_keys(integerIntegerMap) == false);

        QVERIFY(enumIntegerMap.contains(First));
    }

    /*! @brief Checks EnumMap::forEach method (const and non-const). */
    void checkForEach() {
        enumIntegerMap.for_each_value([](int& value){
            value = 42;
        });

        enumIntegerMap.for_each_value([](int value){
            QVERIFY(value == 42);
        });
    }
};

QTEST_APPLESS_MAIN(FixedMapTest)

#include "FixedMapTest.moc"
