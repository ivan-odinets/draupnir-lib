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

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>

#include <QPointer>

class QLabel;
class QPushButton;
class QSlider;
class QToolButton;

#include "draupnir/SettingsRegistry.h"

#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h"

namespace Draupnir::Messages
{

class AbstractMessageListViewConfigMenu;
class AbstractMessageUiBuilder;
class MessageSystemConfigDialog;
class MessageListModel;
class MessageListView;

}; // namespace Draupnir::Messages

namespace Draupnir::Messages
{

/*! @class LogWidget draupnir/message_system/ui/widgets/LogWidget.h
 *  @ingroup MessageSystem
 *  @brief A composite `QWidget` used for displaying and managing logged @ref Draupnir::Messages::Message objects in a
 *         GUI.
 *
 *  @details This widget provides a complete GUI for viewing and filtering @ref Draupnir::Messages::Message objects logged
 *           using @ref Logger object. Internally it uses a @ref Draupnir::Messages::MessageListView to display messages
 *           stored within @ref Draupnir::Messages::MessageListModel of @ref Draupnir::Messages::AbstractMessageHandler /
 *           @ref Draupnir::Messages::MessaheHandlerTemplate and exposes controls for:
 *           - Clearing the log (via `QPushButton`)
 *           - Configuring the @ref Draupnir::Messages::MessageListView and notification settings of the associated @ref
 *             Draupnir::Messages::AbstractMessageHandler / @ref Draupnir::Messages::MessageHandlerTemplate.
 *
 * @todo After refractoring the AbstractMessageUiBuilder / MessageUiBuilderTemplate - this thing should be refractored as
 *       well. */

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Alias for @ref Draupnir::Settings::SettingsBundleTemplate instantiation, which holds settings traits being
     *         used by this @ref LogWidget. */
    using SettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
        Draupnir::Messages::Settings::LogWidget::IconSizeSetting,
        Draupnir::Messages::Settings::LogWidget::DisplayedMessageFieldsSetting,
        Draupnir::Messages::Settings::LogWidget::DisplayedMessageTypesSetting
    >;

    /*! @brief Trivial destructor. */
    ~LogWidget() override = default;

    /*! @brief This method loads the settings for this @ref Draupnir::Messages::AbstractLogWidget object from the specified
     *         SettingsSource.
     * @todo Add logging of the warnings if sth is wrong with the settings. */
    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>(),
                "SettingsSource provided can not populate the SettingsBundle for this AbstractLogWidget.");
        Q_ASSERT_X(source,"AbstractLogWidget<MessageTraits...>::loadSettings",
                   "Specified source is nullptr");
        Q_ASSERT_X(!m_settingsBundle.isValid(),"AbstractLogWidget<MessageTraits...>::loadSettings",
                   "This method must be called only once.");

        m_settingsBundle = source->template getSettingsBundle<SettingsBundle>();
        _applyLoadedSettings();
    }

    /*! @brief Sets the @ref Draupnir::Messages::MessageListModel to be displayed in the internal @ref Draupnir::Messages::MessageListView.
     *  @param model Pointer to a valid @ref Draupnir::Messages::MessageListModel. */
    void setMessageListModel(MessageListModel* model);

    /*! @brief Returns currently used @ref Draupnir::Messages::MessageListModel.
     * @note Displaying the messages from the model is done through the @ref Draupnir::Messages::MessageListProxyModel (
     *       within the @ref Draupnir::Messages::MessageListView widget). This method will return **the original** model
     *       passed to @ref Draupnir::Messages::LogWidget, but **not the** @ref MessageListProxyModel being used internally. */
    MessageListModel* messageListModel() { return p_messageListModel; }

protected:
    template<class... MessageTypes>
    friend class MessageUiBuilderTemplate;

    /*! @brief Constructs a @ref Draupnir::Messages::LogWidget and initializes its internal layout and controls.
     *  @param uiBuilder Required pointer to the @ref Draupnir::Messages::AbstractMessageUiBuilder interface, which will
     *         be used to get some of the nested UI elements within the @ref Draupnir::Messages::LogWidget.
     *  @param parent Optional parent widget. */
    explicit LogWidget(AbstractMessageUiBuilder* uiBuilder, QWidget* parent = nullptr);

    /*! @brief Handles dynamic retranslation when the application language changes.
     *  @param event The event being handled.
     *  @details Updates all visible labels and translated strings in the UI. */
    void changeEvent(QEvent* event) final;

private slots:
    /*! @brief This private slot handles pressing the "Clear Log" button. */
    void _onLogClearClicked();

    /*! @brief This private slot handles pressing the "Configure" button. */
    void _onConfigureClicked();

    /*! @brief This private slot handles changes of displayed message types. */
    void _onMessageTypeFilterChanged(MessageType,bool);

    /*! @brief This private slot handles changes of displayed message fields. */
    void _onMessageFieldVisibilityChanged(Draupnir::Messages::Message::Fields field, bool isDisplayed);

    /*! @brief This private slot handles movement of the icon size selector slider. */
    void _onIconSizeChange(int newSize);

    /*! @brief This private slot handles QSlider::sliderReleased signal from icon selector. Saves the selected icon size
     *         to settings. */
    void _onIconSizeEditFinished();

private:
    void _applyLoadedSettings();
    void _retranslateUi();

    // Non-displayed fields
    SettingsBundle m_settingsBundle;
    MessageListModel* p_messageListModel;
    AbstractMessageUiBuilder* p_uiBuilder;

    // UI-subelements
    MessageListView* w_messagesListView;
    QToolButton* w_configureButton;
    AbstractMessageListViewConfigMenu* w_messageListViewConfigMenu;
    QPointer<MessageSystemConfigDialog> p_configurationDialog;
    QPushButton* w_clearLogButton;
    // Icon size changers
    QLabel* w_iconSizeLabel;
    QSlider* w_iconSizeSlider;
};

}; // namespace Draupnir::Messages

#endif // LOGWIDGET_H
