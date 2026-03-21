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
#include <QCoreApplication>

// Mocks
#include "draupnir-test/mocks/MockSettingsTemplate.h"

// SettingsRegistry
#include "draupnir/SettingsRegistry.h"

// SettingsMenu
#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"
#include "draupnir/ui_bricks/utils/MenuEntryToTraitMapper.h"

#include "draupnir-test/helpers/UiTestHelper.h"
#include "draupnir-test/traits/entries/CheckableMenuEntries.h"
#include "draupnir-test/traits/entries/CustomMenuEntries.h"
#include "draupnir-test/traits/settings/BoolSettingTraits.h"
#include "draupnir-test/traits/settings/IntegerSettingTraits.h"
#include "draupnir-test/traits/settings/StringSettingTraits.h"

#include "draupnir/ui_bricks/handlers/settings_menu/SettingsMenuHandlerTemplate.h"

// Checkable entries
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(CheckableMenuEntry,BoolSettingTrait);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(OtherCheckableMenuEntry,OtherBoolSettingTrait);

// Non-Checkable entries
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(SomeMenuEntry,IntSettingTrait);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(SomeOtherMenuEntry,QStringSettingTrait);

/*! @class SettingsMenuEntriesHandlerIT
 *  @brief This is a test class for testing basic functionality of the SettingsMenuEntriesHandler */

class SettingsMenuEntriesHandlerIT : public QObject
{
    Q_OBJECT

public:
    MockSettingsTemplate<
        BoolSettingTrait, OtherBoolSettingTrait, IntSettingTrait, QStringSettingTrait
    > dummySettingsSource;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        BoolSettingTrait, OtherBoolSettingTrait, IntSettingTrait, QStringSettingTrait
    >;
    SettingsRegistry registry;

    using SettingsMenu = Draupnir::Ui::MenuTemplate<
        CheckableMenuEntry, OtherCheckableMenuEntry,
        SomeMenuEntry, SomeOtherMenuEntry
    >;
    SettingsMenu menu;

    using SettingsMenuHandler = Draupnir::Handlers::SettingsMenuHandler<
        SettingsRegistry,
        CheckableMenuEntry, OtherCheckableMenuEntry, SomeMenuEntry, SomeOtherMenuEntry
    >;
    SettingsMenuHandler handler;

    SettingsMenuEntriesHandlerIT() {
        registry.setBackend(&dummySettingsSource);
        handler.connectActions(&menu);
    }

private slots:
    void test_initialization() {
        QAction* action = menu.getUiElement<CheckableMenuEntry>();
        QCOMPARE(action->isChecked(), false);

        QAction* otherAction = menu.getUiElement<OtherCheckableMenuEntry>();
        QCOMPARE(action->isChecked(), false);

        handler.loadSettings(&registry);

        QVERIFY(action->isChecked() == true);
        QVERIFY(otherAction->isChecked() == false);
    }

    void test_actionTriggering() {
        QAction* otherAction = menu.getUiElement<OtherCheckableMenuEntry>();
        QVERIFY(otherAction->isChecked() == registry.template get<OtherBoolSettingTrait>());

        otherAction->triggered(true);
        QVERIFY(registry.template get<OtherBoolSettingTrait>() == true);

        otherAction->triggered(false);
        QVERIFY(registry.template get<OtherBoolSettingTrait>() == false);
    }

    void test_integer_input() {
        QAction* action = menu.getUiElement<SomeMenuEntry>();
        const int oldValue = registry.get<IntSettingTrait>();
        const int newValue = oldValue+10;

        // First lets reject the dialog
        UiTestHelper::scheduleQInputDialogUserInput(newValue,false);
        action->trigger();
        QCOMPARE(registry.get<IntSettingTrait>(),oldValue);

        // And now accept
        UiTestHelper::scheduleQInputDialogUserInput(newValue,true);
        action->trigger();
        QCOMPARE(registry.get<IntSettingTrait>(),newValue);
    }

    void test_qstring_input() {
        QAction* action = menu.getUiElement<SomeOtherMenuEntry>();
        const auto oldValue{registry.get<QStringSettingTrait>()};
        const auto newValue{oldValue+"[UPDATED]"};

        // First lets reject the dialog
        UiTestHelper::scheduleQInputDialogUserInput(newValue,false);
        action->trigger();
        QCOMPARE(registry.get<QStringSettingTrait>(),oldValue);

        // And now accept
        UiTestHelper::scheduleQInputDialogUserInput(newValue,true);
        action->trigger();
        QCOMPARE(registry.get<QStringSettingTrait>(),newValue);
    }
};

QTEST_MAIN(SettingsMenuEntriesHandlerIT)

#include "SettingsMenuEntriesHandlerIT.moc"
