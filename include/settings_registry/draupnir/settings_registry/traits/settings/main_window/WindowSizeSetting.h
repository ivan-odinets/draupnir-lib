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

#ifndef WINDOWSIZESETTING_H
#define WINDOWSIZESETTING_H

#include <QSize>
#include <QString>

namespace Draupnir::Settings::MainWindow
{

/*! @struct WindowSizeSetting draupnir/settings_registry/traits/settings/main_window/WindowSizeSetting.h
 *  @ingroup SettingsRegistry
 *  @brief Setting trait describing the main application window size.
 *
 *  @details This trait defines a setting that stores and retrieves the window size (width and height) as a QSize. It can
 *           be used directly in a SettingsRegistryTemplate.
 *
 *           Provides:
 *           - using Value = QSize (the stored C++ type);
 *           - static QString key() — returns the persistent key string ("main_window/window_size");
 *           - static Value defaultValue() — returns the default QSize (empty size).
 *
 * @todo Allow changing of the defaultValue behaviour using preprocessor.
 * @todo Test changing of the default value by using macro defines.
 * @todo Update documentation to include this feature. */

struct WindowSizeSetting
{
    /*! @brief Underlying value type. */
    using Value = QSize;

    /*! @brief Return the persistent storage key ("main_window/window_size"). */
    static QString key() { return "main_window/window_size"; }

    /*! @brief Return the default value (empty QSize). */
    static QSize defaultValue() { return QSize{}; }
};

};

#endif // WINDOWSIZESETTING_H
