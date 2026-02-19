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

#include "draupnir/message_system/core/Message.h"

namespace Draupnir::Messages
{

Message::Message(const uint64_t newType, const QIcon& icon, const QString& brief, const QString& what) :
    m_type{newType},
    m_icon{icon},
    m_brief{brief},
    m_what{what},
    m_dateTime{QDateTime::currentDateTime()}
{}

Message::Message(const uint64_t newType, const QIcon& icon, QString&& brief, QString&& what) :
    m_type{newType},
    m_icon{icon},
    m_brief{std::move(brief)},
    m_what{std::move(what)},
    m_dateTime{QDateTime::currentDateTime()}
{}

QString Message::getViewString(const MessageFields& fields) const
{
    static constexpr MessageFields notIconFields{
        MessageField::Brief | MessageField::What | MessageField::DateTime};
    // This method shoud return QString, which is independent of icon display status.
    // To get rid of anything but not Brief, What, DateTime - we use this.
    if ((fields & notIconFields) != m_cachedFields) {
        m_cachedFields = (fields & notIconFields);
        m_cachedView.clear();
        if (fields & MessageField::Brief)
            m_cachedView += brief();
        if (fields & MessageField::What)
            m_cachedView += (m_cachedView.isEmpty() ? "" : "\n") + what();
        if (fields & MessageField::DateTime)
            m_cachedView += (m_cachedView.isEmpty() ? "" : "\n") + dateTime().toString();
    }
    return m_cachedView;
}

}; // namespace Draupnir::Messages
