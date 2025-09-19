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

#ifndef ACTIVEWIDGETINDEXSETTING_H
#define ACTIVEWIDGETINDEXSETTING_H

#include <QString>

namespace Draupnir::Settings
{

/*! @struct ActiveWidgetIndexSetting
 *  @brief Setting trait describing the index of currently active widget (e.g. within QTabWidget)
 *  @details This trait defines a setting that stores and retrieves the active widget index as int.
 *
 *           Provides:
 *           - using Value = int (the stored C++ type);
 *           - static QString key() — returns the persistent key string ("activeWidgetIndex");
 *           - static int defaultValue() — returns the default index = 0.
 *
 *           Example:
 *           @code
 *           SettingsRegistry<ActiveWidgetIndex> registry;
 *           registry.loadSettings();
 *
 *           int index = registry.template get<ActiveWidgetIndex>();
 *           @endcode */

struct ActiveWidgetIndexSetting
{
    /*! @brief Underlying value type. */
    using Value = int;

    /*! @brief Return the persistent storage key ("activeWidgetIndex").
     * @todo Add normal prefix for this setting. */
    static QString key() { return "activeWidgetIndex"; }

    /*! @brief Return the default value - 0. */
    static int defaultValue() { return 0; }
};

};

#endif // ACTIVEWIDGETINDEXSETTING_H
