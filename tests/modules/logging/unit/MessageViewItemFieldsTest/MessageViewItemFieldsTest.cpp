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
#include <QDebug>
#include <QCoreApplication>

#include "draupnir/logging/messages/MessageViewItemFields.h"
#include "draupnir/settings_registry/utils/EnumFlagsSerializer.h"

using namespace Draupnir::Logging;

/*! @class MessageViewItemFieldsTest tests/modules/message_system/unit/MessageViewItemFieldsTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageViewItemFields class. */

class MessageViewItemFieldsTest final : public QObject
{
    Q_OBJECT
private:
    // These should work
    static QString briefAndWhatConfigStringKey() { return QStringList{
        MessageViewItemField::briefConfigKey(), MessageViewItemField::whatConfigKey()
    }.join(','); };
    static QString allManuallyConfigStringKey() { return QStringList{
        MessageViewItemField::briefConfigKey(), MessageViewItemField::whatConfigKey(),
        MessageViewItemField::datetimeConfigKey(), MessageViewItemField::iconConfigKey()
    }.join(','); };

    // These should fail
    static QString allAndNoneConfigKey() { return QStringList{
        MessageViewItemFields::allConfigKey(), MessageViewItemFields::noneConfigKey()
    }.join(','); };
    static QString allAndIconConfigKey() { return QStringList{
        MessageViewItemFields::allConfigKey(), MessageViewItemField::iconConfigKey()
    }.join(','); };

    struct MaskParseCase {
        std::optional<MessageViewItemFields> expected;
        QString configString;
    };

    static inline const std::array parseCases{
        // These should work
        MaskParseCase{ MessageViewItemField::Brief, MessageViewItemField::briefConfigKey() },
        MaskParseCase{ MessageViewItemField::What, MessageViewItemField::whatConfigKey() },
        MaskParseCase{ MessageViewItemField::Icon, MessageViewItemField::iconConfigKey() },
        MaskParseCase{ MessageViewItemField::DateTime, MessageViewItemField::datetimeConfigKey() },
        MaskParseCase{ MessageViewItemField::Brief | MessageViewItemField::What, briefAndWhatConfigStringKey() },
        MaskParseCase{ MessageViewItemFields::All, allManuallyConfigStringKey() },
        // These shouuld fail
        MaskParseCase{ std::optional<MessageViewItemFields>{std::nullopt}, allAndNoneConfigKey() },
        MaskParseCase{ std::optional<MessageViewItemFields>{std::nullopt}, allAndIconConfigKey() }
    };

private slots:
    void test_from_config_string() {
        for (const auto& testCase : parseCases) {
            const auto result = MessageViewItemFields::fromConfigString(testCase.configString);
            if (result != testCase.expected)
                qCritical() << "For config string:" << testCase.configString << "received wrong result.";
            QCOMPARE(result, testCase.expected);
        }
    }

    void test_to_config_string() {
        // Individual keys
        QCOMPARE(MessageViewItemFields::toConfigString(MessageViewItemFields::None), MessageViewItemFields::noneConfigKey());
        QCOMPARE(MessageViewItemField::toConfigString(MessageViewItemField::Value::Brief), MessageViewItemField::briefConfigKey());
        QCOMPARE(MessageViewItemField::toConfigString(MessageViewItemField::Value::What), MessageViewItemField::whatConfigKey());
        QCOMPARE(MessageViewItemField::toConfigString(MessageViewItemField::Value::Icon), MessageViewItemField::iconConfigKey());
        QCOMPARE(MessageViewItemField::toConfigString(MessageViewItemField::Value::DateTime), MessageViewItemField::datetimeConfigKey());
        QCOMPARE(MessageViewItemFields::toConfigString(MessageViewItemFields::All), MessageViewItemFields::allConfigKey());

        // Combitations
        QCOMPARE(MessageViewItemFields::toConfigString(MessageViewItemField::Brief | MessageViewItemField::What), briefAndWhatConfigStringKey());
    }

    void test_message_view_item_fields_settings_settings_serializer() {
        // Briefly (not) check conversion from QString to MessageViewItemFields
        using MessageViewItemFieldsSerializer = Draupnir::Settings::EnumFlagsSerializer<MessageViewItemFields>;
        for (const auto& testCase : parseCases) {
            const auto result = MessageViewItemFieldsSerializer::fromConfigString(testCase.configString);
            if (result != testCase.expected)
                qCritical() << "For config string:" << testCase.configString << "received wrong result.";
            QCOMPARE(result, testCase.expected);
        }
    }
};

QTEST_MAIN(MessageViewItemFieldsTest)

#include "MessageViewItemFieldsTest.moc"
