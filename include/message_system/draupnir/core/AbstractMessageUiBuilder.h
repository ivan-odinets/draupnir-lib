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

#ifndef ABSTRACTMESSAGEUIBUILDER_H
#define ABSTRACTMESSAGEUIBUILDER_H

class QMenu;
class QWidget;

#include "draupnir/core/MessageType.h"
#include "draupnir/ui/menus/NotificationTypeMenu.h"

namespace Draupnir::Messages
{

class LogWidget;
class MessageSystemConfigDialog;
class AbstractMessageTypesSelectorWidget;
class AbstractMessageListViewConfigMenu;
class NotificationTypeMenu;
class AbstractNotificationSettingsWidget;

/*! @class AbstractMessageUiBuilder draupnir/AbstractMessageUiBuilder.h
 *  @ingroup MessageSystem
 *  @brief Abstract base class for GUI builder components of the message system.
 *
 *  @details This class defines the interface for creating GUI widgets used for displaying messages. The implementation
 *           is provided by @ref MessageUiBuilderTemplate class. */

class AbstractMessageUiBuilder
{
public:
    /*! @brief This method should return a @ref LogWidget which is ready to display messages from @ref MessageSystemTemplate
     *         to which this @ref AbstractMessageUiBuilder implementation is belonging.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual LogWidget* createLogWidget(QWidget* parent = nullptr) = 0;

    /*! @brief This method should return a @ref LogWidget (as a pointer to QWidget) ready to display messages
     *         from @ref MessageSystemTemplate to which this @ref AbstractMessageUiBuilder implementation is belonging.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QWidget* createLogWidgetAsQWidget(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a @ref NotificationTypeMenu that allows changing the notification type for specified @ref MessageType.
     *  @param messageType Type of message notification for which will be configured by this @ref NotificationTypeMenu.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual NotificationTypeMenu* createNotificationSettingsMenu(MessageType messageType, QWidget* parent = nullptr) = 0;

    /*! @brief Returns a QMenu containing @ref NotificationTypeMenu entries for specified @ref MessageType entries. @ref MessageType
     *         entries must be specified as combination of the flags passed as variable of type @ref MessageType.
     *  @param messageType Optional type of messages which notifications will be handled by this menu.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QMenu* createNotificationSettingsMenuForTypes(MessageType messageType = MessageType::AllMessages, QWidget* parent = nullptr) = 0;

protected:
    template<class... MessageTypes>
    friend class MessageSystemTemplate;
    friend class LogWidget;
    friend class MessageSystemConfigDialog;

    friend class MessageUiBuilderTemplateIT;

    AbstractMessageUiBuilder() = default;

    /*! @brief This method should return the @ref Draupnir::Messages::MessageListViewConfigMenuTemplate as a pointer to the
     *         @ref Draupnir::Messages::AbstractMessageListViewConfigMenu, which can be used to configure different parameters
     *         of @ref Draupnir::Messages::MessageListView widget.
     *  @details This method is used by the @ref Draupnir::Messages::LogWidget internally to allow user configuration of MessageListView. */
    virtual AbstractMessageListViewConfigMenu* createAbstractMessageListViewConfigMenu(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a ready-to-use widget for configuring notification settings within this MessageSystem.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual AbstractNotificationSettingsWidget* createNotificationSettingsWidgetAsInterface(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a ready-to-user widget for selecting specific MessageTypes. */
    virtual AbstractMessageTypesSelectorWidget* createAbstractMessageTypesSelectorWidget(QWidget* parent = nullptr) = 0;

    /*! @brief Returns MessageSystemConfigDialog capable of editing some of the settings within the MessageSystem. */
    virtual MessageSystemConfigDialog* createAbstractMessageSystemConfigDialog(QWidget* parent = nullptr) = 0;
};

}; // namespace Draupnir::Messages

#endif // ABSTRACTMESSAGEUIBUILDER_H
