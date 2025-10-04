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
#include "draupnir/core/ViewManager.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/mocks/MockSomeMainWindow.h"
#include "draupnir-test/mocks/MockSomeTrayIcon.h"

/*! @class ViewManagerTest tests/ui_bricks/ViewManagerTest/ViewManagerTest.cpp
 *  @brief This test class tests basic functionality of the ViewManager. */

class ViewManagerTest : public QObject
{
    Q_OBJECT

public:
    // Settings, mocked
    using MockSettingsBackend = MockSettingsTemplate<
        Draupnir::Settings::StartHiddenSetting
    >;
    MockSettingsBackend mockedSettings;

    // We will need SettingsResitryTemplate as well
    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        Draupnir::Settings::StartHiddenSetting
    >;
    SettingsRegistry registry;

    Draupnir::Ui::ViewManager<
        SomeMainWindow, SomeTrayIcon
    > viewManager;

    // Init internal fields
    ViewManagerTest() :
        viewManager{}
    {
        registry.setBackend(&mockedSettings);
        viewManager.loadSettings(&registry);
    }

    ~ViewManagerTest() = default;

private slots:
    /*! @brief Tests if default settings were properly initialized. */
    void test_initialization() {
        Draupnir::Ui::ViewManager<SomeMainWindow,SomeTrayIcon> otherViewManager{};

        QCOMPARE(otherViewManager.mainWindow(), nullptr);
        QCOMPARE(otherViewManager.trayIcon(), nullptr);

        QCOMPARE(otherViewManager.getElement<SomeMainWindow>(), nullptr);
        QCOMPARE(otherViewManager.getElement<SomeTrayIcon>(), nullptr);
    }

    void test_settingsLoading() {
        QCOMPARE(viewManager.startHidden(), mockedSettings.get<Draupnir::Settings::StartHiddenSetting>());

        viewManager.setStartHidden(true);
        QCOMPARE(viewManager.startHidden(), mockedSettings.get<Draupnir::Settings::StartHiddenSetting>());
        QCOMPARE(viewManager.startHidden(), true);

        registry.set<Draupnir::Settings::StartHiddenSetting>(false);
        QCOMPARE(viewManager.startHidden(), mockedSettings.get<Draupnir::Settings::StartHiddenSetting>());
        QCOMPARE(viewManager.startHidden(), false);
    }

    void test_uiDisplaying() {
        viewManager.createUi();
        viewManager.mainWindow()->hide();
        viewManager.trayIcon()->hide();
        viewManager.setStartHidden(false);

        viewManager.showUi();
        QCOMPARE(viewManager.mainWindow()->isShown(), true);
        QCOMPARE(viewManager.trayIcon()->isShown(), true);

        viewManager.mainWindow()->hide();
        viewManager.trayIcon()->hide();
        viewManager.setStartHidden(true);

        viewManager.showUi();
        QCOMPARE(viewManager.mainWindow()->isShown(), false);
        QCOMPARE(viewManager.trayIcon()->isShown(), true);
    }

    void test_externalComponentCreation() {
        Draupnir::Ui::ViewManager<SomeMainWindow,SomeTrayIcon> otherViewManager;
        otherViewManager.loadSettings(&registry);
        SomeMainWindow* window = new SomeMainWindow;
        window->hide();
        SomeTrayIcon* trayIcon = new SomeTrayIcon;
        trayIcon->hide();

        otherViewManager.setMainWindow(window);
        otherViewManager.setTrayIcon(trayIcon);
        otherViewManager.setStartHidden(false);
        otherViewManager.showUi();

        QCOMPARE(otherViewManager.mainWindow(), window);
        QCOMPARE(otherViewManager.trayIcon(), trayIcon);
        QCOMPARE(window->isShown(), true);
        QCOMPARE(trayIcon->isShown(), true);
    }
};

QTEST_MAIN(ViewManagerTest)

#include "ViewManagerTest.moc"
