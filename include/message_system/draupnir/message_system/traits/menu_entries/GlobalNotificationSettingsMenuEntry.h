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

#ifndef GLOBALNOTIFICATIONSETTINGSMENUENTRY_H
#define GLOBALNOTIFICATIONSETTINGSMENUENTRY_H

#include "draupnir/message_system/core/AbstractMessageUiBuilder.h"
#include "draupnir/message_system/ui/NotificationSettingsMenuEntriesContext.h"

namespace Draupnir::Messages
{

/*! @class GlobalNotificationSettingsMenuEntry draupnir/message_system/traits/menu_entries/GlobalNotificationSettingsMenuEntry.h
 *  @ingroup MessageSystem
 *  @brief Menu entry for configuring global notification settings.
 *
 *  @details This class provides a factory interface for creating a `QMenu` widget that allows configuration of notification
 *           preferences for **all** registered message types in the associated @ref Draupnir::Messages::MessageSystemTemplate.
 *
 *           Unlike per-message-type menu entries (e.g. @ref NotificationsSettingsMenuEntryTemplate), this entry provides a
 *           single unified configuration menu for all types.
 *
 *           The menu is created using the globally registered @ref AbstractMessageUiBuilder provided via @ref NotificationSettingsMenuContext. */

class GlobalNotificationSettingsMenuEntry
{
public:
    using Type = QMenu;

    /*! @brief Returns `QMenu` capable of configuring notification settings for @ref Draupnir::Messages::MessageSystemTemplate
     *         instantication.
     *  @param parent Optional parent widget.
     *  @return Pointer to newly created @ref Draupnir::Messages::NotificationTypeMenu. */
    static QMenu* createElement(QWidget* parent = nullptr) {
        auto* result = NotificationSettingsMenuContext::uiBuilder()->createNotificationSettingsMenuForTypes(MessageType::AllMessages,parent);
        result->setTitle(displayName());
        return result;
    }

    /*! @brief Returns the display name for this entry.
     *  @return Localized `QString`. */
    static QString displayName() {
        return QObject::tr("Notifications");
    }
};

}; // namespace Draupnir::Messages

#endif // GLOBALNOTIFICATIONSETTINGSMENUENTRY_H
