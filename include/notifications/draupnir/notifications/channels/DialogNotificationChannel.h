/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef DIALOGNOTIFICATIONCHANNEL_H
#define DIALOGNOTIFICATIONCHANNEL_H

#include <QApplication>

#include "draupnir/notifications/core/NotificationTypes.h"
#include "draupnir/messages/ui/windows/MessageDisplayDialog.h"

namespace Draupnir::Notifications
{

/*! @class DialogNotificationChannel draupnir/notifications/channels/DialogNotificationChannel.h
 *  @ingroup Notifications
 *  @brief Notification channel that displays messages in dialogs.
 *
 *  @details Provides metadata identifying the dialog notification type and implements displaying individual messages and message
 *           lists using @ref Draupnir::Messages::MessageDisplayDialog.
 *
 *           Each created dialog owns its displayed message widgets and deletes itself when closed. */

class DialogNotificationChannel
{
public:
    /*! @struct Trait
     *  @brief Metadata describing the dialog notification channel.
     *
     *  @details Provides the notification type identifier, configuration token, and translated user-visible name used by notification
     *           settings and selector widgets. */
    struct Trait {
        /*! @brief Returns the notification type represented by this channel.
         *  @return Dialog notification type identifier. */
        static constexpr NotificationType value() { return NotificationType::DialogNotification; }

        /*! @brief Returns the configuration token for this channel.
         *  @return Token used to serialize the dialog notification type. */
        static QLatin1String configString() { return QLatin1String{"dialog"}; }

        /*! @brief Returns the translated user-visible channel name.
         *  @return Translated name suitable for notification selectors. */
        static QString displayName() { return QObject::tr("Dialog"); }
    };

    /*! @brief Displays a message in a newly created dialog.
     *  @param message Message to display.
     *  @details Creates a self-deleting @ref Draupnir::Messages::MessageDisplayDialog, adds `message` to it, and displays the
     *           dialog.
     * @pre `message` must not be null.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    static void showMessage(Draupnir::Messages::MessagePtr message);

    /*! @brief Displays messages in a newly created dialog.
     *  @param messageList Messages to display.
     *  @details Creates a self-deleting @ref Draupnir::Messages::MessageDisplayDialog, adds all messages from `messageList`,
     *           and displays the dialog.
     * @pre Every pointer contained in `messageList` must be non-null.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    static void showMessageList(const Draupnir::Messages::MessageList& messageList);

private:
    /*! @brief Creates and configures a message display dialog.
     *  @param title Window title assigned to the created dialog.
     *  @return Newly allocated dialog configured to delete itself when closed.
     *  @details The caller is responsible for populating and showing the returned dialog. */
    [[nodiscard]] static Draupnir::Messages::MessageDisplayDialog* _createMessageDialog(const QString& title);
};

}; // namespace Draupnir::Notifications

#endif // DIALOGNOTIFICATIONCHANNEL_H
