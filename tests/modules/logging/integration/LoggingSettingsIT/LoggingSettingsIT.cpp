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

#include "draupnir/logging/messages/categories/MessageCategoryRegistryTemplate.h"
#include "draupnir/logging/traits/settings/LogWidgetSettingsTraits.h"
#include "draupnir/logging/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/logging/traits/categories/NetworkMessageCategoryTrait.h"

#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"

namespace Draupnir::Logging
{

/*! @class LoggingSettingsIT tests/modules/logging/integration/LoggingSettingsIT.cpp
 *  @ingroup LoggingTests
 *  @brief This is an integration test. */

class LoggingSettingsIT final : public QObject
{
    Q_OBJECT
private:
    using SettingsSource = Draupnir::Settings::SettingsSourceMockTemplate<
        Settings::LogWidget::DisplayedMessageViewItemFieldsSetting,
        Settings::LogWidget::DisplayedMessageLevelsSetting,
        Settings::LogWidget::DisplayedMessageCategoriesSetting
    >;
    SettingsSource registry;

    using MessageCategoriesRegistry = MessageCategoryRegistryTemplate<
        DefaultMessageCategoryTrait, NetworkMessageCategoryTrait
    >;

private slots:
    void initTestCase() {
        Draupnir::Settings::ValueSerializerTemplate<MessageCategories>::setMessageCategoriesSerializerInterface(
            new MessageCategoriesRegistry
        );
    }

    void test_message_categories_setting() {
        // Check if All is written properly
        registry.set<Settings::LogWidget::DisplayedMessageCategoriesSetting>(MessageCategories::All);
        QCOMPARE(registry.backend()->value(Settings::LogWidget::DisplayedMessageCategoriesSetting::key()).toString(),
                 MessageCategories::allConfigKey());
        // Check if None is written properly
        registry.set<Settings::LogWidget::DisplayedMessageCategoriesSetting>(MessageCategories::None);
        QCOMPARE(registry.backend()->value(Settings::LogWidget::DisplayedMessageCategoriesSetting::key()).toString(),
                 MessageCategories::noneConfigKey());
    }
};

}; // namespace Draupnir::Logging

QTEST_MAIN(Draupnir::Logging::LoggingSettingsIT)

#include "LoggingSettingsIT.moc"
