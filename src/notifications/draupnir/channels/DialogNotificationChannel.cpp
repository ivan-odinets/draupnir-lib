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

#include "draupnir/notifications/channels/DialogNotificationChannel.h"

namespace Draupnir::Notifications
{

void DialogNotificationChannel::showMessage(Draupnir::Messages::MessagePtr message)
{
    Q_ASSERT(message);

    auto dialog = _createMessageDialog(message->brief() + QLatin1String{" - "} + qApp->applicationName());
    dialog->addMessage(message);
    dialog->show();
};

void DialogNotificationChannel::showMessageList(const Draupnir::Messages::MessageList& messageList)
{
#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Draupnir::Messages::MessagePtr message : messageList)
        Q_ASSERT(message);
#endif // QT_NO_DEBUG

    using namespace Draupnir::Messages;
    auto dialog = _createMessageDialog(qApp->applicationName());
    dialog->addMessageList(messageList);
    dialog->show();
};

Draupnir::Messages::MessageDisplayDialog* DialogNotificationChannel::_createMessageDialog(const QString& title)
{
    auto* result = new Draupnir::Messages::MessageDisplayDialog;
    result->setAttribute(Qt::WA_DeleteOnClose);
    result->setWindowIcon(QApplication::windowIcon());
    result->setWindowTitle(title);
    return result;
}

} // namespace Draupnir::Notifications
