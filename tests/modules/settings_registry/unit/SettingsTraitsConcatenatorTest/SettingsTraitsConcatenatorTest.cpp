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

#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"

#include "draupnir-test/traits/settings/BoolSettingTraits.h"
#include "draupnir-test/traits/settings/DoubleSettingTraits.h"
#include "draupnir-test/traits/settings/StringSettingTraits.h"

class ClassWithBundle
{
public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        BoolSettingTrait, QStringSettingTrait
    >;
};

class OtherClassWithBundle
{
public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        DoubleSettingTrait, QStringSettingTrait, FloatSettingTrait
    >;
};

class ClassWithoutBundle
{
public:
    using NotBundle = void;
};

namespace Draupnir::Settings
{

/*! @class SettingsTraitsConcatenatorTest tests/modules/settings_registry/unit/SettingsTraitsConcatenatorTest/SettingsTraitsConcatenatorTest.cpp
 *  @brief This is a test for SettingsTraitsConcatenator */

class SettingsTraitsConcatenatorTest final : public QObject
{
    Q_OBJECT
private slots:
    void test_complex_bundle_building() {
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            Draupnir::Settings::SettingsBundleTemplate<
                QStringSettingTrait,
                DoubleSettingTrait,
                BoolSettingTrait
            >,
            QStringSettingTrait
        >::toSettingsBundle;

        QCOMPARE(Result::traitCount(), std::size_t{3});
        QCOMPARE(Result::contains<BoolSettingTrait>(), true);
        QCOMPARE(Result::contains<DoubleSettingTrait>(), true);
        QCOMPARE(Result::contains<QStringSettingTrait>(), true);
    }

    void test_empty_bundle_merge() {
        // And from Empty Bundles we should get empty bundles...
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            Draupnir::Settings::SettingsBundleTemplate<>,
            Draupnir::Settings::SettingsBundleTemplate<>,
            Draupnir::Settings::SettingsBundleTemplate<>
        >::toSettingsBundle;

        QCOMPARE(Result::isEmpty(), true);
    }

    void test_bundle_extraction_and_merge() {
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            ClassWithBundle,
            ClassWithoutBundle,
            OtherClassWithBundle
        >::toSettingsBundle;

        QCOMPARE(Result::isEmpty(), false);
        QCOMPARE(Result::traitCount(), 4);
        QCOMPARE(Result::contains_v<BoolSettingTrait>, true);
        QCOMPARE(Result::contains_v<DoubleSettingTrait>, true);
        QCOMPARE(Result::contains_v<QStringSettingTrait>, true);
        QCOMPARE(Result::contains_v<FloatSettingTrait>, true);
    }

    void test_mergint_into_registry() {
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            ClassWithBundle,
            ClassWithoutBundle,
            Draupnir::Settings::SettingsBundleTemplate<>,
            OtherClassWithBundle
        >::toSettingsRegistry;

        QCOMPARE(Result::contains<BoolSettingTrait>(),true);
        QCOMPARE(Result::contains<DoubleSettingTrait>(),true);
        QCOMPARE(Result::contains<QStringSettingTrait>(),true);
        QCOMPARE(Result::contains<FloatSettingTrait>(),true);
    }
};

}; // namespace Draupnir::Settings

QTEST_MAIN(Draupnir::Settings::SettingsTraitsConcatenatorTest)

#include "SettingsTraitsConcatenatorTest.moc"
