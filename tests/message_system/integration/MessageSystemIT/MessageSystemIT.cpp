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

#include "draupnir/Logger.h"
#include "draupnir/MessageSystemTemplate.h"
#include "draupnir/SettingsRegistryTemplate.h"
#include "draupnir/traits/messages/DefaultMessageTraits.h"
#include "draupnir/models/MessageListModel.h"
#include "draupnir/traits/settings/MessageTypeSettingsTrait.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/traits/messages/CustomMessageTrait.h"

namespace Draupnir::Messages
{

/*! @class MessageSystemGeneralIT tests/message_system/GeneralIT/MessageSystemGeneralIT.cpp
 *  @brief This test class tests basic functionality of the MessageSystem.
 *
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

class MessageSystemGeneralIT : public QObject
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
    MessageSystem simpleMessageSystem;

    // We will need SettingsResitryTemplate as well
    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        MessageSystem::SettingsBundle
    >;
    SettingsRegistry registry;

    // Internal things of MessageSystemTemplate
    MessageHandlerInterface* p_messageHandler;
    MessageSystem::MessageHandler* p_messageHandlerTemplate;

    MessageListModel* p_messageListModel;

    // Init internal fields
    MessageSystemGeneralIT() :
        simpleMessageSystem{},
        p_messageHandler{simpleMessageSystem.handler()},
        p_messageHandlerTemplate{static_cast<MessageSystem::MessageHandler*>(p_messageHandler)},
        p_messageListModel{p_messageHandler->messages()}
    {
        registry.setBackend(&mockedSettings);
        simpleMessageSystem.loadSettings(&registry);
    }

    ~MessageSystemGeneralIT() = default;

private slots:
    /*! @brief Tests if default settings were properly initialized. */
    void testInitialization() {

        // Build in types
        QVERIFY(p_messageHandler->notification(DebugMessageTrait::type)   == DebugMessageTrait::defaultNotification);
        QVERIFY(p_messageHandler->notification(InfoMessageTrait::type)    == InfoMessageTrait::defaultNotification);
        QVERIFY(p_messageHandler->notification(WarningMessageTrait::type) == WarningMessageTrait::defaultNotification);
        QVERIFY(p_messageHandler->notification(ErrorMessageTrait::type)   == ErrorMessageTrait::defaultNotification);

        // Custom types
        QVERIFY(p_messageHandler->notification(CustomMessageTrait::type)      == CustomMessageTrait::defaultNotification);

        // At the beginning we have empty p_messageListModel
        QVERIFY(p_messageListModel->rowCount() == 0);
    }

    /*! @brief Tests if logging via Logger singltone is done correctly. */
    void testMessageLogging() {
        logger.setMessageHandler(p_messageHandler);

        p_messageListModel->clear();
        QVERIFY(p_messageListModel->rowCount() == 0);

        int elementCount = 0;
        Message* loggedMessage = nullptr;

        // Debug logging
        const QString debugWhat{"Test Debug"};
        logger.logDebug(debugWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()   == debugWhat);
        QVERIFY(loggedMessage->brief()  == DebugMessageTrait::displayName());
        QVERIFY(loggedMessage->type()   == DebugMessageTrait::type);

        // Info logging
        const QString infoBrief{"Test Info Brief"};
        const QString infoWhat{"Test Info"};

        logger.logInfo(infoBrief,infoWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()  == elementCount);
        QVERIFY(loggedMessage->what()  == infoWhat);
        QVERIFY(loggedMessage->brief() == infoBrief);
        QVERIFY(loggedMessage->type()  == InfoMessageTrait::type);

        // Warning logging
        const QString warningWhat{"Test Warning"};

        logger.logWarning(warningWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()  == warningWhat);
        QVERIFY(loggedMessage->brief() == WarningMessageTrait::displayName());
        QVERIFY(loggedMessage->type()  == WarningMessageTrait::type);

        // Error logging
        const QString errorWhat{"Test Warning"};

        logger.logError(errorWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()  == errorWhat);
        QVERIFY(loggedMessage->brief() == ErrorMessageTrait::displayName());
        QVERIFY(loggedMessage->type()  == ErrorMessageTrait::type);

        // Custom loggins
        const QString customWhat{"Test Custom"};

        logger.logMessage<CustomMessageTrait>(customWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()  == customWhat);
        QVERIFY(loggedMessage->brief() == CustomMessageTrait::displayName());
        QVERIFY(loggedMessage->type()  == CustomMessageTrait::type);

        // Clearing the model
        p_messageListModel->clear();
        QVERIFY(p_messageListModel->rowCount() == 0);
    }

    /*! @brief Testing "batch" logging functionality". */
    void testBatchLogging() {
        MessageGroup group = logger.beginMessageGroup();
        QVERIFY(p_messageHandler->m_messageGroupsMap.contains(group));

        // Test batched logging
        logger.logDebug("One",group);
        logger.logDebug("Two",group);
        logger.logDebug("Three");
        QVERIFY(p_messageHandler->m_messageGroupsMap[group].count() == 2);
        QVERIFY(p_messageListModel->rowCount() == 3);

        logger.flush(group);
        QVERIFY(p_messageHandler->m_messageGroupsMap[group].count() == 0);

        logger.endMessageGroup(group);
        QVERIFY(p_messageHandler->m_messageGroupsMap.contains(group) == false);
    }

    void testSettings() {
    }
};

}; // namespace Draupnir::Messages

QTEST_MAIN(Draupnir::Messages::MessageSystemGeneralIT)

#include "MessageSystemIT.moc"
