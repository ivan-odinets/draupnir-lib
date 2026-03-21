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

#ifndef ACTIONHANDLERCONCEPT_H
#define ACTIONHANDLERCONCEPT_H

#include <concepts>

namespace Draupnir::Handlers {

/*! @namespace ActionHandler draupnir/ui_bricks/concepts/ActionHandlerConcept.h
 *  @ingroup UiBricks
 *  @brief Helper concepts for runtime action handlers.
 *  @details Provides detection concepts for handler types exposing exactly one supported runtime slot-like method:
 *           either `onTriggered()` or `onTriggered(bool)`. */

namespace ActionHandler
{

/*! @brief Checks whether handler provides `void onTriggered(bool)`. */
template<class Handler>
concept HasRuntimeOnTriggeredWithBool = requires(Handler& handler, bool b) {
    { handler.onTriggered(b) } -> std::same_as<void>;
};

/*! @brief Checks whether handler provides `void onTriggered()`. */
template<class Handler>
concept HasRuntimeOnTriggered = requires(Handler& handler) {
    { handler.onTriggered() } -> std::same_as<void>;
};

}; // namespace Draupnir::Handlers::ActionHandler

/*! @ingroup UiBricks
 *  @brief Requires handler to provide exactly one supported runtime `onTriggered` overload.
 *  @details A valid handler must expose either `void onTriggered()` o `void onTriggered(bool)`, but not both at the
 *           same time. */
template<class Handler>
concept ActionHandlerConcept = (
    (ActionHandler::HasRuntimeOnTriggered<Handler> ? 1 : 0) +
    (ActionHandler::HasRuntimeOnTriggeredWithBool<Handler> ? 1 : 0)
) == 1;

}; // namespace Draupnir::Handlers

#endif // ACTIONHANDLERCONCEPT_H
