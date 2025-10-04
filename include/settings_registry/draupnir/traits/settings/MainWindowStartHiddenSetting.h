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

#ifndef MAINWINDOWSTARTHIDDENSETTING_H
#define MAINWINDOWSTARTHIDDENSETTING_H

#include <QString>

namespace Draupnir::Settings
{

/*! @struct MainWindowStartHiddenSetting draupnir/traits/settings/MainWindowStartHiddenSetting.h
 *  @brief Concrete setting trait describing the "Start Hidden" option for the application main window.
 *  @details This struct provides StartHiddenSetting trait with:
 *           - using Value = bool;
 *           - static QString key();
 *           - static bool defaultValue();
 *
 *           It can be used in SettingsRegistryTemplate or any template expecting a SettingTrait.
 *
 * @see SettingsRegistry, SettingsBundle */

struct StartHiddenSetting
{
    /*! @brief Underlying value type. */
    using Value = bool;

    /*! @brief Return the persistent storage key ("main_window/start_hidden"). */
    static QString key() { return "main_window/start_hidden"; }

    /*! @brief Return the default value - false.
     * @todo Add #define to reconfigure this. */
    static bool defaultValue() { return false; }
};

};

#endif // MAINWINDOWSTARTHIDDENSETTING_H
