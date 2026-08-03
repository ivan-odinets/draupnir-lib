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

#ifndef MESSAGEDISPLAYDIALOG_H
#define MESSAGEDISPLAYDIALOG_H

#include <QDebug>
#include <QDialog>

class QDialogButtonBox;
class QScrollArea;
class QVBoxLayout;

#include "draupnir/messages/core/Message.h"

namespace Draupnir::Messages
{

/*! @class MessageDisplayDialog draupnir/messages/ui/windows/MessageDisplayDialog.h
 *  @ingroup Messages
 *  @brief Dialog window used for displaying one or more @ref Draupnir::Messages::Message objects in a scrollable view.
 *
 *  @details This class provides a scrollable dialog for showing one ore several @ref Draupnir::Messages::Message instances
 *           using @ref Draupnir::Messages::MessageDisplayWidget.
 *
 *           Internally, the dialog consists of:
 *           - A `QScrollArea` containing a vertical layout with `MessageDisplayWidget` instances
 *           - A `QDialogButtonBox` with an "OK" button
 *
 *          Each message is wrapped in its own widget and inserted above a final stretch item for correct alignment. */

class MessageDisplayDialog final : public QDialog
{
    Q_OBJECT
public:
    /*! @brief Constructs a @ref Draupnir::Messages::MessageDisplayDialog with optional parent. Initializes internal layout,
     *         scroll area and OK button.
     *  @param parent The parent widget, or nullptr if none. */
    explicit MessageDisplayDialog(QWidget* parent = nullptr);

    /*! @brief Destructor. No manual cleanup required. */
    ~MessageDisplayDialog() final = default;

    /*! @brief Adds a single @ref Draupnir::Messages::MessagePtr to the dialog.
     *  @param message Valid @ref Draupnir::Messages::Ptr object. Must not be nullptr.
     *  @details A new @ref Draupnir::Messages::MessageDisplayWidget is created for the message and inserted into the
     *           scrollable layout.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void addMessage(const MessagePtr& message);

    /*! @brief Adds a single @ref Draupnir::Messages::MessagePtr to the dialog.
     *  @param message Valid @ref Draupnir::Messages::Ptr object. Must not be nullptr.
     *  @details A new @ref Draupnir::Messages::MessageDisplayWidget is created for the message and inserted into the
     *           scrollable layout.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void addMessage(MessagePtr&& message);

    /*! @brief Adds multiple @ref Draupnir::Messages::MessagePtr objects to the dialog.
     *  @param messages List of the @ref Draupnir::Messages::MessagePtr objects. Null entries are not allowed.
     *  @details Convenience method for batch-adding messages.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    void addMessageList(const MessageList& messageList);

    /*! @brief Removes all displayed messages from the dialog.
     *  @details Deletes all child widgets from the scrollable layout. */
    void clear();

private:
    QWidget* w_containerWidget;
    QVBoxLayout* w_scrollableLayout;
    QScrollArea* w_scrollArea;
    QDialogButtonBox* w_buttons;
};

} // namespace Draupnir::Messages

#endif // MESSAGEDISPLAYDIALOG_H
