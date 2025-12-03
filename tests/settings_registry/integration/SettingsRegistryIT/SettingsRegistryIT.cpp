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

#include "draupnir/settings_registry/SettingsRegistryTemplate.h"
#include "draupnir/settings_registry/traits/settings/CentralWidgetIndexSetting.h"
#include "draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h"
#include "draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/settings/SomeCustomDoubleSetting.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

/*! @class SettingsRegistryIT tests/settings_registry/integration/SettingsRegistryIT/SettingsRegistryIT.cpp
 *  @brief This is a very basic test for SettingsRegistryTemplate and related things.
 *
 * @todo Split this test into several: unit-test for SettingsRegistryTemplate, unit test for SettingsBundleTemplate and
 *       integration between SettingsRegistryTemplate and SettingsBundleTemplate.
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

class SettingsRegistryIT final : public QObject
{
    Q_OBJECT

public:
///@name Tested types
///@{
    using MockSettings = MockSettingsTemplate<
        Draupnir::Settings::CentralWidgetIndexSetting,
        Draupnir::Settings::LastUsedDirectorySetting,
        SomeCustomDoubleSetting,
        SomeCustomBoolSetting
    >;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        Draupnir::Settings::CentralWidgetIndexSetting,
        Draupnir::Settings::LastUsedDirectorySetting,
        SomeCustomDoubleSetting,
        SomeCustomBoolSetting
    >;
    using RandomPopulatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomDoubleSetting, Draupnir::Settings::CentralWidgetIndexSetting
    >;
    using RandomUnpopulatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomBoolSetting, Draupnir::Settings::RecentFileListSetting
    >;
///@}

///@name Required fields
///@{
    MockSettings dummySettingsSource;

    SettingsRegistry testedRegistry;
///@}

private slots:
    void initTestCase() {
        testedRegistry.setBackend(&dummySettingsSource);
    }

    /*! @brief Testing if the constexpt methods are correct for the specfic SettingsRegistryTemplate.
     * @todo Extend this test method. */
    void test_constexpr_methods() {
        // Contains
        QCOMPARE(decltype(testedRegistry)::template contains<SomeCustomDoubleSetting>(), true);
        QCOMPARE(decltype(testedRegistry)::template contains<Draupnir::Settings::RecentFileListSetting>(), false);

        // If stuff is empry or not?
        QCOMPARE(Draupnir::Settings::SettingsRegistryTemplate<>::isEmpty(), true);
        QCOMPARE(decltype(testedRegistry)::isEmpty(), false);

        // If bundle can be populated from SettingsRegistryTemplate
        QCOMPARE(RandomPopulatableBundle::template canBeFullyPopulatedFrom<decltype(testedRegistry)>(), true);
        QCOMPARE(RandomUnpopulatableBundle::template canBeFullyPopulatedFrom<decltype(testedRegistry)>(), false);
    }

    /*! @brief Testing if the SettingsRegistryTemplate is correctly initialized and if backend is correctly loaded. */
    void test_initialization() {
        SettingsRegistry otherRegistry;
        // Test non-loaded SettingsRegistry
        QCOMPARE(otherRegistry.settings(), nullptr);
        QCOMPARE(otherRegistry.isLoaded(), false);
    }

    void test_backendLoading() {
        SettingsRegistry otherRegistry;
        MockSettings otherBackend;

        // Load the backend and see what happens
        otherRegistry.setBackend(&otherBackend);
        QCOMPARE(otherRegistry.settings(),static_cast<Draupnir::Settings::SettingsBackendInterface*>(&otherBackend));
        QVERIFY(otherRegistry.isLoaded());

        // By default we should have the default values.
        QCOMPARE(testedRegistry.get<Draupnir::Settings::CentralWidgetIndexSetting>(),
                 Draupnir::Settings::CentralWidgetIndexSetting::defaultValue());
        QCOMPARE(testedRegistry.get<SomeCustomDoubleSetting>(),
                 SomeCustomDoubleSetting::defaultValue());
    }

    void test_set_and_get() {
        double testDouble = M_E;
        int testInteger = 42;

        // Verify that we don't have the test values
        QVERIFY(testedRegistry.template get<Draupnir::Settings::CentralWidgetIndexSetting>() != testInteger);
        QVERIFY(testedRegistry.template get<SomeCustomDoubleSetting>() != testDouble);
        QVERIFY(dummySettingsSource.template get<Draupnir::Settings::CentralWidgetIndexSetting>() != testInteger);
        QVERIFY(dummySettingsSource.template get<SomeCustomDoubleSetting>() != testDouble);

        // Set something
        testedRegistry.template set<Draupnir::Settings::CentralWidgetIndexSetting>(testInteger);
        testedRegistry.template set<SomeCustomDoubleSetting>(testDouble);

        // Check if SettingsRegistry::get method is returning what expected
        QCOMPARE(testedRegistry.template get<Draupnir::Settings::CentralWidgetIndexSetting>(), testInteger);
        QCOMPARE(testedRegistry.template get<SomeCustomDoubleSetting>(), testDouble);

        // Check if values were indeed written to the backend
        QCOMPARE(dummySettingsSource.template get<Draupnir::Settings::CentralWidgetIndexSetting>(), testInteger);
        QCOMPARE(dummySettingsSource.template get<SomeCustomDoubleSetting>(), testDouble);
    }

    void test_bundle_functionality() {
        auto bundleByTrait = testedRegistry.template getSettingBundleForTraits<
            SomeCustomDoubleSetting,Draupnir::Settings::LastUsedDirectorySetting
        >();

        // Check if bundle reports the same as registry
        QCOMPARE(bundleByTrait.template get<Draupnir::Settings::LastUsedDirectorySetting>(),
                 testedRegistry.template get<Draupnir::Settings::LastUsedDirectorySetting>());
        QCOMPARE(bundleByTrait.template get<SomeCustomDoubleSetting>(),
                 testedRegistry.template get<SomeCustomDoubleSetting>());

        // Check if by any magic within bundle / registry wrong value is present
        QString testString = "Hello SettingsRegistry!";
        QVERIFY(bundleByTrait.template get<Draupnir::Settings::LastUsedDirectorySetting>() != testString);
        QVERIFY(testedRegistry.template get<Draupnir::Settings::LastUsedDirectorySetting>() != testString);

        // write to bundle and check again
        bundleByTrait.template set<Draupnir::Settings::LastUsedDirectorySetting>(testString);
        QCOMPARE(bundleByTrait.get<Draupnir::Settings::LastUsedDirectorySetting>(),
                 testString);
        QCOMPARE(testedRegistry.template get<Draupnir::Settings::LastUsedDirectorySetting>(), testString);

        // Check if by any magic within bundle / resgistry wrong value is present
        double testDouble = M_PI * M_E;
        QVERIFY(bundleByTrait.template get<SomeCustomDoubleSetting>() != testDouble);
        QVERIFY(testedRegistry.template get<SomeCustomDoubleSetting>() != testDouble);

        // write to registry and check again
        testedRegistry.template set<SomeCustomDoubleSetting>(testDouble);
        QCOMPARE(bundleByTrait.template get<SomeCustomDoubleSetting>(), testDouble);
        QCOMPARE(testedRegistry.template get<SomeCustomDoubleSetting>(), testDouble);
    }
};

QTEST_MAIN(SettingsRegistryIT)

#include "SettingsRegistryIT.moc"
