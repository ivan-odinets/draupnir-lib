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

#ifndef TRAYNOTIFICATIONHANDLER_H
#define TRAYNOTIFICATIONHANDLER_H

#include "draupnir/notifications/handlers/NotificationHandlerTemplate.h"

#include <QSystemTrayIcon>

#include "draupnir/logging/messages/Message.h"
#include "draupnir/logging/messages/MessageViewItem.h"
#include "draupnir/notifications/traits/notifications/TrayNotificationTrait.h"

namespace Draupnir::Notifications
{

/*! @brief Displays notifications using a system tray icon.
 *  @ingroup Notifications
 *
 *  @details Specializes @ref Draupnir::Notifications::NotificationHandlerTemplate for @ref Draupnir::Notifications::TrayNotificationTrait.
 *           A tray icon must be assigned before displaying notifications. */

template<>
class NotificationHandlerTemplate<TrayNotificationTrait>
{
public:
    /*! @brief Constructs a tray notification handler without an assigned tray icon. */
    NotificationHandlerTemplate() :
        w_trayIcon{nullptr}
    {};

    /*! @brief Displays a tray notification for a single message.
     *  @param message Message to display. Must not be null. */
    void showMessage(Draupnir::Logging::Message* message) {
        Q_ASSERT(message);
        Q_ASSERT(w_trayIcon);
        w_trayIcon->showMessage(
            message->brief(),
            message->what(),
            Draupnir::Logging::MessageViewItem::iconFor(message->type())
        );
    };

    /*! @brief Displays a summary tray notification for a list of messages.
     *  @param messageList Messages represented by the notification. */
    void showMessageList(const Draupnir::Logging::MessageList& messageList) {
#ifndef QT_NO_DEBUG
        // Just to be sure that no nullptrs are here
        for (Draupnir::Logging::Message* message : messageList)
            Q_ASSERT(message);
#endif // QT_NO_DEBUG
        Q_ASSERT(w_trayIcon);

        w_trayIcon->showMessage(
            qApp->applicationName(),
            QObject::tr("There are %1 new events. Check log for details.").arg(messageList.count()),
            QSystemTrayIcon::Information
        );
    };

    /*! @brief Sets the system tray icon used to display notifications.
     *  @param trayIcon Tray icon to use. Must remain valid while assigned. */
    void setTrayIcon(QSystemTrayIcon* trayIcon) { w_trayIcon = trayIcon; }

    /*! @brief Returns the currently assigned system tray icon.
     *  @return Assigned tray icon, or nullptr if none has been set. */
    QSystemTrayIcon* trayIcon() { return w_trayIcon; }

private:
    QSystemTrayIcon* w_trayIcon;
};

}; // namespace Draupnir::Notifications

#endif // TRAYNOTIFICATIONHANDLER_H
