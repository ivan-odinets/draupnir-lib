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

#ifndef GENERICMENUENTRYHANDLER_H
#define GENERICMENUENTRYHANDLER_H

namespace Draupnir::Handlers {

/*! @class GenericMenuEntryHandler draupnir/handlers/AbstractHandlers.h
 *  @ingroup HandlerTemplates
 *  @brief Non-specialized template for menu entry handlers.
 *  @tparam Context - The context type (ignored in this base template).
 *  @tparam HandledEntry - The menu entry trait/class to be handled.
 *
 *  @details This is the primary template for GenericMenuEntryHandler, serving as a catch-all for menu entry types that do
 *           not have a user-provided specialization. Its sole purpose is to trigger a compile-time error when instantiated
 *           with an unsupported menu entry type.
 *
 *           Attempting to instantiate this template directly will always result in a static_assert failure with a clear
 *           error message. You must provide a specialization for every menu entry type you intend to support. */

template<class Context,class HandledEntry>
class GenericMenuEntryHandler
{
    template<class> static inline constexpr bool dependent_false_v = false;
public:
    /*! @brief Dummy constructor, triggers a compile-time error. Instantiating this template always results in a static_assert
     *         failure, indicating that a specialization for the given HandledEntry is missing. */
    GenericMenuEntryHandler() {
        static_assert(dependent_false_v<HandledEntry>,
                "GenericMenuEntryHandler template MUST be specialized for all handled entries.");
    }

    /*! @brief Dummy constructor, triggers a compile-time error. Instantiating this template always results in a static_assert
     *         failure, indicating that a specialization for the given HandledEntry is missing.
     *  @param Unused Reference to context (only required for interface compatibility).  */
    GenericMenuEntryHandler(Context&) {
        static_assert(dependent_false_v<HandledEntry>,
                "GenericMenuEntryHandler template MUST be specialized for all handled entries.");
    }
};

};

#endif // GENERICMENUENTRYHANDLER_H
