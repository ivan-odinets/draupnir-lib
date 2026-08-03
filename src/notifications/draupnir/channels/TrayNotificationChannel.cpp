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

#include "draupnir/notifications/channels/TrayNotificationChannel.h"

#include <QSystemTrayIcon>

#include "draupnir/messages/core/MessageViewItem.h"

namespace Draupnir::Notifications
{

void TrayNotificationChannel::showMessage(Draupnir::Messages::MessagePtr message)
{
    Q_ASSERT(message);
    Q_ASSERT(w_trayIcon);
    w_trayIcon->showMessage(
        message->brief(),
        message->what(),
        Draupnir::Messages::MessageViewItem::iconFor(message->type())
    );
};

void TrayNotificationChannel::showMessageList(const Draupnir::Messages::MessageList& messageList)
{
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Draupnir::Messages::MessagePtr message : messageList)
        Q_ASSERT(message);
#endif // QT_NO_DEBUG
    Q_ASSERT(w_trayIcon);

    w_trayIcon->showMessage(
        qApp->applicationName(),
        QObject::tr("There are %1 new events. Check log for details.").arg(messageList.count()),
        QSystemTrayIcon::Information
    );
};

} // namespace Draupnir::Notifications
