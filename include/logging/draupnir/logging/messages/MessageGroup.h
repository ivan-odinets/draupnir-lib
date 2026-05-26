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

#ifndef MESSAGEGROUP_H
#define MESSAGEGROUP_H

#include <atomic>

#include "draupnir/utils/integer_wrapper.h"

namespace Draupnir::Logging
{

/*! @class MessageGroup draupnir/logging/messages/MessageGroup.h
 *  @ingroup Logging
 *  @brief Represents a logical group of related log messages.
 *
 *  @details This class is a lightweight wrapper around an integer identifier. It is used by @ref Draupnir::Logging::Logger to
 *           associate several log messages with the same logical operation or batch.
 *
 *           Grouped messages may be temporarily accumulated and processed later when the group is flushed or ended. The exact
 *           handling strategy is defined by the configured message handler.
 *
 *           This class is copyable, comparable, and hashable, making it usable as a key in standard and Qt containers such
 *           as `std::map`, `std::unordered_map`, `QMap`, `QHash`, etc.
 *
 * @see @ref Draupnir::Logging::Logger, @ref Draupnir::Logging::MessageHandlerInterface */

class MessageGroup : public draupnir::utils::integer_wrapper<uint64_t,MessageGroup>
{
public:
    /*! @brief Generates a new unique message group identifier.
     *  @return A new @ref MessageGroup object with a unique numeric value.
     * @note The generated value is unique within the current process. */
    static MessageGroup generateUniqueGroup() {
        static std::atomic<uint64_t> counter{1};
        return MessageGroup{counter.fetch_add(1, std::memory_order_relaxed)};
    }

    using draupnir::utils::integer_wrapper<uint64_t,MessageGroup>::integer_wrapper;
    using draupnir::utils::integer_wrapper<uint64_t,MessageGroup>::operator=;

    /*! @brief Returns the internal numeric ID of the group.
     *  @return Integer ID of the group. */
    [[deprecated]] auto id() const { return this->value(); }
};

}; // namespace Draupnir::Logging

namespace std {
    template <>
    struct hash<Draupnir::Logging::MessageGroup> {
        std::size_t operator()(const Draupnir::Logging::MessageGroup& group) const noexcept {
            return std::hash<int>{}(group.value());
        }
    };
}

#include <QHash>

inline uint qHash(const Draupnir::Logging::MessageGroup& key, uint seed = 0) noexcept {
    return qHash(key.value(), seed);
}

#endif // MESSAGEGROUP_H
