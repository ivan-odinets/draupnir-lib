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

#include "draupnir/utils/index_of.h"

using namespace draupnir::utils;

/*! @class IndexOfTest tests/utils/unit/index_of_test/IndexOfTest.cpp
 *  @brief Test class for testing entities present within @ref draupnir/utils/index_of.h. */

class IndexOfTest final : public QObject
{
    Q_OBJECT
public:

private slots:
    void test_index_of() {
        // Present type
        QCOMPARE((index_of<int,int,double,QString>::value), (unsigned long)0);
        QCOMPARE((index_of_v<int,int,double,QString>), (unsigned long)0);

        QCOMPARE((index_of<double,int,double,QString>::value), (unsigned long)1);
        QCOMPARE((index_of_v<double,int,double,QString>), (unsigned long)1);
    }
};

QTEST_APPLESS_MAIN(IndexOfTest)

#include "IndexOfTest.moc"
