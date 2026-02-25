/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MYSETTINGTRAITS_H
#define MYSETTINGTRAITS_H

#include "draupnir/ui_bricks/utils/MenuEntryToTraitMapper.h"

#include "traits/menu_entries/MySettingsMenuEntries.h"

struct DoubleSetting
{
    using Value = double;

    static QString key() { return "draupnir_example/doubleSetting"; }

    static double defaultValue() { return 3.14; }

    static QString settingDescription() { return QString{"This is a double setting:"}; }
};

struct IntSetting
{
    using Value = int;

    static QString key() { return "draupnir_example/intSetting"; }

    static int defaultValue() { return 42; }
};

struct StringSetting
{
    using Value = QString;

    static QString key() { return "draupnir_example/intSetting"; }

    static QString defaultValue() { return QString{"Testing String"}; }
};

DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(SomeDoubleSettinMenuEntry,DoubleSetting);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(SomeIntSettinMenuEntry,IntSetting);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(SomeStringSettinMenuEntry,StringSetting);

#endif // MYSETTINGTRAITS_H
