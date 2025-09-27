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
#include "draupnir/SettingsRegistryTemplate.h"
#include "draupnir/traits/settings/MinimizeOnCloseSetting.h"
#include "draupnir/traits/settings/MinimizeToTraySetting.h"

// SettingsMenu
#include "draupnir/traits/entries/SettingsMenuEntries.h"
#include "draupnir/ui/menus/MenuTemplate.h"

// FileHandlers
#include "draupnir/handlers/settings_menu/SettingsMenuEntriesHandler.h"
#include "draupnir/handlers/settings_menu/MinimizeOnCloseEntryHandler.h"
#include "draupnir/handlers/settings_menu/MinimizeToTrayEntryHandler.h"

// Test things
#include "draupnir-test/handlers/settings_menu/SomeCheckableSettingsEntryHandler.h"
#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/entries/SomeCheckableMenuEntry.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

/*! @class SettingsMenuEntriesHandlerGeneralIT
 *  @headerfile tests/handler_templates/SettingsMenuEntriesHandler/GeneralIT/SettingsMenuEntriesHandlerGeneralIT.cpp
 *  @brief This is a test class for testing basic functionality of the SettingsMenuEntriesHandler */

class SettingsMenuEntriesHandlerGeneralIT : public QObject
{
    Q_OBJECT

public:
    MockSettingsTemplate<
        SomeCustomBoolSetting,
        Draupnir::Settings::MinimizeOnCloseSetting, Draupnir::Settings::MinimizeToTraySetting
    > dummySettingsSource;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        SomeCustomBoolSetting,
        Draupnir::Settings::MinimizeOnCloseSetting, Draupnir::Settings::MinimizeToTraySetting
    >;
    SettingsRegistry registry;

    using SettingsMenu = Draupnir::Menus::MenuTemplate<
        SomeCustomCheckableMenuEntry,
        Draupnir::Menus::MinimizeOnCloseEntry, Draupnir::Menus::MinimizeToTrayEntry
    >;
    SettingsMenu menu;

    using SettingsMenuHandler = Draupnir::Handlers::SettingsMenuEntriesHandler<
        SettingsRegistry,
        SomeCustomCheckableMenuEntry,
        Draupnir::Menus::MinimizeOnCloseEntry, Draupnir::Menus::MinimizeToTrayEntry
    >;
    SettingsMenuHandler handler;

    SettingsMenuEntriesHandlerGeneralIT() {
        registry.setBackend(&dummySettingsSource);
        handler.connectActions(&menu);
    }

private slots:

    void test_initialization() {
        QAction* customAction = menu.get<SomeCustomCheckableMenuEntry>();
        QVERIFY(customAction->isChecked() == false);

        QAction* minimizeOnCloseAction = menu.get<Draupnir::Menus::MinimizeOnCloseEntry>();
        QVERIFY(minimizeOnCloseAction->isChecked() == false);

        handler.setRegistry(&registry);

        QVERIFY(customAction->isChecked() == true);
        QVERIFY(minimizeOnCloseAction->isChecked() == false);
    }

    void test_actionTriggering() {
        QAction* minimizeOnCloseAction = menu.get<Draupnir::Menus::MinimizeOnCloseEntry>();
        QVERIFY(minimizeOnCloseAction->isChecked() == registry.template get<Draupnir::Settings::MinimizeOnCloseSetting>());

        minimizeOnCloseAction->triggered(true);
        QVERIFY(registry.template get<Draupnir::Settings::MinimizeOnCloseSetting>() == true);

        minimizeOnCloseAction->triggered(false);
        QVERIFY(registry.template get<Draupnir::Settings::MinimizeOnCloseSetting>() == false);
    }

};

QTEST_MAIN(SettingsMenuEntriesHandlerGeneralIT)

#include "SettingsMenuEntriesHandlerGeneralIT.moc"
