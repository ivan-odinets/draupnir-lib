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

// Mocks
#include "draupnir-test/mocks/MockSettingsTemplate.h"

// SettingsRegistry
#include "draupnir/SettingsRegistry.h"

// SettingsMenu
#include "draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h"
#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"

#include "draupnir/HandlerTemplates.h"

// Test things
#include "draupnir-test/handlers/settings_menu/SomeCheckableSettingsEntryHandler.h"
#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/entries/SomeCheckableMenuEntry.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

/*! @class SettingsMenuEntriesHandlerGeneralIT
 *  @headerfile tests/handler_templates/SettingsMenuEntriesHandler/GeneralIT/SettingsMenuEntriesHandlerGeneralIT.cpp
 *  @brief This is a test class for testing basic functionality of the SettingsMenuEntriesHandler
 *
 * @todo Refractor this test so that it will have better readability. */

class SettingsMenuEntriesHandlerIT : public QObject
{
    Q_OBJECT

public:
    MockSettingsTemplate<
        SomeCustomBoolSetting,
        Draupnir::Settings::MainWindow::MinimizeOnCloseSetting,
        Draupnir::Settings::MainWindow::MinimizeToTraySetting
    > dummySettingsSource;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        SomeCustomBoolSetting,
        Draupnir::Settings::MainWindow::MinimizeOnCloseSetting,
        Draupnir::Settings::MainWindow::MinimizeToTraySetting,
        Draupnir::Settings::MainWindow::StartHiddenSetting
    >;
    SettingsRegistry registry;

    using SettingsMenu = Draupnir::Ui::MenuTemplate<
        SomeCustomCheckableMenuEntry,
        Draupnir::Ui::MinimizeOnCloseEntry,
        Draupnir::Ui::MinimizeToTrayEntry,
        Draupnir::Ui::StartHiddenMenuEntry
    >;
    SettingsMenu menu;

    using SettingsMenuHandler = Draupnir::Handlers::SettingsMenuEntriesHandler<
        SettingsRegistry,
        SomeCustomCheckableMenuEntry,
        Draupnir::Ui::StartHiddenMenuEntry,
        Draupnir::Ui::MinimizeOnCloseEntry,
        Draupnir::Ui::MinimizeToTrayEntry
    >;
    SettingsMenuHandler handler;

    SettingsMenuEntriesHandlerIT() {
        registry.setBackend(&dummySettingsSource);
        handler.connectActions(&menu);
    }

private slots:

    void test_initialization() {
        QAction* customAction = menu.get<SomeCustomCheckableMenuEntry>();
        QCOMPARE(customAction->isChecked(), false);

        QAction* minimizeOnCloseAction = menu.get<Draupnir::Ui::MinimizeOnCloseEntry>();
        QCOMPARE(minimizeOnCloseAction->isChecked(), false);

        handler.setRegistry(&registry);

        QVERIFY(customAction->isChecked() == true);
        QVERIFY(minimizeOnCloseAction->isChecked() == false);
    }

    void test_actionTriggering() {
        QAction* minimizeOnCloseAction = menu.get<Draupnir::Ui::MinimizeOnCloseEntry>();
        QVERIFY(minimizeOnCloseAction->isChecked() == registry.template get<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>());

        minimizeOnCloseAction->triggered(true);
        QVERIFY(registry.template get<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>() == true);

        minimizeOnCloseAction->triggered(false);
        QVERIFY(registry.template get<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>() == false);
    }

};

QTEST_MAIN(SettingsMenuEntriesHandlerIT)

#include "SettingsMenuEntriesHandlerIT.moc"
