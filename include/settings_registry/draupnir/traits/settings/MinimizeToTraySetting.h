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

#ifndef MINIMIZETOTRAYSETTING_H
#define MINIMIZETOTRAYSETTING_H

#include "StandartSettingTraitTemplates.h"

namespace Draupnir::Settings
{

/*! @struct MinimizeToTraySetting draupnir/traits/settings/MinimizeToTraySetting.h
 *  @brief Concrete setting trait describing the "Minimize to Tray" option.
 *  @details This struct provides MinimizeToTraySetting trait with:
 *           - using Value = bool;
 *           - static QString key();
 *           - static bool defaultValue();
 *
 *           It can be used in SettingsRegistryTemplate or any template expecting a SettingTrait.
 *
 * @see SettingsRegistryTemplate, SettingsBundleTemplate */

struct MinimizeToTraySetting
{
    /*! @brief Underlying value type. */
    using Value = bool;

    /*! @brief Return the persistent storage key ("minimizeToTray").
     * @todo Add normal prefix for this setting. */
    static QString key() { return "minimizeToTray"; }

    /*! @brief Return the default value - home directory of the user.
     * @todo Add #define to reconfigure this. */
    static bool defaultValue() { return false; }
};

};

#endif // MINIMIZETOTRAYSETTING_H
