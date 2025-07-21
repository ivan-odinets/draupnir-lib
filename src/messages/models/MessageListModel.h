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

class Message;

/*! @class MessageListModel draupnir-lib/src/messages/models/MessageListModel.h
 *  @brief This class is a model containing Message objects. This model can be used in the widgets like QListView to display
 *         Message objects contained within this model.
 *
 *  @details QModelIndex objects returned by this model contain internalPointer to the Message objects. To be used by the
 *           MessageListProxyModel.
 *
 *           Supported roles: Qt::DisplayRole, Qt::DecorationRole, Qt::ToolTipRole.
 *
 * @note MessageListModel is responsible for deleting Message objects. This happens in the destructor of MessageListModel and
 *       in the MessageListModel::clear method. */

class MessageListModel final : public QAbstractItemModel
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

    /*! @brief Default constructor. */
    explicit MessageListModel(QObject *parent = nullptr);

    /*! @brief Destructor.
     * @note All Message objects stored inside the model are deleted in the destructor
    ~MessageListModel() final;

    /*! @brief This method is used to add Message objects to the model. Accepts pointer to the Message object. */
    void append(Message* message);

    /*! @brief This method is used to add Message objects to the model. Accepts QList with pointers to the Message* objects. */
    void append(const QList<Message*>& messages);

    /*! @brief This method clears content of this model.
     * @note All Message objects are deleted upon calling this method. */
    void clear();

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

///@name This methods required for inheriting from QAbstractItemModel
///@{
    QModelIndex    index(int row, int column, const QModelIndex &parent = QModelIndex()) const final;
    QModelIndex    parent(const QModelIndex &child) const final;
    int            rowCount(const QModelIndex &parent = QModelIndex()) const final;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const final;
    QVariant       data(const QModelIndex &index, int role = Qt::DisplayRole) const final;
///@}

private:
    quint8 m_displayedContent;
    void _setDisplayedContentBit(DisplayedContent content, bool state);
    QList<Message*> m_data;
};

#endif // MESSAGELISTMODEL_H
