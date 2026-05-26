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

#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H

#include "draupnir/logging/messages/MessageCategories.h"
#include "draupnir/logging/messages/MessageLevels.h"

namespace Draupnir::Logging
{

/*! @class MessageType draupnir/logging/messages/MessageType.h
 *  @ingroup Logging
 *  @brief Represents a complete log message type.
 *
 *  @details A message type is defined by a combination of a @ref Draupnir::Logging::MessageLevel::Value and a @ref
 *           Draupnir::Logging::MessageCategory.
 *
 *           The level describes message severity, while the category describes the logical subsystem or domain that
 *           produced the message. */

class MessageType
{
public:
    /*! @brief Creates a message type from level and category.
     *  @param level Message severity level.
     *  @param category Message category. */
    constexpr MessageType(MessageLevel::Value level, MessageCategory category) :
        m_level{level},
        m_category{category}
    {}

    /*! @brief Returns the message severity level.
     *  @return Message level. */
    constexpr MessageLevel::Value level() const { return m_level; }

    /*! @brief Returns the message category.
     *  @return Message category. */
    constexpr MessageCategory category() const { return m_category; }

    /*! @brief Compares two message types.
     *  @param other Message type to compare with.
     *  @return `true` if both message types have the same level and category; otherwise `false`. */
    constexpr bool operator==(const MessageType& other) const noexcept {
        return (other.m_level == this->m_level) && (other.m_category == this->m_category);
    }

private:
    const MessageLevel::Value m_level;
    const MessageCategory m_category;
};

}

#endif // MESSAGETYPES_H
