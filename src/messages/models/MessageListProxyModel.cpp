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

#include "models/MessageListProxyModel.h"

#include "core/Message.h"

namespace Draupnir::Messages
{

MessageListProxyModel::MessageListProxyModel(QObject* parent) :
    QSortFilterProxyModel{parent},
    m_typeFilter{MessageType::AllMessages}
{}

MessageListProxyModel::~MessageListProxyModel()
{}

void MessageListProxyModel::setMessageTypeFilter(MessageType type)
{
    m_typeFilter = type;
    invalidateFilter();
}

void MessageListProxyModel::setMessageTypeAllowed(MessageType type, bool isVisible)
{
    if (isVisible) {
        uint64_t intValue = m_typeFilter;
        intValue |= type;
        m_typeFilter = intValue;
    } else {
        uint64_t intValue = m_typeFilter;
        intValue &= ~type;
        m_typeFilter = intValue;
    }
    invalidateFilter();
}

bool MessageListProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid())
        return false;

    Message* message = static_cast<Message*>(index.internalPointer());
    Q_ASSERT_X(message, "MessageListProxyModel::filterAcceptsRow",
               "Source model for this proxy model MUST provide QModelIndex having internalPointer");
    return (message->type() & m_typeFilter);
}

}; // namespace Draupnir::Messages
