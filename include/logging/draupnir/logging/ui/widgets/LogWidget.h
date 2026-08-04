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

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>

#include "draupnir/messages/core/MessageViewItemFields.h"
#include "draupnir/messages/categories/MessageCategories.h"
#include "draupnir/logging/traits/settings/log_widget/DisplayedMessageCategoriesSettingTrait.h"
#include "draupnir/logging/traits/settings/log_widget/DisplayedMessageLevelsSettingTrait.h"
#include "draupnir/logging/traits/settings/log_widget/IconSizeSetting.h"
#include "draupnir/logging/traits/settings/log_widget/DisplayedMessageViewItemFieldsSettingTrait.h"
#include "draupnir/settings_registry/core/SettingsBundleTemplate.h"

class QLabel;
class QPushButton;
class QSlider;
class QToolButton;

namespace Draupnir::Messages
{

class MessageListModel;
class MessageListView;

};

namespace Draupnir::Logging
{

/*! @class LogWidget draupnir/logging/ui/widgets/LogWidget.h
 *  @ingroup Logging
 *  @brief A composite `QWidget` used for displaying and managing logged @ref Draupnir::Logging::Message objects in a
 *         GUI.
 *
 *  @details This widget provides a complete GUI for viewing and filtering @ref Draupnir::Logging::Message objects logged
 *           using @ref Logger object. Internally it uses a @ref Draupnir::Logging::MessageListView to display messages
 *           stored within @ref Draupnir::Logging::MessageListModel.
 *
 *           Following controls are available:
 *           - Configuring the @ref Draupnir::Messages::MessageListView and notification settings of the associated @ref
 *             Draupnir::Messages::AbstractMessageHandler / @ref Draupnir::Messages::MessageHandlerTemplate. */

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Alias for @ref Draupnir::Settings::SettingsBundleTemplate instantiation, which holds settings traits being
     *         used by this @ref LogWidget. */
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Logging::Settings::LogWidget::IconSizeSettingTrait,
        Draupnir::Logging::Settings::LogWidget::DisplayedMessageViewItemFieldsSettingTrait,
        Draupnir::Logging::Settings::LogWidget::DisplayedMessageCategoriesSettingTrait
    >;

    /*! @brief Default constructor. Accepts pointer to parent `QWidget` object and creates @ref LogWidget, which needs to be
     *         configured.
     *  @param parent - optional parent `QWidget`.
     *  @details When constructing @ref LogWidget by using this constructor - resulting widget is requires additional configuration
     *           to work properly. This includes:
     *           - Loading of the settings (using @ref LogWidget::loadSettings method);
     *           - Specifying @ref Draupnir::Logging::MessageListModel, messages from which will be displayed using internal
     *             @ref Draupnir::Logging::MessageListView (using @ref LogWidget::setMessageListModel); */
    explicit LogWidget(QWidget* parent = nullptr);

    /*! @brief Trivial destructor. */
    ~LogWidget() override = default;

    /*! @brief This method loads the settings for this @ref LogWidget object from the specified `SettingsSource`.
     * @todo Add logging of the warnings if sth is wrong with the settings.
     * @todo As other `loadSettings`-methods this needs to be standartized. */
    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>(),
            "Provided SettingsSource can not populate the SettingsBundle for this LogWidget.");
        Q_ASSERT_X(source, Q_FUNC_INFO, "Specified source is nullptr");
        Q_ASSERT_X(!m_settingsBundle.isValid(), Q_FUNC_INFO, "This method must be called only once.");

        m_settingsBundle = source->template getSettingsBundle<SettingsBundle>();
        _applyLoadedSettings();
    }

    /*! @brief Sets the @ref Draupnir::Logging::MessageListModel to be displayed in the internal @ref Draupnir::Logging::MessageListView.
     *  @param model Pointer to a valid @ref Draupnir::Logging::MessageListModel. */
    void setMessageListModel(Draupnir::Messages::MessageListModel* model);

    /*! @brief Returns currently used @ref Draupnir::Messages::MessageListModel.
     * @note Displaying the messages from the model is done through the @ref Draupnir::Messages::MessageListProxyModel (
     *       within the @ref Draupnir::Logging::MessageListView widget). This method will return **the original** model
     *       passed to @ref Draupnir::Logging::LogWidget, but **not the** @ref MessageListProxyModel being used internally. */
    Draupnir::Messages::MessageListModel* messageListModel();

protected:
    /*! @brief Handles dynamic retranslation when the application language changes.
     *  @param event The event being handled.
     *  @details Updates all visible labels and translated strings in the UI. */
    void changeEvent(QEvent* event) final;

private slots:
    /*! @brief This private slot handles changes of displayed message types. */
    void _onMessageCategoryFilterChanged(Draupnir::Messages::MessageCategory, bool);

    /*! @brief This private slot handles changes of displayed message fields. */
    void _onMessageFieldVisibilityChanged(Draupnir::Messages::MessageViewItemField::Value field, bool isDisplayed);

    /*! @brief This private slot handles movement of the icon size selector slider. */
    void _onIconSizeChange(int newSize);

    /*! @brief This private slot handles QSlider::sliderReleased signal from icon selector. Saves the selected icon size
     *         to settings. */
    void _onIconSizeEditFinished();

    /*! @brief This private slot handles pressing the "Clear Log" button. */
    void _onLogClearClicked();

private:
    friend class LogWidgetTest;

    void _applyLoadedSettings();
    void _retranslateUi();

    // Non-displayed fields
    SettingsBundle m_settingsBundle;
//    MessageListModel* p_messageListModel;

    // UI-subelements
    Draupnir::Messages::MessageListView* w_messagesListView;
    // Icon size changers
    QLabel* w_iconSizeLabel;
    QSlider* w_iconSizeSlider;

    QPushButton* w_clearLogButton;
};

}; // namespace Draupnir::Logging

#endif // LOGWIDGET_H
