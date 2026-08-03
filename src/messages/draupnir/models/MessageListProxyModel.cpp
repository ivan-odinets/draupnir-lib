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

#include "draupnir/messages/models/MessageListProxyModel.h"

#include "draupnir/messages/models/MessageListModel.h"
#include "draupnir/messages/core/MessageViewItem.h"

namespace Draupnir::Messages
{

MessageListProxyModel::MessageListProxyModel(QObject* parent) :
    QSortFilterProxyModel{parent},
    m_displayedMessageViewItemFields{DefaultDisplayedMessageItemFields},
    m_displayedMessageCategoriesMask{DefaultDisplayedMessageCategories},
    m_displayedMessageLevelsMask{DefaultDisplayedMessageLevels}
{}

void MessageListProxyModel::setSourceModel(QAbstractItemModel* model)
{
    Q_ASSERT(qobject_cast<MessageListModel*>(model));
    QSortFilterProxyModel::setSourceModel(model);
}

MessageListModel* MessageListProxyModel::underlyingMessageListModel() const
{
    Q_ASSERT(qobject_cast<MessageListModel*>(sourceModel()));
    return static_cast<MessageListModel*>(sourceModel());
};

MessagePtr MessageListProxyModel::messageAt(int row) const
{
    Q_ASSERT(row >= 0);
    Q_ASSERT(row < rowCount());
    const QModelIndex proxyIndex = index(row, 0);
    Q_ASSERT(proxyIndex.isValid());
    return messageAt(proxyIndex);
}

MessagePtr MessageListProxyModel::messageAt(const QModelIndex& index) const
{
    Q_ASSERT(index.isValid());
    return underlyingMessageListModel()->messageAt(mapToSource(index));
};

void MessageListProxyModel::setDisplayedMessageViewItemFieldsMask(MessageViewItemFields mask)
{
    if (m_displayedMessageViewItemFields == mask)
        return;

    m_displayedMessageViewItemFields = mask;
    _notifyDisplayedDataChanged();
}

void MessageListProxyModel::setMessageViewItemFieldDisplayed(MessageViewItemField::Value field, bool isVisible)
{
    if (isMessageViewItemFieldDisplayed(field) == isVisible)
        return;

    m_displayedMessageViewItemFields.set_flag(field,isVisible);
    _notifyDisplayedDataChanged();
}

bool MessageListProxyModel::isMessageTypeDisplayed(MessageType type) const
{
    return
        (m_displayedMessageLevelsMask.test_flag(type.level())) &&
        (m_displayedMessageCategoriesMask.test_flag(type.category()));
}

void MessageListProxyModel::setDisplayedMessageCategoriesMask(MessageCategories mask)
{
    if (m_displayedMessageCategoriesMask == mask)
        return;

    m_displayedMessageCategoriesMask = mask;
    invalidateFilter();
}

void MessageListProxyModel::setMessageCategoryDisplayed(MessageCategory category, bool isVisible)
{
    if (isMessageCategoryDisplayed(category) == isVisible)
        return;

    m_displayedMessageCategoriesMask.set_flag(category, isVisible);
    invalidateFilter();
}

void MessageListProxyModel::setDisplayedMessageLevelsMask(MessageLevels mask)
{
    if (m_displayedMessageLevelsMask == mask)
        return;

    m_displayedMessageLevelsMask = mask;
    invalidateFilter();
}

void MessageListProxyModel::setMessageLevelDisplayed(MessageLevel::Value level, bool isVisible)
{
    if (m_displayedMessageLevelsMask.test_flag(level) == isVisible)
        return;

    m_displayedMessageLevelsMask.set_flag(level, isVisible);
    invalidateFilter();
};

QVariant MessageListProxyModel::data(const QModelIndex &index, int role) const
{
    QModelIndex sourceIndex = this->mapToSource(index);

    if (!sourceIndex.isValid() || sourceIndex.column() != 0)
        return QVariant();

    const MessageViewItem& item = underlyingMessageListModel()->itemAt(sourceIndex.row());

    switch (role) {
    case Qt::DisplayRole:
        return item.getViewString(m_displayedMessageViewItemFields);
    case Qt::DecorationRole:
        return isMessageViewItemFieldDisplayed(MessageViewItemField::Icon) ? item.icon() : QIcon{};
    case Qt::ToolTipRole:
        return item.what();
    default:
        return QSortFilterProxyModel::data(index, role);
    }
    return QVariant();
};

bool MessageListProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid())
        return false;

    return isMessageTypeDisplayed(underlyingMessageListModel()->messageAt(index)->type());
}

void MessageListProxyModel::_notifyDisplayedDataChanged()
{
    if (rowCount() == 0 || columnCount() == 0)
        return;

    emit dataChanged(
        index(0, 0),
        index(rowCount() - 1, columnCount() - 1)
    );
}

} // namespace Draupnir::Messages
