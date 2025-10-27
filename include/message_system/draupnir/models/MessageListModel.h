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

/*! @class MessageListModel draupnir/MessageListModel.h
 *  @brief This class is a model containing Message objects. This model can be used in the widgets like QListView to display
 *         Message objects contained within this model.
 *
 *  @details QModelIndex objects returned by this model contain internalPointer to the Message objects. To be used by the
 *           MessageListProxyModel.
 *
 *           Supported roles: Qt::DisplayRole, Qt::DecorationRole, Qt::ToolTipRole.
 *
 * @note MessageListModel is responsible for deleting Message objects. This happens in the destructor of MessageListModel and
 *       in the MessageListModel::clear method.
 *
 * @todo Write a test for this class. */

class MessageListModel final : public QAbstractItemModel
{
    Q_OBJECT
public:
    /*! @brief Default constructor. */
    explicit MessageListModel(QObject *parent = nullptr);

    /*! @brief Destructor.
     * @note All Message objects stored inside the model are deleted in the destructor. */
    ~MessageListModel() final;

    /*! @brief This method is used to add Message objects to the model. Accepts pointer to the Message object. */
    void append(Message* message);

    /*! @brief This method is used to add Message objects to the model. Accepts QList with pointers to the Message* objects. */
    void append(const QList<Message*>& messages);

    /*! @brief This method clears content of this model.
     * @note All Message objects are deleted upon calling this method. */
    void clear();

///@name This methods required for inheriting from QAbstractItemModel
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
