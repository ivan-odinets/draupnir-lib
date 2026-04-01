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

#ifndef INTEGERSETTINGTRAITS_H
#define INTEGERSETTINGTRAITS_H

#include <QString>

/*! @class RestrictedShortSettingTrait draupnir-test/traits/settings/IntegerSettingsTraits.h
 *  @brief Some custom short setting trait for testing. */

class RestrictedShortSettingTrait
{
public:
    using Value = short;
    static QString key() { return "randomRestrictedShort"; }
    static constexpr short defaultValue() { return 42; }

    static constexpr short minimalValue() { return 10; }
    static constexpr short maximalValue() { return 100; }
};

/*! @class ShortSettingTrait draupnir-test/traits/settings/IntegerSettingsTraits.h
 *  @brief Some custom short setting trait for testing. */

class ShortSettingTrait
{
public:
    using Value = short;
    static QString key() { return "randomShort"; }
    static constexpr short defaultValue() { return 42; }
};

/*! @class IntSettingTrait draupnir-test/traits/settings/IntegerSettingsTraits.h
 *  @brief Some custom integer setting trait for testing. */

class IntSettingTrait
{
public:
    using Value = int;
    static QString key() { return "randomInteger"; }
    static constexpr int defaultValue() { return 42; }
};

/*! @class RestrictedIntSettingTrait draupnir-test/traits/settings/IntegerSettingsTraits.h
 *  @brief Some custom integer setting trait for testing. */

class RestrictedIntSettingTrait
{
public:
    using Value = int;
    static QString key() { return "randomRestrictedInteger"; }
    static int defaultValue() { return 42; }

    static constexpr int minimalValue() { return 10; }
    static constexpr int maximalValue() { return 100; }
};

#endif // INTEGERSETTINGTRAITS_H
