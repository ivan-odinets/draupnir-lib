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

#include "draupnir/utils/type_extractors.h"

using namespace draupnir::utils;

/*! @class TypeExtractorsTest tests/utils/unit/type_extractors_test/TypeExtractorsTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/type_extractors.h. */

class TypeExtractorsTest final : public QObject
{
    Q_OBJECT
public:
    TypeExtractorsTest() = default;
    ~TypeExtractorsTest() final = default;

    using MyVector = std::vector<int>;
    using MyTuple = std::tuple<int,double,QString>;
    using MyPair = std::pair<std::tuple<int,double,float>,QString>;

private slots:
    void test_get_template_instantiation() {
        // Extract tuple
        using ShouldBeTuple = typename get_template_instantiation<std::tuple,
            int,double,MyTuple,float, MyVector, QString, MyPair, QList<int>>::type;
        QCOMPARE((std::is_same_v<ShouldBeTuple,MyTuple>), true);

        // Extract std::vector
        using ShouldBeVector = typename get_template_instantiation<std::vector,
            int,double,MyTuple,float, MyVector, QString, MyPair, QList<int>>::type;
        QCOMPARE((std::is_same_v<ShouldBeVector,MyVector>), true);

        // Extract std::pair
        using ShouldBePair = typename get_template_instantiation<std::pair,
            int,double,MyTuple,float, MyVector, QString, MyPair, QList<int>>::type;
        QCOMPARE((std::is_same_v<ShouldBePair,MyPair>), true);
    }
};

QTEST_APPLESS_MAIN(TypeExtractorsTest)

#include "TypeExtractorsTest.moc"
