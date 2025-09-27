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

#ifndef MINIMIZEONCLOSESETTING_H
#define MINIMIZEONCLOSESETTING_H

#include "StandartSettingTraitTemplates.h"

namespace Draupnir::Settings
{

/*! @struct MinimizeOnCloseSetting
 *  @brief Concrete setting trait describing the "Minimize on Close" option.
 *  @details This struct provides MinimizeOnCloseSetting trait with:
 *           - using Value = bool;
 *           - static QString key();
 *           - static bool defaultValue();
 *
 *           It can be used in SettingsRegistryTemplate or any template expecting a SettingTrait.
 *
 * @see SettingsRegistryTemplate, SettingsBundleTemplate */

struct MinimizeOnCloseSetting
{
    /*! @brief Underlying value type. */
    using Value = bool;

    /*! @brief Return the persistent storage key ("minimizeOnClose").
     * @todo Add normal prefix for this setting. */
    static QString key() { return "minimizeOnClose"; }

    /*! @brief Return the default value - home directory of the user.
     * @todo Add #define to reconfigure this. */
    static bool defaultValue() { return false; }
};

};

#endif // MINIMIZEONCLOSESETTING_H
