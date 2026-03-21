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

#ifndef STRINGSETTINGTRAITS_H
#define STRINGSETTINGTRAITS_H

#include <QStringList>

/*! @class QStringSettingTrait draupnir-test/traits/settings/StringSettingsTraits.h
 *  @brief Some custom QString setting trait for testing. */

class QStringSettingTrait
{
public:
    using Value = QString;
    static QString key() { return "randomQString"; }
    static QString defaultValue() { return "QString"; }
};

/*! @class QStringSettingTrait draupnir-test/traits/settings/StringSettingsTraits.h
 *  @brief Some custom QString setting trait for testing. */

class QStringListSettingTrait
{
public:
    using Value = QStringList;
    static QString key() { return "randomQStringList"; }
    static QStringList defaultValue() { return QStringList{"QStringList","Random"}; }
};

/*! @class StdSettingTrait draupnir-test/traits/settings/StringSettingsTraits.h
 *  @brief Some custom std::string setting trait for testing. */

class StdStringSettingTrait
{
public:
    using Value = std::string;
    static QString key() { return "randomStdString"; }
    static std::string defaultValue() { return "StdString"; }
};

#endif // STRINGSETTINGTRAITS_H
