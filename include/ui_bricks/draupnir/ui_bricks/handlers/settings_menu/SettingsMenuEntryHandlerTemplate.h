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

#ifndef SETTINGSMENUENTRYHANDLERTEMPLATE_H
#define SETTINGSMENUENTRYHANDLERTEMPLATE_H

#include "draupnir/ui_bricks/concepts/MenuEntryConcept.h"
#include "draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h"
#include "draupnir/ui_bricks/utils/MenuEntryToTraitMapper.h"
#include "draupnir/ui_bricks/utils/SettingsValueUserInput.h"

namespace Draupnir::Handlers
{

template<class SettingsContext, class SettingsMenuEntry>
class SettingsMenuEntryHandlerTemplate {
    static_assert(!std::is_same_v<SettingsMenuEntry,SettingsMenuEntry>);
};

template<class SettingsContext, class SettingsMenuEntry>
    requires Draupnir::Ui::MenuEntry::IsActionEntry<SettingsMenuEntry>
class SettingsMenuEntryHandlerTemplate<SettingsContext,SettingsMenuEntry> :
    public ActionHandlerTemplate<SettingsMenuEntryHandlerTemplate<SettingsContext,SettingsMenuEntry>>
{
    using SettingTrait = typename MapMenuEntry<SettingsMenuEntry>::ToTrait;
    using SettingsValue = typename SettingTrait::Value;

    static inline constexpr bool boolSettingHandled = std::is_same_v<SettingsValue,bool>;

public:
    SettingsMenuEntryHandlerTemplate(SettingsContext& context) :
        m_context{context}
    {}

    void onTriggered(bool arg) requires(boolSettingHandled) {
        m_context.template set<SettingTrait>(arg);
    }

    void onTriggered() requires(!boolSettingHandled) {
        if constexpr (!std::is_same_v<SettingsValue,bool>) {
            const SettingsValue oldValue = m_context.template get<SettingTrait>();
            std::optional<SettingsValue> result = SettingsValueUserInput<SettingTrait,SettingsValue>(oldValue);
            if (result.has_value()) {
                m_context.template set<SettingTrait>(result);
            }
        }
    }

    void onSettingsLoaded() requires(boolSettingHandled) {
        ActionHandlerTemplate<SettingsMenuEntryHandlerTemplate<SettingsContext,SettingsMenuEntry>>::setActionChecked(m_context.template get<SettingTrait>());
    }

private:
    SettingsContext& m_context;
};

}; // namespace Draupnir::Handlers

#endif // SETTINGSMENUENTRYHANDLERTEMPLATE_H
