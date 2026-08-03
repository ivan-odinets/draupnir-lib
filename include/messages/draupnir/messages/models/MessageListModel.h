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

#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include <QAbstractListModel>

#include <QList>

#include "draupnir/messages/core/Message.h"
#include "draupnir/messages/core/MessageViewItem.h"

namespace Draupnir::Messages
{

/*! @class MessageListModel draupnir/messages/models/MessageListModel.h
 *  @ingroup Messages
 *  @brief This `QAbstractListModel`-derived class is a model of the list of the @ref Draupnir::Messages::MessageViewItem objects.
 *         Can be used in the widgets like `QListView` to display @ref Draupnir::Messages::Message objects through their view
 *         representation (@ref Draupnir::Messages::MessageViewItem).
 *
 *  @details This model is used within this library together with @ref Draupnir::Messages::MessageListProxyModel within the
 *           @ref Draupnir::Messages::MessageListView.
 *
 *           This model supports following roles: `Qt::DisplayRole`, `Qt::DecorationRole`, `Qt::ToolTipRole`.
 *
 * @todo Feature: Extract template base from this class to simplify list model creation. */

class MessageListModel final : public QAbstractListModel
{
    Q_OBJECT
public:
    /*! @brief Constructs an empty message list model.
     *  @param parent Parent object. */
    explicit MessageListModel(QObject *parent = nullptr);

    /*! @brief Destructor. */
    ~MessageListModel() final = default;

    /*! @brief Appends a message to the model.
     *  @param message Message to append.
     * @pre `message` must not be `nullptr`.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void append(MessagePtr message);

    /*! @brief Appends multiple messages to the model.
     *  @param messages Messages to append.
     * @pre `messages` must not contain `nullptr`.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void append(const MessageList& messages);

    /*! @brief Removes all items from the model.
     *  @details Releases the model's shared ownership of the corresponding messages. */
    void clear();

    /*! @brief Returns the item stored at a row.
     *  @param index Zero-based row index.
     *  @return Reference to the stored message view item.
     * @pre `index` must refer to an existing row. */
    [[nodiscard]] const MessageViewItem& itemAt(int index) const;

    /*! @brief Returns the item represented by a model index.
     *  @param index Valid index belonging to this model.
     *  @return Reference to the stored message view item.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] const MessageViewItem& itemAt(const QModelIndex& index) const;

    /*! @brief Returns the message stored at the specified row.
     *  @param index Zero-based row index.
     *  @return Shared pointer to the stored message.
     * @pre `index` must refer to an existing row.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] MessagePtr messageAt(int index) const;

    /*! @brief Returns the message represented by a model index.
     *  @param index Valid model index belonging to this model.
     *  @return Shared pointer to the stored message.
     * @pre `index` must be valid, belong to this model, and refer to column 0.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] MessagePtr messageAt(const QModelIndex& index) const;

///@name This methods required for inheriting from `QAbstractListModel`
///@{
    int rowCount(const QModelIndex &parent = QModelIndex()) const final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const final;
///@}

private:
    QList<MessageViewItem> m_data;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTMODEL_H
