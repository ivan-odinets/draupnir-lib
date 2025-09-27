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

#ifndef LOGWIDGETSETTINGSTRAITS_H
#define LOGWIDGETSETTINGSTRAITS_H

#include "draupnir/traits/settings/StandartSettingTraitTemplates.h"

#include "draupnir/core/MessageType.h"

/*! @file LogWidgetSettingsTraits.h
 *  @brief This file contains setting traits for LoGWidget to be passed to the SettingsRegistryTemplate  */

namespace Draupnir::Messages
{

inline constexpr const char logWidgetIconSize_settingsKey[] = "ui/messagesIconSize";

using MessageIconSizeSetting = Draupnir::Settings::SizeSettingTraitTemplate<
    logWidgetIconSize_settingsKey,
    64
>;

inline constexpr const char logWidgetMessagesShown_settingsKey[] = "ui/messagesShown";

using MessagesShown = Draupnir::Settings::SettingTraitTemplate<
    uint64_t,
    logWidgetMessagesShown_settingsKey,
    MessageType::AllMessages
>;

}; // namespace Draupnir::Messages

#endif // LOGWIDGETSETTINGSTRAITS_H
