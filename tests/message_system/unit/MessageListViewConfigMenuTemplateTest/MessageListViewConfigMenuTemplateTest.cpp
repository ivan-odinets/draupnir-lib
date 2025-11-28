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

#include "draupnir/message_system/ui/menus/MessageListViewConfigMenuTemplate.h"
#include "draupnir/message_system/traits/messages/DefaultMessageTraits.h"

#include "draupnir-test/traits/messages/CustomMessageTrait.h"

namespace Draupnir::Messages
{

/*! @class MessageListViewConfigMenuTemplateTest tests/message_system/unit/MessageListViewConfigMenuTemplateTest.cpp
 *  @brief This test class tests MessageListViewConfigMenuTemplate class. */

class MessageListViewConfigMenuTemplateTest final : public QObject
{
    Q_OBJECT
public:
    MessageListViewConfigMenuTemplateTest() {
        qRegisterMetaType<Draupnir::Messages::Message::Fields>();
        qRegisterMetaType<Draupnir::Messages::MessageType>();
    };
    ~MessageListViewConfigMenuTemplateTest() final = default;

    using MessageListViewConfigMenu = MessageListViewConfigMenuTemplate<
        DebugMessageTrait,
        InfoMessageTrait,
        ErrorMessageTrait,
        CustomMessageTrait
    >;

    MessageListViewConfigMenu* testMenuTemplate = nullptr;
    AbstractMessageListViewConfigMenu* abstractTestMenu = nullptr;

private slots:
    void init() {
        testMenuTemplate = new MessageListViewConfigMenu;
        abstractTestMenu = testMenuTemplate;
    }

    void cleanup() {
        delete testMenuTemplate;
    }

    void test_initialization() {
        // Message parts. Bu default none should be selected
        QCOMPARE(abstractTestMenu->displayedMessageFieldsMask(), Draupnir::Messages::Message::Fields::None);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::Brief), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::What), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::Icon), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::DateTime), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::All), false);

        // Message types. By default none should be selected
        QCOMPARE(abstractTestMenu->displayedMessageTypesMask(), MessageType{Draupnir::Messages::MessageType::None});
        QCOMPARE(abstractTestMenu->isMessageTypeDisplayed(DebugMessageTrait::type), false);
        QCOMPARE(abstractTestMenu->isMessageTypeDisplayed(InfoMessageTrait::type), false);
        QCOMPARE(abstractTestMenu->isMessageTypeDisplayed(ErrorMessageTrait::type), false);
        QCOMPARE(abstractTestMenu->isMessageTypeDisplayed(CustomMessageTrait::type), false);
    }

    void test_setting_displayed_message_parts() {
        // These setter methods should not emit any signals. Lets check.
        QSignalSpy messageFieldVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageFieldVisibilityChanged };
        QSignalSpy messageTypeVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageTypeVisibilityChanged };

        // Verify initial state.
        QVERIFY(abstractTestMenu->displayedMessageFieldsMask() == Draupnir::Messages::Message::Fields::None);

        // Select some random field to be displayed
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Icon, true);
        // Check if everything is updated properly.
        QCOMPARE(abstractTestMenu->displayedMessageFieldsMask(), Draupnir::Messages::Message::Icon);
        QVERIFY(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::Icon));

        // Select some other random field to be displayed
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Brief, true);
        // Check if everything is still updated properly
        QCOMPARE(abstractTestMenu->displayedMessageFieldsMask(), Draupnir::Messages::Message::Icon | Draupnir::Messages::Message::Brief);
        QVERIFY(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::Icon));
        QVERIFY(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::Message::Brief));

        // Select all of the fields manually
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Brief, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::What, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Icon, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::DateTime, true);
        // Check that All action is checked
        QCOMPARE(abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->isChecked(), true);

        // Deselect one of the fields manually
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Brief, false);
        // Check that All action is unchecked
        QCOMPARE(abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->isChecked(), false);

        // Deselect rest of the fields
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::What, false);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Icon, false);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::DateTime, false);
        // Check that All action is still unchecked
        QCOMPARE(abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->isChecked(), false);

        // Check that no signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 0);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
    }

    void test_setting_displayed_message_types() {
        // These setter methods should not emit any signals. Lets check.
        QSignalSpy messageFieldVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageFieldVisibilityChanged };
        QSignalSpy messageTypeVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageTypeVisibilityChanged };

        // Check initial state.
        QVERIFY(abstractTestMenu->displayedMessageTypesMask() == MessageType{Draupnir::Messages::MessageType::None});

        // Select some random message type to be displayed
        abstractTestMenu->setMessageTypeDisplayed(Draupnir::Messages::DebugMessageTrait::type, true);
        // Check if everything was updated properly
        QCOMPARE(abstractTestMenu->displayedMessageTypesMask(), MessageType{Draupnir::Messages::MessageType::Debug});
        QVERIFY(abstractTestMenu->isMessageTypeDisplayed(Draupnir::Messages::MessageType::Debug));

        // Select some other random message type
        abstractTestMenu->setMessageTypeDisplayed(CustomMessageTrait::type, true);
        // Check if everything was updated properly
        QCOMPARE(abstractTestMenu->displayedMessageTypesMask(), MessageType{Draupnir::Messages::MessageType::Debug | CustomMessageTrait::type});
        QVERIFY(abstractTestMenu->isMessageTypeDisplayed(CustomMessageTrait::type));

        // Check that no signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 0);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
    }

    void test_selecting_message_types_by_user() {
        // Not we will actually listen for the signals emission
        QSignalSpy messageFieldVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageFieldVisibilityChanged };
        QSignalSpy messageTypeVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageTypeVisibilityChanged };

        // Check initial state.
        QVERIFY(abstractTestMenu->displayedMessageTypesMask() == MessageType{Draupnir::Messages::MessageType::None});

        // Trigger QAction for random message
        testMenuTemplate->m_typeActionsContainer.getUiElement(CustomMessageTrait::type)->trigger();
        // Check that proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 0);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 1);
        QList<QVariant> signalArgs = messageTypeVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(), 2);
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageType>(), MessageType{CustomMessageTrait::type});
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Trigger QAction for random message again
        testMenuTemplate->m_typeActionsContainer.getUiElement(CustomMessageTrait::type)->triggered(false);
        // Check that proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 0);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 1);
        signalArgs = messageTypeVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(), 2);
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageType>(), MessageType{CustomMessageTrait::type});
        QCOMPARE(signalArgs[1].value<bool>(), false);
    }

    void test_selecting_message_fields_by_user() {
        // Not we will actually listen for the signals emission
        QSignalSpy messageFieldVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageFieldVisibilityChanged };
        QSignalSpy messageTypeVisibilityChangedSpy{ abstractTestMenu, &AbstractMessageListViewConfigMenu::messageTypeVisibilityChanged };

        // Check initial state.
        QVERIFY(abstractTestMenu->displayedMessageFieldsMask() == Message::Fields::None);

        // Select some random message field
        abstractTestMenu->m_messageFieldsContainer.getUiElement(Message::Fields::Icon)->trigger();

        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 1);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
        QList<QVariant> signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::Message::Fields>(), Draupnir::Messages::Message::Fields::Icon);
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Kind-of pre-select some fields
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Brief, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::What, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::Message::Icon, true);

        // Trigger All action
        abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->trigger();
        // Check if the UI is updated
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::Brief), true);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::What), true);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::Icon), true);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::DateTime), true);
        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 1);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
        signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::Message::Fields>(), Message::Fields::DateTime);
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Check if we uncheck All action properly
        abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->trigger();
        // Check if the UI is updated
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::Brief), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::What), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::Icon), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Message::Fields::DateTime), false);

        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 4);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
        QCOMPARE(messageFieldVisibilityChangedSpy[0][1].value<bool>(), false);
        QCOMPARE(messageFieldVisibilityChangedSpy[1][1].value<bool>(), false);
        QCOMPARE(messageFieldVisibilityChangedSpy[2][1].value<bool>(), false);
        QCOMPARE(messageFieldVisibilityChangedSpy[3][1].value<bool>(), false);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageListViewConfigMenuTemplateTest)

#include "MessageListViewConfigMenuTemplateTest.moc"
