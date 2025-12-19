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

#include <QPushButton>

#include "draupnir/MessageSystem.h"

#include "draupnir/message_system/models/MessageListModel.h"
#include "draupnir/message_system/ui/widgets/MessageListView.h"

#include "draupnir-test/mocks/MockSettingsTemplate.h"

namespace Draupnir::Messages
{

/*! @class LogWidgetTest tests/message_system/unit/LogWidgetTest.cpp
 *  @brief This test class tests LogWidget class. */

class LogWidgetTest final : public QObject
{
    Q_OBJECT
public:
    using MessageSystem = Draupnir::Messages::MessageSystemTemplate<>;
    using SettingsRegistry = Draupnir::Settings::SettingsTraitsConcatenator<
        MessageSystem::SettingsBundle
    >::toSettingsRegistry;

    using MockSettings = Draupnir::Settings::SettingsTraitsConcatenator<
        MessageSystem::SettingsBundle
    >::to<MockSettingsTemplate>;

    MockSettings mockedBackend;
    SettingsRegistry settingsRegistry;
    MessageSystem messageSystem;


    LogWidget* widget = nullptr;

private slots:
    void initTestCase() {
        settingsRegistry.setBackend(&mockedBackend);
        messageSystem.loadSettings(&settingsRegistry);
    }

    void cleanup() { delete widget; widget = nullptr; }

    /*! @brief Tests if the @ref Draupnir::Messages::LogWidget can be created using @ref Draupnir::Messages::AbstractMessageUiBuilder
     *         / @ref Draupnir::Messages::MessageUiBuilderTemplate. */
    void test_attached_log_widget() {
        // Create log widget.
        widget = messageSystem.uiBuilder()->createLogWidget();

        // Check if internal things are setup correctly
        QCOMPARE(widget->messageListModel(), messageSystem.handler()->messages());
        QCOMPARE(widget->p_uiBuilder, messageSystem.uiBuilderInterface());
        QVERIFY(widget->w_messageListViewConfigMenu != nullptr);
    }

    /*! @brief Tests if the @ref Draupnir::Messages::LogWidget can be created unattached and than properly attached. */
    void test_detached_log_widget() {
        widget = new LogWidget;

        // Check initial state
        QCOMPARE(widget->messageListModel(), nullptr);
        QCOMPARE(widget->p_uiBuilder, nullptr);
        QCOMPARE(widget->w_messageListViewConfigMenu, nullptr);
    }

    void test_attachTo_method() {
        widget = new LogWidget;

        // Verify initial state
        QVERIFY(widget->messageListModel() == nullptr);
        QVERIFY(widget->p_uiBuilder == nullptr);
        QVERIFY(widget->w_messageListViewConfigMenu == nullptr);

        // Attach to MessageSystem using LogWidget::attachTo method
        widget->attachTo(&messageSystem);
        // Check if everything was setup
        QCOMPARE(widget->messageListModel(), messageSystem.handler()->messages());
        QCOMPARE(widget->p_uiBuilder, messageSystem.uiBuilderInterface());
        QVERIFY(widget->w_messageListViewConfigMenu != nullptr);
    }

    void test_step_by_step_attaching() {
        // Reset settings (to be sure)
        const auto defaultMessageTypeFilter = Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting::defaultValue();
        settingsRegistry.set<Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting>(defaultMessageTypeFilter);

        const auto defaultMessageFields = Draupnir::Messages::Settings::LogWidget::DisplayedMessageFieldsSetting::defaultValue();
        settingsRegistry.set<Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting>(defaultMessageFields);

        // Create widget to be tested
        widget = new LogWidget;

        // Verify initial state
        QVERIFY(widget->messageListModel() == nullptr);
        QVERIFY(widget->p_uiBuilder == nullptr);
        QVERIFY(widget->w_messageListViewConfigMenu == nullptr);

        // Load Settings
        widget->loadSettings(&settingsRegistry);
        QCOMPARE(widget->m_settingsBundle.isValid(), true);

        // Set MessageListModel
        widget->setMessageListModel(messageSystem.handler()->messages());
        QCOMPARE(widget->messageListModel(), messageSystem.handler()->messages());

        // Set MessageSystemSpecific UI Elements
        widget->setMessageSystemSpecificUiElements(messageSystem.uiBuilderInterface());
        QCOMPARE(widget->p_uiBuilder, messageSystem.uiBuilderInterface());
        QVERIFY(widget->w_messageListViewConfigMenu != nullptr);

        // And verify that settings vere correctly loaded and initialized for MessageTypes
        const auto debugMessageType = Draupnir::Messages::MessageType::Debug;
        bool isDebugDisplayed = defaultMessageTypeFilter & debugMessageType;
        QCOMPARE(widget->w_messagesListView->isMessageTypeDisplayed(debugMessageType), isDebugDisplayed);
        QCOMPARE(widget->w_messageListViewConfigMenu->isMessageTypeDisplayed(debugMessageType),isDebugDisplayed);

        // And for message fields
        bool isBriefDisplayed = defaultMessageFields & Message::Brief;
        QCOMPARE(widget->w_messagesListView->isMessageFieldDisplayed(Message::Brief), isBriefDisplayed);
        QCOMPARE(widget->w_messageListViewConfigMenu->isMessageFieldDisplayed(Message::Brief),isBriefDisplayed);
    }

    void test_log_clearing() {
        // Create widget
        widget = messageSystem.uiBuilder()->createLogWidget();

        // "Log" something
        auto messageListModel = messageSystem.handler()->messages();
        messageListModel->append(Message::fromTrait<Draupnir::Messages::DebugMessageTrait>("debug"));
        messageListModel->append(Message::fromTrait<Draupnir::Messages::InfoMessageTrait>("info"));
        messageListModel->append(Message::fromTrait<Draupnir::Messages::WarningMessageTrait>("warning"));
        messageListModel->append(Message::fromTrait<Draupnir::Messages::ErrorMessageTrait>("error"));

        // Verify that stuff is logged
        QVERIFY(widget->messageListModel() != nullptr);
        QVERIFY(widget->messageListModel()->rowCount() == 4);

        // Click Clear button
        widget->w_clearLogButton->click();

        // Check if the model is cleared
        QTRY_COMPARE(widget->messageListModel()->rowCount(), 0);
    }

    void test_icon_size_behaviour() {
        const QSize defaultIconSize = Draupnir::Messages::Settings::LogWidget::IconSizeSetting::defaultValue();
        QVERIFY(defaultIconSize.width() == defaultIconSize.height());
        settingsRegistry.set<Draupnir::Messages::Settings::LogWidget::IconSizeSetting>(defaultIconSize);

        widget = messageSystem.uiBuilder()->createLogWidget();

        // Check if defaults are loaded properly
        QCOMPARE(widget->w_messagesListView->iconSize(), defaultIconSize);
        QCOMPARE(widget->w_iconSizeSlider->value(), defaultIconSize.width());

        // Change setting
        const QSize newSize{42,42};
        QVERIFY(newSize != defaultIconSize);
        widget->w_iconSizeSlider->setValue(newSize.width());
        // Now icon size should be changed
        QTRY_COMPARE(widget->w_messagesListView->iconSize(), newSize);
        // But no settings should be saved
        QCOMPARE(mockedBackend.get<Draupnir::Messages::Settings::LogWidget::IconSizeSetting>(), defaultIconSize);

        // Release slider
        emit widget->w_iconSizeSlider->sliderReleased();
        // Icon size should remain the same
        QTRY_COMPARE(widget->w_messagesListView->iconSize(), newSize);
        // And no settings should be updated
        QCOMPARE(mockedBackend.get<Draupnir::Messages::Settings::LogWidget::IconSizeSetting>(), newSize);

        // Cleanup and restore
        settingsRegistry.set<Draupnir::Messages::Settings::LogWidget::IconSizeSetting>(defaultIconSize);
    }

    void test_message_type_filter_behavior() {
        const auto defaultMessageTypeFilter = Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting::defaultValue();
        settingsRegistry.set<Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting>(defaultMessageTypeFilter);

        widget = messageSystem.uiBuilder()->createLogWidget();
        const auto debugMessageType = Draupnir::Messages::MessageType::Debug;
        bool isDebugDisplayed = defaultMessageTypeFilter & debugMessageType;

        // Check if settings were properly loaded
        QCOMPARE(widget->w_messagesListView->isMessageTypeDisplayed(debugMessageType), isDebugDisplayed);
        QCOMPARE(widget->w_messageListViewConfigMenu->isMessageTypeDisplayed(debugMessageType),isDebugDisplayed);

        // Change settings
        widget->w_messageListViewConfigMenu->getActionForType(debugMessageType)->trigger();

        // Check if settings were properly updated
        QCOMPARE(widget->w_messagesListView->isMessageTypeDisplayed(debugMessageType), !isDebugDisplayed);
        const auto settingsValue = settingsRegistry.get<Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting>();
        QCOMPARE(settingsValue | debugMessageType, defaultMessageTypeFilter);
    }

    void test_message_fileds_display_behaviour() {
        const auto defaultMessageFields = Draupnir::Messages::Settings::LogWidget::DisplayedMessageFieldsSetting::defaultValue();
        settingsRegistry.set<Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting>(defaultMessageFields);

        widget = messageSystem.uiBuilder()->createLogWidget();
        bool isBriefDisplayed = defaultMessageFields & Message::Brief;

        // Check if settings were properly loaded
        QCOMPARE(widget->w_messagesListView->isMessageFieldDisplayed(Message::Brief), isBriefDisplayed);
        QCOMPARE(widget->w_messageListViewConfigMenu->isMessageFieldDisplayed(Message::Brief),isBriefDisplayed);

        // Change settings
        widget->w_messageListViewConfigMenu->getActionForField(Message::Brief)->trigger();

        // Check if settings were properly updated
        QCOMPARE(widget->w_messagesListView->isMessageFieldDisplayed(Message::Brief), !isBriefDisplayed);
        const auto settingsValue = settingsRegistry.get<Draupnir::Messages::Settings::LogWidget::DisplayedMessageFieldsSetting>();
        QCOMPARE(settingsValue | Message::Brief, defaultMessageFields);
    }
};

}; // namespace Draupnir::Messages

QTEST_MAIN(Draupnir::Messages::LogWidgetTest)

#include "LogWidgetTest.moc"
