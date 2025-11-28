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
#include <QCoreApplication>

#include "draupnir/loptr/utils/Terminate.h"

/*! @class TerminateIfEvenTest tests/loptr/unit/TerminateTests/TerminateIfEvenTest/TerminateIfEvenTest.cpp
 *  @brief This class is for testing the Draupnir::Loptr::Terminate::terminateIfEven method.
 *
 * @todo Add some script to execute this test in the context of CI. */

class TerminateIfEvenTest final : public QObject
{
    Q_OBJECT

public:
    TerminateIfEvenTest() = default;
    ~TerminateIfEvenTest() final = default;

private slots:
    /*! @brief This test method should fail. More or less soon. */
    void test_if_we_are_lucky() {
        for (int i = 0; i < 100000; i++) {
            Draupnir::Loptr::Terminate::terminateIfEven();
        };
    }
};

QTEST_MAIN(TerminateIfEvenTest)

#include "TerminateIfEvenTest.moc"
