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

#include "draupnir/utils/tuple_like_merge.h"

#include "draupnir/utils/template_detectors.h"
#include "draupnir/utils/type_presense.h"

using namespace draupnir::utils;

class TupleLikeMergeTest final : public QObject
{
    Q_OBJECT
public:
    TupleLikeMergeTest() = default;
    ~TupleLikeMergeTest() final = default;

    template<class... Things>
    struct SomeTupleLikeThing {
        template<class Thing>
        static bool contains() { return is_one_of_v<Thing,Things...>; }

        static size_t size() { return sizeof...(Things); }
    };

private slots:
    void test_simple_type_merging_into_tuple() {
        // Simple case, merging unique types into tuple
        using Result = tuple_like_merge_t<std::tuple, std::tuple, int, char, double, QString, std::vector<int>>;
        QCOMPARE(std::tuple_size_v<Result>, std::size_t{5});
        QCOMPARE((is_type_in_tuple_v<int,Result>),              true);
        QCOMPARE((is_type_in_tuple_v<char,Result>),             true);
        QCOMPARE((is_type_in_tuple_v<double,Result>),           true);
        QCOMPARE((is_type_in_tuple_v<QString,Result>),          true);
        QCOMPARE((is_type_in_tuple_v<std::vector<int>,Result>), true);
    }

    void test_simple_type_merging_with_duplicates_into_tuple() {
        // Simple case, merging typelist with duplicates into tuple
        using Result = tuple_like_merge_t<std::tuple, std::tuple, int, char, double, double, QString, int, QString>;
        QCOMPARE(std::tuple_size_v<Result>, std::size_t{4});
        QCOMPARE((is_type_in_tuple_v<int,Result>),         true);
        QCOMPARE((is_type_in_tuple_v<char,Result>),        true);
        QCOMPARE((is_type_in_tuple_v<double,Result>),      true);
        QCOMPARE((is_type_in_tuple_v<QString,Result>),     true);
    }

    void test_types_and_tuples_merge_into_tuple() {
        // Merging nested std::tuples when having one of them empty
        using Result = tuple_like_merge_t<
            std::tuple, std::tuple,
            std::tuple<int,double>, std::tuple<double,QString,QString>, int, QString, std::tuple<>
        >;
        QCOMPARE(std::tuple_size_v<Result>, std::size_t{3});
        QCOMPARE((is_type_in_tuple_v<int,Result>),         true);
        QCOMPARE((is_type_in_tuple_v<double,Result>),      true);
        QCOMPARE((is_type_in_tuple_v<QString,Result>),     true);
    }

    void test_types_and_tuples_nested_merge_into_tuple() {
        // Merging nested std::tuples with multiple levels of nesting
        using Result = tuple_like_merge_t<
            std::tuple, std::tuple,
            std::tuple<int,double>, int,
            std::tuple<std::tuple<char,std::tuple<long,std::tuple<QString>>>,QString,QString>,
            QString, std::tuple<>
        >;
        QCOMPARE(std::tuple_size_v<Result>, std::size_t{5});
        QCOMPARE((is_type_in_tuple_v<int,Result>),         true);
        QCOMPARE((is_type_in_tuple_v<double,Result>),      true);
        QCOMPARE((is_type_in_tuple_v<QString,Result>),     true);
        QCOMPARE((is_type_in_tuple_v<char,Result>),        true);
        QCOMPARE((is_type_in_tuple_v<long,Result>),        true);
    }

    void test_tuple_like_merge() {
        // Merging nested std::tuples with multiple levels of nesting into custom structure
        using Result = tuple_like_merge_t<
            std::tuple, SomeTupleLikeThing,
            std::tuple<int,double>, int,
            std::tuple<std::tuple<char,std::tuple<long,std::tuple<QString>>>,QString,QString>,
            QString, std::tuple<>
        >;
        QCOMPARE(Result::size(), 5);
        QCOMPARE(Result::contains<int>(),        true);
        QCOMPARE(Result::contains<double>(),     true);
        QCOMPARE(Result::contains<QString>(),    true);
        QCOMPARE(Result::contains<char>(),       true);
        QCOMPARE(Result::contains<long>(),       true);
    }
};

QTEST_APPLESS_MAIN(TupleLikeMergeTest)

#include "TupleLikeMergeTest.moc"
