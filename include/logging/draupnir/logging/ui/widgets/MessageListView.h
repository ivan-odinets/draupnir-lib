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

#include "draupnir/logging/messages/MessageTypes.h"
#include "draupnir/logging/messages/MessageViewItemFields.h"

namespace Draupnir::Logging
{

class MessageListModel;
class MessageListProxyModel;

/*! @class MessageListView draupnir/logging/ui/widgets/MessageListView.h
 *  @ingroup Logging
 *  @brief `QListView` subclass used for displaying @ref Draupnir::Logging::MessageListModel with filtering and display options.
 *
 *  @details This class extends `QListView` to provide specialized options for displaying @ref Draupnir::Logging::MessageListModel
 *           contents. Internally, it uses a @ref Draupnir::Logging::MessageListProxyModel for filtering messages by category type.
 *           Additionally, it allows control over which fields of each @ref Draupnir::Logging::MessageItemView (e.g., `brief`,
 *           `what`, `icon`, `dateTime`) should be visible.
 *
 *           The widget is also interactive: double-clicking a message opens a @ref Draupnir::Logging::MessageDisplayDialog
 *           containing selected messages. */

class MessageListView final : public QListView
{
    Q_OBJECT
public:
    /*! @brief Default constructor. Initializes internal @ref Draupnir::Logging::MessageListProxyModel, context menu policies,
     *         etc. */
    explicit MessageListView(QWidget* parent = nullptr);

    /*! @brief Destructor. Delete internal @ref Draupnir::Logging::MessageListProxyModel. */
    ~MessageListView() final;

    /*! @brief Pass @ref Draupnir::Messages::MessageListModel here to display.
     * @note Passing any other `QAbstractItemModel`-derivative - will result in UB (or crash in debug). */
    void setModel(QAbstractItemModel* model) override final;

///@name Methos to handle what fields of @ref Draupnir::Logging::MessageViewItem to be displayed.
///@{
    /*! @brief Sets what fields of @ref Draupnir::Logging::MessageViewItem objects will be displayed.
     * @note No signals are emitted while calling this method. */
    void setDisplayedMessageViewItemFieldsMask(MessageViewItemFields mask);

    /*! @brief Returns what fields of @ref Draupnir::Logging::MessageViewItem objects will be displayed. */
    MessageViewItemFields displayedMessageViewItemFieldsMask() const;

    /*! @brief Sets if specific field of @ref Draupnir::Messages::Message object will be displayed.
     * @note When called - signal @ref Draupnir::Logging::MessageListView::messageViewItemFieldVisibilityChanged will be
     *       emited if visibility of the affected field has changed. */
    void setMessageViewItemFieldDisplayed(MessageViewItemField::Value field, bool isVisible);

    /*! @brief Returns `true` if specific field of @ref Draupnir::Logging::MessageViewItem object is displayed. */
    bool isMessageViewItemFieldDisplayed(MessageViewItemField::Value field) const;
///@}

///@name This is a group
///@{
    /*! @brief Sets what ctegories of @ref Draupnir::Logging::MessageViewItem objects this model should allow displaying. */
    void setDisplayedMessageCategoriesMask(MessageCategories mask);

    /*! @brief Returns allowed categories of @ref Draupnir::Logging::MessageViewItem that will be shown by this model. */
    MessageCategories displayedMessageCategoriesMask() const;

    /*! @brief This is a method. */
    void setMessageCategoryDisplayed(MessageCategory category, bool isVisible);

    /*! @brief Returns `true` if the specified category will be displayed by this model. */
    bool isMessageCategoryDisplayed(MessageCategory messageCategory);
///@}

///@name This is a group
///@{
    void setDisplayedMessageLevelsMask(MessageLevels mask);

    MessageLevels displayedMessageLevelsMask() const;

    void setMessageLevelDisplayed(MessageLevel::Value level, bool isVisible);

    bool isMessageLevelDisplayed(MessageLevel::Value level) const;
///@}

signals:
    /*! @brief This signal is emitted when a visibility of specific field of @ref Draupnir::Logging::MessageViewItem
     *         object has changed. */
    void messageViewItemFieldVisibilityChanged(Draupnir::Logging::MessageViewItemField::Value field, bool isVisible);

protected:
    /*! @brief Shows @ref Draupnir::Logging::MessageDisplayDialog by double click. */
    void mouseDoubleClickEvent(QMouseEvent *event) final;

private:
    MessageListModel* p_messageList;
    MessageListProxyModel* p_messageListProxyModel;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTVIEW_H
