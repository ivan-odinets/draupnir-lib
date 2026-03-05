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

#ifndef SETTINGTRAITPRINTER_H
#define SETTINGTRAITPRINTER_H

#include <QTextStream>

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/settings_registry/utils/EnumFlagsSerializer.h"

namespace Draupnir::Settings
{

template<class Stream, class Candidate>
concept StreamInsertable =
    requires(Stream s, const Candidate& v) { s << v; };

/*! @class SettingTraitPrinter draupnir/settings_registry/utils/SettingTraitPrinter.h
 *  @brief This is a class.
 * @todo Refractor, make this stable and provide support for other streams (at least ostream). */

template<SettingTraitConcept Trait>
class SettingTraitPrinter
{
public:
    template<class Output>
    inline static void print(Output&& output, const Trait::Value&) {
        output << "[ComplexSettingTrait]"
               << Qt::endl;
    };
};


template<PrimitiveSettingTraitConcept Trait>
class SettingTraitPrinter<Trait>
{
public:
    template<class Output>
    inline static void print(Output&& output, const Trait::Value& value) {
        output << Trait::key()
               << " = "
               << value
               << Qt::endl;
    };
};

template<PrimitiveSettingTraitConcept Trait>
    requires(draupnir::utils::enum_flags_concept<typename Trait::Value>)
class SettingTraitPrinter<Trait>
{
public:
    template<class Output>
    inline static void print(Output&& output, const Trait::Value& value) {
        output << Trait::key()
               << " = "
               << EnumFlagsSerializer<typename Trait::Value>::toConfigString(value)
               << Qt::endl;
    };
};

}; // namespace Draupnir::Settings

#endif // SETTINGTRAITPRINTER_H
