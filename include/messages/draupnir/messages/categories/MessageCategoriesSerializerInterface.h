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

#ifndef MESSAGECATEGORIESSERIALIZERINTERFACE_H
#define MESSAGECATEGORIESSERIALIZERINTERFACE_H

#include "draupnir/messages/categories/MessageCategories.h"

namespace Draupnir::Messages
{

/*! @class MessageCategoriesSerializerInterface draupnir/messages/categories/MessageCategoriesSerializerInterface.h
 *  @ingroup Messages
 *  @brief Abstract interface for serializing message category masks.
 *
 *  @details Defines bidirectional conversion between @ref Draupnir::Messages::MessageCategories masks and their textual configuration
 *           representation.
 *
 *           Implementations are responsible for recognizing the supported message categories and their corresponding configuration
 *           tokens. */

class MessageCategoriesSerializerInterface
{
public:
    /*! @brief Virtual destructor. */
    virtual ~MessageCategoriesSerializerInterface() = default;

    /*! @brief Deserializes a message category mask from a configuration string.
     *  @param configString Configuration string to deserialize.
     *  @return Deserialized message category mask, or `std::nullopt` if the string is invalid or contains unsupported category tokens. */
    [[nodiscard]] virtual std::optional<Draupnir::Messages::MessageCategories> fromConfigString(const QString& configString) const = 0;

    /*! @brief Serializes a message category mask to its configuration string.
     *  @param mask Message category mask to serialize.
     *  @return Configuration string representing the specified mask. */
    [[nodiscard]] virtual QString toConfigString(Draupnir::Messages::MessageCategories mask) const = 0;
};

} // namespace Draupnir::Messages

#endif // MESSAGECATEGORIESSERIALIZERINTERFACE_H
