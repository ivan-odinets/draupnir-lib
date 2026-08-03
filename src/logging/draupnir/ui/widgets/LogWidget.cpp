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

#include "draupnir/logging/ui/widgets/LogWidget.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>

#include "draupnir/messages/models/MessageListModel.h"
#include "draupnir/messages/ui/widgets/MessageListView.h"

namespace Draupnir::Logging
{

LogWidget::LogWidget(QWidget* parent) :
    QWidget{parent},
    w_messagesListView{new Draupnir::Messages::MessageListView},
    w_iconSizeLabel{new QLabel},
    w_iconSizeSlider{new QSlider{Qt::Horizontal}},
    w_clearLogButton{new QPushButton}
{
    // Setup layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    // Setup UI-subelements: MessageListView
    mainLayout->addWidget(w_messagesListView);
    // Setup control elements:
    QHBoxLayout* controlButtonsLayout = new QHBoxLayout;
    // Clear Log
    controlButtonsLayout->addWidget(w_clearLogButton);
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
    using namespace Draupnir::Logging::Settings::LogWidget;
    w_messagesListView->setIconSize(IconSizeSettingTrait::defaultValue());
    w_messagesListView->setDisplayedMessageCategoriesMask(DisplayedMessageCategoriesSettingTrait::defaultValue());
    w_messagesListView->setDisplayedMessageLevelsMask(DisplayedMessageLevelsSettingTrait::defaultValue());
    w_messagesListView->setDisplayedMessageViewItemFieldsMask(DisplayedMessageViewItemFieldsSettingTrait::defaultValue());
    w_iconSizeSlider->setMinimum(12);
    w_iconSizeSlider->setMaximum(128);

    // Connect signals and slots
    connect(w_iconSizeSlider, &QSlider::valueChanged,
            this, &LogWidget::_onIconSizeChange);
    connect(w_iconSizeSlider, &QSlider::sliderReleased,
            this, &LogWidget::_onIconSizeEditFinished);
}

void LogWidget::setMessageListModel(Draupnir::Messages::MessageListModel* model)
{
    Q_ASSERT_X(model, Q_FUNC_INFO, "Provided model pointer is nullptr");
    w_messagesListView->setModel(model);
}

Draupnir::Messages::MessageListModel* LogWidget::messageListModel()
{
    return static_cast<Draupnir::Messages::MessageListModel*>(w_messagesListView->model());
}

void LogWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    QWidget::changeEvent(event);
}

void LogWidget::_onMessageCategoryFilterChanged(Draupnir::Messages::MessageCategory, bool)
{
    using namespace Draupnir::Logging::Settings::LogWidget;
    m_settingsBundle.template set<DisplayedMessageCategoriesSettingTrait>(
        w_messagesListView->displayedMessageCategoriesMask());
}

void LogWidget::_onMessageFieldVisibilityChanged(Draupnir::Messages::MessageViewItemField::Value, bool)
{
    using namespace Draupnir::Logging::Settings::LogWidget;
    m_settingsBundle.template set<DisplayedMessageViewItemFieldsSettingTrait>(
        w_messagesListView->displayedMessageViewItemFieldsMask());
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
    using namespace Draupnir::Logging::Settings::LogWidget;
    m_settingsBundle.template set<IconSizeSettingTrait>(w_messagesListView->iconSize());
}

void LogWidget::_onLogClearClicked()
{
    // Q_ASSERT_X(p_messageListModel, "LogWidget::_onLogClearClicked",
    //     "MessageListModel must have been set before.");

    // p_messageListModel->clear();
}

void LogWidget::_applyLoadedSettings()
{
    using namespace Draupnir::Logging::Settings::LogWidget;

    // MessageListView -> IconSize
    const QSize size = m_settingsBundle.template get<IconSizeSettingTrait>();
    if (size.height() != size.width()) {
        w_iconSizeSlider->setSliderPosition(IconSizeSettingTrait::defaultValue().height());
        w_messagesListView->setIconSize(IconSizeSettingTrait::defaultValue());
    } else {
        w_iconSizeSlider->setSliderPosition(size.width());
        w_messagesListView->setIconSize(size);
    }

    // MessageListView -> DisplayedMessageCategories
    w_messagesListView->setDisplayedMessageCategoriesMask(m_settingsBundle.get<DisplayedMessageCategoriesSettingTrait>());

    // MessageListView -> DisplayedMessageViewItemFields
    w_messagesListView->setDisplayedMessageViewItemFieldsMask(m_settingsBundle.template get<DisplayedMessageViewItemFieldsSettingTrait>());
}

void LogWidget::_retranslateUi()
{
    w_clearLogButton->setText(tr("Clear Log"));
    w_iconSizeLabel->setText(tr("Icon size:"));
}

}; // namespace Draupnir::Logging
