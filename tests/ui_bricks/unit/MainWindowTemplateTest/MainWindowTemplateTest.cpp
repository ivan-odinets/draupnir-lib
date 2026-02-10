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

#include <QCoreApplication>
#include <QtTest>

#include "draupnir/SettingsRegistry.h" // IWYU pragma: keep
#include "draupnir/UiBricks.h"         // IWYU pragma: keep

#include "draupnir-test/mocks/MockSettingsTemplate.h"

/*! @class MainWindowTemplateTest tests/ui_bricks/unit/MainWindowTemplateTest/MainWindowTemplateTest.cpp
 *  @brief This test class tests basic functionality of the @ref Draupnir::Ui::MainWindowTemplate. */

class MainWindowTemplateTest final : public QObject
{
    Q_OBJECT
public:
    MockSettingsTemplate<
        Draupnir::Settings::MainWindow::MinimizeOnCloseSetting,
        Draupnir::Settings::MainWindow::MinimizeToTraySetting,
        Draupnir::Settings::MainWindow::WindowSizeSetting
    > mockSettingsBackend;

    Draupnir::Settings::SettingsRegistryTemplate<
        Draupnir::Settings::MainWindow::MinimizeOnCloseSetting,
        Draupnir::Settings::MainWindow::MinimizeToTraySetting,
        Draupnir::Settings::MainWindow::WindowSizeSetting
    > registry;

    Draupnir::Ui::TrayIcon trayIcon;

    const QSize defaultWindowSize = QSize{100,100};

private slots:
    void initTestCase() { registry.setBackend(&mockSettingsBackend); }

    void init() {
        registry.set<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>(false);
        registry.set<Draupnir::Settings::MainWindow::MinimizeToTraySetting>(false);
        registry.set<Draupnir::Settings::MainWindow::WindowSizeSetting>(defaultWindowSize);
    }

    void test_window_resize() {
        auto* window = new Draupnir::Ui::MainWindowTemplate<
            Draupnir::Ui::MainWindow::RememberWindowSize
        >;
        window->setAttribute(Qt::WA_DontShowOnScreen);
        window->loadSettings(&registry);

        // Show window
        window->show();
        QVERIFY(window->isVisible());

        // We have proper size of the MainWindowTemplate
        QCOMPARE(window->size(), defaultWindowSize);

        // Resize window and check if indeed settings are updated
        const QSize newSize{42,42};
        window->resize(newSize);
        QCOMPARE(window->size(), newSize);

        // Delete the window to save settings
        delete window;
        // Check if updated
        QCOMPARE(registry.get<Draupnir::Settings::MainWindow::WindowSizeSetting>(), newSize);
        QCOMPARE(mockSettingsBackend.get<Draupnir::Settings::MainWindow::WindowSizeSetting>(), newSize);
    }

    void test_window_minimize_on_close() {
        auto* window = new Draupnir::Ui::MainWindowTemplate<
            Draupnir::Ui::MainWindow::MinimizableOnClose
        >;
        window->setAttribute(Qt::WA_DontShowOnScreen);
        window->loadSettings(&registry);
        QVERIFY(window->minimizeOnClose() == false);

        // Show window
        window->show();
        QVERIFY(window->isVisible());

        // Close window w/o MinimizeOnClose
        QCOMPARE(window->close(), true);
        QCOMPARE(window->isMinimized(), false);
        QCOMPARE(window->isVisible(), false);

        // Change MinimizeOnClose state
        window->show();
        window->setMinimizeOnClose(true);
        QVERIFY(window->isVisible());

        // Try closing again
        QCOMPARE(window->close(), false);
        QCOMPARE(window->isVisible(), true);
        QCOMPARE(window->isMinimized(), true);

        // Cleanup
        delete window;
    }

    void test_window_minimize_to_tray() {
        auto* window = new Draupnir::Ui::MainWindowTemplate<
            Draupnir::Ui::MainWindow::MinimizableToTray,
            Draupnir::Ui::MainWindow::UseTrayIcon<>
        >;
        window->setAttribute(Qt::WA_DontShowOnScreen);
        window->loadSettings(&registry);
        QVERIFY(window->minimizeToTray() == false);

        // Show window
        window->show();
        QVERIFY(window->isVisible());

        // Try minimizing window
        window->setWindowState(Qt::WindowState::WindowMinimized);
        // And check if we minimized
        QCOMPARE(window->isVisible(), true);
        QCOMPARE(window->isMinimized(), true);

        // Maximise, and change settings
        window->setWindowState(window->windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
        window->setMinimizeToTray(true);

        // Try minimizing again
        window->setWindowState(Qt::WindowState::WindowMinimized);
        // Now we are hidden
        QCOMPARE(window->isVisible(), false);
        QCOMPARE(window->isMinimized(), false);

        delete window;
    }

    void test_full_featured_window() {
        Draupnir::Ui::MainWindowTemplate<
            Draupnir::Ui::MainWindow::MinimizableOnClose,
            Draupnir::Ui::MainWindow::MinimizableToTray,
            Draupnir::Ui::MainWindow::RememberWindowSize,
            Draupnir::Ui::MainWindow::UseTrayIcon<Draupnir::Ui::TrayIcon>
        > window;
        window.setAttribute(Qt::WA_DontShowOnScreen);
        window.loadSettings(&registry);
        window.registerTrayIcon(&trayIcon);

        window.show();
        QVERIFY(window.isVisible());
    }
};

QTEST_MAIN(MainWindowTemplateTest)

#include "MainWindowTemplateTest.moc"
