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

#include "draupnir/ui_bricks/utils/CheckableUiElementHelper.h"

/*! @class CheckableUiElementHelperTest tests/ui_bricks/unit/CheckableUiElementHelperTest/CheckableUiElementHelperTest.cpp
 *  @brief This test class tests basic functionality of the CheckableUiElementHelper. */

class CheckableUiElementHelperTest : public QObject
{
    Q_OBJECT
public:
    int currentCallCount = 0;
    bool lastState = false;
    std::function<void(bool state)> callable = [this](bool state){
        currentCallCount++;
        lastState = state;
    };

private slots:
    void init() {
        currentCallCount = 0;
        lastState = false;
    }

    void test_action_creation() {
        QVERIFY(lastState == false);
        QVERIFY(currentCallCount == 0);

        QAction* result = Draupnir::Ui::CheckableUiElementHelper<QAction>::createConnectedUiElement(callable);
        QVERIFY(result->isCheckable());

        // Trigger once
        result->trigger();
        QTRY_COMPARE(currentCallCount,1);
        QTRY_COMPARE(lastState, true);

        // Trigger twice
        result->trigger();
        QTRY_COMPARE(currentCallCount,2);
        QTRY_COMPARE(lastState, false);

        delete result;
    }

    void test_checkbox_creation() {
        QVERIFY(lastState == false);
        QVERIFY(currentCallCount == 0);

        QCheckBox* result = Draupnir::Ui::CheckableUiElementHelper<QCheckBox>::createConnectedUiElement(callable);

        // Trigger once
        result->click();
        QTRY_COMPARE(currentCallCount,1);
        QTRY_COMPARE(lastState, true);

        // Trigger twice
        result->click();
        QTRY_COMPARE(currentCallCount,2);
        QTRY_COMPARE(lastState, false);

        delete result;
    }
};

QTEST_MAIN(CheckableUiElementHelperTest)

#include "CheckableUiElementHelperTest.moc"
