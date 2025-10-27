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

#include <QString>

namespace Draupnir::Settings::MainWindow
{

/*! @struct MinimizeOnCloseSetting draupnir/traits/settings/main_window/MinimizeOnCloseSetting.h
 *  @ingroup SettingsRegistry
 *  @brief Concrete setting trait describing the "Minimize on Close" option for the application main window.
 *
 *  @details This struct provides MinimizeOnCloseSetting trait with:
 *           - using Value = bool;
 *           - static QString key();
 *           - static bool defaultValue();
 *
 * @todo Allow changing of the defaultValue behaviour using preprocessor.
 * @todo Test changing of the default value by using macro defines.
 * @todo Update documentation to include this feature. */

struct MinimizeOnCloseSetting
{
    /*! @brief Underlying value type. */
    using Value = bool;

    /*! @brief Return the persistent storage key ("main_window/minimize_on_close"). */
    static QString key() { return "main_window/minimize_on_close"; }

    /*! @brief Return the default value - home directory of the user. */
    static bool defaultValue() { return false; }
};

}; // namespace Draupnir::Settings::MainWindow

#endif // MINIMIZEONCLOSESETTING_H
