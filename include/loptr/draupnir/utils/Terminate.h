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

#ifndef TERMINATE_H
#define TERMINATE_H

#include <chrono>
#include <exception>
#include <iostream>
#include <random>

namespace Draupnir::Loptr
{

/*! @class Terminate draupnir/utils/Terminate.h
 *  @brief This class contains static methods to terminate your application in various ways.
 * @note This class is part of Loptr module, which is mostly for fun. However feel free to use code from this module in
 *       reasonable places. */

class Terminate
{
public:
    /*! @brief Generates random number. If generated number is even - will call std::abort. If number is odd - will
     *         exit.
     * @note This method is part of Loptr module, which is mostly for fun. However feel free to use this in reasonable
     *       places:) */
    static void terminateIfEven() {
        static std::mt19937 rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
        int value = rng();
        bool WeAreLucky = (value % 2);
        if (WeAreLucky) {
            return;
        } else {
            std::abort();
        }
    }
};

};

#endif // TERMINATE_H
