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

#ifndef RECENTFILESLISTSETTING_H
#define RECENTFILESLISTSETTING_H

#include <QStringList>

namespace Draupnir::Settings
{

struct RecentFileList
{
    /*! @brief Underlying value type. */
    using Value = QStringList;

    /*! @brief Return the persistent storage key ("recentFiles").
     * @todo Add normal prefix for this setting. */
    static QString key() { return "files/recentFiles"; }

    /*! @brief Return the default value - empty QStringList. */
    static QStringList defaultValue() { return QStringList{}; }
};

}

#endif // RECENTFILESLISTSETTING_H
