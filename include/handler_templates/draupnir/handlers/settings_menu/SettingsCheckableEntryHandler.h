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

#ifndef SETTINGSCHECKABLEENTRYHANDLER_H
#define SETTINGSCHECKABLEENTRYHANDLER_H

#include "draupnir/handlers/templates/CheckableActionHandler.h"

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/utils/SettingTraitForEntry.h"

namespace Draupnir::Handlers
{

/*! @class SettingsCheckableEntryHandler
 *  @ingroup HandlerTemplates
 *  @brief This is a base class for checkable settings entries.
 *
 * @todo Write reasonable documentation for this class.
 * @todo Write a test for this class. */

template<class SettingsContext,class MenuEntry>
class SettingsCheckableEntryHandler :
    public CheckableActionHandler<SettingsCheckableEntryHandler<SettingsContext,MenuEntry>>
{
public:
    using SettingsBundle = Settings::SettingsBundleTemplate<typename SettingTraitForEntry<MenuEntry>::type>;

    SettingsCheckableEntryHandler(SettingsContext& context) :
        m_context{context}
    {}

    void onSettingsLoaded() {
        const bool state = m_context.template get<MenuEntry>();
        CheckableActionHandler<SettingsCheckableEntryHandler<SettingsContext,MenuEntry>>::action()->setChecked(state);
    }

    void onTriggered(bool state) {
        m_context.template set<MenuEntry>(state);
    }

private:
    SettingsContext& m_context;
};

};

#endif // SETTINGSCHECKABLEENTRYHANDLER_H
