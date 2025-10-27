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

#ifndef LASTUSEDDIRECTORYSETTING_H
#define LASTUSEDDIRECTORYSETTING_H

#include <QDir>
#include <QString>

namespace Draupnir::Settings
{

/*! @struct LastUsedDirectorySetting draupnir/traits/settings/files/LastUsedDirectorySetting.h
 *  @ingroup SettingsRegistry
 *  @brief Setting trait for tracking the most recently used directory.
 *
 *  @details This trait defines how the last used directory setting is stored and retrieved. It provides:
 *           - The value type (`QString`);
 *           - A storage key ("files/last_used_directory");
 *           - A default value.
 *
 * @todo Allow changing of the defaultValue behaviour using preprocessor.
 * @todo Test changing of the default value by using macro defines.
 * @todo Update documentation to include this feature. */

struct LastUsedDirectorySetting
{
    /*! @brief Underlying value type. */
    using Value = QString;

    /*! @brief Return the persistent storage key ("files/last_used_directory"). */
    static QString key() { return "files/last_used_directory"; }

    /*! @brief Return the default value - home directory of the user. */
    static QString defaultValue() { return QDir::homePath(); }
};

}

#endif // LASTUSEDDIRECTORYSETTING_H
