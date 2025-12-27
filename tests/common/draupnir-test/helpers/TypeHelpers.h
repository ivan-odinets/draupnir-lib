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

#ifndef TYPE_HELPERS_H
#define TYPE_HELPERS_H

#include <QtTest>

#include <type_traits>

#include "draupnir/utils/type_name_utils.h"

#define TYPE_COMPARE(actual, expected) \
    do {\
        if (!QTest::qCompare(std::is_same_v<actual,expected>,true, \
            draupnir::utils::type_pretty_name<actual>().c_str(), \
            draupnir::utils::type_pretty_name<expected>().c_str(), \
            __FILE__,__LINE__)) \
        return;\
    } while (false);

#endif // TYPE_HELPERS_H
