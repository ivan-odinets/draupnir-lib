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

#include "draupnir/utils/type_list.h"

#include "draupnir-test/helpers/TypeHelpers.h"

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
        // Empty List contains nothing
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

        // When uncommented - will static_assert during compile time
        // FirstList::index_of_v<QString>;
    }

    void test_index_of_first_if() {
        QCOMPARE(FirstList::index_of_first_if_v<std::is_floating_point>, std::size_t{0});
        QCOMPARE(ReversedFirstList::index_of_first_if_v<std::is_floating_point>, std::size_t{2});

        // When uncommented - will static_assert during compile time
        // FirstList::index_of_first_if_v<std::is_array>;
    }

    void test_counting() {
        QCOMPARE(EmptyList::count_v<double>,0);
        QCOMPARE(FirstList::count_v<QString>,0);
        QCOMPARE(FirstList::count_v<double>,1);
        QCOMPARE(ListOfDoubles::count_v<double>,3);
    }

    void test_get() {
        // Test FirstList
        TYPE_COMPARE(FirstList::get_t<0>,double);
        TYPE_COMPARE(FirstList::get_t<1>,int);
        TYPE_COMPARE(FirstList::get_t<2>,char);

        // Test ListOfDoubles
        TYPE_COMPARE(ListOfDoubles::get_t<0>,double);
        TYPE_COMPARE(ListOfDoubles::get_t<0>,double);

        // When uncommented - will static_assert duritg compile time
        // EmptyList::get_t<10>;
        // FirstList::get_t<1000>;
    }

    void test_appending_types() {
        // Appending using append_t "method"
        using listWithAppendedQString = ListOfDoubles::append_t<QString>;
        QCOMPARE(listWithAppendedQString::size_v, ListOfDoubles::size_v + 1);
        TYPE_COMPARE(listWithAppendedQString::get_t<3>,QString);

        using listWithAppendedList = ListOfDoubles::append_t<FirstList>;
        QCOMPARE(listWithAppendedList::size_v, ListOfDoubles::size_v + FirstList::size_v);
        QCOMPARE(listWithAppendedList::contains_template_instantiation_v<draupnir::utils::type_list>,false);

        using listWithAppendedEmptyList = ListOfDoubles::append_t<draupnir::utils::type_list<>>;
        TYPE_COMPARE(listWithAppendedEmptyList,ListOfDoubles);

        // Appending using push_back_t "method"
        using listWithPushedQString = ListOfDoubles::push_back_t<QString>;
        TYPE_COMPARE(listWithAppendedQString,listWithPushedQString);

        // When we push_back_t type_list - we will get +1 element, but not concatenation result
        using listWithPushedTypeList = ListOfDoubles::push_back_t<listWithAppendedList>;
        TYPE_COMPARE(listWithPushedTypeList::get_t<3>,listWithAppendedList);
        QCOMPARE(listWithPushedTypeList::contains_template_instantiation_v<draupnir::utils::type_list>,true);
    }

    void test_prepending_types() {
        // Prepending using prepend_t "method"
        using listWithPrependedQString = ListOfDoubles::prepend_t<QString>;
        QCOMPARE(listWithPrependedQString::size_v, ListOfDoubles::size_v + 1);
        TYPE_COMPARE(listWithPrependedQString::get_t<0>,QString);

        using listWithPrependedList = ListOfDoubles::prepend_t<FirstList>;
        QCOMPARE(listWithPrependedList::size_v, ListOfDoubles::size_v + FirstList::size_v);
        QCOMPARE(listWithPrependedList::contains_template_instantiation_v<draupnir::utils::type_list>,false);

        using listWithPrependedEmptyList = ListOfDoubles::append_t<draupnir::utils::type_list<>>;
        TYPE_COMPARE(listWithPrependedEmptyList,ListOfDoubles);

        // Appending using push_front_t "method"
        using listWithPushedQString = ListOfDoubles::push_front_t<QString>;
        TYPE_COMPARE(listWithPrependedQString,listWithPushedQString);

        // When we push_front_t type_list - we will get +1 element, but not concatenation result
        using listWithPushedTypeList = ListOfDoubles::push_front_t<listWithPrependedList>;
        TYPE_COMPARE(listWithPushedTypeList::get_t<0>,listWithPrependedList);
        QCOMPARE(listWithPushedTypeList::contains_template_instantiation_v<draupnir::utils::type_list>,true);
    }

    void test_insert_before() {
        using InsertedInteger = ListOfDoubles::insert_before_t<0,int>;
        QCOMPARE(InsertedInteger::size_v, ListOfDoubles::size_v + 1);
        TYPE_COMPARE(InsertedInteger::get_t<0>,int);
        TYPE_COMPARE(InsertedInteger::get_t<1>,double);

        using InsertedChar = ListOfDoubles::insert_before_t<1,char>;
        QCOMPARE(InsertedChar::size_v, ListOfDoubles::size_v + 1);
        TYPE_COMPARE(InsertedChar::get_t<0>,double);
        TYPE_COMPARE(InsertedChar::get_t<1>,char);

        using InsertedQString = ListOfDoubles::insert_before_t<2,QString>;
        QCOMPARE(InsertedQString::size_v, ListOfDoubles::size_v + 1);
        TYPE_COMPARE(InsertedQString::get_t<0>,double);
        TYPE_COMPARE(InsertedQString::get_t<1>,double);
        TYPE_COMPARE(InsertedQString::get_t<2>,QString);
        TYPE_COMPARE(InsertedQString::get_t<3>,double);

        using InsertIntoEmpty = EmptyList::insert_before_t<0,int>;
        TYPE_COMPARE(InsertIntoEmpty,draupnir::utils::type_list<int>);

        // When uncommented - will static_assert duritg compile time
        // EmptyList::insert_before_t<1,int>;
        // FirstList::insert_before_t<12,int>;
    }

    void test_remove_at() {
        using RemovedZero = FirstList::remove_at_t<0>;
        QCOMPARE(RemovedZero::size_v, FirstList::size_v - 1);
        TYPE_COMPARE(RemovedZero::get_t<0>,int);
        TYPE_COMPARE(RemovedZero::get_t<1>,char);

        using RemovedOne = FirstList::remove_at_t<1>;
        QCOMPARE(RemovedOne::size_v, FirstList::size_v - 1);
        TYPE_COMPARE(RemovedOne::get_t<0>,double);
        TYPE_COMPARE(RemovedZero::get_t<1>,char);

        using RemovedTwo = FirstList::remove_at_t<2>;
        QCOMPARE(RemovedTwo::size_v, FirstList::size_v - 1);
        TYPE_COMPARE(RemovedTwo::get_t<0>,double);
        TYPE_COMPARE(RemovedTwo::get_t<1>,int);

        // When uncommented - will static_assert duritg compile time
        // EmptyList::remove_at_t<0>;
        // FirstList::remove_at_t<12>;
    }

    void test_remove_all() {
        using FirstListWithoutDoubles = draupnir::utils::type_list<int,char>;
        TYPE_COMPARE(FirstListWithoutDoubles::remove_all_t<double>,FirstListWithoutDoubles);

        TYPE_COMPARE(ListOfDoubles::remove_all_t<double>,EmptyList);
    }

    void test_filter_if() {
        TYPE_COMPARE(ListOfDoubles::filter_if_t<std::is_floating_point>, ListOfDoubles);

        TYPE_COMPARE(ListOfDoubles::filter_if_t<std::is_pointer>, EmptyList);

//        using ListWithTwoPointers= draupnir::utils::type_list<
//            void*, int, double, char*, float
//        >;
        // This may change in future
        using TwoPointers = draupnir::utils::type_list<void*,char*>;
        QCOMPARE(ListWithTwoPointers::filter_if_t<std::is_pointer>::size_v, std::size_t{2});
        QCOMPARE(ListWithTwoPointers::filter_if_t<std::is_pointer>::all_of_v<std::is_pointer>,true);
        TYPE_COMPARE(ListWithTwoPointers::filter_if_t<std::is_pointer>, TwoPointers);
    }

    void test_convert_to() {
        using ExpectedTuple = std::tuple<double, int, char>;
        using OtherTuple = std::tuple<double, double, double>;

        TYPE_COMPARE(FirstList::convert_to_t<std::tuple>,ExpectedTuple);
        TYPE_COMPARE(ListOfDoubles::convert_to_t<std::tuple>,OtherTuple);
    }

    void test_transform() {
        using ListOfVectors = ListOfDoubles::transform_t<std::vector>;
        QCOMPARE(ListOfVectors::size_v, ListOfDoubles::size_v);
        TYPE_COMPARE(ListOfVectors::get_t<0>,std::vector<double>);
        QCOMPARE(ListOfVectors::contains_template_instantiation_v<std::vector>,true);
        QCOMPARE(ListOfDoubles::contains_template_instantiation_v<std::vector>,false);
    }

    void test_slice() {
//        using ListWithTwoPointers= draupnir::utils::type_list<
//            void*, int, double, char*, float
//        >;
        using Expected = draupnir::utils::type_list<int,double,char*>;
        using Result12 = typename ListWithTwoPointers::slice_t<1,3>;
        TYPE_COMPARE(Result12,Expected);

        using ShouldBeEmpty = EmptyList::slice_t<0,0>;
        TYPE_COMPARE(ShouldBeEmpty,EmptyList);

        using ShouleBeFirstList = FirstList::slice_t<0,2>;
        TYPE_COMPARE(ShouleBeFirstList,FirstList);

        using ShouldBeInt = FirstList::slice_t<1,1>;
        TYPE_COMPARE(ShouldBeInt,draupnir::utils::type_list<int>);

        // When uncommented - will static_assert during compile time
        // FirstList::slice_t<1,100>;
        // FirstList::slice_t<100,1>;
        // FirstList::slice_t<100,1000>;
        // FirstList::slice_t<0,3>;
        // EmptyList::slice_t<0,1>;
    }

    void test_unique_lists() {
        // Non empty list
        using Result = NonUniqueList::unique_types_t;
        QCOMPARE(Result::size_v, UniqueList::size_v);
        QCOMPARE(Result::contains_v<double>, true);
        QCOMPARE(Result::contains_v<QString>, true);
        QCOMPARE(Result::contains_v<int>, true);

        // Empty list
        TYPE_COMPARE(EmptyList::unique_types_t,EmptyList);
    }

    void test_is_subset_of() {
        QCOMPARE(ListOfDoubles::is_subset_of_v<FirstList>, true);
        QCOMPARE(FirstList::is_subset_of_v<ListOfDoubles>, false);

        QCOMPARE(EmptyList::is_subset_of_v<FirstList>, true);

        // When uncommented - will static_assert during compile time
        // FirstList::is_subset_of_v<int>;
    }

    void test_is_superset_of() {
        QCOMPARE(ListOfDoubles::is_superset_of_v<FirstList>, false);
        QCOMPARE(FirstList::is_superset_of_v<ListOfDoubles>, true);

        QCOMPARE(EmptyList::is_superset_of_v<FirstList>, false);

        // When uncommented - will static_assert during compile time
        // FirstList::is_superset_of_v<int>;
    }

    void test_reverse() {
        // Reverse non-empty list
        using Result = FirstList::reverse_t;
        QCOMPARE(Result::size_v,FirstList::size_v);
        TYPE_COMPARE(Result, ReversedFirstList);

        // Reverse empty list
        TYPE_COMPARE(EmptyList::reverse_t,EmptyList);
    };

    void test_union() {
        TYPE_COMPARE(FirstList,ListOfDoubles::union_t<FirstList>);
        TYPE_COMPARE(FirstList,FirstList::union_t<ListOfDoubles>);

        TYPE_COMPARE(FirstList,EmptyList::union_t<FirstList>);
        TYPE_COMPARE(FirstList,FirstList::union_t<EmptyList>);

        // When uncommented - will static_assert during compile time
        // FirstList::union_t<int>;
    }

    void test_intersection() {
        using Result_FirstList_intersect_ListOfDoubles = draupnir::utils::type_list<double>;
        TYPE_COMPARE(Result_FirstList_intersect_ListOfDoubles, FirstList::intersection_t<ListOfDoubles>);
        TYPE_COMPARE(Result_FirstList_intersect_ListOfDoubles, ListOfDoubles::intersection_t<FirstList>);

        TYPE_COMPARE(FirstList::intersection_t<EmptyList>,EmptyList);
        TYPE_COMPARE(EmptyList::intersection_t<FirstList>,EmptyList);

        // When uncommented - will static_assert during compile time
        // FirstList::intersection_t<int>;
    }

    void test_type_list_from_template_instantiation() {
        using FirstTuple = std::tuple<double,int,char>;
        TYPE_COMPARE(draupnir::utils::type_list_from_template_instantiation_t<FirstTuple>,FirstList);

        // When uncommented - will static_assert during compile time
        // draupnir::utils::type_list_from_template_instantiation_t<int>;
    }
};

QTEST_APPLESS_MAIN(TypeListTest)

#include "TypeListTest.moc"
