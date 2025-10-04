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

#ifndef SOMERANDOMWIDGETINDEXSETTING_H
#define SOMERANDOMWIDGETINDEXSETTING_H

#include <QString>

/*! @class SomeRandomWidgetIndexSetting draupnir-test/traits/settings/SomeRandomWidgetIndexSetting.h
 *  @brief This is a test setting trait for storing a int variable which is a active widget index. */

class SomeRandomWidgetIndexSetting
{
public:
    using Value = int;
    static QString key() { return "widgetIndex"; }
    static int defaultValue() { return 0; }
};

#endif // SOMERANDOMWIDGETINDEXSETTING_H
