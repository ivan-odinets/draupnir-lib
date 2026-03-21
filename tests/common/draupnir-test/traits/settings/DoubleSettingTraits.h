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

#ifndef DOUBLESETTINGTRAITS_H
#define DOUBLESETTINGTRAITS_H

#include <cmath>

#include <QString>

/*! @class FloatSettingTrait draupnir-test/traits/settings/FloatSettingTrait.h
 *  @brief Some custom float setting trait for testing. */

class FloatSettingTrait
{
public:
    using Value = float;
    static QString key() { return "randomFloat"; }
    static float defaultValue() { return M_E; }
};

/*! @class DoubleSettingTrait draupnir-test/traits/settings/DoubleSettingTrait.h
 *  @brief Some custom double setting trait for testing. */

class DoubleSettingTrait
{
public:
    using Value = double;
    static QString key() { return "randomDouble"; }
    static double defaultValue() { return M_PI; }
};

#endif // DOUBLESETTINGTRAITS_H
