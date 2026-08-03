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

#include "draupnir/messages/core/Message.h"
#include "draupnir/messages/core/MessageType.h"
#include "draupnir/messages/core/MessageViewItemFields.h"

namespace Draupnir::Messages
{

class MessageListModel;

/*! @class MessageListProxyModel draupnir/messages/models/MessageListProxyModel.h
 *  @ingroup Messages
 *  @brief Proxy model for @ref Draupnir::Messages::MessageListModel.
 *
 *  @details This proxy model filters messages by @ref Draupnir::Messages::MessageLevel and @ref Draupnir::Messages::MessageCategory
 *           and adjusts the data returned for display according to the configured @ref Draupnir::Messages::MessageViewItemFields
 *           mask.
 *
 *           By default, all message levels, all message categories, and all view item fields are displayed.
 *
 * @todo Feature: Extract template base from this class to simplify proxy list model creation. */

class MessageListProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*! @brief Default mask of message view-item fields displayed by the proxy. */
    static inline constexpr MessageViewItemFields
        DefaultDisplayedMessageItemFields = MessageViewItemFields::All;

    /*! @brief Default mask of message categories accepted by the proxy. */
    static inline constexpr MessageCategories
        DefaultDisplayedMessageCategories = MessageCategories::All;

    /*! @brief Default mask of message levels accepted by the proxy. */
    static inline constexpr MessageLevels
        DefaultDisplayedMessageLevels = MessageLevels::All;

    /*! @brief Constructs a proxy model displaying all message fields and accepting all message categories and levels.
     *  @param parent Optional parent object. */
    explicit MessageListProxyModel(QObject* parent = nullptr);

    /*! @brief Destroys the proxy model. */
    ~MessageListProxyModel() final = default;

    /*! @brief Sets the underlying message list model.
     *  @param model Source model to use.
     * @pre @p model must point to a valid @ref Draupnir::Messages::MessageListModel.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void setSourceModel(QAbstractItemModel* model) override final;

    /*! @brief Returns the underlying message list model.
     *  @return Non-owning pointer to the source @ref Draupnir::Messages::MessageListModel.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] MessageListModel* underlyingMessageListModel() const;

    /*! @brief Returns the message represented by an index.
     *  @param index Valid index.
     *  @return Shared pointer to the represented message.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] MessagePtr messageAt(int index) const;

    /*! @brief Returns the message represented by a proxy-model index.
     *  @param index Valid index belonging to this proxy model.
     *  @return Shared pointer to the represented message.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] MessagePtr messageAt(const QModelIndex& index) const;

/// @name Displayed message view-item fields
/// @{
    /*! @brief Sets the mask of message view-item fields used for display.
     *  @param mask Fields that should be included in the displayed representation. */
    void setDisplayedMessageViewItemFieldsMask(MessageViewItemFields mask);

    /*! @brief Returns the currently displayed message view-item fields.
     *  @return Current displayed-fields mask. */
    [[nodiscard]] MessageViewItemFields displayedMessageViewItemFieldsMask() const { return m_displayedMessageViewItemFields; }

    /*! @brief Changes the visibility of one message view-item field.
     *  @param field Field whose visibility should be changed.
     *  @param isVisible Whether the field should be displayed. */
    void setMessageViewItemFieldDisplayed(MessageViewItemField::Value field, bool isVisible);

    /*! @brief Checks whether a message view-item field is displayed.
     *  @param field Field to check.
     *  @return `true` if the field is displayed; otherwise `false`. */
    [[nodiscard]] bool isMessageViewItemFieldDisplayed(MessageViewItemField::Value field) const {
        return m_displayedMessageViewItemFields.test_flag(field);
    }
///@}

    /*! @brief Checks whether a message type passes the current filters.
     *  @param type Message type to check.
     *  @return `true` if both its category and level are enabled; otherwise `false`. */
    [[nodiscard]] bool isMessageTypeDisplayed(MessageType type) const;

/// @name Displayed message categories
/// @{
    /*! @brief Sets the mask of message categories accepted by the proxy.
     *  @param mask Categories that should be displayed. */
    void setDisplayedMessageCategoriesMask(MessageCategories mask);

    /*! @brief Returns the currently displayed message categories.
     *  @return Current category-filter mask. */
    [[nodiscard]] MessageCategories displayedMessageCategoriesMask() const { return m_displayedMessageCategoriesMask; }

    /*! @brief Changes whether one message category is displayed.
     *  @param category Category whose visibility should be changed.
     *  @param isVisible Whether messages of the category should be displayed. */
    void setMessageCategoryDisplayed(MessageCategory category, bool isVisible);

    /*! @brief Checks whether a message category is displayed.
     *  @param messageCategory Category to check.
     *  @return `true` if messages of the category are displayed; otherwise `false`. */
    [[nodiscard]]
    bool isMessageCategoryDisplayed(MessageCategory messageCategory) const {
        return m_displayedMessageCategoriesMask.test_flag(messageCategory);
    }
///@}

///@name Displayed message levels
///@{

    /*! @brief Sets the mask of message levels accepted by the proxy.
     *  @param mask Levels that should be displayed. */
    void setDisplayedMessageLevelsMask(MessageLevels mask);

    /*! @brief Returns the currently displayed message levels.
     *  @return Current level-filter mask. */
    [[nodiscard]] MessageLevels displayedMessageLevelsMask() const { return m_displayedMessageLevelsMask; }

    /*! @brief Changes whether one message level is displayed.
     *  @param level Level whose visibility should be changed.
     *  @param isVisible Whether messages of the level should be displayed. */
    void setMessageLevelDisplayed(MessageLevel::Value level, bool isVisible);

    /*! @brief Checks whether a message level is displayed.
     *  @param level Level to check.
     *  @return `true` if messages of the level are displayed; otherwise `false`. */
    [[nodiscard]]bool isMessageLevelDisplayed(MessageLevel::Value level) const {
        return m_displayedMessageLevelsMask.test_flag(level);
    }
///@}

    /*! @brief Returns presentation data for a proxy-model index.
     *  @param index Proxy-model index whose data is requested.
     *  @param role Requested Qt item-data role.
     *  @return Display text assembled according to the configured field mask, the message icon, tooltip text, or data forwarded
     *          from the source model for roles not handled explicitly. */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const final;

protected:
    /*! @brief Determines whether a source-model row passes the configured filters.
     *  @param sourceRow Row in the source model.
     *  @param sourceParent Parent index in the source model.
     *  @return `true` if the row represents a message whose category and level are enabled; otherwise `false`. */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const final;

private:
    void _notifyDisplayedDataChanged();

    /*! @brief Fields included in the displayed message representation. */
    MessageViewItemFields    m_displayedMessageViewItemFields;

    /*! @brief Message categories accepted by the proxy filter. */
    MessageCategories        m_displayedMessageCategoriesMask;

    /*! @brief Message levels accepted by the proxy filter. */
    MessageLevels            m_displayedMessageLevelsMask;
};

} // namespace Draupnir::Messages

#endif // MESSAGELISTPROXYMODEL_H
