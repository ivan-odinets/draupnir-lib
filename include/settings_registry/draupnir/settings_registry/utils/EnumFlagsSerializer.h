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

#ifndef ENUMFLAGSSERIALIZER_H
#define ENUMFLAGSSERIALIZER_H

#include <optional>

#include <QString>

#include "draupnir/utils/flags.h"

namespace Draupnir::Settings
{

/*! @class EnumFlagsSerializer draupnir/settings_registry/utils/EnumFlagsSerializer.h
 *  @ingroup SettingsRegistry
 *  @brief This is a class.
 * @todo Write Reasonable documentation. */

template<draupnir::utils::enum_flags_concept EnumFlags>
class EnumFlagsSerializer
{
    template<typename Integer,class Unused = void>
    struct _Helper {};

    template<class Unused>
    struct _Helper<short, Unused> {
        static short fromString(const QString& string, bool* ok, int base)
            { return string.toShort(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned short, Unused> {
        static unsigned short fromString(const QString& string, bool* ok, int base)
        { return string.toUShort(ok,base); }
    };

    template<class Unused>
    struct _Helper<int, Unused> {
        static int fromString(const QString& string, bool* ok, int base)
        { return string.toInt(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned int, Unused> {
        static unsigned int fromString(const QString& string, bool* ok, int base)
        { return string.toUInt(ok,base); }
    };

    template<class Unused>
    struct _Helper<long, Unused> {
        static long fromString(const QString& string, bool* ok, int base)
        { return string.toLong(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned long, Unused> {
        static short fromString(const QString& string, bool* ok, int base)
        { return string.toULong(ok,base); }
    };

    template<class Unused>
    struct _Helper<long long, Unused> {
        static long long fromString(const QString& string, bool* ok, int base)
        { return string.toLongLong(ok,base); }
    };

    template<class Unused>
    struct _Helper<unsigned long long, Unused> {
        static long long fromString(const QString& string, bool* ok, int base)
        { return string.toULongLong(ok,base); }
    };

public:
    using Enum = typename EnumFlags::enum_type;

    static std::optional<EnumFlags> fromConfigString(const QString& string) {
        bool ok = false;
        const auto value = _Helper<std::underlying_type_t<Enum>>::fromString(string,&ok,2);
        return (ok) ?
            std::optional<EnumFlags>{EnumFlags{value}} :
            std::nullopt;
    }

    static QString toConfigString(const EnumFlags& value) {
        return QString::number(value.value(),2);
    }
};

}; // namespace Draupnir::Settings

#endif // ENUMFLAGSSERIALIZER_H
