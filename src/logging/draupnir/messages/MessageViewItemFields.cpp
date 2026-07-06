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

#include "draupnir/logging/messages/MessageViewItemFields.h"

namespace Draupnir::Logging
{

QString MessageViewItemField::briefDisplayName()
{
    return QObject::tr("Brief");
}

QString MessageViewItemField::whatDisplayName()
{
    return QObject::tr("What");
}

QString MessageViewItemField::dateTimeDisplayName()
{
    return QObject::tr("Timestamp");
}

QString MessageViewItemField::iconDisplayName()
{
    return QObject::tr("Icon");
}

QString MessageViewItemField::toDisplayName(Value value)
{
    switch (value) {
    case Logging::MessageViewItemField::Brief:
        return briefDisplayName();
    case Logging::MessageViewItemField::What:
        return whatDisplayName();
    case Logging::MessageViewItemField::DateTime:
        return dateTimeDisplayName();
    case Logging::MessageViewItemField::Icon:
        return iconDisplayName();
    }
    Q_UNREACHABLE();
    Q_ASSERT(false);
    return QString{};
}

QString MessageViewItemFields::toDisplayName(MessageViewItemFields fields)
{
    const auto value = fields.value();

    Q_ASSERT((value & ~All) == 0);
    Q_ASSERT(value != None);
    Q_ASSERT((value & (value - 1)) == 0);
    return MessageViewItemField::toDisplayName(static_cast<MessageViewItemField::Value>(fields.value()));
}

}; // namespace Draupnir::Logging
