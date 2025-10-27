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

#include "draupnir/ui/widgets/MessageListView.h"

#include <QDebug>
#include <QMouseEvent>

#include "draupnir/models/MessageListModel.h"
#include "draupnir/models/MessageListProxyModel.h"
#include "draupnir/ui/windows/MessageDisplayDialog.h"

namespace Draupnir::Messages
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
    p_messageListProxyModel->deleteLater();
}

void MessageListView::setModel(QAbstractItemModel* model)
{
    Q_ASSERT_X(qobject_cast<MessageListModel*>(model),"MessageListView::setModel",
               "Provided object is not MessageListModel");

    p_messageListProxyModel->setSourceModel(model);

    // If we have sth else but not MessageLIstModel - we should crash in Debug. If we have this in Release. Well.
    // Shit Happens. Not using qobject_cast because its slower and by design we shluld have here only MessageListModel
    p_messageList = static_cast<MessageListModel*>(model);
}

void MessageListView::applyMessageTypeFilter(MessageType type)
{
    p_messageListProxyModel->setMessageTypeFilter(type);
}

MessageType MessageListView::messageTypeFilter() const
{
    return p_messageListProxyModel->messageTypeFilter();
}

bool MessageListView::isBriefDisplayed() const
{
    return p_messageListProxyModel->isBriefDisplayed();
}

void MessageListView::setBriefDisplayed(bool state)
{
    p_messageListProxyModel->setBriefDisplayed(state);
}

bool MessageListView::isWhatDisplayed() const
{
    return p_messageListProxyModel->isWhatDisplayed();
}

void MessageListView::setWhatDisplayed(bool state)
{
    p_messageListProxyModel->setWhatDisplayed(state);
}

bool MessageListView::isDateTimeDisplayed() const
{
    return p_messageListProxyModel->isDateTimeDisplayed();
}

void MessageListView::setDateTimeDisplayed(bool state)
{
    p_messageListProxyModel->setDateTimeDisplayed(state);
}

bool MessageListView::isIconDisplayed() const
{
    return p_messageListProxyModel->isIconDisplayed();
}

void MessageListView::setIconDisplayed(bool state)
{
    p_messageListProxyModel->setIconDisplayed(state);
}

void MessageListView::setMessageTypeDisplayed(MessageType type, bool displayed)
{
    p_messageListProxyModel->setMessageTypeAllowed(type,displayed);
    emit messageTypeVisibilityChanged(type,displayed);
}

void MessageListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    const QModelIndex proxyIndex = QListView::indexAt(event->pos());
    const QModelIndex sourceIndex = p_messageListProxyModel->mapToSource(proxyIndex);

    const QItemSelection proxySelection = selectionModel()->selection();
    const QModelIndexList selectedIndexes = p_messageListProxyModel->mapSelectionToSource(proxySelection).indexes();

    if (!sourceIndex.isValid())
        return;

    QList<Message*> messagesList;
    for (const QModelIndex& index : selectedIndexes) {
        Message* message = static_cast<Message*>(index.internalPointer());
        messagesList.append(message);
    }

    MessageDisplayDialog* dialog = new MessageDisplayDialog{this};
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->addMessageList(messagesList);
    dialog->show();
}

}; // namespace Draupnir::Messages
