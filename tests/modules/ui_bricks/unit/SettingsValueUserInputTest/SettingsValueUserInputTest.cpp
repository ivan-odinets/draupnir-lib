/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir-test/helpers/UiTestHelper.h"
#include "draupnir-test/traits/settings/DoubleSettingTraits.h"
#include "draupnir-test/traits/settings/IntegerSettingTraits.h"
#include "draupnir-test/traits/settings/StringSettingTraits.h"

#include "draupnir/ui_bricks/utils/SettingsValueUserInput.h"

namespace Draupnir::Ui {

/*! @class SettingsValueUserInputTest tests/modules/ui_bricks/unit/SettingsValueUserInputTest/SettingsValueUserInputTest.cpp
 *  @brief Test class for the SettingsValueUserInput. */

class SettingsValueUserInputTest final : public QObject
{
    Q_OBJECT
private:
    template<Settings::SettingTraitConcept Trait, class Value>
    static std::optional<typename Trait::Value> getInputResult(Value oldValue, Value newValue, bool shouldBeAccepted) {
        UiTestHelper::scheduleQInputDialogUserInput(newValue,shouldBeAccepted);
        return Handlers::SettingsValueUserInput<Trait,typename Trait::Value>::getValue(oldValue);
    }

private slots:
    void test_short_input() {
        constexpr short oldInt = 42;
        constexpr short newInt = 142;
        // Test canceling dialog
        auto result = getInputResult<ShortSettingTrait,short>(oldInt,newInt,false);
        QVERIFY(result.has_value() == false);

        // Test accepting dialog
        result = getInputResult<ShortSettingTrait,short>(oldInt,newInt,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), newInt);

        // Test inputing values higher than short can contain.
        result = getInputResult<ShortSettingTrait,int>(oldInt,100000,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), std::numeric_limits<short>::max());

        // Test inputing values lower than short can contain.
        result = getInputResult<ShortSettingTrait,int>(oldInt,-100000,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), std::numeric_limits<short>::min());

        // Test restricted short value from above
        result = getInputResult<RestrictedShortSettingTrait,short>(oldInt,1000,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), RestrictedShortSettingTrait::maximalValue());

        // Test restricted short value from below
        result = getInputResult<RestrictedShortSettingTrait,short>(oldInt,-1000,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), RestrictedShortSettingTrait::minimalValue());
    }

    void test_integer_input() {
        constexpr int oldInt = 42;
        constexpr int newInt = 142;
        // Test canceling dialog
        auto result = getInputResult<IntSettingTrait,int>(oldInt,newInt,false);
        QVERIFY(result.has_value() == false);

        // Test accepting dialog
        result = getInputResult<IntSettingTrait,int>(oldInt,newInt,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), newInt);

        // Test restricted integer value from above
        result = getInputResult<RestrictedIntSettingTrait,int>(oldInt,1000,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), RestrictedIntSettingTrait::maximalValue());

        // Test restricted integer value from below
        result = getInputResult<RestrictedIntSettingTrait,int>(oldInt,-1000,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), RestrictedIntSettingTrait::minimalValue());
    }

    void test_float_input() {
        constexpr float oldFloat = 1.55;
        constexpr float newFloat = 42.42;
        // Test canceling dialog
        auto result = getInputResult<FloatSettingTrait>(oldFloat,newFloat,false);
        QVERIFY(!result.has_value());

        // Test accepting dialog
        result = getInputResult<FloatSettingTrait>(oldFloat,newFloat,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), newFloat);
    }

    void test_double_input() {
        constexpr double oldDouble = 1.55;
        constexpr double newDouble = 42.42;
        // Test canceling dialog
        auto result = getInputResult<DoubleSettingTrait>(oldDouble,newDouble,false);
        QVERIFY(!result.has_value());

        // Test accepting dialog
        result = getInputResult<DoubleSettingTrait>(oldDouble,newDouble,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), newDouble);
    }

    void test_stdtring_input() {
        const std::string oldString = "Very Old String";
        const std::string newString = "Very New String";
        // Test canceling dialog
        auto result = getInputResult<StdStringSettingTrait,std::string>(oldString,newString,false);
        QVERIFY(result.has_value() == false);

        // Test accepting dialog
        result = getInputResult<StdStringSettingTrait,std::string>(oldString,newString,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), newString);
    }

    void test_qtring_input() {
        const QString oldString = "Very Old String";
        const QString newString = "Very New String";
        // Test canceling dialog
        auto result = getInputResult<QStringSettingTrait,QString>(oldString,newString,false);
        QVERIFY(result.has_value() == false);

        // Test accepting dialog
        result = getInputResult<QStringSettingTrait,QString>(oldString,newString,true);
        QVERIFY(result.has_value());
        QCOMPARE(result.value(), newString);
    }
};

} // namespace Draupnir::Ui

QTEST_MAIN(Draupnir::Ui::SettingsValueUserInputTest)

#include "SettingsValueUserInputTest.moc"
