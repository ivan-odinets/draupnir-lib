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

#ifndef DIALOGNOTIFICATIONHANDLER_H
#define DIALOGNOTIFICATIONHANDLER_H

#include <QApplication>

#include "draupnir/notifications/handlers/NotificationHandlerTemplate.h"

#include "draupnir/notifications/concepts/NotificationHandlerConcepts.h"
#include "draupnir/logging/messages/MessageViewItem.h"
#include "draupnir/logging/ui/windows/MessageDisplayDialog.h"
#include "draupnir/notifications/traits/notifications/DialogNotificationTrait.h"

namespace Draupnir::Notifications
{

/*! @brief Displays notifications using message dialogs.
 *  @ingroup Notifications
 *
 *  @details Specializes @ref Draupnir::Notifications::NotificationHandlerTemplate for @ref Draupnir::Notififcations::DialogNotificationTrait.
 *           Created message view items remain valid until the corresponding dialog is destroyed. */

template<>
class NotificationHandlerTemplate<DialogNotificationTrait>
{
public:
    /*! @brief Displays a single message in a new dialog.
     *  @param message Message to display. Must not be null. */
    static void showMessage(Draupnir::Logging::Message* message) {
        Q_ASSERT(message);

        auto dialog = _createMessageDialog(message->brief() + QLatin1String{" - "} + qApp->applicationName());
        auto messageView = new Draupnir::Logging::MessageViewItem{message};
        dialog->addMessage(messageView);
        QObject::connect(dialog, &QObject::destroyed, [messageView](){ delete messageView; });
        dialog->show();
    };

    /*! @brief Displays a list of messages in a new dialog.
     *  @param messageList Messages to display. */
    static void showMessageList(const Draupnir::Logging::MessageList& messageList) {
#ifndef QT_NO_DEBUG
        // Just to be sure that no nullptrs are here
        for (Draupnir::Logging::Message* message : messageList)
            Q_ASSERT(message);
#endif // QT_NO_DEBUG

        using namespace Draupnir::Logging;
        auto dialog = _createMessageDialog(qApp->applicationName());
        QList<MessageViewItem*> messageViewsList;
        for (Message* message : messageList)
            messageViewsList.append(new MessageViewItem{message});
        dialog->addMessageList(messageViewsList);
        QObject::connect(dialog, &QObject::destroyed, [messageViewsList](){ qDeleteAll(messageViewsList); });
        dialog->show();
    };

private:
    /*! @brief Creates and configures a message display dialog.
     *  @param title Window title of the dialog.
     *  @return Newly allocated dialog that deletes itself when closed. */
    static Draupnir::Logging::MessageDisplayDialog* _createMessageDialog(const QString& title) {
        auto* result = new Draupnir::Logging::MessageDisplayDialog;
        result->setAttribute(Qt::WA_DeleteOnClose);
        result->setWindowIcon(QApplication::windowIcon());
        result->setWindowTitle(title);
        return result;
    }
};

};

#endif // DIALOGNOTIFICATIONHANDLER_H
