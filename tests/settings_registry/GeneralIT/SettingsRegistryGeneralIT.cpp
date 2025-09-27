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

#include "draupnir/SettingsRegistryTemplate.h"
#include "draupnir/traits/settings/ActiveWidgetIndexSetting.h"
#include "draupnir/traits/settings/LastUsedDirectorySetting.h"
#include "draupnir/traits/settings/RecentFilesListSetting.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/settings/SomeCustomDoubleSetting.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

/*! @class SettingsRegistryGeneralIT tests/settings_registry/GeneralIT/SettingsRegistryGeneralIT.cpp
 *  @brief This is a very basic test for SettingsRegistryTemplate and related things. */

class SettingsRegistryGeneralIT : public QObject
{
    Q_OBJECT

public:
    SettingsRegistryGeneralIT() {
        testedRegistry.setBackend(&dummySettingsSource);
    }

    MockSettingsTemplate<
        Draupnir::Settings::ActiveWidgetIndexSetting,
        Draupnir::Settings::LastUsedDirectorySetting,
        SomeCustomDoubleSetting,
        SomeCustomBoolSetting
    > dummySettingsSource;

    Draupnir::Settings::SettingsRegistryTemplate<
        Draupnir::Settings::ActiveWidgetIndexSetting,
        Draupnir::Settings::LastUsedDirectorySetting,
        SomeCustomDoubleSetting,
        SomeCustomBoolSetting
    > testedRegistry;

    using RandomPoplatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomDoubleSetting, Draupnir::Settings::ActiveWidgetIndexSetting
    >;
    using RandomUnpoplatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomBoolSetting, Draupnir::Settings::RecentFileListSetting
    >;

private slots:
    void test_compileTimeCorrectness() {
        // Initial state of backend
        QVERIFY(testedRegistry.settings() == static_cast<Draupnir::Settings::SettingsBackendInterface*>(&dummySettingsSource));
        QVERIFY(testedRegistry.isLoaded() == true);

        // Contains
        QVERIFY(decltype(testedRegistry)::template contains<SomeCustomDoubleSetting>() == true);
        QVERIFY(decltype(testedRegistry)::template contains<Draupnir::Settings::RecentFileListSetting>() == false);

        // If stuff is empry or not?
        QVERIFY(decltype(testedRegistry)::isEmpty() == false);
        QVERIFY(Draupnir::Settings::SettingsRegistryTemplate<>::isEmpty() == true);

        // If bundle can be populated from SettingsRegistryTemplate
        QVERIFY(RandomPoplatableBundle::template canBeFullyPopulatedFrom<decltype(testedRegistry)>() == true);
        QVERIFY(RandomUnpoplatableBundle::template canBeFullyPopulatedFrom<decltype(testedRegistry)>() == false);
    }

    void test_initialization() {
        QVERIFY(testedRegistry.get<Draupnir::Settings::ActiveWidgetIndexSetting>() == Draupnir::Settings::ActiveWidgetIndexSetting::defaultValue());
        QVERIFY(testedRegistry.get<SomeCustomDoubleSetting>() == SomeCustomDoubleSetting::defaultValue());
    }

    void test_readingWriting() {

        double testDouble = M_E;
        int testInteger = 42;

        testedRegistry.template set<Draupnir::Settings::ActiveWidgetIndexSetting>(testInteger);
        testedRegistry.template set<SomeCustomDoubleSetting>(testDouble);

        QVERIFY(testedRegistry.template get<Draupnir::Settings::ActiveWidgetIndexSetting>() == testInteger);
        QVERIFY(testedRegistry.template get<SomeCustomDoubleSetting>() == testDouble);

        QVERIFY(dummySettingsSource.template get<Draupnir::Settings::ActiveWidgetIndexSetting>() == testInteger);
        QVERIFY(dummySettingsSource.template get<SomeCustomDoubleSetting>() == testDouble);
    }

    void test_bundles() {
        auto bundleByTrait = testedRegistry.template getSettingBundleForTraits<
                SomeCustomDoubleSetting,Draupnir::Settings::LastUsedDirectorySetting
        >();

        // Check if bundle reports the same as registry
        QVERIFY(bundleByTrait.template get<Draupnir::Settings::LastUsedDirectorySetting>() ==
                testedRegistry.template get<Draupnir::Settings::LastUsedDirectorySetting>());
        QVERIFY(bundleByTrait.template get<SomeCustomDoubleSetting>() ==
                testedRegistry.template get<SomeCustomDoubleSetting>());


        // Check if by any magic within bundle / registry wrong value is present
        QString writtenToBundle = "Hello SettingsRegistry!";
        QVERIFY(bundleByTrait.template get<Draupnir::Settings::LastUsedDirectorySetting>() != writtenToBundle);
        QVERIFY(testedRegistry.template get<Draupnir::Settings::LastUsedDirectorySetting>() != writtenToBundle);

        // write to bundle and check again
        bundleByTrait.template set<Draupnir::Settings::LastUsedDirectorySetting>(writtenToBundle);
        QVERIFY(bundleByTrait.get<Draupnir::Settings::LastUsedDirectorySetting>() == writtenToBundle);
        QVERIFY(testedRegistry.template get<Draupnir::Settings::LastUsedDirectorySetting>() == writtenToBundle);

        // Check if by any magic within bundle / resgistry wrong value is present
        double writtenToRegistry = M_PI * M_E;
        QVERIFY(bundleByTrait.template get<SomeCustomDoubleSetting>() != writtenToRegistry);
        QVERIFY(testedRegistry.template get<SomeCustomDoubleSetting>() != writtenToRegistry);

        // write to registry and check again
        testedRegistry.template set<SomeCustomDoubleSetting>(writtenToRegistry);
        QVERIFY(bundleByTrait.template get<SomeCustomDoubleSetting>() == writtenToRegistry);
        QVERIFY(testedRegistry.template get<SomeCustomDoubleSetting>() == writtenToRegistry);
    }
};

QTEST_MAIN(SettingsRegistryGeneralIT)

#include "SettingsRegistryGeneralIT.moc"
