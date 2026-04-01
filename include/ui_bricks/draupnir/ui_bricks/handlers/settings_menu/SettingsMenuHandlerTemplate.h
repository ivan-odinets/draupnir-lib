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

#ifndef SETTINGSMENUHANDLERTEMPLATE_H
#define SETTINGSMENUHANDLERTEMPLATE_H

#include "draupnir/ui_bricks/handlers/templates/MenuEntryHandlerContainer.h"

#include "draupnir/ui_bricks/concepts/MenuEntryConcept.h"
#include "draupnir/ui_bricks/handlers/settings_menu/SettingsMenuEntryHandlerTemplate.h"

namespace Draupnir::Handlers
{

/*! @brief This is an alias.
 * @todo Introduce concepts here. */

template<class SettingsRegistry, Ui::MenuEntryConcept... Entries>
class SettingsMenuHandler
{
    using _HandlerContainer = MenuEntryHandlerContainer<
        SettingsMenuHandler<SettingsRegistry,Entries...>,
        SettingsMenuEntryHandlerTemplate,
        Entries...
    >;

public:
    SettingsMenuHandler() :
        p_registry{nullptr},
        m_handlers{}
    {
        m_handlers.initialize(this);
    }
    ~SettingsMenuHandler() = default;

    void loadSettings(SettingsRegistry* registry) {
        Q_ASSERT(p_registry == nullptr);
        p_registry = registry;

        m_handlers.onSettingsLoaded();
    }

    template<class MenuContainer>
    void connectActions(MenuContainer* container) {
        m_handlers.template connectActions<MenuContainer>(container);
    }

    /*! @brief Returns a const reference to the value of the specified setting trait. A registry must be attached via `loadSettings()`. In
     *         debug builds this is enforced by `Q_ASSERT`. A compile-time check ensures the trait is supported by the `SettingsRegistry`
     *         type.
     *  @tparam SettingTrait Setting trait type.
     *  @return Const reference to the stored value. */
    template<class SettingTrait>
    const typename SettingTrait::Value& get() const {
        static_assert(SettingsRegistry::template contains<SettingTrait>());
        Q_ASSERT(p_registry);
        return p_registry->template get<SettingTrait>();
    }

    template<class SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        static_assert(SettingsRegistry::template contains<SettingTrait>());
        Q_ASSERT(p_registry);
        p_registry->template set<SettingTrait>(value);
    }

private:
    SettingsRegistry* p_registry;
    _HandlerContainer m_handlers;
};

};

#endif // SETTINGSMENUHANDLERTEMPLATE_H
