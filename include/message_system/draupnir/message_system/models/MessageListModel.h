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

#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include <QAbstractItemModel>

#include <QList>

namespace Draupnir::Messages
{

class Message;

/*! @class MessageListModel draupnir/message_system/models/MessageListModel.h
 *  @ingroup MessageSystem
 *  @brief This `QAbstractItemModel`-derived class is a model of the list of the @ref Draupnir::Messages::Message objects.
 *         Can be used in the widgets like `QListView` to display @ref Draupnir::Messages::Message objects.
 *
 *  @details This model is used within this library together with @ref Draupnir::Messages::MessageListProxyModel within the
 *           @ref Draupnir::Messages::MessageListView.
 *
 *           This model supports following roles: `Qt::DisplayRole`, `Qt::DecorationRole`, `Qt::ToolTipRole`.
 *
 *           `QModelIndex` objects returned by this model contain internalPointer to the @ref Draupnir::Messages::Message
 *           objects.
 *
 * @note @ref Draupnir::Messages::MessageListModel is responsible for deleting @ref Draupnir::Messages::Message objects contained
 *       within it. This happens in the destructor and in the MessageListModel::clear method.
 *
 * @todo Write a unit-test for this class. */

class MessageListModel final : public QAbstractItemModel
{
    Q_OBJECT
public:
    /*! @brief Default constructor. */
    explicit MessageListModel(QObject *parent = nullptr);

    /*! @brief Destructor.
     * @note All @ref Draupnir::Messages::Message objects are deleted when destroying the @ref Draupnir::Messages::MessageListModel */
    ~MessageListModel() final;

    /*! @brief Adds single @ref Draupnir::Messages::Message object to the model.
     * @note @ref Draupnir::Messages::MessageListModel will "take ownership" of the @ref Draupnir::Messages::Message object
     *       provided and will delete it in its constructor or when using MEssageLIstModel::clear method. */
    void append(Message* message);

    /*! @brief Adds a list of the @ref Draupnir::Messages::Message objects to the model.
     * @note @ref Draupnir::Messages::MessageListModel will "take ownership" of the @ref Draupnir::Messages::Message objects
     *       provided and will delete it in its constructor or when using MEssageLIstModel::clear method. */
    void append(const QList<Message*>& messages);

    /*! @brief This method clears content of this model.
     * @note All @ref Draupnir::Messages::Message objects are deleted upon calling this method. */
    void clear();

///@name This methods required for inheriting from `QAbstractItemModel`
///@{
    QModelIndex    index(int row, int column, const QModelIndex &parent = QModelIndex()) const final;
    QModelIndex    parent(const QModelIndex &child) const final;
    int            rowCount(const QModelIndex &parent = QModelIndex()) const final;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const final;
    QVariant       data(const QModelIndex &index, int role = Qt::DisplayRole) const final;
///@}

private:
    QList<Message*> m_data;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTMODEL_H
