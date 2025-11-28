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

#include "draupnir/message_system/ui/widgets/MessageTypesSelectorWidgetTemplate.h"
#include "draupnir/message_system/traits/messages/DefaultMessageTraits.h"

#include "draupnir-test/traits/messages/CustomMessageTrait.h"

namespace Draupnir::Messages
{

/*! @class MessageTypesSelectorWidgetTemplateTest
 *  @brief This test class tests MessageTypesSelectorWidgetTemplate class. */

class MessageTypesSelectorWidgetTemplateTest final : public QObject
{
    Q_OBJECT
public:
    MessageTypesSelectorWidgetTemplateTest() {
        qRegisterMetaType<Draupnir::Messages::MessageType>();
    };
    ~MessageTypesSelectorWidgetTemplateTest() final = default;

    using MessageTypesSelectorWidget = MessageTypesSelectorWidgetTemplate<
        DebugMessageTrait,
        InfoMessageTrait,
        ErrorMessageTrait,
        CustomMessageTrait
    >;

    MessageTypesSelectorWidget* widget = nullptr;

private slots:
    void init() {
        widget = new MessageTypesSelectorWidget;
    }

    void cleanup() {
        delete widget;
    }

    void test_initialization() {
        // Message types. By default none should be selected
        QCOMPARE(widget->displayedMessageTypesMask(), MessageType{Draupnir::Messages::MessageType::None});
        QCOMPARE(widget->isMessageTypeDisplayed(DebugMessageTrait::type), false);
        QCOMPARE(widget->isMessageTypeDisplayed(InfoMessageTrait::type), false);
        QCOMPARE(widget->isMessageTypeDisplayed(ErrorMessageTrait::type), false);
        QCOMPARE(widget->isMessageTypeDisplayed(CustomMessageTrait::type), false);
    }

    void test_setting_displayed_message_types() {
        // These setter methods should not emit any signals. Lets check.
        QSignalSpy messageTypeVisibilityChangedSpy{ widget, &AbstractMessageTypesSelectorWidget::messageTypeVisibilityChanged };

        // Check initial state.
        QVERIFY(widget->displayedMessageTypesMask() == MessageType{Draupnir::Messages::MessageType::None});

        // Select some random message type to be displayed
        widget->setMessageTypeDisplayed(Draupnir::Messages::DebugMessageTrait::type, true);
        // Check if everything was updated properly
        QCOMPARE(widget->displayedMessageTypesMask(), MessageType{Draupnir::Messages::MessageType::Debug});
        QVERIFY(widget->isMessageTypeDisplayed(Draupnir::Messages::MessageType::Debug));

        // Select some other random message type
        widget->setMessageTypeDisplayed(CustomMessageTrait::type, true);
        // Check if everything was updated properly
        QCOMPARE(widget->displayedMessageTypesMask(), MessageType{Draupnir::Messages::MessageType::Debug | CustomMessageTrait::type});
        QVERIFY(widget->isMessageTypeDisplayed(CustomMessageTrait::type));

        // Check that no signals were emitted
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 0);
    }

    void test_selecting_message_types_by_user() {
        // Now we will actually listen for the signals emission
        QSignalSpy messageTypeVisibilityChangedSpy{ widget, &AbstractMessageTypesSelectorWidget::messageTypeVisibilityChanged };

        // Check initial state.
        QVERIFY(widget->displayedMessageTypesMask() == MessageType{Draupnir::Messages::MessageType::None});

        // Trigger QAction for random message
        widget->m_typeCheckBoxContainer.getUiElement(CustomMessageTrait::type)->click();
        // Check that proper signals were emitted
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 1);
        QList<QVariant> signalArgs = messageTypeVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(), 2);
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageType>(), MessageType{CustomMessageTrait::type});
        QCOMPARE(signalArgs[1].value<bool>(), true);

        // Trigger QAction for random message again
        widget->m_typeCheckBoxContainer.getUiElement(CustomMessageTrait::type)->click();
        // Check that proper signals were emitted
        QTRY_COMPARE(messageTypeVisibilityChangedSpy.count(), 1);
        signalArgs = messageTypeVisibilityChangedSpy.takeFirst();
        QCOMPARE(signalArgs.count(), 2);
        QCOMPARE(signalArgs[0].value<Draupnir::Messages::MessageType>(), MessageType{CustomMessageTrait::type});
        QCOMPARE(signalArgs[1].value<bool>(), false);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageTypesSelectorWidgetTemplateTest)

#include "MessageTypesSelectorWidgetTemplateTest.moc"
