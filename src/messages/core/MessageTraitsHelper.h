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

#ifndef MESSAGETRAITSHELPER_H
#define MESSAGETRAITSHELPER_H

#include "MessageType.h"

/*! @class MessageTraitsHelper draupnir-lib/src/core/MessageTraitsHelper.h
 *  @brief This class is a helper class to work with MessageTraits... template packs.
 *  @tparam Args... A pack of MessageTraits. */

template<class... Args>
class MessageTraitsHelper
{
public:
    /*! @brief Static constexpr member with size of provided Args... pack. */
    static constexpr std::size_t N = sizeof...(Args);

    /*! @brief Static constexpr C-style array of supported MessageIDs within this Args... pack. */
    static constexpr MessageType supportedMessageIds[] = { Args::type... };

    /*! @brief Static constexpr method returning true if all types within Args... are unique. */
    static constexpr bool hasUniqueIds() {
        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = i + 1; j < N; ++j)
                if (supportedMessageIds[i] == supportedMessageIds[j])
                    return false;

        return true;
    }

    /*! @brief Static constexpr method returning true if all types within Args.. pack are flags (have
     *         only one bit being set). */
    static constexpr bool hasFlags() {
        for (std::size_t i = 0; i < N; ++i)
            if (!((supportedMessageIds[i].id() != 0) && ((supportedMessageIds[i].id() & (supportedMessageIds[i].id() - 1)) == 0)))
                return false;

        return true;
    }
};

#endif // MESSAGETRAITSHELPER_H
