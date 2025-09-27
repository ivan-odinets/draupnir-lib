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

#ifndef SOMECUSTOMBOOLSETTING_H
#define SOMECUSTOMBOOLSETTING_H

#include <QString>

/*! @class SomeCustomBoolSetting draupnir-test/traits/settings/SomeCustomBoolSetting.h
 *  @brief This is a test setting trait for storing a bool variable. */

class SomeCustomBoolSetting
{
public:
    using Value = bool;
    static QString key() { return "RandomKey"; }
    static bool defaultValue() { return true; }
};

#endif // SOMECUSTOMBOOLSETTING_H
