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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QDebug>
#include <QIcon>
#include <QList>

#include "draupnir/messages/core/MessageType.h"

namespace Draupnir::Messages
{

class Message;

/*! @brief Shared owning pointer to a @ref Draupnir::Messages::Message.
 *  @ingroup Messages
 *
 *  @details Copies of @ref Draupnir::Messages::MessagePtr share ownership of the referenced message. The message is destroyed
 *           when the last @ref MessagePtr referencing it is destroyed or reset. */

using MessagePtr = QSharedPointer<Message>;

/*! @class Message draupnir/messages/core/Message.h
 *  @ingroup Messages
 *  @brief Represents an application message describing an occurred event. */

class Message final
{
    Q_DISABLE_COPY(Message);
public:
    /*! @brief Creates a new message with empty brief text.
     *  @param text Message text.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category.
     *  @return Newly created @ref Draupnir::Messages::MessagePtr object.
     *  @details The returned MessagePtr shares ownership of the created message. The message remains alive while at least one MessagePtr references it. */
    static MessagePtr create(
        const QString& text,
        MessageLevel::Value messageLevel,
        MessageCategory messageCategory = MessageCategory::Default
    ) {
        return MessagePtr{new Message{ MessageType{messageLevel, messageCategory}, "", text }};
    }

    /*! @brief Creates a new message with brief and full text.
     *  @param brief Short message summary.
     *  @param what Full message text.
     *  @param messageLevel Message severity level.
     *  @param messageCategory Message category.
     *  @return Newly created @ref Draupnir::Messages::MessagePtr object.
     *  @details The returned MessagePtr shares ownership of the created message. The message remains alive while at least one MessagePtr references it. */
    static MessagePtr create(
        const QString& brief,
        const QString& what,
        MessageLevel::Value messageLevel,
        MessageCategory messageCategory = MessageCategory::Default
    ) {
        return MessagePtr{new Message{ MessageType{messageLevel, messageCategory}, brief, what }};
    }

    /*! @brief Returns type of this @ref Message object. */
    MessageType type() const { return m_type; };

    /*! @brief Returns brief description of this @ref Message object. */
    QString brief() const { return m_brief; }

    /*! @brief Returns text of this @ref Message object. */
    QString what() const { return m_what; };

    /*! @brief Returns `QDateTime` when this @ref Message object was created. */
    QDateTime dateTime() const { return m_dateTime; }

private:
    /*! @brief Constructor. Creates a message object.
     *  @param newType Message type.
     *  @param brief Short message summary.
     *  @param what Full message text. */
    Message(const MessageType newType, const QString& brief, const QString& what) :
        m_type{newType},
        m_brief{brief},
        m_what{what},
        m_dateTime{QDateTime::currentDateTime()}
    {}

    /*! @brief Constructor. Creates a message object using movable strings.
     *  @param newType Message type.
     *  @param brief Short message summary.
     *  @param what Full message text. */
    Message(const MessageType newType, QString&& brief, QString&& what) :
        m_type{newType},
        m_brief{std::move(brief)},
        m_what{std::move(what)},
        m_dateTime{QDateTime::currentDateTime()}
    {}

    const MessageType m_type;
    const QString m_brief;
    const QString m_what;
    const QDateTime m_dateTime;
};

/*! @brief Convenience alias for a list of owned message pointers.
 *  @ingroup Messages */
using MessageList = QList<MessagePtr>;

} // namespace Draupnir::Messages

Q_DECLARE_METATYPE(Draupnir::Messages::Message*);
Q_DECLARE_METATYPE(Draupnir::Messages::MessagePtr);
Q_DECLARE_METATYPE(Draupnir::Messages::MessageList);

#endif // MESSAGE_H
