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

#include "draupnir/settings_registry/utils/OptionalSettingsBundle.h"

#include "draupnir-test/traits/settings/SomeCustomDoubleSetting.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

/*! @class OptionalSettingsBundleTest tests/settings_registry/unit/OptionalSettingsBundleTest/OptionalSettingsBundleTest.cpp
 *  @brief This class tests @ref Draupnir::Settings::OptionalSettingsBundle. */

class OptionalSettingsBundleTest final : public QObject
{
    Q_OBJECT

public:
    using NonEmptyBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomDoubleSetting, SomeCustomBoolSetting
    >;

    class ShouldHaveInt :
        public Draupnir::Settings::OptionalSettingsBundle<NonEmptyBundle, false>
    { [[maybe_unused]] int field; };

    class ShouldBeMoreThanInt :
        public Draupnir::Settings::OptionalSettingsBundle<NonEmptyBundle, true>
    { [[maybe_unused]] int field; };

    class FromNonEmptyBundle :
        public Draupnir::Settings::NonEmptySettingsBundle<NonEmptyBundle>
    { [[maybe_unused]] int field; };

    class FromEmptyBundle :
        public Draupnir::Settings::NonEmptySettingsBundle<Draupnir::Settings::SettingsBundleTemplate<>>
    { [[maybe_unused]] int field; };

private slots:
    void test_optional_settings_bundle_sizes() {
        QCOMPARE(sizeof(ShouldHaveInt),sizeof(int));
        QVERIFY(sizeof(ShouldBeMoreThanInt) > sizeof(int));
    }

    void test_non_empty_settings_bundle_sizes() {
        QCOMPARE(sizeof(FromEmptyBundle),sizeof(int));
        QVERIFY(sizeof(FromNonEmptyBundle) > sizeof(int));
    }
};

QTEST_MAIN(OptionalSettingsBundleTest)

#include "OptionalSettingsBundleTest.moc"
