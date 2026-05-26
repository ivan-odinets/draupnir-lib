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

#include "draupnir/ui_bricks/core/ViewManager.h"

#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"
#include "draupnir-test/mocks/MockSomeMainWindow.h"
#include "draupnir-test/mocks/MockSomeTrayIcon.h"

/*! @class ViewManagerTest tests/modules/ui_bricks/unit/ViewManagerTest/ViewManagerTest.cpp
 *  @brief This test class tests basic functionality of the ViewManager. */

class ViewManagerTest : public QObject
{
    Q_OBJECT

public:
    // Settings, mocked
    using SettingsSource = Draupnir::Settings::SettingsSourceMockTemplate<
        Draupnir::Settings::MainWindow::StartHiddenSetting
    >;
    SettingsSource settings;

    Draupnir::Ui::ViewManager<
        SomeMainWindow, SomeTrayIcon
    > viewManager;

private slots:
    void initTestCase() { viewManager.loadSettings(&settings); }

    /*! @brief Tests if default settings were properly initialized. */
    void test_initialization() {
        Draupnir::Ui::ViewManager<SomeMainWindow,SomeTrayIcon> otherViewManager{};

        QCOMPARE(otherViewManager.mainWindow(), nullptr);
        QCOMPARE(otherViewManager.trayIcon(), nullptr);

        QCOMPARE(otherViewManager.getElement<SomeMainWindow>(), nullptr);
        QCOMPARE(otherViewManager.getElement<SomeTrayIcon>(), nullptr);
    }

    void test_settingsLoading() {
        QCOMPARE(viewManager.startHidden(), settings.get<Draupnir::Settings::MainWindow::StartHiddenSetting>());

        viewManager.setStartHidden(true);
        QCOMPARE(viewManager.startHidden(), settings.get<Draupnir::Settings::MainWindow::StartHiddenSetting>());
        QCOMPARE(viewManager.startHidden(), true);

        settings.set<Draupnir::Settings::MainWindow::StartHiddenSetting>(false);
        QCOMPARE(viewManager.startHidden(), settings.get<Draupnir::Settings::MainWindow::StartHiddenSetting>());
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
        otherViewManager.loadSettings(&settings);
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
