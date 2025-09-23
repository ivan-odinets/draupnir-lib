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

#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QListView>

#include "core/MessageType.h"

namespace Draupnir::Messages
{

class MessageListModel;
class MessageListProxyModel;

/*! @class MessageListView draupnir-lib/src/messages/widgets/MessageListView.h
 *  @brief QListView subclass tailored for displaying `MessageListModel` with filtering and display options.
 *  @details This class extends `QListView` to provide specialized support for displaying `MessageListModel` contents. Internally,
 *           it uses a `MessageListProxyModel` for filtering messages by category (type). Additionally, it allows fine-grained
 *           control over which fields of each `Message` (e.g., `brief`, `what`, `icon`, `dateTime`) should be visible.
 *
 *           The widget is also interactive: double-clicking a message opens a `MessageDisplayDialog` containing selected messages.
 *
 * @note A valid MessageListModel must be set before calling any of the display configuration methods, otherwise assertions will
 *       be triggered in debug builds or UB will happen in release.
 *
 * @see MessageListModel, MessageListProxyModel, MessageDisplayDialog */

class MessageListView final : public QListView
{
    Q_OBJECT
public:
    /*! @brief Default constructor. */
    explicit MessageListView(QWidget* parent = nullptr);

    /*! @brief Destructor. Delete internal MessageListProxyModel. */
    ~MessageListView() final;

    /*! @brief Pass MessageListModel* here to display.
     * @note Passing any other QAbstractItemModel-derivative - will result in UB (or crash in debug). */
    void setModel(QAbstractItemModel* model) override final;

    /*! @brief Sets what type of Message objects should be displayed within this MessageListView.
     * @note No signals are emitted while calling this method. */
    void applyMessageTypeFilter(MessageType type);

    /*! @brief Returns what type of Message objects are displayed within this MessageListView. */
    MessageType messageTypeFilter() const;

    /*! @brief Returns true if this widget will display content of Message::brief.
     * @note Calling this method before specifying model - will result in UB (or crash in debug). */
    bool isBriefDisplayed() const;

    /*! @brief Allows setting if this widget will display content of Message::brief.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    void setBriefDisplayed(bool state);

    /*! @brief Returns true if this widget will display content of Message::what.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    bool isWhatDisplayed() const;

    /*! @brief Allows setting if this widget will display content of Message::what.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    void setWhatDisplayed(bool state);

    /*! @brief Returns true if this widget will display content of Message::dateTime.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    bool isDateTimeDisplayed() const;

    /*! @brief Allows setting if this widget will display content of Message::dateTime.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    void setDateTimeDisplayed(bool state);

    /*! @brief Returns true if this widget will display content of Message::icon.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    bool isIconDisplayed() const;

    /*! @brief Allows setting if this widget will display content of Message::icon.
     * @note Calling this method before specifying model - will result in UB (or crash in debug).*/
    void setIconDisplayed(bool state);

signals:
    /*! @brief This signal is emitted when a visibility of specific MessageType has changed. */
    void messageTypeVisibilityChanged(MessageType messageType, bool isVisible);

public slots:
    /*! @todo This method is used to enable / disable viewing of the individual message types.
     * @note After triggering - signal messageTypeVisibilityChanged is emited. */
    void setMessageTypeDisplayed(MessageType type, bool displayed);

protected:
    /*! @brief Override this to show MessageDisplayDialog by double click. */
    void mouseDoubleClickEvent(QMouseEvent *event) final;

private:
    void _retranslateUi();

    MessageListModel* p_messageList;
    MessageListProxyModel* p_messageListProxyModel;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTVIEW_H
