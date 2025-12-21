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

#include "draupnir/utils/type_list.h"

/*! @class TypeListTest tests/utils/unit/type_presense_test/TypeListTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/type_list.h. */

class TypeListTest final : public QObject
{
    Q_OBJECT
public:
    using EmptyList          = draupnir::utils::type_list<>;
    using OneElementList     = draupnir::utils::type_list<double>;
    using FirstList          = draupnir::utils::type_list<double,int,char>;
    using ReversedFirstList  = draupnir::utils::type_list<char,int,double>;
    using ListWithTwoPointers= draupnir::utils::type_list<
        void*, int, double, char*, float
    >;
    using ListWithTemplates  = draupnir::utils::type_list<
        std::vector<int>,
        char,
        int,
        std::tuple<int,char,long>
    >;
    using ListOfDoubles      = draupnir::utils::type_list<
        double, double, double
    >;
    using NonUniqueList      = draupnir::utils::type_list<
        double, double, double, int, int, QString, int, QString, double
    >;
    using UniqueList         = draupnir::utils::type_list<
        double, int, QString
    >;

private slots:
    void test_size() {
        QCOMPARE(EmptyList::size_v, std::size_t{0});
        QCOMPARE(FirstList::size_v, std::size_t{3});

        QCOMPARE(EmptyList::is_empty_v, true);
        QCOMPARE(FirstList::is_empty_v, false);
    };

    void test_contains() {
        // Empty List contains nothin
        QCOMPARE(EmptyList::contains_v<int>, false);
        QCOMPARE(EmptyList::contains_template_instantiation_v<std::vector>, false);

        // List with templates
        QCOMPARE(FirstList::contains_v<double>, true);
        QCOMPARE(FirstList::contains_v<QString>, false);
        QCOMPARE(FirstList::contains_template_instantiation_v<std::tuple>, false);

        // List with templates
        QCOMPARE(ListWithTemplates::contains_v<int>, true);
        QCOMPARE(ListWithTemplates::contains_v<QString>, false);
        QCOMPARE(ListWithTemplates::contains_template_instantiation_v<std::tuple>, true);

        // Checking any_of
        QCOMPARE(FirstList::any_of_v<std::is_floating_point>, true);
        QCOMPARE(ListWithTemplates::any_of_v<std::is_floating_point>, false);

        // Checking all_of
        QCOMPARE(FirstList::all_of_v<std::is_floating_point>,false);
        QCOMPARE(ListOfDoubles::all_of_v<std::is_floating_point>, true);

        // Checking none_of
        QCOMPARE(FirstList::none_of_v<std::is_union>,true);
        QCOMPARE(FirstList::none_of_v<std::is_floating_point>,false);
    }

    void test_index_of() {
        // Getting indexes of types
        QCOMPARE(FirstList::index_of_v<double>, std::size_t{0});
        QCOMPARE(FirstList::index_of_v<int>, std::size_t{1});
        QCOMPARE(FirstList::index_of_v<char>, std::size_t{2});

        // And specific template instantiations
        QCOMPARE(ListWithTemplates::index_of_v<std::vector<int>>,std::size_t{0});
    }

    void test_get() {
        // Test FirstList
        QCOMPARE((std::is_same_v<double,FirstList::get_t<0>>), true);
        QCOMPARE((std::is_same_v<int,FirstList::get_t<1>>), true);
        QCOMPARE((std::is_same_v<char,FirstList::get_t<2>>), true);
        // When uncommented - will static_assert duritg compile time
        //QCOMPARE((std::is_same_v<char,ListOne::get_t<3>>), true);

        // Test ListOfDoubles
        QCOMPARE((std::is_same_v<double,ListOfDoubles::get_t<0>>),true);
    }

    void test_appending_types() {
        // Appending using append_t "method"
        using listWithAppendedQString = ListOfDoubles::append_t<QString>;
        QCOMPARE(listWithAppendedQString::size_v, ListOfDoubles::size_v + 1);
        QCOMPARE((std::is_same_v<QString,listWithAppendedQString::get_t<3>>),true);

        using listWithAppendedList = ListOfDoubles::append_t<FirstList>;
        QCOMPARE(listWithAppendedList::size_v, ListOfDoubles::size_v + FirstList::size_v);
        QCOMPARE(listWithAppendedList::contains_template_instantiation_v<draupnir::utils::type_list>,false);

        using listWithAppendedEmptyList = ListOfDoubles::append_t<draupnir::utils::type_list<>>;
        QCOMPARE((std::is_same_v<listWithAppendedEmptyList,ListOfDoubles>), true);

        // Appending using push_back_t "method"
        using listWithPushedQString = ListOfDoubles::push_back_t<QString>;
        QCOMPARE((std::is_same_v<listWithAppendedQString,listWithPushedQString>), true);

        // When we push_back_t type_list - we will get +1 element, but not concatenation result
        using listWithPushedTypeList = ListOfDoubles::push_back_t<listWithAppendedList>;
        QCOMPARE((std::is_same_v<listWithPushedTypeList::get_t<3>,listWithAppendedList>), true);
        QCOMPARE(listWithPushedTypeList::contains_template_instantiation_v<draupnir::utils::type_list>,true);
    }

    void test_prepending_types() {
        // Prepending using prepend_t "method"
        using listWithPrependedQString = ListOfDoubles::prepend_t<QString>;
        QCOMPARE(listWithPrependedQString::size_v, ListOfDoubles::size_v + 1);
        QCOMPARE((std::is_same_v<QString,listWithPrependedQString::get_t<0>>),true);

        using listWithPrependedList = ListOfDoubles::prepend_t<FirstList>;
        QCOMPARE(listWithPrependedList::size_v, ListOfDoubles::size_v + FirstList::size_v);
        QCOMPARE(listWithPrependedList::contains_template_instantiation_v<draupnir::utils::type_list>,false);

        using listWithPrependedEmptyList = ListOfDoubles::append_t<draupnir::utils::type_list<>>;
        QCOMPARE((std::is_same_v<listWithPrependedEmptyList,ListOfDoubles>), true);

        // Appending using push_front_t "method"
        using listWithPushedQString = ListOfDoubles::push_front_t<QString>;
        QCOMPARE((std::is_same_v<listWithPrependedQString,listWithPushedQString>), true);

        // When we push_front_t type_list - we will get +1 element, but not concatenation result
        using listWithPushedTypeList = ListOfDoubles::push_front_t<listWithPrependedList>;
        QCOMPARE((std::is_same_v<listWithPushedTypeList::get_t<0>,listWithPrependedList>), true);
        QCOMPARE(listWithPushedTypeList::contains_template_instantiation_v<draupnir::utils::type_list>,true);
    }

    void test_insert_before() {
        using InsertedInteger = ListOfDoubles::insert_before_t<0,int>;
        QCOMPARE(InsertedInteger::size_v, ListOfDoubles::size_v + 1);
        QCOMPARE((std::is_same_v<InsertedInteger::get_t<0>,int>),true);
        QCOMPARE((std::is_same_v<InsertedInteger::get_t<1>,double>),true);

        using InsertedChar = ListOfDoubles::insert_before_t<1,char>;
        QCOMPARE(InsertedChar::size_v, ListOfDoubles::size_v + 1);
        QCOMPARE((std::is_same_v<InsertedChar::get_t<0>,double>),true);
        QCOMPARE((std::is_same_v<InsertedChar::get_t<1>,char>),true);

        using InsertedQString = ListOfDoubles::insert_before_t<2,QString>;
        QCOMPARE(InsertedQString::size_v, ListOfDoubles::size_v + 1);
        QCOMPARE((std::is_same_v<InsertedQString::get_t<0>,double>),true);
        QCOMPARE((std::is_same_v<InsertedQString::get_t<1>,double>),true);
        QCOMPARE((std::is_same_v<InsertedQString::get_t<2>,QString>),true);
        QCOMPARE((std::is_same_v<InsertedQString::get_t<3>,double>),true);
    }

    void test_remove_at() {
        using RemovedZero = FirstList::remove_at_t<0>;
        QCOMPARE(RemovedZero::size_v, FirstList::size_v - 1);
        QCOMPARE((std::is_same_v<RemovedZero::get_t<0>,double>), false);
        QCOMPARE((std::is_same_v<RemovedZero::get_t<0>,int>), true);
        QCOMPARE((std::is_same_v<RemovedZero::get_t<1>,char>), true);

        using RemovedOne = FirstList::remove_at_t<1>;
        QCOMPARE(RemovedOne::size_v, FirstList::size_v - 1);
        QCOMPARE((std::is_same_v<RemovedOne::get_t<0>,double>), true);
        QCOMPARE((std::is_same_v<RemovedOne::get_t<1>,int>), false);
        QCOMPARE((std::is_same_v<RemovedZero::get_t<1>,char>), true);

        using RemovedTwo = FirstList::remove_at_t<2>;
        QCOMPARE(RemovedTwo::size_v, FirstList::size_v - 1);
        QCOMPARE((std::is_same_v<RemovedTwo::get_t<0>,double>), true);
        QCOMPARE((std::is_same_v<RemovedTwo::get_t<1>,int>), true);
        QCOMPARE((std::is_same_v<RemovedTwo::get_t<1>,char>), false);
    }

    void test_remove_all() {
        using FirstListWithoutDoubles = draupnir::utils::type_list<double,int,char>;
        QCOMPARE((std::is_same_v<FirstListWithoutDoubles::remove_all_t<double>,draupnir::utils::type_list<int,char>>),true);

        using ShouldBeEmpty = ListOfDoubles::remove_all_t<double>;
        QCOMPARE((std::is_same_v<ShouldBeEmpty,EmptyList>),true);
    }

    void test_filter_if() {
        using OnlyFloatingPoint = ListOfDoubles::filter_if_t<std::is_floating_point>;
        QCOMPARE((std::is_same_v<OnlyFloatingPoint,ListOfDoubles>),true);

        using ShouldBeEmpty = ListOfDoubles::filter_if_t<std::is_pointer>;
        QCOMPARE((std::is_same_v<ShouldBeEmpty,EmptyList>), true);

        using ShouldBeTwo = ListWithTwoPointers::filter_if_t<std::is_pointer>;
        QCOMPARE(ShouldBeTwo::size_v, std::size_t{2});
        QCOMPARE(ShouldBeTwo::all_of_v<std::is_pointer>,true);
    }

    void test_convert_to() {
        using ExpectedTuple = std::tuple<double, int, char>;
        using UnexpectedTuple = std::tuple<double, double, double>;

        QCOMPARE((std::is_same_v<FirstList::convert_to_t<std::tuple>,ExpectedTuple>), true);
        QCOMPARE((std::is_same_v<FirstList::convert_to_t<std::tuple>,UnexpectedTuple>), false);
    }

    void test_transform() {
        using ListOfVectors = ListOfDoubles::transform_t<std::vector>;
        QCOMPARE(ListOfVectors::size_v, ListOfDoubles::size_v);
        QCOMPARE((std::is_same_v<ListOfVectors::get_t<0>,std::vector<double>>), true);
        QCOMPARE(ListOfVectors::contains_template_instantiation_v<std::vector>,true);
        QCOMPARE(ListOfDoubles::contains_template_instantiation_v<std::vector>,false);
    }

    void test_unique_lists() {
        // Non empty list
        using Result = NonUniqueList::unique_types_t;
        QCOMPARE(Result::size_v, UniqueList::size_v);
        QCOMPARE(Result::contains_v<double>, true);
        QCOMPARE(Result::contains_v<QString>, true);
        QCOMPARE(Result::contains_v<int>, true);

        // Empty list
        QCOMPARE((std::is_same_v<EmptyList::unique_types_t,EmptyList>), true);
    }

    void test_reverse() {
        // Reverse non-empty list
        using Result = FirstList::reverse_t;
        QCOMPARE(Result::size_v,FirstList::size_v);
        QCOMPARE((std::is_same_v<Result, ReversedFirstList>),true);

        // Reverse empty list
        QCOMPARE((std::is_same_v<EmptyList::reverse_t,EmptyList>), true);
    };
};

QTEST_APPLESS_MAIN(TypeListTest)

#include "TypeListTest.moc"
