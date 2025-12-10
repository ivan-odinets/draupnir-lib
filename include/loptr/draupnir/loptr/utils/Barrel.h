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

#ifndef BARREL_H
#define BARREL_H

namespace Draupnir::Loptr
{

/*! @class Barrel draupnir/loptr/utils/Barrel.h
 *  @brief This class contains static methods to do a barrel roll.
 *
 * @note This class is part of Loptr module, which is mostly for fun. However feel free to use code from this module in
 *       reasonable places. */

class Barrel
{
public:
    /*! @brief Static constexpr member containing default duration of a barrel roll. */
    static constexpr int defaultDuration = 1000;

    /*! @brief Performs a barrel roll.
     *  @details Triggers a one-shot “barrel roll” visual effect. The exact behaviour depends on the current platform and
     *           integration, but the general idea is to temporarily rotate or animate the application UI in a and then
     *           restore it back to its normal state.
     *
     *           This function is designed as a fire-and-forget helper: it does not return any value and does not require
     *           the caller to manage the animation’s lifetime explicitly. */
    static void doABarrelRoll(int duration = defaultDuration);
};

}; // namespace Draupnir::Loptr

#endif // BARREL_H
