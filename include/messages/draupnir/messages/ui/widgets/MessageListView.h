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

#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>

#include "draupnir/messages/categories/MessageCategories.h"
#include "draupnir/messages/core/MessageLevels.h"
#include "draupnir/messages/core/MessageViewItemFields.h"

namespace Draupnir::Messages
{

class MessageListModel;
class MessageListProxyModel;

/*! @class MessageListView draupnir/messages/ui/widgets/MessageListView.h
 *  @ingroup Messages
 *  @brief List view for displaying and filtering messages.
 *
 *  @details The view uses a @ref Draupnir::Messages::MessageListProxyModel to filter messages by category and level and to control
 *           which @ref Draupnir::Messages::MessageViewItem fields are displayed.
 *
 *           Double-clicking a message opens a @ref Draupnir::Messages::MessageDisplayDialog containing the currently selected
 *           messages. */

class MessageListView final : public QListView
{
    Q_OBJECT
public:
    /*! @brief Default constructor. Initializes internal @ref Draupnir::Messages::MessageListProxyModel, context menu policies,
     *         etc. */
    explicit MessageListView(QWidget* parent = nullptr);

    /*! @brief Destructor. Delete internal @ref Draupnir::Messages::MessageListProxyModel. */
    ~MessageListView() final;

    /*! @brief Sets the source message model displayed by this view.
     *  @param model Message list model to display, or `nullptr` to clear the current source model.
     * @note Models of types other than @ref Draupnir::Messages::MessageListModel are rejected.
     * @todo Feature: Allow choosing behaviour in the release build: UB vs some fallback. */
    void setModel(QAbstractItemModel* model) override final;

///@name Displayed message fields
///@{
    /*! @brief Sets what fields of @ref Draupnir::Messages::MessageViewItem objects will be displayed.
     * @note No signals are emitted while calling this method. */
    void setDisplayedMessageViewItemFieldsMask(MessageViewItemFields mask);

    /*! @brief Returns what fields of @ref Draupnir::Messages::MessageViewItem objects will be displayed. */
    [[nodiscard]] MessageViewItemFields displayedMessageViewItemFieldsMask() const;

    /*! @brief Sets if specific field of @ref Draupnir::Messages::Message object will be displayed.
     * @note When called - signal @ref Draupnir::Messages::MessageListView::messageViewItemFieldVisibilityChanged will be
     *       emited if visibility of the affected field has changed. */
    void setMessageViewItemFieldDisplayed(MessageViewItemField::Value field, bool isVisible);

    /*! @brief Returns `true` if specific field of @ref Draupnir::Messages::MessageViewItem object is displayed. */
    [[nodiscard]] bool isMessageViewItemFieldDisplayed(MessageViewItemField::Value field) const;
///@}

///@name Message category filtering
///@{
    /*! @brief Sets what categories of @ref Draupnir::Messages::MessageViewItem objects this model should allow displaying. */
    void setDisplayedMessageCategoriesMask(MessageCategories mask);

    /*! @brief Returns allowed categories of @ref Draupnir::Messages::MessageViewItem that will be shown by this model. */
    [[nodiscard]] MessageCategories displayedMessageCategoriesMask() const;

    /*! @brief Sets whether messages of the specified category are displayed.
     *  @param category Message category whose visibility should be changed.
     *  @param isVisible `true` to display messages of `category`; `false` to hide them.
     * @note Emits @ref Draupnir::Messages::MessageListView::messageCategoryVisibilityChanged if the category visibility has
     *       changed. */
    void setMessageCategoryDisplayed(MessageCategory category, bool isVisible);

    /*! @brief Returns `true` if the specified category will be displayed by this model. */
    [[nodiscard]] bool isMessageCategoryDisplayed(MessageCategory messageCategory) const;
///@}

///@name Message level filtering
///@{
    /*! @brief Sets the mask of message levels displayed by this view.
     *  @param mask Message level mask to display.
     * @note No visibility change signals are emitted by this method. */
    void setDisplayedMessageLevelsMask(MessageLevels mask);

    /*! @brief Returns the mask of message levels displayed by this view.
     *  @return Currently displayed message level mask. */
    [[nodiscard]] MessageLevels displayedMessageLevelsMask() const;

    /*! @brief Sets whether messages of the specified level are displayed.
     *  @param level Message level whose visibility should be changed.
     *  @param isVisible `true` to display messages of `level`; `false` to hide them.
     * @note Emits @ref Draupnir::Messages::MessageListView::messageLevelVisibilityChanged if the level visibility has changed. */
    void setMessageLevelDisplayed(MessageLevel::Value level, bool isVisible);

    /*! @brief Returns whether messages of the specified level are displayed.
     *  @param level Message level to query.
     *  @return `true` if messages of `level` are displayed; otherwise, `false`. */
    [[nodiscard]] bool isMessageLevelDisplayed(MessageLevel::Value level) const;
///@}

signals:
    /*! @brief Emitted when visibility of a message view item field changes.
     *  @param field Field whose visibility has changed.
     *  @param isVisible New visibility state of `field`. */
    void messageViewItemFieldVisibilityChanged(Draupnir::Messages::MessageViewItemField::Value field, bool isVisible);

    /*! @brief Emitted when visibility of a message category changes.
     *  @param category Category whose visibility has changed.
     *  @param isVisible New visibility state of `category`. */
    void messageCategoryVisibilityChanged(Draupnir::Messages::MessageCategory category, bool isVisible);

    /*! @brief Emitted when visibility of a message level changes.
     *  @param level Message level whose visibility has changed.
     *  @param isVisible New visibility state of `level`. */
    void messageLevelVisibilityChanged(Draupnir::Messages::MessageLevel::Value level, bool isVisible);

protected:
    /*! @brief Opens a message display dialog for the current selection.
     *  @param event Mouse double-click event.
     *  @details If the double-click occurs over a valid message index, a @ref Draupnir::Messages::MessageDisplayDialog is created and
     *           populated with the currently selected messages.
     * @todo Question: Do we need to call the base class handler? */
    void mouseDoubleClickEvent(QMouseEvent *event) final;

private:
    MessageListProxyModel* const p_messageListProxyModel;
};

} // namespace Draupnir::Messages

#endif // MESSAGELISTVIEW_H
