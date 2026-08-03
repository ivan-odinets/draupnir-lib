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

#include "draupnir/messages/ui/widgets/MessageListView.h"

#include <QDebug>
#include <QMouseEvent>

#include "draupnir/messages/models/MessageListModel.h"
#include "draupnir/messages/models/MessageListProxyModel.h"
#include "draupnir/messages/ui/windows/MessageDisplayDialog.h"

namespace Draupnir::Messages
{

MessageListView::MessageListView(QWidget* parent) :
    QListView{parent},
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
    Q_ASSERT_X(qobject_cast<MessageListModel*>(model), Q_FUNC_INFO,
        "QAbstractItemModel specified is not MessageListModel.");



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
    if (isMessageCategoryDisplayed(category) == isVisible)
        return;

    p_messageListProxyModel->setMessageCategoryDisplayed(category, isVisible);
    emit messageCategoryVisibilityChanged(category, isVisible);
}

bool MessageListView::isMessageCategoryDisplayed(MessageCategory messageCategory) const
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
    if (isMessageLevelDisplayed(level) == isVisible)
        return;

    p_messageListProxyModel->setMessageLevelDisplayed(level, isVisible);
    emit messageLevelVisibilityChanged(level, isVisible);
}

bool MessageListView::isMessageLevelDisplayed(MessageLevel::Value level) const
{
    return p_messageListProxyModel->isMessageLevelDisplayed(level);
}

void MessageListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    const QModelIndex proxyIndex = QListView::indexAt(event->pos());
    if (!proxyIndex.isValid())
        return;

    QModelIndexList selectedIndexes = selectionModel()->selection().indexes();
    if (!selectedIndexes.contains(proxyIndex))
        selectedIndexes = {proxyIndex};
    MessageList messagesList;
    messagesList.reserve(selectedIndexes.count());
    for (const QModelIndex& index : selectedIndexes)
        messagesList.append(p_messageListProxyModel->messageAt(index));

    MessageDisplayDialog* dialog = new MessageDisplayDialog{this};
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->addMessageList(messagesList);
    dialog->show();
}

} // namespace Draupnir::Messages
