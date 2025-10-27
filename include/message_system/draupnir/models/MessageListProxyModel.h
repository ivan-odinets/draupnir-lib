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

#include "draupnir/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageListProxyModel draupnir/models/MessageListProxyModel.h
 *  @ingroup MessageSystem
 *  @brief This class is a proxy model for MessageListModel which is used for filtering  Message objects. By default this model
 *         will accept any message type.
 *  @details Filtering is done by bitwise comparison between the value returned by Message::type and m_allowedType
 *           stored in this model. See MessageType.
 *
 * @todo Finish with displaying selected fields of the Message objects. */

class MessageListProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*! @brief This enum holds flag for fields of Message objects, which can be displayed by this model. */
    enum DisplayedContent {
        Brief      = 0b00000001,  /*!< @brief Brief summary of Message (Message::brief). */
        What       = 0b00000010,  /*!< @brief Details what happened (Message::what). */
        DateTime   = 0b00000100,  /*!< @brief DateTime of Message (Message::dateTime). */
        Icon       = 0b00001000,  /*!< @brief Icon of Message (Message::icon). */
        All        = Brief | What | DateTime | Icon,  /*!< @brief All of the above. */
    };

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

    /*! @brief Returns true if this model will display content of Message::brief. */
    bool isBriefDisplayed() const      { return m_displayedContent & Brief; }

    /*! @brief Allows setting if this model will display content of Message::brief. */
    void setBriefDisplayed(bool state);

    /*! @brief Returns true if this model will display content of Message::what. */
    bool isWhatDisplayed() const       { return m_displayedContent & What; }

    /*! @brief Allows setting if this model will display content of Message::what. */
    void setWhatDisplayed(bool state);

    /*! @brief Returns true if this model will display content of Message::dateTime. */
    bool isDateTimeDisplayed() const   { return m_displayedContent & DateTime; }

    /*! @brief Allows setting if this model will display content of Message::dateTime. */
    void setDateTimeDisplayed(bool state);

    /*! @brief Returns true if this model will display content of Message::icon. */
    bool isIconDisplayed() const       { return m_displayedContent & Icon; }

    /*! @brief Allows setting if this model will display content of Message::icon. */
    void setIconDisplayed(bool state);

    /*! @brief This is a method.
     * @todo Document Me. */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const final;

protected:
    /*! @brief Returns true if the index is valid and the Message object to which this index is pointing is having the allowed
     *         type. */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const final;

private:
    quint8 m_displayedContent;
    void _setDisplayedContentBit(DisplayedContent content, bool state);
    MessageType m_typeFilter;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTPROXYMODEL_H
