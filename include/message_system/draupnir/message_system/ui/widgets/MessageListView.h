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

#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/message_system/core/MessageFields.h"

namespace Draupnir::Messages
{

class MessageListModel;
class MessageListProxyModel;

/*! @class MessageListView draupnir/message_system/ui/widgets/MessageListView.h
 *  @ingroup MessageSystem
 *  @brief `QListView` subclass used for displaying @ref Draupnir::Messages::MessageListModel with filtering and display options.
 *
 *  @details This class extends `QListView` to provide specialized options for displaying @ref Draupnir::Messages::MessageListModel
 *           contents. Internally, it uses a @ref Draupnir::Messages::MessageListProxyModel for filtering messages by category type.
 *           Additionally, it allows control over which fields of each @ref Draupnir::Messages::Message (e.g., `brief`,
 *           `what`, `icon`, `dateTime`) should be visible.
 *
 *           The widget is also interactive: double-clicking a message opens a @ref Draupnir::Messages::MessageDisplayDialog
 *           containing selected messages. */

class MessageListView final : public QListView
{
    Q_OBJECT
public:
    /*! @brief Default constructor. Initializes internal @ref Draupnir::Messages::MessageListProxyModel, context menu policies,
     *         etc. */
    explicit MessageListView(QWidget* parent = nullptr);

    /*! @brief Destructor. Delete internal @ref Draupnir::Messages::MessageListProxyModel. */
    ~MessageListView() final;

    /*! @brief Pass @ref Draupnir::Messages::MessageListModel here to display.
     * @note Passing any other `QAbstractItemModel`-derivative - will result in UB (or crash in debug). */
    void setModel(QAbstractItemModel* model) override final;

    /*! @brief Sets what type of @ref Draupnir::Messages::Message objects should be displayed within this widget.
     * @note No signals are emitted while calling this method. */
    void setDisplayedMessageTypesMask(MessageType type);

    /*! @brief This method returns mask of message types which are displayed by this @ref Draupnir::Messages::MessageListView
     *         widget. */
    MessageType displayedMessageTypesMask() const;

    /*! @brief Returns whether the specified @ref Draupnir::Messages::MessageType is currently selected for display. */
    bool isMessageTypeDisplayed(MessageType messageType);

    /*! @brief Sets what fields of the @ref Draupnir::Messages::Message objects should be displayed within this widget. */
    void setDisplayedMessageFieldsMask(MessageFields fields);

    /*! @brief This method returns mask of the parts of the @ref Draupnir::Messages::Message object which are marked as
     *         displayed within the @ref Draupnir::Messages::MessageListView widget. */
    MessageFields displayedMessageFieldsMask() const;

    /*! @brief Returns true if specified field of the @ref Draupnir::Messages::Message object is displayed. */
    bool isMessageFieldDisplayed(MessageField field) const;

signals:
    /*! @brief This signal is emitted when a visibility of specific @ref Draupnir::Messages::MessageType has changed. */
    void messageTypeVisibilityChanged(Draupnir::Messages::MessageType messageType, bool isVisible);

    /*! @brief This signal is emitted when a visibility of specific field of @ref Draupnir::Messages::Message object
     *         has changed. */
    void messageFieldVisibilityChanged(Draupnir::Messages::MessageField field, bool isVisible);

protected:
    /*! @brief Override this to show @ref Draupnir::Messages::MessageDisplayDialog by double click. */
    void mouseDoubleClickEvent(QMouseEvent *event) final;

public slots:
    /*! @todo This method is used to enable / disable viewing of the individual message types.
     * @note After triggering - signal @ref Draupnir::Messages::MessageListView::messageTypeVisibilityChanged is emited. */
    void setMessageTypeDisplayed(Draupnir::Messages::MessageType type, bool displayed);

    /*! @todo This method is used to toggle visibility of the different fields of of the @ref Draupnir::Message::Message
     *        objects.
     * @note After triggering - signal @ref Draupnir::Messages::MessageListView::messageFieldVisibilityChanged is emited. */
    void setMessageFieldDisplayed(MessageField field, bool isVisible);

private:
    MessageListModel* p_messageList;
    MessageListProxyModel* p_messageListProxyModel;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTVIEW_H
