/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/messages/core/MessageTypeIconProvider.h"
#include "draupnir/messages/traits/MessageTypeTraitTemplate.h"
#include "draupnir/message_system/core/MessageHandlerTemplate.h"
#include "draupnir/messages/categories/MessageCategories.h"
#include "draupnir/messages/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/messages/core/MessageViewItem.h"
#include "draupnir/notifications/core/NotificationTypesSerializerTemplate.h"
#include "draupnir/notifications/utils/NotificationTypesSerializer.h"
#include "draupnir/notifications/channels/DialogNotificationChannel.h"

#include "draupnir-test/helpers/MessageDisplayDialogHelpers.h"
#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"

using namespace Draupnir::Logging;
using namespace Draupnir::Messages;
using namespace Draupnir::MessageSystem;
using namespace Draupnir::Notifications;
using namespace Draupnir::Settings;

/*! @class MessageHandlerTemplateIT tests/modules/notifications/integration/MessageHandlerTemplateIT.cpp
 *  @ingroup MessageSystemTests
 *  @brief Integration test for the @ref Draupnir::MessageSystem::MessageHandlerTemplate class. */

class MessageHandlerTemplateIT final : public QObject
{
    Q_OBJECT
private:
///@name MessageType traits
///@{
    using DefaultInfoTypeTrait = MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Info>;
    using DefaultWarningTypeTrait = MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Warning>;
    using DefaultErrorTypeTrait = MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Error>;
///@}

    using MessageHandler = MessageHandlerTemplate<
        TemplateArgs::MessageTypeTraitsWrapper<
            DefaultInfoTypeTrait, DefaultWarningTypeTrait, DefaultErrorTypeTrait
        >,
        TemplateArgs::NotificationChannelsWrapper<DialogNotificationChannel>
    >;

    using SettingsRegistryMocked = Draupnir::Settings::SettingsSourceMockTemplate<
        MessageNotificationSettingTraitTemplate<MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Info>>,
        MessageNotificationSettingTraitTemplate<MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Warning>>,
        MessageNotificationSettingTraitTemplate<MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Error>>
    >;

    MessagePtr dummyMessage = nullptr;
    MessageList dummyMessageList = MessageList{};
    SettingsRegistryMocked* settingsSource = nullptr;
    MessageHandler* messageHandler = nullptr;

private slots:
    void initTestCase() {
        // Icon Provider
        MessageViewItem::registerIconProvider(new MessageTypeIconProvider);

        // NotificationTypesSerializer
        NotificationTypesSerializer::registerImplementation(
            new NotificationTypesSerializerTemplate<DialogNotificationChannel::Trait> );

        dummyMessage = Message::create("Very Important Message", MessageLevel::Info);
        dummyMessageList = MessageList{
            Message::create("One",   MessageLevel::Debug),
            Message::create("two",   MessageLevel::Info),
            Message::create("Three", MessageLevel::Warning),
            Message::create("Four",  MessageLevel::Error)
        };

        settingsSource = new SettingsRegistryMocked;
    }

    void cleanupTestCase() {
        delete settingsSource; settingsSource = nullptr;
    }

    void init() {
        messageHandler = new MessageHandler;
        settingsSource->reset();
    }

    void cleanup() {
        delete messageHandler; messageHandler = nullptr;
    }

    void test_notification_types_for() {
        // Given
        QVERIFY(MessageNotificationSettingTraitTemplate<DefaultInfoTypeTrait>::defaultValue() != DialogNotificationChannel::Trait::value());
        settingsSource->set<MessageNotificationSettingTraitTemplate<DefaultInfoTypeTrait>>(
            DialogNotificationChannel::Trait::value()
        );
        // When
        messageHandler->loadSettings(settingsSource);
        // Then
        QCOMPARE(
            messageHandler->notificationTypesFor(DefaultInfoTypeTrait::type()),
            DialogNotificationChannel::Trait::value()
        );
    }

    void test_set_notification_types_for() {
        // Given
        // QVERIFY(MessageNotificationSettingTrait<DefaultInfoTypeTrait>::defaultValue() != DialogNotificationTrait::value());
        QCOMPARE(
            settingsSource->get<MessageNotificationSettingTraitTemplate<DefaultInfoTypeTrait>>(),
            MessageNotificationSettingTraitTemplate<DefaultInfoTypeTrait>::defaultValue()
        );
        messageHandler->loadSettings(settingsSource);
        QCOMPARE(
            messageHandler->notificationTypesFor(DefaultInfoTypeTrait::type()),
            MessageNotificationSettingTraitTemplate<DefaultInfoTypeTrait>::defaultValue()
        );
        // When
        messageHandler->setNotificationTypesFor(
            DefaultInfoTypeTrait::type(),
            DialogNotificationChannel::Trait::value()
        );
        // Then
        QCOMPARE(
            settingsSource->get<MessageNotificationSettingTraitTemplate<DefaultInfoTypeTrait>>(),
            DialogNotificationChannel::Trait::value()
        );
    }

    void test_handle_message() {
        messageHandler->loadSettings(settingsSource);
        messageHandler->setNotificationTypesFor(dummyMessage->type(), NotificationType::DialogNotification);
        messageHandler->handleMessage(dummyMessage);

        QVERIFY(MessageDisplayDialogHelpers::dialogWasDisplayedWith({dummyMessage}));
        QCOMPARE(messageHandler->messages()->rowCount(), 1);
        QCOMPARE(messageHandler->messages()->messageAt(0), dummyMessage);
    }

    void test_handle_message_list() {
        messageHandler->loadSettings(settingsSource);

        // Our MessageHandler can not display Debug messages (which is fine).
        MessageList dummyDisplayableMessageList;
        for (const auto& message : dummyMessageList) {
            if (MessageHandler::isMessageTypeKnown(message->type()))
                dummyDisplayableMessageList.append(message);
        }
        // Lets force to show everything as a dialog...
        for (const auto& message : dummyDisplayableMessageList)
            messageHandler->setNotificationTypesFor(message->type(), NotificationType::DialogNotification);

        // When
        messageHandler->handleMessageList(dummyMessageList);

        // Than
        // No Debug level messages.
        auto validationResult = MessageDisplayDialogHelpers::dialogWasDisplayedWith(dummyDisplayableMessageList);
        if (!validationResult) QFAIL(validationResult.error().toStdString().c_str());
        // But everything was placed into the model.
        QCOMPARE(messageHandler->messages()->rowCount(), dummyMessageList.count());
    }
};

QTEST_MAIN(MessageHandlerTemplateIT)

#include "MessageHandlerTemplateIT.moc"
