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

#ifndef MESSAGETYPETRAITTEMPLATE_H
#define MESSAGETYPETRAITTEMPLATE_H

#include "draupnir/messages/concepts/MessageCategoryTraitConcept.h"
#include "draupnir/messages/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageTypeTraitTemplate draupnir/messages/traits/MessageTypeTraitTemplate.h
 *  @ingroup Messages
 *  @brief Defines a message type trait from a category trait and severity level.
 *  @tparam CategoryTrait Message category trait satisfying @ref Draupnir::Messages::MessageCategoryTraitConcept.
 *  @tparam Level Built-in severity level associated with the message type.
 *
 *  @details Combines a compile-time message category trait with a built-in message severity level. The resulting trait
 *           provides the complete @ref Draupnir::Messages::MessageType and its configuration string.
 *
 *           The configuration string is formed by concatenating the category configuration token with the severity-level
 *           configuration token.
 *
 *
 * @see Draupnir::Messages::MessageLevel
 * @see Draupnir::Messages::MessageTypeTraitConcept */

template<MessageCategoryTraitConcept CategoryTrait, MessageLevel::Value Level>
class MessageTypeTraitTemplate
{
public:
    MessageTypeTraitTemplate() = delete;
    Q_DISABLE_COPY_MOVE(MessageTypeTraitTemplate);

    /*! @brief Returns the message type represented by this trait.
     *  @return Message type combining `Level` with the category supplied by `CategoryTrait`. */
    static constexpr MessageType type() {
        return MessageType(Level, CategoryTrait::value());
    }

    /*! @brief Returns the configuration string for this message type.
     *  @return Latin-1 string containing the category and severity-level configuration tokens.
     *  @details The returned string references static internal storage and remains valid for the lifetime of the application.
     * @todo Question: Decide on the proper formatting of the onfig string.
     * @todo Question: Check if here there is a way to switch to the consteval / constexpr context. */
    static QLatin1String configString() {
        static QByteArray storage =
            CategoryTrait::configString().latin1() +
            MessageLevel::Serializer::toConfigString(Level).toLatin1();
        return QLatin1String{storage};
    }
};

} // namespace Draupnir::Messages

#endif // MESSAGETYPETRAITTEMPLATE_H
