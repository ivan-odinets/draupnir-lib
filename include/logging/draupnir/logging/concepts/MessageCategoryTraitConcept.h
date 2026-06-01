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

#ifndef MESSAGECATEGORYTRAITCONCEPT_H
#define MESSAGECATEGORYTRAITCONCEPT_H

#include "draupnir/logging/messages/categories/MessageCategories.h"

namespace Draupnir::Logging
{

/*! @ingroup Logging
 *  @brief Concept for compile-time message category traits.
 *  @tparam Candidate Type to check.
 *
 *  @details A message category trait describes a single logging/message category known at compile time.
 *
 *           A valid trait must provide:
 *           - `static MessageCategory id()` — stable category identifier.
 *           - `static QString displayName()` — human-readable name for UI.
 *           - `static QString configKey()` — stable string key used in config files.
 *
 */

template<class Candidate>
concept MessageCategoryTraitConcept = requires {
    { Candidate::id() } -> std::same_as<MessageCategory>;
    { Candidate::displayName() } -> std::same_as<QString>;
    { Candidate::configKey() } -> std::same_as<QString>;
};

}; // namespace Draupnir::Logging

#endif // MESSAGECATEGORYTRAITCONCEPT_H
