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

#include "draupnir-test/helpers/MessageDisplayDialogHelpers.h"

#include "draupnir/messages/ui/widgets/MessageDisplayWidget.h"
#include "draupnir-test/helpers/UiTestHelper.h"

MessageDisplayDialogHelpers::DialogPtr MessageDisplayDialogHelpers::waitForDialog()
{
    return UiTestHelper::waitForFirstTopLevelWidget<Draupnir::Messages::MessageDisplayDialog>(1500);
}

std::expected<void, QString> MessageDisplayDialogHelpers::dialogHasExpectedContent(
    MessageDisplayDialogHelpers::DialogPtr dialog,
    const Draupnir::Messages::MessageList& expectedMessages
) {
    const auto childWidgets = dialog->findChildren<Draupnir::Messages::MessageDisplayWidget*>();
    if (childWidgets.size() != expectedMessages.count())
        return std::unexpected{QString("MessageDisplayDialog is expected to show %1 messages. Found: %2 nested widgets.")
                                   .arg(expectedMessages.count()).arg(childWidgets.size())};

    Draupnir::Messages::MessageList displayedMessages;
    // Lets check that all messages, from the dialog are expected
    for (int i = 0; i < childWidgets.size(); i++) {
        auto current = childWidgets.at(i)->message();
        if (!expectedMessages.contains(childWidgets.at(i)->message())) {
            return std::unexpected{QString{"Message (brief=%1; what=%2; dateTime=%3) is expected to be displayed, but it is not."}
                                       .arg(current->brief()).arg(current->what()).arg(current->dateTime().toString())};
        }
        displayedMessages.append(std::move(current));
    }

    // Lets check that all messages expected displayed
    for (int i = 0; i < expectedMessages.count(); i++) {
        const auto& current = expectedMessages.at(i);
        if (!displayedMessages.contains(current)) {
            return std::unexpected{QString{"Message (brief=%1; what=%2; dateTime=%3) is displayed, but it is not expected to be displayed."}
                                       .arg(current->brief()).arg(current->what()).arg(current->dateTime().toString())};
        }
    }

    return std::expected<void, QString>{};
}

std::expected<void, QString> MessageDisplayDialogHelpers::dialogWasDisplayedWith(const Draupnir::Messages::MessageList& content)
{
    using namespace Draupnir::Messages;

    auto dialogPtr = waitForDialog();
    if (dialogPtr == nullptr)
        return std::unexpected{QString("No MessageDisplayDialog found.")};

    const auto areExpectedThingsShown = dialogHasExpectedContent(dialogPtr,content);
    dialogPtr->close();

    QElapsedTimer timer;
    timer.start();

    while (dialogPtr && timer.elapsed() < 1000) {
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }

    if (!dialogPtr.isNull())
        return std::unexpected{QString{"MessageDisplayDialog is expected to be deletable on closure."}};

    return areExpectedThingsShown;
}
