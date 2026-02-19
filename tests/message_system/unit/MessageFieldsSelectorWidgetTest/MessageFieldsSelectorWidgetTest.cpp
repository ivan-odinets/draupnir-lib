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

#include "draupnir/message_system/ui/widgets/MessageFieldsSelectorWidget.h"

namespace Draupnir::Messages
{

/*! @class MessageListViewConfigMenuTemplateTest tests/message_system/unit/MessageListViewConfigMenuTemplateTest.cpp
 *  @brief This test class tests MessageListViewConfigMenuTemplate class. */

class MessageFieldsSelectorWidgetTest final : public QObject
{
    Q_OBJECT
public:
    MessageFieldsSelectorWidgetTest() {
        qRegisterMetaType<Draupnir::Messages::MessageFields>();
        qRegisterMetaType<Draupnir::Messages::MessageField>();
    };
    ~MessageFieldsSelectorWidgetTest() final = default;

    MessageFieldsSelectorWidget* widget = nullptr;

private slots:
    void init() {
        widget = new MessageFieldsSelectorWidget;
    }

    void cleanup() {
        delete widget;
    }

    void test_initialization() {
        // Message fields. Bu default none should be selected
        QCOMPARE(widget->displayedMessageFieldsMask(), Draupnir::Messages::MessageField::None);
        QCOMPARE(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief), false);
        QCOMPARE(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::What), false);
        QCOMPARE(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon), false);
        QCOMPARE(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::DateTime), false);
        QCOMPARE(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::All), false);
    }

    void test_setting_displayed_message_fields() {
        // These setter methods should not emit any signals. Lets check.
        QSignalSpy messageFieldVisibilityChangedSpy{ widget, &MessageFieldsSelectorWidget::messageFieldVisibilityChanged };

        // Verify initial state.
        QVERIFY(widget->displayedMessageFieldsMask() == Draupnir::Messages::MessageField::None);

        // Select some random field to be displayed
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, true);
        // Check if everything is updated properly.
        QCOMPARE(widget->displayedMessageFieldsMask(), Draupnir::Messages::MessageField::Icon);
        QVERIFY(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon));

        // Select some other random field to be displayed
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, true);
        // Check if everything is still updated properly
        QCOMPARE(widget->displayedMessageFieldsMask(), Draupnir::Messages::MessageField::Icon | Draupnir::Messages::MessageField::Brief);
        QVERIFY(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon));
        QVERIFY(widget->isMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief));

        // Select all of the fields manually
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, true);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::What, true);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, true);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::DateTime, true);
        // Check that All action is checked
        QCOMPARE(widget->m_messageFieldsContainer.showAllUiElement()->isChecked(), true);

        // Deselect one of the fields manually
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, false);
        // Check that All action is unchecked
        QCOMPARE(widget->m_messageFieldsContainer.showAllUiElement()->isChecked(), false);

        // Deselect rest of the fields
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::What, false);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, false);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::DateTime, false);
        // Check that All action is still unchecked
        QCOMPARE(widget->m_messageFieldsContainer.showAllUiElement()->isChecked(), false);

        // Check that no signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 0);
    }

    void test_selecting_message_fields_by_user() {
        // Not we will actually listen for the signals emission
        QSignalSpy messageFieldVisibilityChangedSpy{ widget, &MessageFieldsSelectorWidget::messageFieldVisibilityChanged };

        // Check initial state.
        QVERIFY(widget->displayedMessageFieldsMask() == MessageField::None);

        // Select some random message field
        widget->m_messageFieldsContainer.getUiElement(MessageField::Icon)->click();

        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 1);
        QList<QVariant> signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageField>(), Draupnir::Messages::MessageField::Icon);
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Kind-of pre-select some fields
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Brief, true);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::What, true);
        widget->setMessageFieldDisplayed(Draupnir::Messages::MessageField::Icon, true);

        // Trigger All action
        widget->m_messageFieldsContainer.showAllUiElement()->click();
        // Check if the UI is updated
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Brief), true);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::What), true);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Icon), true);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::DateTime), true);
        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 1);
        signalArgs = messageFieldVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageField>(), MessageField::DateTime);
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Check if we uncheck All action properly
        widget->m_messageFieldsContainer.showAllUiElement()->click();
        // Check if the UI is updated
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Brief), false);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::What), false);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::Icon), false);
        QCOMPARE(widget->isMessageFieldDisplayed(MessageField::DateTime), false);

        // Check if proper signals were emitted
        QTRY_COMPARE(messageFieldVisibilityChangedSpy.count(), 4);
        QCOMPARE(messageFieldVisibilityChangedSpy[0][1].value<bool>(), false);
        QCOMPARE(messageFieldVisibilityChangedSpy[1][1].value<bool>(), false);
        QCOMPARE(messageFieldVisibilityChangedSpy[2][1].value<bool>(), false);
        QCOMPARE(messageFieldVisibilityChangedSpy[3][1].value<bool>(), false);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageFieldsSelectorWidgetTest)

#include "MessageFieldsSelectorWidgetTest.moc"
