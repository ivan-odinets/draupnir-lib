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

#ifndef NOTIFICATIONSETTINGSMENUENTRIESCONTEXT_H
#define NOTIFICATIONSETTINGSMENUENTRIESCONTEXT_H

#include <QtGlobal>

class AbstractMessageUiBuilder;

namespace Draupnir::Messages
{

/*! @class NotificationSettingsMenuContext draupnir/message_system/ui/NotificationSettingsMenuContext.h
 *  @ingroup MessageSystem
 *  @brief Static context holder for accessing the current @ref AbstractMessageUiBuilder within notification settings menu
 *         entries.
 *
 *  @details This utility class provides a global static pointer to the current @ref Draupnir::Messages::AbstractMessageUiBuilder
 *           instance used by notification settings-related UI components.
 *
 *           It is primarily used to allow stateless or trait-based menu entries (such as @ref GlobalNotificationSettingsMenuEntry
 *           or @ref NotificationsSettingsMenuEntryTemplate) to access UI builder services without explicit parameter passing or
 *           deep coupling.
 *
 *  @note This class only stores a non-owning pointer — lifetime must be managed externally.
 *  @warning Since the pointer is stored statically, make sure to clear or reset it when the referenced object is destroyed,
 *           to avoid dangling pointers. */

class NotificationSettingsMenuContext
{
public:
    /*! @brief Sets the shared UI builder to be used by settings menu entries.
     *  @param uiBuilder Pointer to the UI builder implementation (non-owning). */
    static void setUiBuilder(AbstractMessageUiBuilder* uiBuilder) {
        Q_ASSERT_X(uiBuilder,"NotificationSettingsMenuContext::setUiBuilder",
                   "Specifice AbstractMessageUiBuilder* pointer is nullptr.");
        p_uiBuilder = uiBuilder;
    }

protected:
    friend class GlobalNotificationSettingsMenuEntry;
    template<class MessageTrait>
    friend class NotificationsSettingsMenuEntryTemplate;

    /*! @brief Retrieves the UI builder pointer currently stored in the context. This is intended for internal use by friend classes.
     *  @return Pointer to the current @ref AbstractMessageUiBuilder. */
    static AbstractMessageUiBuilder* uiBuilder() {
        Q_ASSERT_X(p_uiBuilder,"NotificationSettingsMenuContext::uiBuilder",
                   "p_uiBuilder must be set before by using NotificationSettingsMenuContext::setUiBuilder method.");
        return p_uiBuilder;
    }

private:
    inline static AbstractMessageUiBuilder* p_uiBuilder = nullptr;
};


} // namespace Draupnir::Messages

#endif // NOTIFICATIONSETTINGSMENUENTRIESCONTEXT_H
