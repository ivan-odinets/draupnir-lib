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

#include "draupnir/utils/template_constructors.h"

#include <QVector>

#include <vector>

using namespace draupnir::utils;

/*! @class TemplateConstructorsTest
 *  @brief Test class for testing entities present within @ref draupnir/utils/template_constructors.h
 *
 * @todo Refractor this test so that it will have better readability and cover more cases.
 * @todo Add some script to execute this test in the context of CI. */

class TemplateConstructorsTest final : public QObject
{
    Q_OBJECT
public:
    using TupleOne = std::tuple<int*,int*,int*>;
    using TupleTwo = std::tuple<QString*,QString*,QString*>;

private slots:
    void test_create_tuple_new() {
        TupleOne one = create_tuple_new<TupleOne>();
        // Check first
        QVERIFY(std::get<0>(one) != nullptr);
        // By default created int will be 0.
        QCOMPARE((*std::get<0>(one)), 0);
        // Check second
        QVERIFY(std::get<1>(one) != nullptr);
        // By default created int will be 0.
        QCOMPARE((*std::get<1>(one)), 0);
        // Check third
        QVERIFY(std::get<2>(one) != nullptr);
        // By default created int will be 0.
        QCOMPARE((*std::get<2>(one)), 0);

        TupleTwo two = create_tuple_new<TupleTwo>();
        // Check first
        QVERIFY(std::get<0>(two) != nullptr);
        // By default created int will be 0.
        QCOMPARE((*std::get<0>(two)), QString{});
        // Check second
        QVERIFY(std::get<1>(one) != nullptr);
        // By default created int will be 0.
        QCOMPARE((*std::get<1>(two)), QString{});
        // Check third
        QVERIFY(std::get<2>(two) != nullptr);
        // By default created int will be 0.
        QCOMPARE((*std::get<2>(two)), QString{});

        std::apply([](auto*... elements){
            (delete elements, ...);
        }, one);

        std::apply([](auto*... elements){
            (delete elements, ...);
        }, two);
    }

    void test_make_zero_value() {
        int integer = make_zero_value<int>();
        QCOMPARE(integer,0);

        QString string = make_zero_value<QString>();
        QCOMPARE(string, QString{});

        std::pair<int,int> pair = make_zero_value<std::pair<int,int>>();
        QCOMPARE(pair.first, 0);
        QCOMPARE(pair.second, 0);

        std::pair<int,int*> otherPair = make_zero_value<std::pair<int,int*>>();
        QCOMPARE(otherPair.first, 0);
        QCOMPARE(otherPair.second, nullptr);

        std::tuple<int,int,int> tuple = make_zero_value<std::tuple<int,int,int>>();
        QCOMPARE((std::get<0>(tuple)),0);
        QCOMPARE((std::get<1>(tuple)),0);
        QCOMPARE((std::get<2>(tuple)),0);

        std::tuple<int,int*,int> otherTuple = make_zero_value<std::tuple<int,int*,int>>();
        QCOMPARE((std::get<0>(otherTuple)),0);
        QCOMPARE((std::get<1>(otherTuple)),nullptr);
        QCOMPARE((std::get<2>(otherTuple)),0);
    }
};

QTEST_APPLESS_MAIN(TemplateConstructorsTest)

#include "TemplateConstructorsTest.moc"
