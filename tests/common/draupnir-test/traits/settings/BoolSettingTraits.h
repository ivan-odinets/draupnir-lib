/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef BOOLSETTINGTRAITS_H
#define BOOLSETTINGTRAITS_H

#include <QString>

/*! @class BoolSettingTrait draupnir-test/traits/settings/BoolSettingTrait.h
 *  @brief This is a test setting trait for storing a bool variable. */

class BoolSettingTrait
{
public:
    using Value = bool;
    static QString key() { return "RandomKey"; }
    static constexpr bool defaultValue() { return true; }
};

/*! @class OtherBoolSettingTrait draupnir-test/traits/settings/OtherBoolSettingTrait.h
 *  @brief This is a test setting trait for storing a bool variable. */

class OtherBoolSettingTrait
{
public:
    using Value = bool;
    static QString key() { return "OtherRandomKey"; }
    static constexpr bool defaultValue() { return false; }
};

#endif // BOOLSETTINGTRAITS_H
