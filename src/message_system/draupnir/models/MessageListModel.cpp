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

#include "draupnir/models/MessageListModel.h"

#include "draupnir/core/Message.h"

namespace Draupnir::Messages
{

MessageListModel::MessageListModel(QObject *parent) :
    QAbstractItemModel{parent},
    m_displayedContent{All}
{}

MessageListModel::~MessageListModel()
{
    for (auto message : m_data)
        delete message;

    m_data.clear();
}

void MessageListModel::append(Message* message)
{
    Q_ASSERT_X(message,"MessageListModel::append","Provided Message* is nullptr");
    int lastIndex = m_data.count();
    beginInsertRows(QModelIndex(),lastIndex,lastIndex+1);
    m_data.append(message);
    endInsertRows();
}

void MessageListModel::append(const QList<Message*>& messages)
{
    if (messages.isEmpty())
        return;

#ifndef QT_NO_DEBUG
    // Just to be sure that no nullptrs are here
    for (Message* message : messages) {
        Q_ASSERT_X(message, "MessageListModel::append", "One of the provided Message* is nullptr!");
    }
#endif // QT_NO_DEBUG

    int lastIndex = m_data.count();
    beginInsertRows(QModelIndex(),lastIndex,lastIndex + messages.count() - 1);
    m_data.append(messages);
    endInsertRows();
}

void MessageListModel::clear()
{
    beginResetModel();
    for (auto message : m_data) {
        delete message;
    }
    m_data.clear();
    endResetModel();
}

void MessageListModel::setBriefDisplayed(bool state)
{
    if (state == isBriefDisplayed())
        return;

    _setDisplayedContentBit(Brief,state);
    emit dataChanged(
        index(0,0),
        index(m_data.size() - 1,0),
        {Qt::DisplayRole}
    );
}

void MessageListModel::setWhatDisplayed(bool state)
{
    if (state == isWhatDisplayed())
        return;

    _setDisplayedContentBit(What,state);
    emit dataChanged(
        index(0,0),
        index(m_data.size() - 1,0),
        {Qt::DisplayRole}
    );
}

void MessageListModel::setDateTimeDisplayed(bool state)
{
    if (state == isDateTimeDisplayed())
        return;

    _setDisplayedContentBit(DateTime,state);
    emit dataChanged(
        index(0,0),
        index(m_data.size() - 1,0),
        {Qt::DisplayRole}
    );
}

void MessageListModel::setIconDisplayed(bool state)
{
    if (state == isIconDisplayed())
        return;

    _setDisplayedContentBit(Icon,state);
    emit dataChanged(
        index(0,0),
        index(m_data.size() - 1,0),
        {Qt::DecorationRole}
    );
}

QModelIndex MessageListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || parent.isValid()) {
        return QModelIndex();
    }

    return createIndex(row, column, m_data[row]);
}

QModelIndex MessageListModel::parent(const QModelIndex& child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int MessageListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

int MessageListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant MessageListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.column() != 0)
        return QVariant();

    const Message* const message = static_cast<Message*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:{
            QString result;
            if (isBriefDisplayed())     result += message->brief();
            if (isWhatDisplayed())      result += (result.isEmpty() ? "" : "\n") + message->what();
            if (isDateTimeDisplayed())  result += (result.isEmpty() ? "" : "\n") + message->dateTime().toString();
            return result;
        }
        case Qt::DecorationRole:{
            return message->icon();
        }
        case Qt::ToolTipRole:{
            return message->what();
        }
    }

    return QVariant();
}

void MessageListModel::_setDisplayedContentBit(DisplayedContent content, bool state)
{
    if (state)
        m_displayedContent |= content;
    else
        m_displayedContent &= ~content;
}

}; // namespace Draupnir::Messages
