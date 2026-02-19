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

#include "draupnir/message_system/ui/widgets/LogWidget.h"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QToolTip>
#include <QVBoxLayout>

#include "draupnir/message_system/models/MessageListModel.h"

#include "draupnir/message_system/core/AbstractMessageUiBuilder.h"
#include "draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h"
#include "draupnir/message_system/ui/widgets/MessageListView.h"
#include "draupnir/message_system/ui/menus/AbstractMessageListViewConfigMenu.h"
#include "draupnir/message_system/ui/windows/MessageSystemConfigDialog.h"

namespace Draupnir::Messages
{

LogWidget::LogWidget(QWidget* parent) :
    QWidget{parent},
    p_messageListModel{nullptr},
    p_uiBuilder{nullptr},
    w_messagesListView{new MessageListView},
    w_configureButton{new QToolButton},
    w_messageListViewConfigMenu{nullptr},
    p_configurationDialog{nullptr},
    w_clearLogButton{new QPushButton},
    w_iconSizeLabel{new QLabel},
    w_iconSizeSlider{new QSlider{Qt::Horizontal}}
{
    _setupGeneralUi();
}

void LogWidget::setMessageListModel(MessageListModel* model)
{
    Q_ASSERT_X(model, "LogWidget::setMessageListModel",
               "Provided model pointer is nullptr");
    p_messageListModel = model;

    w_messagesListView->setModel(model);
}

void LogWidget::setMessageSystemSpecificUiElements(AbstractMessageUiBuilder* uiBuilder)
{
    Q_ASSERT_X(uiBuilder,"LogWidget::setMessageSpecificUiElements",
               "Provided AbstractMessageUiBuilder* pointer is nullptr.");

    p_uiBuilder = uiBuilder;
    w_messageListViewConfigMenu = uiBuilder->createAbstractMessageListViewConfigMenu();

    // Load Settings
    using namespace Draupnir::Messages::Settings::LogWidget;
    const auto messagesMask = m_settingsBundle.template get<DisplayedMessageTypesSetting>();
    w_messageListViewConfigMenu->setDisplayedMessageTypesMask(messagesMask);

    const auto messageFieldsMask = m_settingsBundle.template get<DisplayedMessageFieldsSetting>();
    w_messageListViewConfigMenu->setDisplayedMessageFieldsMask(messageFieldsMask);

    w_configureButton->setMenu(w_messageListViewConfigMenu);
    connect(w_messageListViewConfigMenu, &AbstractMessageListViewConfigMenu::messageTypeVisibilityChanged,
            w_messagesListView, &MessageListView::setMessageTypeDisplayed);
    connect(w_messageListViewConfigMenu, &AbstractMessageListViewConfigMenu::messageFieldVisibilityChanged,
            w_messagesListView, &MessageListView::setMessageFieldDisplayed);
}

void LogWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QWidget::changeEvent(event);
}

void LogWidget::_onLogClearClicked()
{
    Q_ASSERT_X(p_messageListModel, "LogWidget::_onLogClearClicked",
               "MessageListModel must have been set before.");

    p_messageListModel->clear();
}

void LogWidget::_onConfigureClicked()
{
    Q_ASSERT_X(p_uiBuilder,"LogWidget::_onConfigureClicked",
               "LogWidget should be either created using AbstractMessageUiBuilder or it should be attached to MessageSystem"
               " using LogWidget::attachTo method.");
    if (p_configurationDialog) {
        p_configurationDialog->show();
        return;
    }

    p_configurationDialog = p_uiBuilder->createAbstractMessageSystemConfigDialog();
    p_configurationDialog->setWindowIcon(qApp->windowIcon());
    p_configurationDialog->setWindowTitle(tr("Configure Messages - ").append(qApp->applicationName()));
    p_configurationDialog->setAttribute(Qt::WA_DeleteOnClose);
    p_configurationDialog->loadSettings(&m_settingsBundle);

    // Display current config of the LogWidget
    p_configurationDialog->setDisplayedMessageFieldsMask(w_messagesListView->displayedMessageFieldsMask());
    p_configurationDialog->setDisplayedMessageTypesMask(w_messagesListView->displayedMessageTypesMask());

    // Connect signals and slots for message fields
    connect(p_configurationDialog, &MessageSystemConfigDialog::messageFieldVisibilityChanged,
            w_messagesListView, &MessageListView::setMessageFieldDisplayed);
    connect(w_messagesListView, &MessageListView::messageFieldVisibilityChanged,
            p_configurationDialog, &MessageSystemConfigDialog::setMessagePartDisplayed);

    // Connect signals and slots for message types
    connect(p_configurationDialog, &MessageSystemConfigDialog::messageTypeVisibilityChanged,
            w_messagesListView, &MessageListView::setMessageTypeDisplayed);
    connect(w_messagesListView, &MessageListView::messageTypeVisibilityChanged,
            p_configurationDialog, &MessageSystemConfigDialog::setMessageTypeDisplayed);

    p_configurationDialog->show();
}

void LogWidget::_applyLoadedSettings()
{
    using namespace Draupnir::Messages::Settings::LogWidget;

    // MessageListView -> IconSize
    const QSize size = m_settingsBundle.template get<IconSizeSetting>();
    if (size.height() != size.width()) {
        w_iconSizeSlider->setSliderPosition(IconSizeSetting::defaultValue().height());
        w_messagesListView->setIconSize(IconSizeSetting::defaultValue());
    } else {
        w_iconSizeSlider->setSliderPosition(size.width());
        w_messagesListView->setIconSize(size);
    }

    // MessageListView -> DisplayedMessageTypes
    const quint64 messagesMask = m_settingsBundle.template get<DisplayedMessageTypesSetting>();
    w_messagesListView->setDisplayedMessageTypesMask(messagesMask);

    // MessageListView -> DisplayedMessageFields
    const auto messageFieldsMask = m_settingsBundle.template get<DisplayedMessageFieldsSetting>();
    w_messagesListView->setDisplayedMessageFieldsMask(messageFieldsMask);
}

void LogWidget::_setupGeneralUi()
{
    // Setup layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    // Setup UI-subelements: MessageListView
    mainLayout->addWidget(w_messagesListView);
    // Setup control elements:
    QHBoxLayout* controlButtonsLayout = new QHBoxLayout;
    // Clear Log
    controlButtonsLayout->addWidget(w_clearLogButton);
    // Configure MessageSystem button
    w_configureButton->setPopupMode(QToolButton::MenuButtonPopup);
    controlButtonsLayout->addWidget(w_configureButton);
    // Stretch
    controlButtonsLayout->addStretch();
    // Icon size related things
    controlButtonsLayout->addWidget(w_iconSizeLabel);
    controlButtonsLayout->addWidget(w_iconSizeSlider);
    // Add control elements layout
    mainLayout->addLayout(controlButtonsLayout);
    // Apply Layout
    setLayout(mainLayout);
    // Setup initial strings
    _retranslateUi();

    // Display default settings values
    using namespace Draupnir::Messages::Settings::LogWidget;
    w_messagesListView->setIconSize(IconSizeSetting::defaultValue());
    w_messagesListView->setDisplayedMessageTypesMask(DisplayedMessageTypesSetting::defaultValue());
    w_messagesListView->setDisplayedMessageFieldsMask(DisplayedMessageFieldsSetting::defaultValue());
    w_iconSizeSlider->setMinimum(12);
    w_iconSizeSlider->setMaximum(128);

    // Connect signals and slots
    connect(w_clearLogButton, &QPushButton::clicked,
            this, &LogWidget::_onLogClearClicked);
    connect(w_configureButton,&QPushButton::clicked,
            this, &LogWidget::_onConfigureClicked);
    connect(w_messagesListView, &MessageListView::messageTypeVisibilityChanged,
            this, &LogWidget::_onMessageTypeFilterChanged);
    connect(w_messagesListView, &MessageListView::messageFieldVisibilityChanged,
            this, &LogWidget::_onMessageFieldVisibilityChanged);
    connect(w_iconSizeSlider, &QSlider::valueChanged,
            this, &LogWidget::_onIconSizeChange);
    connect(w_iconSizeSlider, &QSlider::sliderReleased,
            this, &LogWidget::_onIconSizeEditFinished);
}

void LogWidget::_retranslateUi()
{
    w_configureButton->setText("Configure");
    w_clearLogButton->setText(tr("Clear Log"));
    w_iconSizeLabel->setText(tr("Icon size:"));
}

void LogWidget::_onMessageTypeFilterChanged(MessageType,bool)
{
    using namespace Draupnir::Messages::Settings::LogWidget;
    m_settingsBundle.template set<DisplayedMessageTypesSetting>(w_messagesListView->displayedMessageTypesMask().id());
}

void LogWidget::_onMessageFieldVisibilityChanged(Draupnir::Messages::MessageField, bool)
{
    using namespace Draupnir::Messages::Settings::LogWidget;
    m_settingsBundle.template set<DisplayedMessageFieldsSetting>(w_messagesListView->displayedMessageFieldsMask());
}

void LogWidget::_onIconSizeChange(int newSize)
{
    w_messagesListView->setIconSize(QSize(newSize,newSize));

    const QPoint pos = w_iconSizeSlider->mapToGlobal(QPoint(w_iconSizeSlider->width() * (newSize - w_iconSizeSlider->minimum()) /
                    (w_iconSizeSlider->maximum() - w_iconSizeSlider->minimum()), w_iconSizeSlider->height() / 2));
    QToolTip::showText(pos, QString::number(newSize), w_iconSizeSlider);
}

void LogWidget::_onIconSizeEditFinished()
{
    using namespace Draupnir::Messages::Settings::LogWidget;
    m_settingsBundle.template set<IconSizeSetting>(w_messagesListView->iconSize());
}

}; // namespace Draupnir::Messages
