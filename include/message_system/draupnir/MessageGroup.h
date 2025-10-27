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

#ifndef MESSAGEGROUP_H
#define MESSAGEGROUP_H

#include <chrono>
#include <random>
#include <functional>

namespace Draupnir::Messages
{

/*! @class MessageGroup draupnir/MessageGroup.h
 *  @brief Represents a logical group of related log messages.
 *
 *  @details The MessageGroup class is a lightweight wrapper around an integer ID, used to group related log messages together.
 *           Messages assigned to the same group are immediately shown in the MessageListModel (see MessageHandler)
 *           when logged via the Logger. Displaying the actual notification (system tray or message box) is postponed
 *           until this group is flushed or ended.
 *
 *           This class is copyable, comparable, and hashable, making it usable as a key in standard and Qt containers such as
 *           std::map, std::unordered_map, QMap, QHash, etc. */

class MessageGroup
{
public:
    /*! @brief Returns MessageGroup object with random integer id.
     * @todo Improve randomness of this method. */
    static MessageGroup generateUniqueGroup() {
        static std::mt19937 rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        int randomPart = static_cast<int>(rng() & 0xFFFF);

        return MessageGroup{static_cast<int>((now ^ randomPart) & 0x7FFFFFFF)};
    }

    /*! @brief Constructs a MessageGroup with the given ID.
     *  @param id The unique integer identifier for this group. */
    explicit MessageGroup(int id = 0) :
        m_id{id}
    {}

    /*! @brief Returns the internal numeric ID of the group.
     *  @return Integer ID of the group. */
    int id() const { return m_id; }

    /*! @brief Equality operator.
     *  @param other Another MessageGroup.
     *  @return True if both groups have the same ID. */
    bool operator==(const MessageGroup& other) const { return m_id == other.m_id; }

    /*! @brief Inequality operator.
     *  @param other Another MessageGroup.
     *  @return True if groups have different IDs. */
    bool operator!=(const MessageGroup& other) const { return m_id != other.m_id; }

    /*! @brief Less-than operator.
     *  @param other Another MessageGroup.
     *  @return True if this group's ID is less than the other's. */
    bool operator<(const MessageGroup& other) const { return m_id < other.m_id; }

    /*! @brief Greater-than operator.
     *  @param other Another MessageGroup.
     *  @return True if this group's ID is greater than the other's. */
    bool operator>(const MessageGroup& other) const { return m_id > other.m_id; }

private:
    int m_id;
};

}; // namespace Draupnir::Messages

namespace std {
    template <>
    struct hash<Draupnir::Messages::MessageGroup> {
        std::size_t operator()(const Draupnir::Messages::MessageGroup& group) const noexcept {
            return std::hash<int>{}(group.id());
        }
    };
}

#include <QHash>

inline uint qHash(const Draupnir::Messages::MessageGroup& key, uint seed = 0) noexcept {
    return qHash(key.id(), seed);
}

#endif // MESSAGEGROUP_H
