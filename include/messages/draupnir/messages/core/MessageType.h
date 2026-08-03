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

#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include "draupnir/messages/categories/MessageCategories.h"
#include "draupnir/messages/core/MessageLevels.h"

namespace Draupnir::Messages
{

/*! @class MessageType draupnir/messages/core/MessageType.h
 *  @ingroup Messages
 *  @brief Represents a complete message type.
 *
 *  @details A message type combines a @ref Draupnir::Messages::MessageLevel::Value with a @ref Draupnir::Messages::MessageCategory.
 *
 *           The level describes the message severity, while the category identifies the logical subsystem or domain associated
 *           with the message.
 *
 *           MessageType is a lightweight value type. It is copyable, assignable, and comparable. */

class MessageType
{
public:
    /*! @brief Creates a message type from level and category.
     *  @param level Message severity level.
     *  @param category Message category. */
    constexpr MessageType(MessageLevel::Value level, MessageCategory category) noexcept :
        m_level{level},
        m_category{category}
    {}

    /*! @brief Returns the message severity level.
     *  @return Message level. */
    [[nodiscard]] constexpr MessageLevel::Value level() const noexcept { return m_level; }

    /*! @brief Returns the message category.
     *  @return Message category. */
    [[nodiscard]] constexpr MessageCategory category() const noexcept { return m_category; }

    /*! @brief Compares two message types.
     *  @param other Message type to compare with.
     *  @return `true` if both message types have the same level and category; otherwise `false`. */
    [[nodiscard]] constexpr bool operator==(const MessageType&) const noexcept = default;

private:
    MessageLevel::Value m_level;
    MessageCategory m_category;
};

} // namespace Draupnir::Messages

#endif // MESSAGETYPE_H
