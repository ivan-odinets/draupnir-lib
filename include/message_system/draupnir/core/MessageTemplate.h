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

#ifndef MESSAGETEMPLATE_H
#define MESSAGETEMPLATE_H

#include "draupnir/core/Message.h"

class Logger;

namespace Draupnir::Messages
{

/*! @class MessageTemplate draupnir/core/MessageTemplate.h
 *  @brief This is a helper class to simplify construction of the Message objects from MessageTraits. */

template<class MessageTrait>
class MessageTemplate final : public Message
{
public:

protected:
    friend class ::Logger;
    friend class MessageHandler; // For dummy message

    MessageTemplate(const QString& text) :
        Message{MessageTrait::type, MessageTrait::icon(), MessageTrait::displayName(), text}
    {}

    MessageTemplate(const QString& brief, const QString& text) :
        Message{MessageTrait::type, MessageTrait::icon(), brief, text}
    {}
};

}; // namespace Draupnir::Messages

#endif // MESSAGETEMPLATE_H
