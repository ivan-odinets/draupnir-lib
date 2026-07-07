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

#include <QtTest>

#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"

#include "draupnir/logging/ui/widgets/LogWidget.h"
#include "draupnir/logging/ui/widgets/MessageListView.h"
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"

namespace Draupnir::Logging {

/*! @class LogWidgetTest tests/modules/logging/unit/LogWidgetTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::LogWidget class. */

class LogWidgetTest final : public QObject
{
    Q_OBJECT
private:
    using SettingsSource = Draupnir::Settings::SettingsTraitsConcatenator<
         LogWidget::SettingsBundle
    >::to<Draupnir::Settings::SettingsSourceMockTemplate>;
    SettingsSource settingsSource;

    LogWidget* widget = nullptr;

private slots:
    void init() { widget = new LogWidget; }
    void cleanup() { delete widget; widget = nullptr; }

    void test_icon_size_behaviour() {
        const QSize oldSize = settingsSource.template get<Draupnir::Logging::Settings::LogWidget::IconSizeSetting>();
        const QSize newSize{42,42};
        QVERIFY(widget->w_messagesListView->iconSize() != newSize);
        settingsSource.template set<Draupnir::Logging::Settings::LogWidget::IconSizeSetting>(newSize);
        // Load settings
        widget->loadSettings(&settingsSource);
        // Check if icon size is loaded
        QCOMPARE(widget->w_messagesListView->iconSize(), newSize);
        QCOMPARE(widget->w_iconSizeSlider->value(), newSize.width());

        // Restore state
        settingsSource.template set<Draupnir::Logging::Settings::LogWidget::IconSizeSetting>(oldSize);
    }
};

} // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::LogWidgetTest)

#include "LogWidgetTest.moc"
