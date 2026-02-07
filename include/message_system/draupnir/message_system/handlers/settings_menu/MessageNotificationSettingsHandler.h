/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MESSAGENOTIFICATIONSETTINGSENTRYHANDLER_H
#define MESSAGENOTIFICATIONSETTINGSENTRYHANDLER_H

#include <forward_list>

#include "draupnir/message_system/concepts/MessageTraitConcept.h"
#include "draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h"
#include "draupnir/ui_bricks/handlers/settings_menu/SettingsMenuEntryHandlerTemplate.h"
#include "draupnir/message_system/traits/menu_entries/MessageNotificationSettingsEntryTemplate.h"
#include "draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h"

namespace Draupnir::Handlers
{

/*! @headerfile draupnir/message_system/handlers/settings_menu/MessageNotificationSettingsHandler.h
 *  @ingroup MessageSystem
 *  @brief Settings handler specialization for notification-type settings UI entry.
 *  @tparam SettingsContext A settings context type providing `get<Trait>()` and `set<Trait>(value)` APIs.
 *  @tparam MessageType     Message trait/type whose notification setting is being handled.
 *
 *  @details This handler binds one (or multiple) UI menus of type @ref Draupnir::Messages::NotificationTypeMenu to a single
 *           setting @ref Draupnir::Messages::MessageTypeSettingsTrait<MessageType>.
 *
 *           The binding is two-way:
 *           - UI -> Settings: when user changes notification type in the menu, the handler writes the new value to `SettingsContext`.
 *           - Settings -> UI: when settings are loaded, the handler updates all connected menus to reflect the stored value.
 *
 * @warning Lifetime: this handler stores raw pointers to menus. If any menu is destroyed before this handler, calling @ref onSettingsLoaded()
 *          will lead to use-after-free. */

template<class SettingsContext, Draupnir::Messages::MessageTraitConcept MessageType>
class SettingsMenuEntryHandlerTemplate<SettingsContext,Draupnir::Messages::MessageNotificationSettingsEntryTemplate<MessageType>>
{
public:
    /*! @brief Constructs handler bound to the given settings context.
     *  @param context Reference to settings context used for reads/writes of the notification setting. */
    SettingsMenuEntryHandlerTemplate(SettingsContext& context) :
        m_context{context}
    {}

    /*! @brief Connects a notification type menu instance to this handler.
     *  @param menu Pointer to the menu UI object to be synchronized with settings.
     *  @details Stores the menu pointer internally and connects its @ref Draupnir::Messages::NotificationTypeMenu::notificationTypeChanged
     *           signal to update the corresponding setting. */
    void connect(Draupnir::Messages::NotificationTypeMenu* menu) {
        m_menuList.push_front(menu);

        QObject::connect(menu, &Draupnir::Messages::NotificationTypeMenu::notificationTypeChanged,
            [this](Messages::Notification::Type type){
            m_context.template set<Draupnir::Messages::MessageTypeSettingsTrait<MessageType>>(type);
        });
    }

    /*! @brief Synchronizes all connected menus from the current settings value.
     *  @details Reads the notification type from SettingsContext and applies it to every previously connected menu via @ref connect().
     * @note Intended to be called after settings are loaded. */
    void onSettingsLoaded() {
        for (auto menu : m_menuList)
            menu->setNotificationType(m_context.template get<Draupnir::Messages::MessageTypeSettingsTrait<MessageType>>());
    }

private:
    SettingsContext& m_context;
    std::forward_list<
        Messages::NotificationTypeMenu*
    > m_menuList;
};

/*! @brief Convenience alias for notification settings handler bound to a specific entry type.
 *  @tparam SettingsContext Settings context type.
 *  @tparam Entry           Entry type (typically NotificationsSettingsMenuEntryTemplate<MessageType>). */

template<class SettingsContext, class Entry>
using MessageNotificationSettingsHandler = SettingsMenuEntryHandlerTemplate<SettingsContext,Entry>;

}; // namespace Draupnir::Handlers

#endif // MESSAGENOTIFICATIONSETTINGSENTRYHANDLER_H
