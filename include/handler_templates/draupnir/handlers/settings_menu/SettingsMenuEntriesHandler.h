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

#ifndef SETTINGSMENUENTRIESHANDLER_H
#define SETTINGSMENUENTRIESHANDLER_H

#include "draupnir/handlers/templates/GenericMenuEntriesHandler.h"
#include "draupnir/utils/SettingTraitForEntry.h"

namespace Draupnir::Handlers
{

/*! @class SettingsMenuEntriesHandler
 *  @ingroup HandlerTemplates
 *  @headerfile draupnir/handlers/settings_menu/SettingsMenuEntriesHandler.h
 *  @brief This class is for handling Settings menu entries.
 *
 * @todo Write detailed documentation for this class. */

template<class Registry, class... Entries>
class SettingsMenuEntriesHandler :
        public GenericMenuEntriesHandler<SettingsMenuEntriesHandler<Registry,Entries...>,Entries...>
{
public:
    /*! @brief Attach a settings registry instance.
     *  @param registry Non-null pointer to a live SettingsRegistry.
     * @note Must be called before get()/set(). */
    void setRegistry(Registry* registry) {
        Q_ASSERT_X(registry, "SettingsEntriesHandlerContext<SettingsRegistry>::setRegistry",
                   "Provided Registry* pointer is nullptr.");
        p_registry = registry;

        GenericMenuEntriesHandler<SettingsMenuEntriesHandler<Registry,Entries...>,Entries...>::template loadSettings<Registry>(registry);
    }

    /*! @brief Read a setting value mapped from a menu entry trait.
     *  @details Resolves the corresponding SettingTrait as Settings::SettingTraitForEntry<SettingMenuEntry>::type and forwards
     *           to SettingsRegistry::getSetting<SettingTrait>().
     *  @tparam SettingMenuEntry Menu entry trait type.
     *  @return The current value of the mapped setting (by value). */
    template<class SettingMenuEntry>
    const typename Draupnir::Handlers::SettingTraitForEntry<SettingMenuEntry>::type::Value& get() const {
        static_assert(Registry::template contains<typename Draupnir::Handlers::SettingTraitForEntry<SettingMenuEntry>::type>(),
                "SettingsRegistry specified does not have trait for the provided SettingMenuEntry.");
        return p_registry->template get<typename Draupnir::Handlers::SettingTraitForEntry<SettingMenuEntry>::type>();
    }

    /*! @brief Write a setting value mapped from a menu entry trait and persist it.
     *  @details Resolves the corresponding SettingTrait as Settings::SettingTraitForEntry<SettingMenuEntry>::type and forwards
     *           to SettingsRegistry::setSetting<SettingTrait>(value).
     *  @tparam SettingMenuEntry Menu entry trait type.
     *  @param value New value to store. */
    template<class SettingMenuEntry>
    void set(const typename Draupnir::Handlers::SettingTraitForEntry<SettingMenuEntry>::type::Value& value) {
        static_assert(Registry::template contains<typename Draupnir::Handlers::SettingTraitForEntry<SettingMenuEntry>::type>(),
                "SettingsRegistry specified does not have trait for the provided SettingMenuEntry.");
        p_registry->template set<typename Draupnir::Handlers::SettingTraitForEntry<SettingMenuEntry>::type>(value);
    }

private:
    Registry* p_registry;
};

};

#endif // SETTINGSMENUENTRIESHANDLER_H
