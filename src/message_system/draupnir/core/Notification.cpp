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

#include "draupnir/core/Notification.h"

#include <QDebug>

namespace Draupnir::Messages
{

static const QLatin1String noNotification_key{             "none"    };
static const QLatin1String messageBoxNotification_key{     "msg"     };

#ifndef QT_NO_SYSTEMTRAYICON
static const QLatin1String trayMessageNotification_key{    "tray"    };
#endif //QT_NO_SYSTEMTRAYICON

Notification::Type Notification::fromConfigString(const QString& string)
{
    if (string == noNotification_key)
        return None;

    if (string == messageBoxNotification_key)
        return MessageBoxType;

#ifndef QT_NO_SYSTEMTRAYICON
    if (string == trayMessageNotification_key)
        return Systemtray;
#endif //QT_NO_SYSTEMTRAYICON

#ifndef QT_NO_DEBUG
    qDebug() << "Provided string correspons to none of the Notification::Type values. string="<<string;
#endif // QT_NO_DEBUG

    return UnknownType;
}

QString Notification::toConfigString(Notification::Type type)
{
    switch (type) {
    case None:
        return noNotification_key;
    case MessageBoxType:
        return messageBoxNotification_key;
#ifndef QT_NO_SYSTEMTRAYICON
    case Systemtray:
        return trayMessageNotification_key;
#endif //QT_NO_SYSTEMTRAYICON
    case UnknownType:
        return QLatin1String("unknown");
    }
    Q_UNREACHABLE();
    return QString();
}

QString Notification::toDisplayString(Notification::Type type)
{
    switch (type) {
    case None:
        return tr("None");
    case MessageBoxType:
        return tr("Message Box");
#ifndef QT_NO_SYSTEMTRAYICON
    case Systemtray:
        return tr("Tray");
#endif // QT_NO_SYSTEMTRAYICON
    case UnknownType:
        Q_ASSERT(false);
        return QString();
    }
    Q_UNREACHABLE();
    return QString();
}

}; // namespace Draupnir::Messages
