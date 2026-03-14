/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/utils/filter_if.h"
#include "draupnir/utils/type_presense.h"

using namespace draupnir::utils;

/*! @class FilterIfTest tests/modules/utils/unit/filter_if_test/FilterIfTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/filter_if.h. */

class FilterIfTest final : public QObject
{
    Q_OBJECT
public:
    FilterIfTest() = default;
    ~FilterIfTest() final = default;

private slots:
    void test_filter_if() {
        using Result = filter_if<std::is_floating_point,double,int,char,float>::to_container<std::tuple>;

        QCOMPARE(std::tuple_size_v<Result>,std::size_t{2});
        QCOMPARE((is_type_in_tuple_v<int,Result>), false);
        QCOMPARE((is_type_in_tuple_v<char,Result>), false);
        QCOMPARE((is_type_in_tuple_v<double,Result>), true);
        QCOMPARE((is_type_in_tuple_v<float,Result>), true);
    }

    void test_filter_if_nothing_matches() {
        using Result = filter_if<std::is_floating_point,int,char,short,QString>::to_container<std::tuple>;

        QCOMPARE(std::tuple_size_v<Result>,std::size_t{0});
        QCOMPARE((is_type_in_tuple_v<int,Result>), false);
        QCOMPARE((is_type_in_tuple_v<char,Result>), false);
        QCOMPARE((is_type_in_tuple_v<short,Result>), false);
        QCOMPARE((is_type_in_tuple_v<QString,Result>), false);
    }

    void test_filter_if_nothing_given() {
        using Result = filter_if<std::is_floating_point>::to_container<std::tuple>;

        QCOMPARE(std::tuple_size_v<Result>,std::size_t{0});
    }
};

QTEST_APPLESS_MAIN(FilterIfTest)

#include "FilterIfTest.moc"
