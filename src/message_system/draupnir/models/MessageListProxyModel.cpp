/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#include "draupnir/message_system/models/MessageListProxyModel.h"

#include "draupnir/message_system/core/Message.h"

namespace Draupnir::Messages
{

MessageListProxyModel::MessageListProxyModel(QObject* parent) :
    QSortFilterProxyModel{parent},
    m_displayedMessageFieldsMask{MessageField::All},
    m_displayedMessageTypesMask{MessageType::AllMessages}
{}

void MessageListProxyModel::setDisplayedMessageTypesMask(MessageType mask)
{
    if (m_displayedMessageTypesMask == mask)
        return;

    m_displayedMessageTypesMask = mask;
    invalidateFilter();
}

void MessageListProxyModel::setMessageTypeDisplayed(MessageType type, bool isVisible)
{
    if (isVisible) {
        uint64_t intValue = m_displayedMessageTypesMask;
        intValue |= type;
        m_displayedMessageTypesMask = intValue;
    } else {
        uint64_t intValue = m_displayedMessageTypesMask;
        intValue &= ~type;
        m_displayedMessageTypesMask = intValue;
    }
    invalidateFilter();
}

void MessageListProxyModel::setDisplayedMessageFieldsMask(MessageFields mask)
{
    m_displayedMessageFieldsMask = mask;
}

void MessageListProxyModel::setMessageFieldDisplayed(MessageField field, bool isVisible)
{
    if (isMessageFieldDisplayed(field) == isVisible)
        return;

    m_displayedMessageFieldsMask.setFlag(field,isVisible);

    emit dataChanged(
        index(0,0),
        index(rowCount() - 1, columnCount() - 1)
    );
}

QVariant MessageListProxyModel::data(const QModelIndex &index, int role) const
{
    QModelIndex sourceIndex = this->mapToSource(index);

    if (!sourceIndex.isValid() || sourceIndex.column() != 0)
        return QVariant();

    const Message* const message = static_cast<Message*>(sourceIndex.internalPointer());

    switch (role) {
        case Qt::DisplayRole:{
            return message->getViewString(this->m_displayedMessageFieldsMask);
        }
        case Qt::DecorationRole:{
            return isMessageFieldDisplayed(MessageField::Icon) ? message->icon() : QIcon{};
        }
        case Qt::ToolTipRole:{
            return message->what();
        }
    }

    return QVariant();
};

bool MessageListProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid())
        return false;

    Message* message = static_cast<Message*>(index.internalPointer());
    Q_ASSERT_X(message, "MessageListProxyModel::filterAcceptsRow",
               "Source model for this proxy model MUST provide QModelIndex having internalPointer");
    return (message->type() & m_displayedMessageTypesMask);
}

}; // namespace Draupnir::Messages
