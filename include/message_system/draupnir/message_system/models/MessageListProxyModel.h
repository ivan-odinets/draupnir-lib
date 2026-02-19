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

#ifndef MESSAGELISTPROXYMODEL_H
#define MESSAGELISTPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "draupnir/message_system/core/MessageFields.h"
#include "draupnir/message_system/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageListProxyModel draupnir/message_system/models/MessageListProxyModel.h
 *  @ingroup MessageSystem
 *  @brief This class is a proxy model for @ref Draupnir::Messages::MessageListModel to allow filtering and formatting of the
 *         displayed @ref Draupnir::Messages::Message objects. By default this model will accept any message type and display
 *         everything.
 *
 *  @details Filtering is done by bitwise comparison between the value returned by Message::type and `m_allowedType` field
 *           stored in this model. See @ref Draupnir::Messages::MessageType. */

class MessageListProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*! @brief Default constructor. By default this filter model will accept all messages and display all fields of the
     *         @ref Draupnir::Messages::Message objects. */
    explicit MessageListProxyModel(QObject* parent = nullptr);

    /*! @brief Destructor. Trivial. */
    ~MessageListProxyModel() final = default;

    /*! @brief Sets what type of @ref Draupnir::Messages::Message objects this model should allow displaying. */
    void setDisplayedMessageTypesMask(MessageType mask);

    /*! @brief Returns allowed @ref Draupnir::Messages::Message::Type for @ref Draupnir::Messages::Message objects that
     *         will be shown by this model. */
    MessageType displayedMessageTypesMask() const { return m_displayedMessageTypesMask; }

    /*! @brief This method allows enabling/disabling filtering of @ref Draupnir::Messages::Message objects with specified
     *         @ref Draupnir::Messages::MessageType.
     * @todo Refractor this method so that it will change sth only if really needed. */
    void setMessageTypeDisplayed(MessageType type, bool isVisible);

    /*! @brief Returns `true` if the specified `messageType` will be displayed by this model. */
    bool isMessageTypeDisplayed(MessageType messageType) { return m_displayedMessageTypesMask & messageType; }

    /*! @brief Sets what fields of @ref Draupnir::Messages::Message objects will be displayed. */
    void setDisplayedMessageFieldsMask(MessageFields mask);

    /*! @brief Returns what fields of @ref Draupnir::Messages::Message objects will be displayed. */
    MessageFields displayedMessageFieldsMask() const { return m_displayedMessageFieldsMask; }

    /*! @brief Sets if specific field of @ref Draupnir::Messages::Message object will be displayed. */
    void setMessageFieldDisplayed(MessageField field, bool isVisible);

    /*! @brief Returns `true` if specific field of @ref Draupnir::Messages::Message object is displayed. */
    bool isMessageFieldDisplayed(MessageField field) const { return m_displayedMessageFieldsMask.testFlag(field); }

    /*! @brief This method is used to adjust displayed data in accordance to configured fields mask. */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const final;

protected:
    /*! @brief Returns true if the index is valid and the @ref Draupnir::Messages::Message object to which this index is
     *         pointing is having the allowed `type`. */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const final;

private:
    MessageFields m_displayedMessageFieldsMask;
    MessageType m_displayedMessageTypesMask;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTPROXYMODEL_H
