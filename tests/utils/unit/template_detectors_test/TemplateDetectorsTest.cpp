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

#include "draupnir/utils/template_detectors.h"

#include <QVector>

#include <vector>

using namespace draupnir::utils;

class DefaultConstructor
{
public:
    DefaultConstructor() = default;
    ~DefaultConstructor() = default;
};

class DefaultArgumentsConstructor
{
public:
    DefaultArgumentsConstructor(const QString& someString = QString{});
    ~DefaultArgumentsConstructor() = default;
};

class ArgumentedConstructor
{
public:
    ArgumentedConstructor(const QString& someString) { qDebug() << someString; }
    ~ArgumentedConstructor() = default;
};

/*! @class TemplateDetectorsTest tests/utils/unit/template_detectors_test/TemplateDetectorsTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/template_detectors.h. */

class TemplateDetectorsTest final : public QObject
{
    Q_OBJECT
public:
    using PairOne = std::pair<int,double>;
    using PairTwo = std::pair<int,int>;

    using PtrPair = std::pair<double*,QString*>;
    using AlmostPairPtr = std::pair<double,double*>;

    using TupleOne = std::tuple<int,double,QString>;
    using TupleTwo = std::tuple<int,int,int>;

    using PointerTuple = std::tuple<int*,double*,QString*>;
    using AlmostPtrTuple = std::tuple<double,int*,QString>;

    using VectorOne = std::vector<int>;
    using VectorTwo = std::vector<QString>;

private slots:
    void test_is_instantiation_of() {
        // Check if tuple is recognized as tuple
        QCOMPARE((is_instantiation_of<TupleOne,std::tuple>::value), true);
        QCOMPARE((is_instantiation_of_v<TupleOne,std::tuple>), true);

        // Check if vector is recognized as vector
        QCOMPARE((is_instantiation_of<VectorOne,std::vector>::value), true);
        QCOMPARE((is_instantiation_of_v<VectorOne,std::vector>), true);

        // Check that tuple is not vector
        QCOMPARE((is_instantiation_of<TupleOne,std::vector>::value), false);
        QCOMPARE((is_instantiation_of_v<TupleOne,std::vector>), false);

        // Check that vector is not tuple
        QCOMPARE((is_instantiation_of<VectorOne,std::tuple>::value), false);
        QCOMPARE((is_instantiation_of_v<VectorOne,std::tuple>), false);
    }

    void test_is_tuple() {
        // Check if tuples are recognized as tuples
        QCOMPARE((is_tuple<TupleOne>::value),true);
        QCOMPARE((is_tuple_v<TupleOne>),true);

        QCOMPARE((is_tuple<TupleTwo>::value),true);
        QCOMPARE((is_tuple_v<TupleTwo>),true);

        // Check if non-tuples are recognized as non-tuples
        QCOMPARE((is_tuple<VectorOne>::value),false);
        QCOMPARE((is_tuple_v<VectorOne>),false);

        QCOMPARE((is_tuple<VectorTwo>::value),false);
        QCOMPARE((is_tuple_v<VectorTwo>),false);
    }

    void test_is_pair() {
        // Check if pairs are recognized as pairs
        QCOMPARE((is_pair<PairOne>::value),true);
        QCOMPARE((is_pair_v<PairOne>),true);

        QCOMPARE((is_pair<PairTwo>::value),true);
        QCOMPARE((is_pair_v<PairTwo>),true);

        // Check if non-pairs are recognized as non-pairs
        QCOMPARE((is_pair<VectorOne>::value),false);
        QCOMPARE((is_pair_v<VectorOne>),false);

        QCOMPARE((is_pair<VectorTwo>::value),false);
        QCOMPARE((is_tuple_v<VectorTwo>),false);

        QCOMPARE((is_pair<TupleOne>::value),false);
        QCOMPARE((is_pair_v<TupleOne>),false);

        QCOMPARE((is_pair<TupleTwo>::value),false);
        QCOMPARE((is_pair_v<TupleTwo>),false);
    }

    void test_is_tuple_ptr_only() {
        QCOMPARE((is_tuple_ptr_only<TupleOne>::value), false);
        QCOMPARE((is_tuple_ptr_only_v<TupleOne>), false);

        QCOMPARE((is_tuple_ptr_only<AlmostPtrTuple>::value), false);
        QCOMPARE((is_tuple_ptr_only_v<AlmostPtrTuple>), false);

        QCOMPARE((is_tuple_ptr_only<PointerTuple>::value), true);
        QCOMPARE((is_tuple_ptr_only_v<PointerTuple>), true);
    }

    void test_is_pair_of_ptr() {
        QCOMPARE((is_pair_of_ptr<PairOne>::value), false);
        QCOMPARE((is_pair_of_ptr_v<PairOne>), false);

        QCOMPARE((is_pair_of_ptr<AlmostPairPtr>::value), false);
        QCOMPARE((is_pair_of_ptr_v<AlmostPairPtr>), false);

        QCOMPARE((is_pair_of_ptr<PtrPair>::value), true);
        QCOMPARE((is_pair_of_ptr_v<PtrPair>), true);
    }

    void test_is_tuple_like_pointees_default_constructible() {
        // Checking tuples, containing pointers which pointees can be default constructed.
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::tuple<int*,QString*,DefaultArgumentsConstructor*>>::value), true);
        QCOMPARE((is_tuple_like_pointees_default_constructible_v<
            std::tuple<int*,QString*,DefaultArgumentsConstructor*>>), true);
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::tuple<int*,QString*,DefaultArgumentsConstructor*>>::value), true);
        QCOMPARE((is_tuple_like_pointees_default_constructible_v<
            std::tuple<int*,QString*,DefaultArgumentsConstructor*>>), true);

        // Checking tuples, containing pointers which pointees can not be default constructed.
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::tuple<ArgumentedConstructor*,ArgumentedConstructor*>>::value), false);
        QCOMPARE((is_tuple_like_pointees_default_constructible_v<
            std::tuple<ArgumentedConstructor*,ArgumentedConstructor*>>), false);

        // Checking tuples with non-pointer types
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::tuple<ArgumentedConstructor*,int>>::value), false);
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::tuple<int,int>>::value), false);

        // Checking pairs of default-constructible pointers
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::pair<int*,DefaultArgumentsConstructor*>>::value), true);
        QCOMPARE((is_tuple_like_pointees_default_constructible_v<
            std::pair<int*,DefaultArgumentsConstructor*>>), true);

        // Checking pairs of pointers which pointees can not be default constructed.
        QCOMPARE((is_tuple_like_pointees_default_constructible<
            std::pair<ArgumentedConstructor*,DefaultArgumentsConstructor*>>::value), false);
        QCOMPARE((is_tuple_like_pointees_default_constructible_v<
            std::pair<ArgumentedConstructor*,DefaultArgumentsConstructor*>>), false);
    }

    void test_is_container_pointees_default_constructible() {
        // Checking std::vectors, of pointers which pointees can be default constructed.
        QCOMPARE((is_container_pointees_default_constructible<
            std::vector<DefaultConstructor*>>::value), true);
        QCOMPARE((is_container_pointees_default_constructible_v<
            std::vector<DefaultConstructor*>>), true);
        QCOMPARE((is_container_pointees_default_constructible<
            std::vector<DefaultArgumentsConstructor*>>::value), true);
        QCOMPARE((is_container_pointees_default_constructible_v<
            std::vector<DefaultArgumentsConstructor*>>), true);

        // Checking std::vectors, of pointers which pointees can not be default constructed.
        QCOMPARE((is_container_pointees_default_constructible<
            std::vector<ArgumentedConstructor*>>::value), false);
        QCOMPARE((is_container_pointees_default_constructible_v<
            std::vector<ArgumentedConstructor*>>), false);
    }

    void test_is_collection_pointees_default_constructible() {
        // Checking std::vectors, of pointers which pointees can be default constructed.
        QCOMPARE((is_collection_pointees_default_constructible<
            std::vector<DefaultConstructor*>>::value), true);
        QCOMPARE((is_collection_pointees_default_constructible_v<
            std::vector<DefaultConstructor*>>), true);

        // Checking std::vectors, of pointers which pointees can not be default constructed.
        QCOMPARE((is_collection_pointees_default_constructible<
            std::vector<ArgumentedConstructor*>>::value), false);
        QCOMPARE((is_collection_pointees_default_constructible_v<
            std::vector<ArgumentedConstructor*>>), false);

        // Checking pairs of default-constructible pointers
        QCOMPARE((is_collection_pointees_default_constructible<
            std::pair<int*,DefaultArgumentsConstructor*>>::value), true);
        QCOMPARE((is_collection_pointees_default_constructible_v<
            std::pair<int*,DefaultArgumentsConstructor*>>), true);

        // Checking pairs of pointers which pointees can not be default constructed.
        QCOMPARE((is_collection_pointees_default_constructible<
            std::pair<ArgumentedConstructor*,DefaultArgumentsConstructor*>>::value), false);
        QCOMPARE((is_collection_pointees_default_constructible_v<
            std::pair<ArgumentedConstructor*,DefaultArgumentsConstructor*>>), false);
    }
};

QTEST_APPLESS_MAIN(TemplateDetectorsTest)

#include "TemplateDetectorsTest.moc"
