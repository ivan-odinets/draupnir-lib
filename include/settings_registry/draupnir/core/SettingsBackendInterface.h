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

#ifndef SETTINGSBACKENDINTERFACE_H
#define SETTINGSBACKENDINTERFACE_H

#include <QVariant>
#include <QString>

namespace Draupnir::Settings
{

/*! @class SettingsBackendInterface draupnir/core/SettingsBackendInterface.h
 *  @brief This is a base interface class for a custom backends which can be used by the SettingsRegistryTemplate */

class SettingsBackendInterface
{
public:
    /*! @brief Virtual trivial destructor. */
    virtual ~SettingsBackendInterface() = default;

///@name Group of virtual methods to be implemented while subclassing the SettingsBackendInterface
///@{
    /*! @brief Should check if a value is existing for a provided key. */
    virtual bool contains(const QString& key) const = 0;

    /*! @brief Should return value from the settings storage associated with provided key. */
    virtual QVariant value(const QString& key, const QVariant& defaultValue = QVariant{}) = 0;

    /*! @brief Should set value within the settings storage associated with provided key. */
    virtual void setValue(const QString& key, const QVariant& value) = 0;
///@}
};

}; // namespace Draupnir::Settings

#endif // SETTINGSBACKENDINTERFACE_H
