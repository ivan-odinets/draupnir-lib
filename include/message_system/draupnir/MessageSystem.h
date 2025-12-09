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

#ifndef MESSAGESYSTEM_H
#define MESSAGESYSTEM_H

/*! @file draupnir/MessageSystem.h
 *  @ingroup MessageSystem
 *  @brief This file is a general include to use the @ref MessageSystem module. */

#include "Logger.h" // IWYU pragma: keep

// Core things
#include "draupnir/message_system/core/AbstractMessageUiBuilder.h"   // IWYU pragma: keep
#include "draupnir/message_system/core/MessageGroup.h"               // IWYU pragma: keep
#include "draupnir/message_system/core/MessageSystemTemplate.h"      // IWYU pragma: keep

// Required for custom message types
#include "draupnir/message_system/core/MessageType.h"      // IWYU pragma: keep
#include "draupnir/message_system/core/Notification.h"     // IWYU pragma: keep

// Traits to be used with MenuTemplate / MenuBarTemplate
#include "draupnir/message_system/traits/menu_entries/GlobalNotificationSettingsMenuEntry.h"       // IWYU pragma: keep
#include "draupnir/message_system/traits/menu_entries/NotificationsSettingsMenuEntryTemplate.h"    // IWYU pragma: keep

#endif // MESSAGESYSTEM_H
