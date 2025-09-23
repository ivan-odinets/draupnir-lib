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

#ifndef MESSAGEUIBUILDER_H
#define MESSAGEUIBUILDER_H

class QMenu;
class QWidget;

#include "core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageUiBuilder draupnir-lib/include/messages/MessageUiBuilder.h
 *  @brief Abstract interface for GUI builder components of the message system.
 *  @details This class defines the interface for creating GUI widgets used for displaying messages. The implementation is provided
 *           by MessageUiBuilderTemplate class.
 *
 * @see MessageUiBuilderTemplate, MessageSystemTemplate. */

class MessageUiBuilder
{
public:
    /*! @brief This method should return a LogWidget (as a pointer to QWidget) ready to be used with MessageSystemTemplate
     *         to which this MessageUiBuilder implementation is belonging.
     *  @details The returned widget is configured and ready to display messages coming from the corresponding message handler.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QWidget* createConfiguredLogWidget(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a QMenu containing NotificationTypeMenu entries for all available MessageType
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QMenu* createConfiguredGlobalNotificationsMenu(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a NotificationTypeMenu (as a pointer to QMenu) that allows changing the notification type for specified
     *         MessageType.
     *  @param messageType Type of message notification for which will be configured by this NotificationTypeMenu.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QMenu* createConfiguredNotificationMenu(MessageType messageType, QWidget* parent = nullptr) const = 0;

    /*! @brief Returns a ready-to-use widget for configuring notification settings within this MessageSystem.
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QWidget* createConfiguredNotificationSettingsWidget(QWidget* parent = nullptr) const = 0;

protected:
    template<class... MessageTypes>
    friend class MessageSystemTemplate;

    MessageUiBuilder() = default;
};

}; // namespace Draupnir::Messages

#endif // MESSAGEUIBUILDER_H
