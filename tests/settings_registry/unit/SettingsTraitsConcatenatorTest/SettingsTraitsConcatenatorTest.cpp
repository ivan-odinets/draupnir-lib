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

#include "draupnir/settings_registry/traits/settings/CentralWidgetIndexSetting.h"
#include "draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h"
#include "draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h"
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"

#include "draupnir-test/traits/settings/SomeCustomDoubleSetting.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

class ClassWithBundle
{
public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomBoolSetting, Draupnir::Settings::LastUsedDirectorySetting
    >;
};

class OtherClassWithBundle
{
public:
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        SomeCustomDoubleSetting, Draupnir::Settings::LastUsedDirectorySetting,
        Draupnir::Settings::RecentFileListSetting
    >;
};

class ClassWithoutBundle
{
public:
    using NotBundle = void;
};

namespace Draupnir::Settings
{

/*! @class SettingsTraitsConcatenatorTest tests/settings_registry/unit/SettingsTraitsConcatenatorTest/SettingsTraitsConcatenatorTest.cpp
 *  @brief This is a test for SettingsTraitsConcatenator */

class SettingsTraitsConcatenatorTest final : public QObject
{
    Q_OBJECT
public:

private slots:
    void test_complex_bundle_building() {
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            Draupnir::Settings::SettingsBundleTemplate<
                Draupnir::Settings::CentralWidgetIndexSetting,
                Draupnir::Settings::LastUsedDirectorySetting,
                SomeCustomDoubleSetting,
                SomeCustomBoolSetting
            >,
            Draupnir::Settings::RecentFileListSetting/*,
            SettingsBundleTemplate<
                SettingsBundleTemplate<>,
                Draupnir::Settings::SettingsBundleTemplate<
                    SomeCustomDoubleSetting,
                    SomeCustomBoolSetting,
                    Draupnir::Settings::CentralWidgetIndexSetting
                >,
                Draupnir::Settings::CentralWidgetIndexSetting
            >*/
        >::toSettingsBundle;

        QCOMPARE(Result::traitCount(), std::size_t{5});
        QCOMPARE(Result::contains<SomeCustomBoolSetting>(), true);
        QCOMPARE(Result::contains<SomeCustomDoubleSetting>(), true);
        QCOMPARE(Result::contains<Draupnir::Settings::CentralWidgetIndexSetting>(), true);
        QCOMPARE(Result::contains<Draupnir::Settings::RecentFileListSetting>(), true);
        QCOMPARE(Result::contains<Draupnir::Settings::LastUsedDirectorySetting>(), true);
    }

    void test_empty_bundle_merge() {
        // And from Empty Bundles we should get empty bundles...
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            Draupnir::Settings::SettingsBundleTemplate<>,
            Draupnir::Settings::SettingsBundleTemplate<>,
            Draupnir::Settings::SettingsBundleTemplate<
                // Draupnir::Settings::SettingsBundleTemplate<>,
                // Draupnir::Settings::SettingsBundleTemplate<
                //     Draupnir::Settings::SettingsBundleTemplate<>
                // >,
                //Draupnir::Settings::SettingsBundleTemplate<>
            >
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
        QCOMPARE(Result::contains_v<SomeCustomBoolSetting>, true);
        QCOMPARE(Result::contains_v<SomeCustomDoubleSetting>, true);
        QCOMPARE(Result::contains_v<Draupnir::Settings::LastUsedDirectorySetting>, true);
        QCOMPARE(Result::contains_v<Draupnir::Settings::RecentFileListSetting>, true);
    }

    void test_mergint_into_registry() {
        using Result = Draupnir::Settings::SettingsTraitsConcatenator<
            ClassWithBundle,
            ClassWithoutBundle,
            Draupnir::Settings::SettingsBundleTemplate<>,
            OtherClassWithBundle,
            Draupnir::Settings::CentralWidgetIndexSetting
        >::toSettingsRegistry;

        QCOMPARE(Result::contains<SomeCustomBoolSetting>(),true);
        QCOMPARE(Result::contains<SomeCustomDoubleSetting>(),true);
        QCOMPARE(Result::contains<Draupnir::Settings::LastUsedDirectorySetting>(),true);
        QCOMPARE(Result::contains<Draupnir::Settings::CentralWidgetIndexSetting>(),true);
        QCOMPARE(Result::contains<Draupnir::Settings::RecentFileListSetting>(),true);
    }
};

}; // namespace Draupnir::Settings

QTEST_MAIN(Draupnir::Settings::SettingsTraitsConcatenatorTest)

#include "SettingsTraitsConcatenatorTest.moc"
