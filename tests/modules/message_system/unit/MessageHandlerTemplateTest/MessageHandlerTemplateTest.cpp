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

#include "draupnir/message_system/core/MessageHandlerTemplate.h"

#include "draupnir/settings_registry/SettingsRegistryTemplate.h"
#include "draupnir/message_system/traits/messages/DefaultMessageTraits.h"
#include "draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h"
#include "draupnir/message_system/models/MessageListModel.h"
#include "draupnir/message_system/ui/windows/MessageDisplayDialog.h"

#include "draupnir-test/helpers/UiTestHelper.h"
#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/messages/CustomMessageTrait.h"

namespace Draupnir::Messages
{

/*! @class MessageHandlerTemplateTest tests/modules/message_system/unit/MessageHandlerTemplateTest.cpp
 *  @brief This test class tests MessageHandlerTemplate class. */

class MessageHandlerTemplateTest final : public QObject
{
    Q_OBJECT
public:
    // Settings, mocked
    using MockSettingsBackend = MockSettingsTemplate<
        MessageTypeSettingsTrait<DebugMessageTrait>,
        MessageTypeSettingsTrait<InfoMessageTrait>,
        MessageTypeSettingsTrait<WarningMessageTrait>,
        MessageTypeSettingsTrait<ErrorMessageTrait>,
        MessageTypeSettingsTrait<CustomMessageTrait>
    >;
    MockSettingsBackend mockedSettings;

    using SettingsRegistry = Settings::SettingsRegistryTemplate<
        MessageTypeSettingsTrait<DebugMessageTrait>,
        MessageTypeSettingsTrait<InfoMessageTrait>,
        MessageTypeSettingsTrait<WarningMessageTrait>,
        MessageTypeSettingsTrait<ErrorMessageTrait>,
        MessageTypeSettingsTrait<CustomMessageTrait>
    >;
    SettingsRegistry settingsRegsitry;

    using MessageHandler = MessageHandlerTemplate<
        DebugMessageTrait,
        InfoMessageTrait,
        WarningMessageTrait,
        ErrorMessageTrait,
        CustomMessageTrait
    >;

    MessageHandlerTemplateTest() {
        qRegisterMetaType<Draupnir::Messages::Notification::Type>();
        settingsRegsitry.setBackend(&mockedSettings);
    }
    ~MessageHandlerTemplateTest() final = default;

private slots:
    void test_initialization() {
        MessageHandler handler;
        handler.template loadSettings<SettingsRegistry>(&settingsRegsitry);

        QVERIFY(handler.messages() != nullptr);

        // Test runtime
        const auto expectedDebug = Draupnir::Messages::DebugMessageTrait::defaultNotification;
        QCOMPARE(handler.notification(MessageType::Debug), expectedDebug);

        // Test compile-time
        QCOMPARE(handler.template notification<Draupnir::Messages::DebugMessageTrait>(), expectedDebug);
    }

    void test_setNotification() {
        MessageHandler handler;
        handler.template loadSettings<SettingsRegistry>(&settingsRegsitry);

        const auto expectedDebug = Notification::UnknownType;
        QVERIFY(handler.notification(MessageType::Debug) != expectedDebug);

        QSignalSpy notificarionSignalSpy(&handler, &AbstractMessageHandler::notificationTypeChanged);
        handler.setNotification(MessageType::Debug, expectedDebug);

        QCOMPARE(notificarionSignalSpy.count(),1); // Signal was emitted only once
        auto arguments = notificarionSignalSpy.takeFirst();
        QCOMPARE(arguments.count(), 2);
        QCOMPARE(arguments.at(0).value<quint64>(), MessageType::Debug);
        QCOMPARE(arguments.at(1).value<Notification::Type>(), expectedDebug);
    }

    void test_message_box_notification() {
        Message* dummyMessage = Message::fromTrait<CustomMessageTrait>(QString{"Test Message"});
        MessageHandler handler;

        bool messageBoxWasShown = false;
        UiTestHelper::scheduleForTopLevelWidgets<MessageDisplayDialog>([&messageBoxWasShown](MessageDisplayDialog* dialog){
            dialog->accept();
            messageBoxWasShown = true;
        },1000);
        handler.showMessage(dummyMessage,Notification::MessageBoxType);
        QTRY_VERIFY(messageBoxWasShown);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageHandlerTemplateTest)

#include "MessageHandlerTemplateTest.moc"
