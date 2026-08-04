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

#ifndef MESSAGECATEGORIES_H
#define MESSAGECATEGORIES_H

#include <QMetaType>

#include "draupnir/utils/integer_wrapper.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Messages
{

/*! @class MessageCategory draupnir/messages/categories/MessageCategories.h
 *  @ingroup Messages
 *  @brief Represents a log message category identifier.
 *
 *  @details Message categories are used in combination with @ref Draupnir::Messages::MessageLevel to form a complete @ref
 *           Draupnir::Messages::MessageType. Categories are represented as bit-like integer identifiers, allowing them
 *           to be combined into category masks where needed. */

class MessageCategory final : public draupnir::utils::integer_wrapper<quint64, MessageCategory>
{
    using _Base = draupnir::utils::integer_wrapper<quint64, MessageCategory>;

public:
    using _Base::integer_wrapper;
    using _Base::operator=;

    /*! @enum MessageCategory::Value
     *  @brief Built-in message category values. */
    enum Value : _Base::underlying_type {
        /*! @brief Default message category used when no custom category is specified. */
        Default             = 0b1,
        /*! @brief Category for network-related messages. */
        Network             = 0b10,
        /*! @brief First value reserved for user-defined message categories. */
        FirstCustomCategory = 0b100,
    };

    /*! @brief Returns the category identifier following another category.
     *  @param previousCategory Previous category identifier.
     *  @return Category whose numeric value is shifted one bit to the left.
     *  @pre The value of `previousCategory` must leave enough space for a one-bit left shift without overflowing the underlying
     *       integer. */
    [[nodiscard]] static constexpr MessageCategory nextType(MessageCategory previousCategory) noexcept {
        return MessageCategory{previousCategory << 1};
    }
};

/*! @class MessageCategories draupnir/messages/categories/MessageCategories.h
 *  @ingroup Messages
 *  @brief Flag mask containing zero or more message categories.
 *
 *  @details MessageCategories combines @ref Draupnir::Messages::MessageCategory values into a single mask. It provides predefined
 *           empty and complete masks together with configuration tokens for these special values.
 *
 *           The type is copyable, assignable, comparable, and hashable. */

class MessageCategories final : public draupnir::utils::flags<MessageCategory>
{
    using _Base = draupnir::utils::flags<MessageCategory>;

public:
    using _Base::flags;

    using _Base::operator=;

    /*! @brief Empty category mask. */
    static constexpr _Base::primitive_integer None = 0;

    /*! @brief Mask containing every bit supported by the underlying type.
     * @note This includes bits not currently assigned to registered message categories. */
    static constexpr _Base::primitive_integer All =
        std::numeric_limits<MessageCategory::underlying_type>::max();

    /*! @brief Returns the configuration token for the @ref None mask.
     *  @return Latin-1 string containing `"none"`. */
    [[nodiscard]] static QLatin1String noneConfigKey()     { return QLatin1String{"none"}; }

    /*! @brief Returns the configuration token for the @ref All mask.
     *  @return Latin-1 string containing `"all"`. */
    [[nodiscard]] static QLatin1String allConfigKey()      { return QLatin1String{"all"}; }
};

/*! @brief Calculates a Qt hash value for a message category.
 *  @ingroup Messages
 *  @param key Message category to hash.
 *  @param seed Initial hash seed.
 *  @return Hash value of the underlying category identifier. */

[[nodiscard]] inline std::size_t qHash(const Draupnir::Messages::MessageCategory& key, std::size_t seed = 0) noexcept {
    return ::qHash(key.value(), seed);
}

/*! @brief Calculates a Qt hash value for a message category mask.
 *  @ingroup Messages
 *  @param key Message category mask to hash.
 *  @param seed Initial hash seed.
 *  @return Hash value of the underlying mask value. */

[[nodiscard]] inline std::size_t qHash(const Draupnir::Messages::MessageCategories& key, std::size_t seed = 0) noexcept {
    return ::qHash(key.value().value(), seed);
}

} // Draupnir::Messages

namespace std
{

/*! @ingroup Messages
 *  @brief Standard hash implementation for @ref Draupnir::Messages::MessageCategory. */

template<>
struct hash<Draupnir::Messages::MessageCategory>
{
    [[nodiscard]] std::size_t operator()(const Draupnir::Messages::MessageCategory& category) const noexcept {
        return std::hash<Draupnir::Messages::MessageCategory::underlying_type>{}(category.value());
    }
};

/*! @ingroup Messages
 *  @brief Standard hash implementation for @ref Draupnir::Messages::MessageCategories. */

template<>
struct hash<Draupnir::Messages::MessageCategories>
{
    [[nodiscard]] std::size_t operator()(const Draupnir::Messages::MessageCategories& categories) const noexcept {
        return std::hash<Draupnir::Messages::MessageCategories::flag_type::underlying_type>{}(categories.value().value());
    }
};

} // namespace std

Q_DECLARE_METATYPE(Draupnir::Messages::MessageCategory);
Q_DECLARE_METATYPE(Draupnir::Messages::MessageCategories);

#endif // MESSAGECATEGORIES_H
