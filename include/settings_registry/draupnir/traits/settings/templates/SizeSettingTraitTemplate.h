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

#ifndef SIZESETTINGTRAITTEMPLATE_H
#define SIZESETTINGTRAITTEMPLATE_H

#include <QSize>
#include <QString>

namespace Draupnir::Settings
{

/*! @struct SizeSettingTraitTemplate draupnir/traits/settings/StandartSettingTraitTemplates.h
 *  @ingroup SettingsRegistry
 *  @brief Generic template for defining a setting trait for storing a QSize object
 *  @tparam settingsKey     Pointer to a global constexpr string key.
 *  @tparam defaultWidth    Default width.
 *  @tparam defaultHeight   Default height.
 *
 *  @details This struct provides a concise way to declare new QSize setting traits. A setting trait defined via this template
 *           supplies:
 *           - using QSize — the underlying C++ value type;
 *           - static QString key() — the storage key as a QString, built from @p settingsKey;
 *           - static QSize defaultValue() — the default value constructed from provided width and height
 *
 * @todo Add usage example. */

template<const char* settingsKey, int defaultWidth, int defaultHeight = defaultWidth>
struct SizeSettingTraitTemplate
{
    /*! @brief Underlying value type. */
    using Value = QSize;

    /*! @brief Return the persistent key as a QString. */
    static QString key() { return QString{settingsKey}; }

    /*! @brief Return the default value. */
    static QSize defaultValue() { return QSize{defaultWidth,defaultHeight}; }
};

}; // namespace Draupnir::Settings

#endif // SIZESETTINGTRAITTEMPLATE_H
