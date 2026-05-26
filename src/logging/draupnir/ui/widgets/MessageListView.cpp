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

#include "draupnir/logging/ui/widgets/MessageListView.h"

#include <QDebug>
#include <QMouseEvent>

#include "draupnir/logging/models/MessageListModel.h"
#include "draupnir/logging/models/MessageListProxyModel.h"
#include "draupnir/logging/ui/windows/MessageDisplayDialog.h"

namespace Draupnir::Logging
{

MessageListView::MessageListView(QWidget* parent) :
    QListView{parent},
    p_messageList{nullptr},
    p_messageListProxyModel{new MessageListProxyModel}
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ContiguousSelection);

    QListView::setModel(p_messageListProxyModel);
}

MessageListView::~MessageListView()
{
    delete p_messageListProxyModel;
}

void MessageListView::setModel(QAbstractItemModel* model)
{
    Q_ASSERT_X(qobject_cast<MessageListModel*>(model), "MessageListView::setModel",
        "QAbstractItemModel specified is not MessageListModel.");

    // If we have sth else but not MessageListModel - we should crash in Debug. If we have this in Release. Well.
    // Shit Happens. Not using qobject_cast because its slower and by design we should have here only MessageListModel
    p_messageList = static_cast<MessageListModel*>(model);

    p_messageListProxyModel->setSourceModel(model);
}

void MessageListView::setDisplayedMessageViewItemFieldsMask(MessageViewItemFields mask)
{
    p_messageListProxyModel->setDisplayedMessageViewItemFieldsMask(mask);
}

MessageViewItemFields MessageListView::displayedMessageViewItemFieldsMask() const
{
    return p_messageListProxyModel->displayedMessageViewItemFieldsMask();
}

void MessageListView::setMessageViewItemFieldDisplayed(MessageViewItemField::Value field, bool isVisible)
{
    if (p_messageListProxyModel->isMessageViewItemFieldDisplayed(field) == isVisible)
        return;

    p_messageListProxyModel->setMessageViewItemFieldDisplayed(field, isVisible);
    emit messageViewItemFieldVisibilityChanged(field, isVisible);
}

bool MessageListView::isMessageViewItemFieldDisplayed(MessageViewItemField::Value field) const
{
    return p_messageListProxyModel->isMessageViewItemFieldDisplayed(field);
}

void MessageListView::setDisplayedMessageCategoriesMask(MessageCategories mask)
{
    p_messageListProxyModel->setDisplayedMessageCategoriesMask(mask);
}

MessageCategories MessageListView::displayedMessageCategoriesMask() const
{
    return p_messageListProxyModel->displayedMessageCategoriesMask();
}

void MessageListView::setMessageCategoryDisplayed(MessageCategory category, bool isVisible)
{
    p_messageListProxyModel->setMessageCategoryDisplayed(category, isVisible);
}

bool MessageListView::isMessageCategoryDisplayed(MessageCategory messageCategory)
{
    return p_messageListProxyModel->isMessageCategoryDisplayed(messageCategory);
}

void MessageListView::setDisplayedMessageLevelsMask(MessageLevels mask)
{
    p_messageListProxyModel->setDisplayedMessageLevelsMask(mask);
}

MessageLevels MessageListView::displayedMessageLevelsMask() const
{
    return p_messageListProxyModel->displayedMessageLevelsMask();
}

void MessageListView::setMessageLevelDisplayed(MessageLevel::Value level, bool isVisible)
{
    p_messageListProxyModel->setMessageLevelDisplayed(level, isVisible);
}

bool MessageListView::isMessageLevelDisplayed(MessageLevel::Value level) const
{
    return p_messageListProxyModel->isMessageLevelDisplayed(level);
}

void MessageListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    const QModelIndex proxyIndex = QListView::indexAt(event->pos());
    const QModelIndex sourceIndex = p_messageListProxyModel->mapToSource(proxyIndex);

    const QItemSelection proxySelection = selectionModel()->selection();
    const QModelIndexList selectedIndexes = p_messageListProxyModel->mapSelectionToSource(proxySelection).indexes();

    if (!sourceIndex.isValid())
        return;
    if (selectedIndexes.count() == 0) {
        qDebug() << "WTF?";
    }

    QList<MessageViewItem*> messagesList;
    for (const QModelIndex& index : selectedIndexes) {
        MessageViewItem* message = static_cast<MessageViewItem*>(index.internalPointer());
        Q_ASSERT_X(message,"MessageListView::mouseDoubleClickEvent",
            "QModelInxex provided by MessageListProxyModel does not contain valid internalPointer. ");
        messagesList.append(message);
    }

    MessageDisplayDialog* dialog = new MessageDisplayDialog{this};
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->addMessageList(messagesList);
    dialog->show();
}

}; // namespace Draupnir::Messages
