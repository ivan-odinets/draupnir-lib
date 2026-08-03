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

#ifndef MESSAGEDISPLAYWIDGET_H
#define MESSAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QSize>

class QLabel;

#include "draupnir/messages/core/Message.h"

namespace Draupnir::Messages
{

/*! @class MessageDisplayWidget draupnir/messages/ui/widgets/MessageDisplayWidget.h
 *  @ingroup Messages
 *  @brief Widget for displaying the details of a single message.
 *
 *  @details Displays the brief description, detailed text, creation time, and icon of a @ref Draupnir::Messages::Message.
 *
 *           The widget consists of an icon and a form layout containing labels for the textual message fields.
 *
 *           The displayed message may be replaced or cleared at runtime. User-facing labels are updated automatically
 *           when the application language changes. */

class MessageDisplayWidget final : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Constructs an empty message details widget.
     *  @param parent Parent widget. */
    explicit MessageDisplayWidget(QWidget *parent = nullptr);

    /*! @brief Clears the currently displayed message.
     *  @details Removes all displayed message data and releases the widget's shared ownership of the previously displayed
     *           message. */
    ~MessageDisplayWidget() final = default;

    /*! @brief Clears the currently displayed message.
     *  @details Removes all displayed message data and releases the widget's shared ownership of the previously displayed
     *           message. */
    void clear();

    /*! @brief Displays a message while retaining shared ownership of it.
     *  @param message Message to display.
     *  @details Copies the supplied shared pointer into the widget. The underlying message object itself is not copied.
     * @pre `message` must not be null.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void showMessage(const MessagePtr& message);

    /*! @brief Displays a message by transferring its shared pointer.
     *  @param message Message pointer whose ownership is moved into the widget.#
     *  @details Moves the supplied shared pointer into the widget without incrementing its reference count.
     * @pre `message` must not be null.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void showMessage(MessagePtr&& message);

    /*! @brief Returns the currently displayed message.
     *  @return Shared pointer to the displayed message, or a null pointer when the widget is empty. */
    MessagePtr message() const { return p_message; }

protected:
    /*! @brief Handles application language changes.
     *  @param event Change event received by the widget.
     *  @details Retranslates user-facing labels when @p event represents `QEvent::LanguageChange`. */
    void changeEvent(QEvent* event) final;

private:
    void _updateView();
    void _retranslateUi();

    MessagePtr p_message;

    QSize m_iconSize;

    QLabel* w_messageBriefNameLabel;
    QLabel* w_messageBriefLabel;
    QLabel* w_messageWhatNameLabel;
    QLabel* w_messageWhatLabel;
    QLabel* w_messageDateTimeNameLabel;
    QLabel* w_messageDateTimeLabel;
    QLabel* w_messageIconLabel;
};

} // namespace Draupnir::Messages

#endif // MESSAGEDISPLAYWIDGET_H
