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

#ifndef MESSAGEUIBUILDERINTERFACE_H
#define MESSAGEUIBUILDERINTERFACE_H

class QMenu;
class QWidget;

#include "draupnir/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageUiBuilderInterface draupnir/MessageUiBuilder.h
 *  @ingroup MessageSystem
 *  @brief Abstract interface for GUI builder components of the message system.
 *
 *  @details This class defines the interface for creating GUI widgets used for displaying messages. The implementation
 *           is provided by @ref MessageUiBuilderTemplate class.
 *
 * @todo Add method to get @ref LogWidgetInterface instance.
 * @todo Add method to get @ref MessageNotificationSettingsWidget.
 * @todo Add method(s) to get not QMenu, but NotificationTypeMenu instead.
 * @todo Cover this class (together with MessageUiBuilderTemplate) with tests. */

class MessageUiBuilderInterface
{
public:
    /*! @brief This method should return a @ref LogWidget (as a pointer to QWidget) ready to display messages from
     *         @ref MessageSystemTemplate to which this MessageUiBuilderInterface implementation is belonging.
     *  @param parent Optional parent widget.
     *  @details The returned widget is configured and ready to display messages coming from the corresponding message
     *           handler.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QWidget* createConfiguredLogWidget(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a QMenu containing @ref NotificationTypeMenu entries for all available @ref MessageType
     *  @param parent Optional parent widget.
     * @note It is the caller's responsibility to manage memory properly. */
    virtual QMenu* createConfiguredGlobalNotificationsMenu(QWidget* parent = nullptr) = 0;

    /*! @brief Returns a @ref NotificationTypeMenu (as a pointer to QMenu) that allows changing the notification type
     *         for specified @ref MessageType.
     *  @param messageType Type of message notification for which will be configured by this @ref NotificationTypeMenu.
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

    MessageUiBuilderInterface() = default;
};

}; // namespace Draupnir::Messages

#endif // MESSAGEUIBUILDERINTERFACE_H
