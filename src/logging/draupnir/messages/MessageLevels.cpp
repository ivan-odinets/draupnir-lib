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

#include "draupnir/logging/messages/MessageLevels.h"

namespace Draupnir::Logging
{

QString MessageLevel::debugDisplayName()
{
    return QObject::tr("Debug");
}

QString MessageLevel::infoDisplayName()
{
    return QObject::tr("Info");
}

QString MessageLevel::warningDisplayName()
{
    return QObject::tr("Warning");
}

QString MessageLevel::errorDisplayName()
{
    return QObject::tr("Error");
}

QString MessageLevel::toDisplayName(MessageLevel::Value value)
{
    switch (value) {
        case Debug:   return debugDisplayName();
        case Info:    return infoDisplayName();
        case Warning: return warningDisplayName();
        case Error:   return errorDisplayName();
    }
    Q_UNREACHABLE();
    Q_ASSERT(false);
    return QString{};
}

}; // namespace Draupnir::Logging
