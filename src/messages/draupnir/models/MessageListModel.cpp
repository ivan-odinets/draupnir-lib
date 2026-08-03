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

#include "draupnir/messages/models/MessageListModel.h"

#include "draupnir/messages/core/MessageViewItem.h"

namespace Draupnir::Messages
{

MessageListModel::MessageListModel(QObject *parent) :
    QAbstractListModel{parent}
{}

void MessageListModel::append(MessagePtr message)
{
    Q_ASSERT_X(message, "MessageListModel::append", "Provided Message* is nullptr.");

    int lastIndex = m_data.count();
    beginInsertRows(QModelIndex(), lastIndex, lastIndex);
    m_data.append(std::move(MessageViewItem{message}));
    endInsertRows();
}

void MessageListModel::append(const QList<MessagePtr>& messages)
{
    if (messages.isEmpty())
        return;

#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (MessagePtr message : messages) {
        Q_ASSERT_X(message, "MessageListModel::append", "One of the provided Message* is nullptr.");
    }
#endif // QT_NO_DEBUG

    int lastIndex = m_data.count();
    beginInsertRows(QModelIndex(), lastIndex, lastIndex + messages.count() - 1);
    for (auto message : messages)
        m_data.append(std::move(MessageViewItem{message}));
    endInsertRows();

}

void MessageListModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

const MessageViewItem& MessageListModel::itemAt(int index) const
{
    Q_ASSERT(index < m_data.count());
    return m_data.at(index);
}

const MessageViewItem& MessageListModel::itemAt(const QModelIndex& index) const
{
    Q_ASSERT(index.isValid());
    return itemAt(index.row());
}

MessagePtr MessageListModel::messageAt(int index) const
{
    return itemAt(index).message();
}

MessagePtr MessageListModel::messageAt(const QModelIndex& index) const
{
    return itemAt(index).message();
}

int MessageListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

QVariant MessageListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.column() != 0)
        return QVariant();

    const auto& messageView = itemAt(index.row());

    switch (role) {
        case Qt::DisplayRole:{
            QString result;
            result += messageView.brief();
            result += (result.isEmpty() ? "" : "\n") + messageView.what();
            result += (result.isEmpty() ? "" : "\n") + messageView.dateTime().toString();
            return result;
        }
        case Qt::DecorationRole:{
            return messageView.icon();
        }
        case Qt::ToolTipRole:{
            return messageView.what();
        }
    }

    return QVariant{};
}

}; // namespace Draupnir::Messages
