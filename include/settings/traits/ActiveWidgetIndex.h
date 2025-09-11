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

#ifndef ACTIVEWIDGETINDEX_H
#define ACTIVEWIDGETINDEX_H

#include <QString>

namespace Draupnir::Settings
{

/*! @struct ActiveWidgetIndex
 *  @brief Setting trait describing the index of currently active widget (e.g. within QTabWidget)
 *  @details This trait defines a setting that stores and retrieves the active widget index as int. It does not correspond to
 *           a specific menu entry (hence Entry is set to void), but can be used directly in a SettingsRegistry or similar
 *           infrastructure.
 *
 *           Provides:
 *           - using Entry = void (no associated menu entry);
 *           - using Value = int (the stored C++ type);
 *           - static QString key() — returns the persistent key string ("activeWidgetIndex");
 *           - static Value defaultValue() — returns the default index = 0.
 *
 *           Example:
 *           @code
 *           SettingsRegistry<ActiveWidgetIndex> registry;
 *           registry.loadSettings(appSettings);
 *
 *           int index = registry.getSetting<ActiveWidgetIndex>();
 *           @endcode */

struct ActiveWidgetIndex {
    using Entry = void;      //!< No associated menu entry.
    using Value = int;       //!< Underlying value type.

    /*! @brief Return the persistent storage key ("activeWidgetIndex"). */
    static QString key() { return "activeWidgetIndex"; }

    /*! @brief Return the default value - 0. */
    static int defaultValue() { return 0; }
};

};

#endif // ACTIVEWIDGETINDEX_H
