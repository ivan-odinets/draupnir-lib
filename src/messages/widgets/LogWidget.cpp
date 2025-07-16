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

#include "LogWidget.h"

#include <QDebug>
#include <QLabel>
#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QToolTip>
#include <QVBoxLayout>

#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    #include "AppSettings.h"
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS

#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    #include "MessageSettingsInterface.h"
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

#include "../models/MessageListModel.h"
#include "MessageListView.h"

static const QSize defaultIconSize{64,64};
static const uint64_t defaultShownMessageType = MessageType::AllMessages;

#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)
static const QLatin1String iconSize_settingsKey{"ui/messagesIconSize"};
static const QLatin1String shownMessageTypes_settingsKey{"ui/messagesShown"};
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

LogWidget::LogWidget(QWidget* parent) :
    QWidget{parent},
#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)
    p_settings{              nullptr},
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    p_messageListModel{      nullptr},
    w_messagesListView{      new MessageListView},
    w_configureViewButton{   new QToolButton},
    w_detailsMenu{           nullptr},
    w_clearLogButton{        new QPushButton},
    w_iconSizeLabel{         new QLabel},
    w_iconSizeSlider{        new QSlider{Qt::Horizontal}}
{
    // Setup UI
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(w_messagesListView);

    QHBoxLayout* controlButtonsLayout = new QHBoxLayout;
    controlButtonsLayout->addWidget(w_clearLogButton);
    w_configureViewButton->setPopupMode(QToolButton::MenuButtonPopup);
    controlButtonsLayout->addWidget(w_configureViewButton);
    controlButtonsLayout->addStretch();
    controlButtonsLayout->addWidget(w_iconSizeLabel);
    controlButtonsLayout->addWidget(w_iconSizeSlider);
    mainLayout->addLayout(controlButtonsLayout);

    setLayout(mainLayout);

    // Setup initial strings
    _retranslateUi();

    w_messagesListView->setIconSize(defaultIconSize);
    w_messagesListView->applyMessageTypeFilter(defaultShownMessageType);
    w_iconSizeSlider->setMinimum(12);
    w_iconSizeSlider->setMaximum(128);

    // Connect signals and slots
    connect(w_clearLogButton,     &QPushButton::clicked,
            this,                 &LogWidget::_onLogClearClicked);
    connect(w_configureViewButton,&QPushButton::clicked,
            this,                 &LogWidget::_onConfigureClicked);
    connect(w_iconSizeSlider,     &QSlider::valueChanged,
            this,                 &LogWidget::_onIconSizeChange);
    connect(w_iconSizeSlider,     &QSlider::sliderReleased,
            this,                 &LogWidget::_onIconSizeChangeFinished);
    connect(w_messagesListView,   &MessageListView::messageTypeVisibilityChanged,
            this,                 &LogWidget::_onMessageTypeFilterChanged);
}

LogWidget::~LogWidget()
{}

#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)

#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
void LogWidget::loadSettings(AppSettings* settings)
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS
#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
void LogWidget::loadSettings(MessageSettingsInterface* settings)
#endif
{
    Q_ASSERT_X(p_settings == nullptr, "void LogWidget::loadSettings",
               "This method should be called once");
    Q_ASSERT_X(settings != nullptr, "void LogWidget::loadSettings",
               "Provided AppSettings* pointer is nullptr");

    p_settings = settings;

    const QSize messageIconSize = p_settings->value(
        iconSize_settingsKey, defaultIconSize
    ).toSize();

    if (messageIconSize.height() != messageIconSize.width()) {
        qWarning() << iconSize_settingsKey << " value from config file is not square. Using default value and fixing the config file.";
        p_settings->setValue(
            iconSize_settingsKey,
            defaultIconSize
        );
        w_iconSizeSlider->setSliderPosition(defaultIconSize.width());
        w_messagesListView->setIconSize(defaultIconSize);
    } else {
        w_iconSizeSlider->setSliderPosition(messageIconSize.width());
        w_messagesListView->setIconSize(messageIconSize);
    }

    const MessageType messageTypeFilter = p_settings->value(
        shownMessageTypes_settingsKey
    ).toULongLong();

    w_messagesListView->applyMessageTypeFilter(messageTypeFilter);
    w_detailsMenu->displayFilterConfig(w_messagesListView->messageTypeFilter());
}

#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

void LogWidget::setMessageListModel(MessageListModel* model)
{
    Q_ASSERT_X(model, "LogWidget::setMessageListModel",
               "Provided model pointer is nullptr");
    p_messageListModel = model;

    w_messagesListView->setModel(model);
}

void LogWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QWidget::changeEvent(event);
}

void LogWidget::setViewConfigButtonMenu(MessageViewConfigMenu* menu)
{
    w_detailsMenu = menu;
    w_detailsMenu->displayFilterConfig(w_messagesListView->messageTypeFilter());

    connect(menu, &MessageViewConfigMenu::messageTypeVisibilityChanged,
            w_messagesListView, &MessageListView::setMessageTypeDisplayed);

    w_configureViewButton->setMenu(menu);
}

void LogWidget::_onLogClearClicked()
{
    Q_ASSERT_X(p_messageListModel, "LogWidget::_handleLogClearing",
               "Handler must be set");

    p_messageListModel->clear();
}

void LogWidget::_onConfigureClicked()
{
    if (p_viewConfigDialog) {
        p_viewConfigDialog->show();
        return;
    }

    p_viewConfigDialog = createDialog();
    p_viewConfigDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(p_viewConfigDialog, &MessageViewConfigDialog::messageTypeViewChanged,
            w_messagesListView, &MessageListView::setMessageTypeDisplayed);

    connect(w_messagesListView, &MessageListView::messageTypeVisibilityChanged,
            p_viewConfigDialog, &MessageViewConfigDialog::displayTypeSelected);

    p_viewConfigDialog->show();
}

void LogWidget::_onMessageTypeFilterChanged(MessageType,bool)
{
#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)
    p_settings->setValue(
        shownMessageTypes_settingsKey,
        static_cast<qulonglong>(w_messagesListView->messageTypeFilter().id())
    );
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
}

void LogWidget::_onIconSizeChange(int newSize)
{
    w_messagesListView->setIconSize(QSize(newSize,newSize));

    const QPoint pos = w_iconSizeSlider->mapToGlobal(QPoint(w_iconSizeSlider->width() * (newSize - w_iconSizeSlider->minimum()) /
                    (w_iconSizeSlider->maximum() - w_iconSizeSlider->minimum()), w_iconSizeSlider->height() / 2));
    QToolTip::showText(pos, QString::number(newSize), w_iconSizeSlider);
}

void LogWidget::_onIconSizeChangeFinished()
{
#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined(DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)
    const QSize iconSize = w_messagesListView->iconSize();

    p_settings->setValue(
        iconSize_settingsKey,
        iconSize
    );
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
}

void LogWidget::_retranslateUi()
{
    w_configureViewButton->setText("Configure View");
    w_clearLogButton->setText(tr("Clear Log"));
    w_iconSizeLabel->setText(tr("Icon size:"));
}
