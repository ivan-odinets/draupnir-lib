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

#ifndef MESSAGENOTIFICATIONSETTINGSENTRYTEMPLATE_H
#define MESSAGENOTIFICATIONSETTINGSENTRYTEMPLATE_H

#include "draupnir/message_system/concepts/MessageTraitConcept.h"
#include "draupnir/message_system/ui/menus/NotificationTypeMenu.h"
#include "draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h"

namespace Draupnir::Messages
{

/*! @class MessageNotificationSettingsEntryTemplate draupnir/message_system/traits/menu_entries/MessageNotificationSettingsEntryTemplate.h
 *  @ingroup MessageSystem
 *  @brief Trait template for notification settings menu entries.
 *  @tparam MessageTrait A message trait class that defines metadata for a specific message type.
 *
 *  @details This template provides a static interface for generating `QMenu` entries corresponding to a specific message type’s
 *           notification settings.
 *
 *           The provided `MessageTrait` must contain:
 *           - `static constexpr MessageType type` — unique identifier for the message type;
 *           - `static QString displayName()` — user-visible name for the message type. */

template<MessageTraitConcept MessageTrait>
class MessageNotificationSettingsEntryTemplate
{
public:
    /*! @brief Type alias for the widget produced by this entry. */
    using Type = NotificationTypeMenu;

    /*! @brief Returns a new instance of the @ref Draupnir::Messages::NotificationTypeMenu with the display name.
     *  @param parent Optional parent widget.
     *  @return Pointer to newly created @ref Draupnir::Messages::NotificationTypeMenu.
     * @note Title of the menu will be set using MessageTrait::displayName(). */
    static NotificationTypeMenu* createElement(QWidget* parent = nullptr) {
        auto* result = new NotificationTypeMenu{parent};
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

#endif // MESSAGENOTIFICATIONSETTINGSENTRYTEMPLATE_H
