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

class QComboBox;
class QLabel;
class QListView;
class QPushButton;
class QSlider;
class QToolButton;

#include "MessageType.h"

class AppSettings;
class MessageListModel;
class MessageListView;
class MessageHandler;
class MessageSettingsInterface;
class MessageViewConfigDialog;
class MessageViewConfigMenu;

/*! @class LogWidget draupnir-lib/src/messages/widgets/LogWidget.h
 *  @brief A composite QWidget used for displaying and managing logged Message objects in a GUI.
 *  @details This widget provides a complete GUI for viewing and filtering `Message` objects in the application. Internally it
 *           uses a `MessageListView` to display messages stored within MessageListModel of MessageHandler, and exposes controls
 *           for:
 *           - Clearing the log (via QPushButton)
 *           - Configuring the MessageListView (via config dialog and QToolButton dropdown menu)
 *           - Adjusting icon size (via QSlider)
 *
 *           This widget consists of two parts: LogWidget itself, providing QWidget-compatable interface to be inserted in the
 *           GUI and LogWidgetTemplate - which is used to "forward" message types registered via MessageSystemTemplate to the
 *           nested elements of the LogWidget. LogWidgetTemplate is responsible for message-type-specific parts of the LogWidget.
 *
 *           LogWidget supports dynamic retranslation and can persist user preferences using `AppSettings`.
 *
 * @note Before use, `loadSettings()` should be called to configure the widget.
 *
 * @see MessageHandler, Message, MessageListModel, MessageListProxyModel, MessageListView.
 * @todo Warnings if settings are wrong.
 * @todo Implement fucking settings prefixes. */

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Constructs a LogWidget and initializes its internal layout and controls.
     *  @param parent Optional parent widget. */
    explicit LogWidget(QWidget* parent = nullptr);

    /*! @brief Destructor. */
    ~LogWidget();

#if defined(DRAUPNIR_MSGSYS_APP_SETTINGS) || defined (DRAUPNIR_MSGSYS_CUSTOM_SETTINGS)

    /*! @brief Loads settings for the LogWidget from the given settings object.
     *  @param settings Pointer to the AppSettings or MessageSettingsInterface instance.
     * @note This method will be present when either DRAUPNIR_MSGSYS_APP_SETTINGS or DRAUPNIR_MSGSYS_CUSTOM_SETTINGS macros are enabled.
     *       When first macro is enabled - this method will accept AppSettings pointer. When second - MessageSystemInterface. */
#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    void loadSettings(AppSettings* settings);
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS

#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    void loadSettings(MessageSettingsInterface* settings);
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

#endif // DRAUPNIR_MSGSYS_APP_SETTINGS || DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

    /*! @brief Sets the MessageListModel to be displayed in the message list view.
     *  @param model Pointer to a valid MessageListModel. */
    void setMessageListModel(MessageListModel* model);

protected:
    /*! @brief Handles dynamic retranslation when the application language changes.
     *  @details Updates all visible labels and translated strings in the UI.
     *  @param event The event being handled. */
    void changeEvent(QEvent* event) final;

    /*! @brief This protected method is used by the LogWidgetTemplate to specify the menu used for configuration the
     *         MessageListView. */
    void setViewConfigButtonMenu(MessageViewConfigMenu* menu);

    /*! @brief This method is implemented by the LogWidgetTemplate class and returns a pointer to the dialog, which can
     *         be used to configure the MessageListView. */
    virtual MessageViewConfigDialog* createDialog(QWidget* parent = nullptr) = 0;

private slots:
    void _onLogClearClicked();
    void _onConfigureClicked();
    void _onMessageTypeFilterChanged(MessageType,bool);
    void _onIconSizeChange(int newSize);
    void _onIconSizeChangeFinished();

private:
    void _retranslateUi();

#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    AppSettings* p_settings;
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS

#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    MessageSettingsInterface*          p_settings;
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

    MessageListModel*                  p_messageListModel;

    MessageListView*                   w_messagesListView;
    // MessageListView configurator
    QToolButton*                       w_configureViewButton;
    MessageViewConfigMenu*             w_detailsMenu;
    QPointer<MessageViewConfigDialog>  p_viewConfigDialog;
    QPushButton*                       w_clearLogButton;
    // Icon size changers
    QLabel*                            w_iconSizeLabel;
    QSlider*                           w_iconSizeSlider;
};

#include "../menus/MessageViewConfigMenu.h"
#include "../windows/MessageViewConfigDialog.h"

/*! @class LogWidgetTemplate draupnir-lib/src/messages/widgets/LogWidget.h
 *  @brief This template class is used to receive information about supported message types from MessageUiBuilderTemplate and
 *         implements message-type-specific functionality of the LogWidget.
 * @see LogWidget, MessageUiBuilder */

template<class... MessageTraits>
class LogWidgetTemplate final : public LogWidget
{
public:
    /*! @brief Constructor. Initializes the base class (LogWidget) and calls the LogWidget::setViewConfigButtonMenu method
     *         to set the menu capable to configure the MessageListView. */
    explicit LogWidgetTemplate(QWidget* parent = nullptr) :
        LogWidget{parent}
    {
        setViewConfigButtonMenu(new MessageViewConfigMenuTemplate<MessageTraits...>);
    }

    /*! @brief Trivial final destructor. */
    ~LogWidgetTemplate() final = default;

protected:
    /*! @brief Implementation of the LogWidget::createDialog method. Returns the dialog which can be used to configure the
     *         MessageListView of the LogWidget.
     * @note Memory management is done within LogWidget. */
    MessageViewConfigDialog* createDialog(QWidget* parent = nullptr) {
        return new MessageViewConfigDialogTemplate<MessageTraits...>{parent};
    }
};

#endif // LOGWIDGET_H
