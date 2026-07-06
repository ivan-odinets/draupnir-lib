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

#ifndef SETTINGSSERIALIZATIONCONCEPTS_H
#define SETTINGSSERIALIZATIONCONCEPTS_H

#include <QLatin1String>

#include "draupnir/utils/concepts/flags_concepts.h"
#include "draupnir/utils/concepts/type_concepts.h"

namespace Draupnir::Settings
{

template<class Candidate>
concept SerializableValueWrapperConcept = requires {
    requires(
        draupnir::utils::enum_or_integer_like_concept<decltype(Candidate::value())> ||
        draupnir::utils::flags_like_concept<decltype(Candidate::value())>
    );
    { Candidate::configString() } -> std::same_as<QLatin1String>;
};

/*! @brief Checks whether an enum-flags-like type provides custom config serialization to string.
 *  @tparam Candidate Type to test.
 *
 *  @details This concept is satisfied when `Candidate` provides a static member functions with the following signatures:
 *           @code
 *           static QString toConfigString(const Candidate&);
 *           static std::optional<Candidate> fromConfigString(const QString&);
 *           @endcode
 * @todo Important: Allow validation of any type serialization through this concept. */

template<class Candidate>
concept HasCustomEnumFlagsConfigSerialization =
    requires(const Candidate& enumFlags, QString& configString) {
        { Candidate::toConfigString(enumFlags) } -> std::same_as<QString>;
        { Candidate::fromConfigString(configString) } -> std::same_as<std::optional<Candidate>>;
    };

template<class Serializer, class Value>
concept ValueSerializableBy =
    requires(const QString& string, const Value& value) {
        { Serializer::toConfigString(value) } -> std::same_as<QString>;
        { Serializer::fromConfigString(string) } -> std::same_as<std::optional<Value>>;
    };

}; // namespace Draupnir::Settings

#endif // SETTINGSSERIALIZATIONCONCEPTS_H
