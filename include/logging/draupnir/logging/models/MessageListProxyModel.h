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

#include "draupnir/logging/messages/MessageTypes.h"
#include "draupnir/logging/messages/MessageViewItemFields.h"

namespace Draupnir::Logging
{

/*! @class MessageListProxyModel draupnir/message_system/models/MessageListProxyModel.h
 *  @ingroup Logging
 *  @brief This class is a proxy model for @ref Draupnir::Logging::MessageListModel to allow filtering and formatting of the
 *         displayed @ref Draupnir::Logging::MessageViewItem objects. By default this model will accept any message type and
 *         display everything. */

class MessageListProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    static inline constexpr MessageViewItemFields DefaultDisplayedMessageItemFields =
        MessageViewItemFields::All;

    /*! @brief Default constructor. By default this filter model will accept all messages and display all fields of the
     *         @ref Draupnir::Logging::MessageViewItem objects. */
    explicit MessageListProxyModel(QObject* parent = nullptr);

    /*! @brief Destructor. Trivial. */
    ~MessageListProxyModel() final = default;

///@name Methos to handle what fields of @ref Draupnir::Logging::MessageViewItem to be displayed.
///@{
    /*! @brief Sets what fields of @ref Draupnir::Logging::MessageViewItem objects will be displayed. */
    void setDisplayedMessageViewItemFieldsMask(MessageViewItemFields mask);

    /*! @brief Returns what fields of @ref Draupnir::Logging::MessageViewItem objects will be displayed. */
    MessageViewItemFields displayedMessageViewItemFieldsMask() const { return m_displayedMessageViewItemFields; }

    /*! @brief Sets if specific field of @ref Draupnir::Messages::Message object will be displayed. */
    void setMessageViewItemFieldDisplayed(MessageViewItemField::Value field, bool isVisible);

    /*! @brief Returns `true` if specific field of @ref Draupnir::Logging::MessageViewItem object is displayed. */
    bool isMessageViewItemFieldDisplayed(MessageViewItemField::Value field) const { return m_displayedMessageViewItemFields.test_flag(field); }
///@}

    bool isMessageTypeDisplayed(MessageType type) const;

///@name This is a group
///@{
    /*! @brief Sets what ctegories of @ref Draupnir::Logging::MessageViewItem objects this model should allow displaying. */
    void setDisplayedMessageCategoriesMask(MessageCategories mask);

    /*! @brief Returns allowed categories of @ref Draupnir::Logging::MessageViewItem that will be shown by this model. */
    MessageCategories displayedMessageCategoriesMask() const { return m_displayedMessageCategoriesMask; }

    /*! @brief This is a method. */
    void setMessageCategoryDisplayed(MessageCategory category, bool isVisible);

    /*! @brief Returns `true` if the specified category will be displayed by this model. */
    bool isMessageCategoryDisplayed(MessageCategory messageCategory) { return m_displayedMessageCategoriesMask.test_flag(messageCategory); }
///@}

///@name This is a group
///@{
    void setDisplayedMessageLevelsMask(MessageLevels mask);

    MessageLevels displayedMessageLevelsMask() const { return m_displayedMessageLevelsMask; }

    void setMessageLevelDisplayed(MessageLevel::Value level, bool isVisible);

    bool isMessageLevelDisplayed(MessageLevel::Value level) const { return m_displayedMessageLevelsMask.test_flag(level); }
///@}

    /*! @brief This method is used to adjust displayed data in accordance to configured fields mask. */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const final;

protected:
    /*! @brief Returns true if the index is valid and the @ref Draupnir::Messages::Message object to which this index is
     *         pointing is having the allowed `type`. */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const final;

private:
    MessageCategories     m_displayedMessageCategoriesMask;
    MessageLevels         m_displayedMessageLevelsMask;
    MessageViewItemFields m_displayedMessageViewItemFields;
};

}; // namespace Draupnir::Logging

#endif // MESSAGELISTPROXYMODEL_H
