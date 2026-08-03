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
#include "draupnir/message_system/core/MessageSystemTypeBuilder.h"
#include "draupnir/messages/ui/widgets/MessageDisplayWidget.h"
#include "draupnir/messages/traits/MessageTypeTraitTemplate.h"
#include "draupnir/message_system/core/MessageHandlerTemplate.h"
#include "draupnir/messages/categories/MessageCategories.h"
#include "draupnir/messages/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/messages/traits/categories/NetworkMessageCategoryTrait.h"
#include "draupnir/messages/core/MessageViewItem.h"
#include "draupnir/notifications/core/NotificationTypesSerializerTemplate.h"
#include "draupnir/notifications/channels/DialogNotificationChannel.h"
#include "draupnir/notifications/utils/NotificationTypesSerializer.h"

#include "draupnir-test/helpers/MessageDisplayDialogHelpers.h"
#include "draupnir-test/mocks/SettingsSourceMockTemplate.h"

using namespace Draupnir::Logging;
using namespace Draupnir::Messages;
using namespace Draupnir::MessageSystem;
using namespace Draupnir::Notifications;
using namespace Draupnir::Settings;

/*! @class MessageSystemTemplateIT tests/modules/notifications/integration/MessageSystemTemplateIT.cpp
 *  @ingroup MessageSystemTests
 *  @brief Integration test for the @ref Draupnir::MessageSystem::MessageHandlerTemplate class. */

class MessageSystemTemplateIT final : public QObject
{
    Q_OBJECT
private:
///@name MessageType traits
///@{
    using DefaultInfoTypeTrait = MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Info>;
    using DefaultWarningTypeTrait = MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Warning>;
    using DefaultErrorTypeTrait = MessageTypeTraitTemplate<DefaultMessageCategoryTrait, MessageLevel::Error>;
///@}

    using MessageSystem = typename MessageSystemTypeBuilder::start
        ::withCategories<DefaultMessageCategoryTrait, NetworkMessageCategoryTrait>
        ::notifyFor<DefaultInfoTypeTrait, DefaultWarningTypeTrait, DefaultErrorTypeTrait>
        ::via<DialogNotificationChannel>
    ::type;

    using SettingsRegistryMocked = typename Draupnir::Settings::SettingsTraitsConcatenator<
        MessageSystem::SettingsBundle
    >::to<Draupnir::Settings::SettingsSourceMockTemplate>;

    MessagePtr dummyMessage = nullptr;
    MessageList dummyMessageList = MessageList{};
    SettingsRegistryMocked* settingsSource = nullptr;
    MessageSystem* messageSystem = nullptr;

private slots:
    void initTestCase() {
        dummyMessage = Message::create("Very Important Message", MessageLevel::Info);
        dummyMessageList = MessageList{
            Message::create("One",   MessageLevel::Debug),
            Message::create("two",   MessageLevel::Info),
            Message::create("Three", MessageLevel::Warning),
            Message::create("Four",  MessageLevel::Error)
        };

        settingsSource = new SettingsRegistryMocked;
        messageSystem = new MessageSystem;
    }

    void cleanupTestCase() {
        delete messageSystem; messageSystem = nullptr;
    }

    void test_logging_messages() {
        messageSystem->loadSettings(settingsSource);
        messageSystem->handler()->setNotificationTypesFor(dummyMessage->type(), NotificationType::DialogNotification);

        Logger::get().logMessage(dummyMessage);

        QVERIFY(MessageDisplayDialogHelpers::dialogWasDisplayedWith({dummyMessage}));
        QCOMPARE(messageSystem->handler()->messages()->rowCount(), 1);
        QCOMPARE(messageSystem->handler()->messages()->messageAt(0), dummyMessage);

        Logger::get().logMessageList(dummyMessageList);
        QCOMPARE(messageSystem->handler()->messages()->rowCount(), 1+dummyMessageList.count());
    }
};

QTEST_MAIN(MessageSystemTemplateIT)

#include "MessageSystemTemplateIT.moc"
