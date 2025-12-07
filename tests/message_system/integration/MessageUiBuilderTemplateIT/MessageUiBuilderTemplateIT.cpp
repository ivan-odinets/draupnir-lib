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

#include "draupnir/MessageSystem.h"
#include "draupnir/settings_registry/SettingsRegistryTemplate.h"
#include "draupnir/message_system/traits/messages/DefaultMessageTraits.h"
#include "draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h"
#include "draupnir/message_system/ui/widgets/AbstractNotificationSettingsWidget.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/messages/CustomMessageTrait.h"

namespace Draupnir::Messages
{

/*! @class MessageUiBuilderTemplateIT
 *  @brief This test class tests basic functionality of the MessageSystem.
 *
 * @todo Refractor this test so that it will have better readability.  */

class MessageUiBuilderTemplateIT : public QObject
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

    // MessageSystemTemplate to be tested
    using MessageSystem = MessageSystemTemplate<CustomMessageTrait>;
    MessageSystem messageSystem;

    // We will need SettingsResitryTemplate as well
    using SettingsRegistry = Draupnir::Settings::SettingsTraitsConcatenator<
        MessageSystem::SettingsBundle
    >::toSettingsRegistry;
    SettingsRegistry settingsRegistry;

    // Internal things of MessageSystemTemplate
    MessageSystem::MessageHandler* p_messageHandler;
    MessageSystem::MessageUiBuilder* p_uiBuilder;

    // Init internal fields
    MessageUiBuilderTemplateIT() :
        messageSystem{},
        p_messageHandler{messageSystem.handler()},
        p_uiBuilder{messageSystem.uiBuilder()}
    {
        settingsRegistry.setBackend(&mockedSettings);
        messageSystem.loadSettings(&settingsRegistry);
    }

    ~MessageUiBuilderTemplateIT() = default;

private slots:
    void test_settings_menu() {
        // Create menus
        auto debugNotifications = p_uiBuilder->createNotificationSettingsMenu(MessageType::Debug);
        auto infoNotifications = p_uiBuilder->createNotificationSettingsMenu(MessageType::Info);

        auto messageHandler = messageSystem.handler();

        // We have the same types displayed and used within the MessageHandler
        QCOMPARE(debugNotifications->notificationType(), messageHandler->notification(MessageType::Debug));
        QCOMPARE(infoNotifications->notificationType(), messageHandler->notification(MessageType::Info));

        // Now we trigger, and...
        debugNotifications->getActionFor(Notification::MessageBoxType)->trigger();
        infoNotifications->getActionFor(Notification::MessageBoxType)->trigger();

        // Still we have the same types displayed and used within the MessageHandler
        QCOMPARE(debugNotifications->notificationType(), messageHandler->notification(MessageType::Debug));
        QCOMPARE(infoNotifications->notificationType(), messageHandler->notification(MessageType::Info));

        delete debugNotifications;
        delete infoNotifications;
    }

    void test_settings_menu_and_widget_sync() {
        // Create NotificationSettingsWidget
        auto notificationSettingsWidget = p_uiBuilder->createNotificationSettingsWidget();

        // Create menus
        auto debugNotifications = p_uiBuilder->createNotificationSettingsMenu(MessageType::Debug);
        auto infoNotifications = p_uiBuilder->createNotificationSettingsMenu(MessageType::Info);

        // Displayed things are the same
        QCOMPARE(notificationSettingsWidget->notificationType(MessageType::Debug), debugNotifications->notificationType());
        QCOMPARE(notificationSettingsWidget->notificationType(MessageType::Info),   infoNotifications->notificationType());

        // Change the notification types
        debugNotifications->setNotificationType(Notification::Type::MessageBoxType);
        debugNotifications->notificationTypeChanged(Notification::Type::MessageBoxType);
        infoNotifications->setNotificationType(Notification::Type::Systemtray);
        infoNotifications->notificationTypeChanged(Notification::Type::Systemtray);

        // Displayed things are still the same
        QCOMPARE(notificationSettingsWidget->notificationType(MessageType::Debug),  debugNotifications->notificationType());
        QCOMPARE(notificationSettingsWidget->notificationType(MessageType::Info),   infoNotifications->notificationType());

        // Although this is a test, some cleanup may be usefull
        delete notificationSettingsWidget;
        delete debugNotifications;
        delete infoNotifications;
    }

    void test_log_widget() {
        auto logWidget = p_uiBuilder->createLogWidget();
        QCOMPARE(logWidget->messageListModel(), p_messageHandler->messages());
    }
};

}; // namespace Draupnir::Messages

QTEST_MAIN(Draupnir::Messages::MessageUiBuilderTemplateIT)

#include "MessageUiBuilderTemplateIT.moc"
