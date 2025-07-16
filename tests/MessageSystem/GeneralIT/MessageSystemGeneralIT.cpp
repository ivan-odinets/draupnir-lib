#include <QtTest>
#include <QCoreApplication>

#include "Logger.h"
#include "MessageSystemTemplate.h"

#include "../../src/messages/core/DefaultMessageTraits.h"
#include "../../src/messages/models/MessageListModel.h"

#include "../mocks/MockSettings.h"

class CustomMsgTrait
{
public:
    static constexpr MessageType type = MessageType::nextType(MessageType::FirstCustomType);
    static constexpr const char* settingsKey = "CUSTOM";
    static constexpr Notification::Type defaultNotification = Notification::MessageBoxType;
    static QString displayName() { return QObject::tr("TEST"); }
    static const QIcon& icon() {
        static const QIcon msgIcon = QIcon{};
        return msgIcon;
    }
};

/*! @class MessageSystemGeneralIT draupnir-lib/tests/MessageSystem/GeneralIT/MessageSystemGeneralIT.cpp
 *  @brief This test class tests basic functionality of the MessageSystem. */

class MessageSystemGeneralIT : public QObject
{
    Q_OBJECT

public:
    // Settings, mocked
    MockSettings<
        DebugMessageTrait,
        InfoMessageTrait,
        WarningMessageTrait,
        ErrorMessageTrait,
        CustomMsgTrait
    > mockedSettings;

    // MessageSystemTemplate to be tested
    MessageSystemTemplate<CustomMsgTrait> simpleMessageSystem;

    // Internal things of MessageSystemTemplate
    MessageHandler* p_messageHandler;
    MessageHandlerTemplate<CustomMsgTrait>* p_messageHandlerTemplate;

    MessageListModel* p_messageListModel;

    // Init internal fields
    MessageSystemGeneralIT() :
        simpleMessageSystem{},
        p_messageHandler{simpleMessageSystem.handler()},
        p_messageHandlerTemplate{static_cast<MessageHandlerTemplate<CustomMsgTrait>*>(p_messageHandler)},
        p_messageListModel{p_messageHandler->messages()}
    {}

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
        QVERIFY(p_messageHandler->notification(CustomMsgTrait::type)      == CustomMsgTrait::defaultNotification);

        // At the beginning we have empty p_messageListModel
        QVERIFY(p_messageListModel->rowCount() == 0);
    }

    /*! @brief Tests if logging via Logger singltone is done correctly. */
    void testMessageLogging() {
        logger->setMessageHandler(p_messageHandler);

        p_messageListModel->clear();
        QVERIFY(p_messageListModel->rowCount() == 0);

        int elementCount = 0;
        Message* loggedMessage = nullptr;

        // Debug logging
        const QString debugWhat{"Test Debug"};
        logger->logDebug(debugWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()   == debugWhat);
        QVERIFY(loggedMessage->brief()  == DebugMessageTrait::displayName());
        QVERIFY(loggedMessage->type()   == DebugMessageTrait::type);

        // Info logging
        const QString infoBrief{"Test Info Brief"};
        const QString infoWhat{"Test Info"};

        logger->logInfo(infoBrief,infoWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()  == elementCount);
        QVERIFY(loggedMessage->what()  == infoWhat);
        QVERIFY(loggedMessage->brief() == infoBrief);
        QVERIFY(loggedMessage->type()  == InfoMessageTrait::type);

        // Warning logging
        const QString warningWhat{"Test Warning"};

        logger->logWarning(warningWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()  == warningWhat);
        QVERIFY(loggedMessage->brief() == WarningMessageTrait::displayName());
        QVERIFY(loggedMessage->type()  == WarningMessageTrait::type);

        // Error logging
        const QString errorWhat{"Test Warning"};

        logger->logError(errorWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()  == errorWhat);
        QVERIFY(loggedMessage->brief() == ErrorMessageTrait::displayName());
        QVERIFY(loggedMessage->type()  == ErrorMessageTrait::type);

        // Custom loggins
        const QString customWhat{"Test Custom"};

        logger->logMessage<CustomMsgTrait>(customWhat);
        loggedMessage = static_cast<Message*>(p_messageListModel->index(elementCount,0).internalPointer());
        elementCount++;

        QVERIFY(p_messageListModel->rowCount()   == elementCount);
        QVERIFY(loggedMessage->what()  == customWhat);
        QVERIFY(loggedMessage->brief() == CustomMsgTrait::displayName());
        QVERIFY(loggedMessage->type()  == CustomMsgTrait::type);

        // Clearing the model
        p_messageListModel->clear();
        QVERIFY(p_messageListModel->rowCount() == 0);
    }

    /*! @brief Testing "batch" logging functionality". */
    void testBatchLogging() {
        MessageGroup group = logger->beginMessageGroup();
        QVERIFY(p_messageHandler->m_messageGroupsMap.contains(group));

        // Test batched logging
        logger->logDebug("One",group);
        logger->logDebug("Two",group);
        logger->logDebug("Three");
        QVERIFY(p_messageHandler->m_messageGroupsMap[group].count() == 2);
        QVERIFY(p_messageListModel->rowCount() == 3);

        logger->flush(group);
        QVERIFY(p_messageHandler->m_messageGroupsMap[group].count() == 0);

        logger->endMessageGroup(group);
        QVERIFY(p_messageHandler->m_messageGroupsMap.contains(group) == false);
    }

    void testSettings() {
        simpleMessageSystem.loadSettings(&mockedSettings);

        auto type = DebugMessageTrait::type;
        p_messageHandler->setNotification(type, Notification::MessageBoxType);
        QVERIFY(mockedSettings.dummySettingsMap[type].toString() == Notification::toConfigString(Notification::MessageBoxType));
        QVERIFY(p_messageHandler->notification(type)  == Notification::MessageBoxType);
    }
};

QTEST_MAIN(MessageSystemGeneralIT)

#include "MessageSystemGeneralIT.moc"
