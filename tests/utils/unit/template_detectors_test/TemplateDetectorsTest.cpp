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

/*! @class TemplateDetectorsTest
 *  @brief Test class for testing entities present within @ref draupnir/utils/template_detectors.h
 *
 * @todo Refractor this test so that it will have better readability and cover more cases.
 * @todo Add some script to execute this test in the context of CI. */

class TemplateDetectorsTest final : public QObject
{
    Q_OBJECT
public:
    using PairOne = std::pair<int,double>;
    using PairTwo = std::pair<int,int>;

    using TupleOne = std::tuple<int,double,QString>;
    using TupleTwo = std::tuple<int,int,int>;

    using VectorOne = std::vector<int>;
    using VectorTwo = std::vector<QString>;

private slots:
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
};

QTEST_APPLESS_MAIN(TemplateDetectorsTest)

#include "TemplateDetectorsTest.moc"
