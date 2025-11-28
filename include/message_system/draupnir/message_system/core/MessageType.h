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

#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <cstdint>
#include <QMetaType>

namespace Draupnir::Messages
{

/*! @class MessageType draupnir/message_system/core/MessageType.h
 *  @ingroup MessageSystem
 *  @brief This class is used to represent and manipulate message type identifiers.
 *
 *  @details @ref MessageType is a lightweight wrapper over a 64-bit integer that provides type-safe, constexpr-friendly message
 *           identifiers. It's used to distinguish between various message categories such as Debug, Info, Warning, Error, and
 *           custom types.
 *
 *           While adding custom message types the best would be to use MessageType::nextType method and start these types
 *           from DefaultMessageTypes::FirstCustomType enum key.
 *
 * @note This class is designed to work efficiently in compile-time contexts.
 *
 * @todo Refractor and optimize this class. */

class MessageType
{
public:
    /*! @brief Predefined default message types. */
    enum DefaultMessageTypes : quint64 {
        None            = 0b0000'0000,
        Debug           = 0b0000'0001,
        Info            = 0b0000'0010,
        Warning         = 0b0000'0100,
        Error           = 0b0000'1000,
        FirstCustomType = 0b0001'0000, /*!< @brief Custom message types MUST start from this enum value. They must be bit masks
                                                   and can be automatically obtained by using MessageType::nextType method. */
        /*! @brief This enum key is used by MessageListProxyModel to match all registered message types. */
        AllMessages     = 0b1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111
    };

    /*! @brief Constructs a @ref MessageType with the given value.
     *  @param id Raw numeric identifier for the message type. */
    constexpr MessageType(quint64 id = 0) :
        m_id{id}
    {}

    /*! @brief Implicit conversion to `uint64_t`.
     *  @return The internal message type ID. */
    constexpr operator quint64() const { return m_id; }

    /*! @brief Returns the raw identifier.
     *  @return The internal message type ID. */
    constexpr quint64 id() const { return m_id; }

    /*! @brief Bitwise left shift operator.
     *  @param shift Number of bits to shift.
     *  @return A new MessageType with the shifted value. */
    constexpr MessageType operator<<(int shift) const { return MessageType{m_id << shift}; }

    /*! @brief Equality operator.
     *  @param other The other MessageType to compare with.
     *  @return True if both message types have the same ID, false otherwise. */
    constexpr bool operator==(MessageType other) const { return m_id == other.m_id; }

    /*! @brief Inequality operator.
     *  @param other The other MessageType to compare with.
     *  @return True if the message types have different IDs, false otherwise. */
    constexpr bool operator!=(MessageType other) const { return m_id != other.m_id; }

    /*! @brief Less-than comparison operator.
     *  @param other The other MessageType to compare with.
     *  @return True if this message type has a smaller ID than the other. */
    constexpr bool operator<(MessageType other) const { return m_id < other.m_id; }

    /*! @brief Returns the next available message type ID.
     *  @param prevType The previous MessageType.
     *  @return A new MessageType with the value shifted left by 1. */
    static constexpr MessageType nextType(MessageType prevType) { return MessageType{prevType.id() << 1}; }

private:
    quint64 m_id;
};

}; // Draupnir::Messages

Q_DECLARE_METATYPE(Draupnir::Messages::MessageType);

#endif // MESSAGETYPE_H
