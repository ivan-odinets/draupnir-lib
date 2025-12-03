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

#ifndef NOTIFICATIONSSETTINGSMENUENTRYTEMPLATE_H
#define NOTIFICATIONSSETTINGSMENUENTRYTEMPLATE_H

#include "draupnir/message_system/core/AbstractMessageUiBuilder.h"
#include "draupnir/message_system/ui/NotificationSettingsMenuEntriesContext.h"

namespace Draupnir::Messages
{

/*! @class NotificationsSettingsMenuEntryTemplate draupnir/message_system/traits/menu_entries/NotificationsSettingsMenuEntryTemplate.h
 *  @ingroup MessageSystem
 *  @brief Trait template for notification settings menu entries.
 *  @tparam MessageTrait A message trait class that defines metadata for a specific message type.
 *
 *  @details This template provides a static interface for generating `QMenu` entries corresponding to a specific message type’s
 *           notification settings. It uses the global UI builder registered in @ref NotificationSettingsMenuContext to create
 *           an appropriate @ref NotificationTypeMenu widget for the message type.
 *
 *           The provided `MessageTrait` must contain:
 *           - `static constexpr MessageType type` — unique identifier for the message type;
 *           - `static QString displayName()` — user-visible name for the message type. */

template<class MessageTrait>
class NotificationsSettingsMenuEntryTemplate
{
public:
    /*! @brief Type alias for the widget produced by this entry (always QMenu). */
    using Type = QMenu;

    /*! @brief Returns a new instance of the @ref Draupnir::Messages::NotificationTypeMenu with the display name.
     *  @param parent Optional parent widget.
     *  @return Pointer to newly created @ref Draupnir::Messages::NotificationTypeMenu.
     * @note Title of the menu will be set using MessageTrait::displayName(). */
    static QMenu* createElement(QWidget* parent = nullptr) {
        auto* result = NotificationSettingsMenuContext::uiBuilder()->createNotificationSettingsMenu(MessageTrait::type, parent);
        result->setTitle(MessageTrait::displayName());
        return result;
    }

    /*! @brief Returns the display name for this entry.
     *  @return Localized `QString`. */
    static QString displayName() {
        return MessageTrait::displayName();
    }
};

}; // namespace Draupnir::Messages

#endif // NOTIFICATIONSSETTINGSMENUENTRYTEMPLATE_H
