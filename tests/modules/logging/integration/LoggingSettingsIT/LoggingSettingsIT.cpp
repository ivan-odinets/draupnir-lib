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

#include "draupnir/logging/traits/settings/log_widget/DisplayedMessageCategoriesSettingTrait.h"
#include "draupnir/logging/traits/settings/log_widget/DisplayedMessageLevelsSettingTrait.h"
#include "draupnir/logging/traits/settings/log_widget/DisplayedMessageViewItemFieldsSettingTrait.h"
#include "draupnir/logging/traits/settings/log_widget/IconSizeSetting.h"
#include "draupnir/messages/categories/MessageCategoriesSerializerTemplate.h"
#include "draupnir/messages/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/messages/traits/categories/NetworkMessageCategoryTrait.h"
#include "draupnir/messages/utils/MessageCategoriesSerializer.h"

#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"

using namespace Draupnir::Logging;
using namespace Draupnir::Logging::Settings::LogWidget;
using namespace Draupnir::Messages;
using namespace Draupnir::Settings;

/*! @class LoggingSettingsIT tests/modules/logging/integration/LoggingSettingsIT.cpp
 *  @ingroup LoggingTests
 *  @brief This is an integration test. */

class LoggingSettingsIT final : public QObject
{
    Q_OBJECT
private:
    using SettingsSource = SettingsSourceMockTemplate<
        DisplayedMessageViewItemFieldsSettingTrait,
        DisplayedMessageLevelsSettingTrait,
        DisplayedMessageCategoriesSettingTrait,
        IconSizeSettingTrait
    >;
    SettingsSource registry;

    using MessageCategoriesSerializer = MessageCategoriesSerializerTemplate<
        DefaultMessageCategoryTrait, NetworkMessageCategoryTrait
    >;

private slots:
    void initTestCase() {
        ValueSerializerTemplate<MessageCategories>::registerImplementation(
            new MessageCategoriesSerializer
        );
    }

    void test_message_categories_setting() {
        // Check if individual things are written properly
        registry.set<DisplayedMessageCategoriesSettingTrait>(DefaultMessageCategoryTrait::value());
        QCOMPARE(
            registry.backend()->value(DisplayedMessageCategoriesSettingTrait::key()).toString(),
            DefaultMessageCategoryTrait::configString()
        );

        // Check if All is written properly
        registry.set<DisplayedMessageCategoriesSettingTrait>(MessageCategories::All);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageCategoriesSettingTrait::key()).toString(),
            MessageCategories::allConfigKey()
        );

        // Check if None is written properly
        registry.set<DisplayedMessageCategoriesSettingTrait>(MessageCategories::None);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageCategoriesSettingTrait::key()).toString(),
            MessageCategories::noneConfigKey()
        );
    }

    void test_message_fields_setting() {
        // Check if individual things are written properly
        registry.set<DisplayedMessageViewItemFieldsSettingTrait>(MessageViewItemField::Brief);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageViewItemFieldsSettingTrait::key()).toString(),
            MessageViewItemField::briefConfigToken()
        );
        registry.set<DisplayedMessageViewItemFieldsSettingTrait>(MessageViewItemField::What);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageViewItemFieldsSettingTrait::key()).toString(),
            MessageViewItemField::whatConfigToken()
        );

        // Check if All is written properly
        registry.set<DisplayedMessageViewItemFieldsSettingTrait>(MessageViewItemFields::All);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageViewItemFieldsSettingTrait::key()).toString(),
            MessageViewItemFields::allConfigToken()
        );
    }

    void test_message_levels_setting() {
        // Check if individual things are written properly
        registry.set<DisplayedMessageLevelsSettingTrait>(MessageLevel::Warning);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageLevelsSettingTrait::key()).toString(),
            MessageLevel::warningToken()
        );

        // Check if All written properly
        registry.set<DisplayedMessageLevelsSettingTrait>(MessageLevels::All);
        QCOMPARE(
            registry.backend()->value(DisplayedMessageLevelsSettingTrait::key()).toString(),
            MessageLevels::allToken()
        );
    }

    void test_icon_size_setting() {
        const QSize newSize{42,42};
        QVERIFY(newSize != registry.get<IconSizeSettingTrait>());

        // Check if individual things are written properly
        registry.set<IconSizeSettingTrait>(newSize);
        QCOMPARE(
            registry.backend()->value(IconSizeSettingTrait::key()),
            newSize
        );
    }
};

QTEST_MAIN(LoggingSettingsIT)

#include "LoggingSettingsIT.moc"
