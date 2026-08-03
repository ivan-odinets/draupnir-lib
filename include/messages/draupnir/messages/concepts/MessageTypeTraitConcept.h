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

#ifndef MESSAGETYPETRAITCONCEPT_H
#define MESSAGETYPETRAITCONCEPT_H

#include "draupnir/messages/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @concept MessageTypeTraitConcept draupnir/messages/concepts/MessageTypeTraitConcept.h
 *  @ingroup Messages
 *  @brief Defines the requirements for message type trait classes.
 *  @tparam Candidate Type whose message type trait interface is validated.
 *
 *  @details A message type trait associates a compile-time trait class with a concrete @ref Draupnir::Messages::MessageType
 *           and its configuration token.
 *
 *           A conforming type must provide the following static functions:
 *           - `type()`, returning a @ref Draupnir::Messages::MessageType;
 *           - `configString()`, returning the corresponding `QLatin1String`
 *             configuration token.
 * @todo Cleanup: Extract concept and standartise contract for the configString() / value() methods. */

template<class Candidate>
concept MessageTypeTraitConcept = requires {
    { Candidate::type() } -> std::same_as<Draupnir::Messages::MessageType>;
    { Candidate::configString() } -> std::same_as<QLatin1String>;
};

} // namespace Draupnir::Messages

#endif // MESSAGETYPETRAITCONCEPT_H
