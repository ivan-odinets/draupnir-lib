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

#include "draupnir/message_system/ui/widgets/MessageListView.h"

#include <QDebug>
#include <QMouseEvent>

#include "draupnir/message_system/models/MessageListModel.h"

#include "draupnir/message_system/models/MessageListProxyModel.h"
#include "draupnir/message_system/ui/windows/MessageDisplayDialog.h"

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

void MessageListView::setDisplayedMessageTypesMask(MessageType type)
{
    p_messageListProxyModel->setDisplayedMessageTypesMask(type);
}

MessageType MessageListView::displayedMessageTypesMask() const
{
    return p_messageListProxyModel->displayedMessageTypesMask();
}

bool MessageListView::isMessageTypeDisplayed(MessageType messageType)
{
    return p_messageListProxyModel->isMessageTypeDisplayed(messageType);
}

void MessageListView::setDisplayedMessageFieldsMask(std::underlying_type_t<Message::Fields> fields)
{
    p_messageListProxyModel->setDisplayedMessageFieldsMask(fields);
}

std::underlying_type_t<Message::Fields> MessageListView::displayedMessageFieldsMask() const
{
    return p_messageListProxyModel->displayedMessageFieldsMask();
}

bool MessageListView::isMessageFieldDisplayed(Message::Fields field) const
{
    return p_messageListProxyModel->isMessageFieldDisplayed(field);
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
        Q_ASSERT_X(message,"MessageListView::mouseDoubleClickEvent",
                   "QModelInxex provided by MessageListProxyModel does not contain valid internalPointer. ");
        messagesList.append(message);
    }

    MessageDisplayDialog* dialog = new MessageDisplayDialog{this};
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->addMessageList(messagesList);
    dialog->show();
}

void MessageListView::setMessageTypeDisplayed(MessageType type, bool displayed)
{
    p_messageListProxyModel->setMessageTypeDisplayed(type,displayed);
    emit messageTypeVisibilityChanged(type,displayed);
}

void MessageListView::setMessageFieldDisplayed(Message::Fields field, bool isVisible)
{
    p_messageListProxyModel->setMessageFieldDisplayed(field, isVisible);
    emit messageFieldVisibilityChanged(field, isVisible);
}

}; // namespace Draupnir::Messages
