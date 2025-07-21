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

#ifndef MESSAGELISTPROXYMODEL_H
#define MESSAGELISTPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "MessageType.h"

/*! @class MessageListProxyModel core/messages/MessageListProxyModel.h
 *  @brief This class is a proxy model for MessageListModel which is used for filtering  Message objects. By default this model
 *         will accept any message type.
 *  @details Filtering is done by bitwise comparison between the value returned by Message::type and m_allowedType
 *           stored in this model. See MessageType. */

class MessageListProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*! @brief Default constructor. By default this filter model will accept all messages. */
    explicit MessageListProxyModel(QObject* parent = nullptr);

    /*! @brief Destructor. Does nothing. */
    ~MessageListProxyModel() final;

    /*! @brief Sets what type of Message objects this model should allow displaying. */
    void setMessageTypeFilter(MessageType type);

    /*! @brief Returns allowed Message::Type for Message objects that will be shown by this model. */
    MessageType messageTypeFilter() const { return m_typeFilter; }

    /*! @brief This method allows enabling/disabling filtering of Message objects with specified MessageType. */
    void setMessageTypeAllowed(MessageType type, bool isVisible);

protected:
    /*! @brief Returns true if the index is valid and the Message object to which this index is pointing is having the allowed
     *         type. */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const final;

private:
    MessageType m_typeFilter;
};

#endif // MESSAGELISTPROXYMODEL_H
