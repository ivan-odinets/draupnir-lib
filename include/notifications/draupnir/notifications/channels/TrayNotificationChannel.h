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

#ifndef TRAYNOTIFICATIONCHANNEL_H
#define TRAYNOTIFICATIONCHANNEL_H

#include "draupnir/notifications/core/NotificationTypes.h"
#include "draupnir/messages/core/Message.h"

class QSystemTrayIcon;

namespace Draupnir::Notifications
{

/*! @class TrayNotificationChannel draupnir/notifications/channels/TrayNotificationChannel.h
 *  @ingroup Notifications
 *  @brief Notification channel that displays messages through a system tray icon.
 *
 *  @details Provides metadata identifying the tray notification type and implements displaying individual messages and message summaries
 *           using an externally assigned `QSystemTrayIcon`.
 *
 *           The channel does not own the assigned tray icon. The tray icon
 *           must remain valid while it is assigned to the channel.
 */

class TrayNotificationChannel
{
public:
    /*! @struct Trait
     *  @brief Metadata describing the tray notification channel.
     *  @details Provides the notification type identifier, configuration token, and translated user-visible name used by notification
     *           settings and selector widgets. */
    struct Trait {
        /*! @brief Returns the notification type represented by this channel.
         *  @return Tray notification type identifier. */
        static constexpr NotificationType value() { return NotificationType::TrayNotification; }

        /*! @brief Returns the configuration token for this channel.
         *  @return Token used to serialize the tray notification type. */
        static QLatin1String configString() { return QLatin1String{"tray"}; }

        /*! @brief Returns the translated user-visible channel name.
         *  @return Translated name suitable for notification selectors. */
        static QString displayName() { return QObject::tr("Tray"); }
    };

    /*! @brief Constructs a tray notification channel without an assigned tray icon. */
    TrayNotificationChannel() :
        w_trayIcon{nullptr}
    {};

    /*! @brief Displays a tray notification for a message.
     *  @param message Message to display.
     * @pre `message` must not be null.
     * @pre A valid tray icon must be assigned using @ref setTrayIcon.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void showMessage(Draupnir::Messages::MessagePtr message);

    /*! @brief Displays a summary tray notification for multiple messages.
     *  @param messageList Messages represented by the notification.
     *  @pre Every pointer contained in `messageList` must be non-null.
     *  @pre A valid tray icon must be assigned using @ref setTrayIcon.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void showMessageList(const Draupnir::Messages::MessageList& messageList);

    /*! @brief Assigns the system tray icon used to display notifications.
     *  @param trayIcon Tray icon to assign, or `nullptr` to clear the current assignment.
     * @note The channel does not take ownership of `trayIcon`.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void setTrayIcon(QSystemTrayIcon* trayIcon) { w_trayIcon = trayIcon; }

    /*! @brief Returns the currently assigned system tray icon.
     *  @return Assigned tray icon, or `nullptr` if none is assigned. */
    [[nodiscard]] QSystemTrayIcon* trayIcon() { return w_trayIcon; }

private:
    QSystemTrayIcon* w_trayIcon;
};

} // namespace Draupnir::Notifications

#endif // TRAYNOTIFICATIONCHANNEL_H
