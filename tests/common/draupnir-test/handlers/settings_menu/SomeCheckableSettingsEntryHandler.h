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

#ifndef SOMECHECKABLESETTINGSENTRYHANDLER_H
#define SOMECHECKABLESETTINGSENTRYHANDLER_H

#include "draupnir/ui_bricks/handlers/settings_menu/SettingsCheckableEntryHandler.h"

#include "draupnir-test/traits/entries/SomeCheckableMenuEntry.h"
#include "draupnir-test/traits/settings/SomeCustomBoolSetting.h"

namespace Draupnir::Handlers
{

template<class Context,class MenuEntry>
class GenericMenuEntryHandler;

/*! @class GenericMenuEntryHandler<Context,SomeCustomMenuEntryTrait>
 *  @headerfile draupnir-test/handlers/settings_menu/SomeCheckableSettingsEntryHandler.h
 *  @brief Implementation of GenericMenuEntryHandler for SomeCustomMenuEntryTrait */

template<class Context>
class GenericMenuEntryHandler<Context,SomeCustomCheckableMenuEntry> :
        public SettingsCheckableEntryHandler<Context, SomeCustomCheckableMenuEntry>
{
public:
    GenericMenuEntryHandler(Context& context) :
        SettingsCheckableEntryHandler<Context,SomeCustomCheckableMenuEntry>{context}
    {}
};

template<> struct SettingTraitForEntry<SomeCustomCheckableMenuEntry> {
    using type = SomeCustomBoolSetting;
};

}; // Draupnir::Handlers

#endif // SOMECHECKABLESETTINGSENTRYHANDLER_H
