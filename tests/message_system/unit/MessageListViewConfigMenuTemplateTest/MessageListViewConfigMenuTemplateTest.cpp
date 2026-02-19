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
        qRegisterMetaType<Draupnir::Messages::MessageFields>();
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
        QCOMPARE(abstractTestMenu->displayedMessageFieldsMask(), Draupnir::Messages::MessageField::None);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::What), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::DateTime), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::All), false);

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
        QVERIFY(abstractTestMenu->displayedMessageFieldsMask() == Draupnir::Messages::MessageField::None);

        // Select some random field to be displayed
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, true);
        // Check if everything is updated properly.
        qDebug() << abstractTestMenu->displayedMessageFieldsMask();
        qDebug() << Draupnir::Messages::MessageField::Icon;
        QCOMPARE(abstractTestMenu->displayedMessageFieldsMask(), Draupnir::Messages::MessageField::Icon);
        QVERIFY(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon));

        // Select some other random field to be displayed
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, true);
        // Check if everything is still updated properly
        QCOMPARE(abstractTestMenu->displayedMessageFieldsMask(), Draupnir::Messages::MessageField::Icon | Draupnir::Messages::MessageField::Brief);
        QVERIFY(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon));
        QVERIFY(abstractTestMenu->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief));

        // Select all of the fields manually
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::What, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::DateTime, true);
        // Check that All action is checked
        QCOMPARE(abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->isChecked(), true);

        // Deselect one of the fields manually
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, false);
        // Check that All action is unchecked
        QCOMPARE(abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->isChecked(), false);

        // Deselect rest of the fields
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::What, false);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, false);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::DateTime, false);
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
        QVERIFY(abstractTestMenu->displayedMessageFieldsMask() == MessageField::None);

        // Select some random message field
        abstractTestMenu->m_messageFieldsContainer.getUiElement(MessageField::Icon)->trigger();

        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 1);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
        QList<QVariant> signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageField>(), Draupnir::Messages::MessageField::Icon);
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Kind-of pre-select some fields
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::What, true);
        abstractTestMenu->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, true);

        // Trigger All action
        abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->trigger();
        // Check if the UI is updated
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::Brief), true);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::What), true);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::Icon), true);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::DateTime), true);
        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 1);
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
        signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageField>(), MessageField::DateTime);
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Check if we uncheck All action properly
        abstractTestMenu->m_messageFieldsContainer.showAllUiElement()->trigger();
        // Check if the UI is updated
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::Brief), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::What), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::Icon), false);
        QCOMPARE(abstractTestMenu->isMessageFieldDisplayed(MessageField::DateTime), false);

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
