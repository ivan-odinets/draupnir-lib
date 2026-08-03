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

#ifndef MESSAGE_DISPLAY_DIALOG_HELPERS
#define MESSAGE_DISPLAY_DIALOG_HELPERS

#include <expected>

#include "draupnir/messages/ui/windows/MessageDisplayDialog.h"

/*! @class MessageDisplayDialogHelpers draupnir-tests/common/MessageDisplayDialogHelpers.h
 *  @ingroup MessagesTests
 *  @brief Provides reusable checks for message display dialog tests.
 *
 *  @details Contains helpers for waiting for a @ref Draupnir::Messages::MessageDisplayDialog, validating its displayed messages,
 *  closing it, and verifying its deletion. */

class MessageDisplayDialogHelpers
{
public:
    /*! @brief Guarded pointer to a message display dialog. */
    using DialogPtr = QPointer<Draupnir::Messages::MessageDisplayDialog>;

    /*! @brief Result of a dialog verification operation.
     *  @details Contains no value on success and a diagnostic message on failure. */
    using CheckResult = std::expected<void, QString>;

    /*! @brief Waits for a message display dialog to appear.
     *  @return Guarded pointer to the first matching top-level dialog, or a null pointer when no dialog appears before the timeout. */
    [[nodiscard]] static DialogPtr waitForDialog();

    /*! @brief Verifies the messages displayed by a dialog.
     *  @param dialog Dialog to inspect.
     *  @param expectedMessages Messages expected to be displayed.
     *  @return Successful result when the displayed content matches `expectedMessages`; otherwise, a diagnostic error.
     * @pre `dialog` must not be null. */
    [[nodiscard]] static CheckResult dialogHasExpectedContent(DialogPtr dialog, const Draupnir::Messages::MessageList& expectedMessages);

    /*! @brief Waits for a dialog, verifies its content, and closes it.
     *  @param expectedMessages Messages expected to be displayed.
     *  @return Successful result when the dialog appears with the expected content and is deleted after closure; otherwise, a diagnostic
     *          error. */
    [[nodiscard]] static CheckResult dialogWasDisplayedWith(const Draupnir::Messages::MessageList& content);
};

#endif // MESSAGE_DISPLAY_DIALOG_HELPERS
