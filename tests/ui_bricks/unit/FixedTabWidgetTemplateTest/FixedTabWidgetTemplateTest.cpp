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

#include "draupnir/SettingsRegistry.h"

#include "draupnir/settings_registry/traits/settings/CentralWidgetIndexSetting.h"

#include "draupnir/ui_bricks/ui/widgets/FixedCentralTabWidgetTemplate.h"
#include "draupnir/ui_bricks/ui/widgets/FixedTabWidgetTemplate.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/settings/SomeRandomWidgetIndexSetting.h"
#include "draupnir-test/traits/widgets/SomeRandomWidgetTabTraits.h"

/*! @class FixedTabWidgetTemplateTest tests/ui_bricks/unit/FixedTabWidgetTemplateTest/FixedTabWidgetTemplateTest.cpp
 *  @brief This test class tests basic functionality of the FixedTabWidgetTemplate.
 *
 * @todo Refractor this test so that it will have better readability. */

class FixedTabWidgetTemplateTest final : public QObject
{
    Q_OBJECT

public:
    using MockBackend = MockSettingsTemplate<
        Draupnir::Settings::CentralWidgetIndexSetting,
        SomeRandomWidgetIndexSetting
    >;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        Draupnir::Settings::CentralWidgetIndexSetting,
        SomeRandomWidgetIndexSetting
    >;

    using TabWidgetOne = Draupnir::Ui::FixedCentralTabWidgetTemplate<
        LineEditTrait,
        PushButtonTrait
    >;

    using TabWidgetWithSetting = Draupnir::Ui::FixedTabWidgetTemplate<
        SomeRandomWidgetIndexSetting,
        LineEditTrait,
        PushButtonTrait
    >;

    using TabWidgetWithoutSetting =Draupnir::Ui::FixedTabWidgetTemplate<
        void,
        LineEditTrait,
        PushButtonTrait
    >;

    MockBackend backend;
    SettingsRegistry registry;

    // Init internal fields
    FixedTabWidgetTemplateTest() {
        registry.setBackend(&backend);
    }

    ~FixedTabWidgetTemplateTest() final = default;

private slots:

    void test_initialization() {
        auto tabWidgetOne = new TabWidgetOne;
        tabWidgetOne->template loadSettings<SettingsRegistry>(&registry);

        QCOMPARE(tabWidgetOne->count(), 2);

        QVERIFY(tabWidgetOne->getWidgetByIndex<QLineEdit>(0) != nullptr);
        QVERIFY(tabWidgetOne->getWidgetByIndex<QPushButton>(1) != nullptr);

        QCOMPARE(tabWidgetOne->tabText(0), LineEditTrait::displayName());
        QCOMPARE(tabWidgetOne->tabText(1), PushButtonTrait::displayName());

        QCOMPARE(tabWidgetOne->tabToolTip(0), QString{});
        QCOMPARE(tabWidgetOne->tabToolTip(1), PushButtonTrait::tooltip());

        delete tabWidgetOne;
    };

    void test_settings_loading() {
        TabWidgetOne* oneMoreWidget = new TabWidgetOne;
        // Test 0
        registry.set<Draupnir::Settings::CentralWidgetIndexSetting>(0);
        oneMoreWidget->loadSettings<SettingsRegistry>(&registry);
        QCOMPARE(oneMoreWidget->currentIndex(), 0);

        // Reset Widget
        delete oneMoreWidget;
        oneMoreWidget = new TabWidgetOne;

        // Test 1
        registry.set<Draupnir::Settings::CentralWidgetIndexSetting>(1);
        oneMoreWidget->loadSettings<SettingsRegistry>(&registry);
        QCOMPARE(oneMoreWidget->currentIndex(), 1);

        // Reset Widget
        delete oneMoreWidget;
        oneMoreWidget = new TabWidgetOne;

        // Test invalid thing.
        registry.set<Draupnir::Settings::CentralWidgetIndexSetting>(42);
        oneMoreWidget->loadSettings<SettingsRegistry>(&registry);
        QCOMPARE(oneMoreWidget->currentIndex(), Draupnir::Settings::CentralWidgetIndexSetting::defaultValue());

        delete oneMoreWidget;
    }

    void test_widget_injecting() {
        QPushButton* button = new QPushButton;
        TabWidgetOne* oneMoreWidget = new TabWidgetOne{
            nullptr, // parent
            nullptr, // first tab (QLineEdit)
            button   // second tab (QPushButton)
        };

        // Widget is correctly initialized
        QCOMPARE(oneMoreWidget->count(), 1);
        QCOMPARE(oneMoreWidget->getWidget<QLineEdit>(), nullptr);
        QCOMPARE(oneMoreWidget->getWidget<QPushButton>(), button);
        QCOMPARE(oneMoreWidget->tabText(0), PushButtonTrait::displayName());
        QCOMPARE(oneMoreWidget->tabToolTip(0),PushButtonTrait::tooltip());

        // Create line edit
        QLineEdit* lineEdit = new QLineEdit;
        oneMoreWidget->setWidget<LineEditTrait>(lineEdit);
        // Widget was inserted at proper location
        QCOMPARE(oneMoreWidget->count(), 2);
        QCOMPARE(oneMoreWidget->getWidget<QLineEdit>(), lineEdit);
        QCOMPARE(oneMoreWidget->getWidget<QPushButton>(), button);
        QCOMPARE(oneMoreWidget->tabText(0), LineEditTrait::displayName());
        QCOMPARE(oneMoreWidget->tabToolTip(0), QString{});
        QCOMPARE(oneMoreWidget->tabText(1), PushButtonTrait::displayName());
        QCOMPARE(oneMoreWidget->tabToolTip(1),PushButtonTrait::tooltip());

        // Create QPushButton replacement
        QPushButton* newButton = new QPushButton;
        QPointer<QPushButton> pButton{button};
        QVERIFY(!pButton.isNull());

        // Replace QPushButton
        oneMoreWidget->setWidget<PushButtonTrait>(newButton);
        // Old widget was deleted
        QVERIFY(pButton.isNull());
        // Widget was inserted at proper location
        QCOMPARE(oneMoreWidget->count(), 2);
        QCOMPARE(oneMoreWidget->getWidget<QLineEdit>(), lineEdit);
        QCOMPARE(oneMoreWidget->getWidget<QPushButton>(), newButton);
        QCOMPARE(oneMoreWidget->tabText(0), LineEditTrait::displayName());
        QCOMPARE(oneMoreWidget->tabToolTip(0), QString{});
        QCOMPARE(oneMoreWidget->tabText(1), PushButtonTrait::displayName());
        QCOMPARE(oneMoreWidget->tabToolTip(1),PushButtonTrait::tooltip());
    }

    void test_widget_wtithout_settings() {
        auto tabWidgetWithoutSettings = new TabWidgetWithoutSetting;

        QCOMPARE(tabWidgetWithoutSettings->currentIndex(), 0);

        delete tabWidgetWithoutSettings;
    }
};

QTEST_MAIN(FixedTabWidgetTemplateTest)

#include "FixedTabWidgetTemplateTest.moc"
