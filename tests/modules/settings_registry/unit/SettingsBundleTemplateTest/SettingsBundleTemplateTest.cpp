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

#include "draupnir/settings_registry/SettingsRegistryTemplate.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/settings/BoolSettingTraits.h"
#include "draupnir-test/traits/settings/DoubleSettingTraits.h"
#include "draupnir-test/traits/settings/StringSettingTraits.h"
#include "draupnir-test/traits/settings/WidgetIndexSettingTraits.h"

namespace Draupnir::Settings
{

/*! @class SettingsBundleTemplateTest tests/modules/settings_registry/unit/SettingsBundleTemplateTest.cpp
 *  @brief This is a test for SettingsBundleTemplate
 * @todo Make reasonable version of the test_print_to test method. */

class SettingsBundleTemplateTest final : public QObject
{
    Q_OBJECT

public:
///@name Tested types
///@{
    using MockSettings = MockSettingsTemplate<
        WidgetIndexSettingTrait,
        DoubleSettingTrait,
        BoolSettingTrait
    >;

    using EmptyBundle = Draupnir::Settings::SettingsBundleTemplate<>;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        WidgetIndexSettingTrait,
        DoubleSettingTrait,
        BoolSettingTrait
    >;

    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        WidgetIndexSettingTrait,
        DoubleSettingTrait,
        BoolSettingTrait
    >;

    using RandomPopulatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        WidgetIndexSettingTrait, DoubleSettingTrait
    >;
    using RandomUnpopulatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        BoolSettingTrait, QStringSettingTrait
    >;
///@}

///@name Required fields
///@{
    MockSettings dummySettingsSource;
    SettingsRegistry settingsRegistry;

///@}

private slots:
    void initTestCase() {
        settingsRegistry.setBackend(&dummySettingsSource);
    }

    void test_constexpr_methods() {
        // Check SettingsBundleTemplate<> static things
        QCOMPARE(EmptyBundle::isEmpty(), true);
        QCOMPARE(EmptyBundle::isEmpty_v, true);

        QCOMPARE(EmptyBundle::contains<BoolSettingTrait>(), false);
        QCOMPARE(EmptyBundle::contains_v<BoolSettingTrait>, false);

        QCOMPARE(RandomPopulatableBundle::canBeFullyPopulatedFrom<EmptyBundle>(), false);
        QCOMPARE(RandomPopulatableBundle::canBeFullyPopulatedFrom_v<EmptyBundle>, false);

        // Check SettingsBundleTemplate with some traits
        QCOMPARE(SettingsBundle::isEmpty(), false);
        QCOMPARE(SettingsBundle::isEmpty_v, false);

        QCOMPARE(SettingsBundle::contains<DoubleSettingTrait>(), true);
        QCOMPARE(SettingsBundle::contains_v<DoubleSettingTrait>, true);

        QCOMPARE(SettingsBundle::contains<QStringSettingTrait>(), false);
        QCOMPARE(SettingsBundle::contains_v<QStringSettingTrait>, false);

        QCOMPARE(RandomPopulatableBundle::canBeFullyPopulatedFrom<SettingsBundle>(), true);
        QCOMPARE(RandomPopulatableBundle::canBeFullyPopulatedFrom_v<SettingsBundle>, true);

        QCOMPARE(RandomUnpopulatableBundle::canBeFullyPopulatedFrom<SettingsBundle>(), false);
        QCOMPARE(RandomUnpopulatableBundle::canBeFullyPopulatedFrom_v<SettingsBundle>, false);
    }

    void test_initialization() {
        // We need this for test to work
        QVERIFY(settingsRegistry.settings() != nullptr);

        SettingsBundle nonLoadedBundle;
        QCOMPARE(nonLoadedBundle.settings(), nullptr);
        QCOMPARE(nonLoadedBundle.isLoaded(), false);
        QCOMPARE(nonLoadedBundle.isValid(), false);

        SettingsBundle bundle{&dummySettingsSource};
        QCOMPARE(bundle.settings(), &dummySettingsSource);
        QCOMPARE(bundle.isLoaded(), true);
        QCOMPARE(bundle.isValid(), false);

        SettingsBundle populatedBundle = settingsRegistry.getSettingsBundle<SettingsBundle>();
        QCOMPARE(populatedBundle.settings(), &dummySettingsSource);
        QCOMPARE(populatedBundle.isLoaded(), true);
        QCOMPARE(populatedBundle.isValid(), true);
    }

    void test_set_and_get() {
        const double testDouble = M_E;
        const int testInteger = 42;

        SettingsBundle populatedBundle = settingsRegistry.getSettingsBundle<SettingsBundle>();

        // Verify that we don't have the test values
        QVERIFY(populatedBundle.template get<WidgetIndexSettingTrait>() != testInteger);
        QVERIFY(populatedBundle.template get<DoubleSettingTrait>() != testDouble);
        QVERIFY(dummySettingsSource.template get<WidgetIndexSettingTrait>() != testInteger);
        QVERIFY(dummySettingsSource.template get<DoubleSettingTrait>() != testDouble);

        // Set something
        populatedBundle.template set<WidgetIndexSettingTrait>(testInteger);
        populatedBundle.template set<DoubleSettingTrait>(testDouble);
        // Check if SettingsRegistry::get method is returning what expected
        QCOMPARE(populatedBundle.template get<WidgetIndexSettingTrait>(), testInteger);
        QCOMPARE(populatedBundle.template get<DoubleSettingTrait>(), testDouble);

        // Check if values were indeed written to the backend
        QCOMPARE(dummySettingsSource.template get<WidgetIndexSettingTrait>(), testInteger);
        QCOMPARE(dummySettingsSource.template get<DoubleSettingTrait>(), testDouble);

        // Reset things through the registry
        populatedBundle.template set<WidgetIndexSettingTrait>(WidgetIndexSettingTrait::defaultValue());
        populatedBundle.template set<DoubleSettingTrait>(DoubleSettingTrait::defaultValue());
    }

    void test_subbundle_functionality() {
        const double testDouble = M_PI*M_E;
        const int testInteger = 42*42;

        SettingsBundle populatedBundle = settingsRegistry.getSettingsBundle<SettingsBundle>();
        RandomPopulatableBundle subBundle = populatedBundle.getSettingsBundle<RandomPopulatableBundle>();

        // Verify that we don't have the test values
        QVERIFY(subBundle.template get<WidgetIndexSettingTrait>() != testInteger);
        QVERIFY(subBundle.template get<DoubleSettingTrait>() != testDouble);
        QVERIFY(dummySettingsSource.template get<WidgetIndexSettingTrait>() != testInteger);
        QVERIFY(dummySettingsSource.template get<DoubleSettingTrait>() != testDouble);

        // Write to subbundle
        subBundle.template set<WidgetIndexSettingTrait>(testInteger);
        subBundle.template set<DoubleSettingTrait>(testDouble);
        // Check if SettingsRegistry::get method is returning what expected
        QCOMPARE(subBundle.template get<WidgetIndexSettingTrait>(), testInteger);
        QCOMPARE(subBundle.template get<DoubleSettingTrait>(), testDouble);

        // Check if values were indeed written to the backend
        QCOMPARE(dummySettingsSource.template get<WidgetIndexSettingTrait>(), testInteger);
        QCOMPARE(dummySettingsSource.template get<DoubleSettingTrait>(), testDouble);
    }

    void test_print_to() {
        SettingsBundle populatedBundle = settingsRegistry.getSettingsBundle<SettingsBundle>();
        populatedBundle.printTo(qDebug());
        populatedBundle.printTo(qInfo());
    }
};

}; // namespace Draupnir::Settings

QTEST_MAIN(Draupnir::Settings::SettingsBundleTemplateTest)

#include "SettingsBundleTemplateTest.moc"
