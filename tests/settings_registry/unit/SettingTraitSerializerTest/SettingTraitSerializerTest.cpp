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

#include "draupnir/settings_registry/utils/SettingTraitSerializer.h"
#include "draupnir/settings_registry/traits/settings/files/LastUsedDirectorySetting.h"
#include "draupnir/settings_registry/traits/settings/files/RecentFilesListSetting.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/settings/SomeCustomDoubleSetting.h"

/*! @class SettingTraitSerializerTest tests/settings_registry/unit/SettingTraitSerializerTest/SettingTraitSerializerTest.cpp
 *  @brief This test class tests functionality of the SettingTraitSerializer.
 *
 * @todo Add custom complex setting trait support. */

class SettingTraitSerializerTest final : public QObject
{
    Q_OBJECT

public:
    using MockBackend = MockSettingsTemplate<
        Draupnir::Settings::LastUsedDirectorySetting,
        Draupnir::Settings::RecentFileListSetting
    >;

    using QStringSerializer = Draupnir::Settings::SettingTraitSerializer<
        MockBackend,
        Draupnir::Settings::LastUsedDirectorySetting
    >;
    QString dummyString{"I Am The String"};

    using QStringListSerializer = Draupnir::Settings::SettingTraitSerializer<
        MockBackend,
        Draupnir::Settings::RecentFileListSetting
    >;
    QStringList dummyStringList{
        "/etc/passwd",
        "/def/urandom",
        "/dev/ass"
    };

    MockBackend mockBackend;

    SettingTraitSerializerTest() = default;
    ~SettingTraitSerializerTest() = default;

private slots:
    void test_get_default_value() {
        using TestSerializer = Draupnir::Settings::SettingTraitSerializer<
            MockBackend,
            SomeCustomDoubleSetting
        >;

        QCOMPARE(TestSerializer::get(&mockBackend), SomeCustomDoubleSetting::defaultValue());
    }

    void test_set_and_get() {
        QStringSerializer::set(&mockBackend,dummyString);
        QCOMPARE(QStringSerializer::get(&mockBackend), dummyString);

        QStringListSerializer::set(&mockBackend, dummyStringList);
        QCOMPARE(QStringListSerializer::get(&mockBackend), dummyStringList);
    }
};

QTEST_MAIN(SettingTraitSerializerTest)

#include "SettingTraitSerializerTest.moc"
