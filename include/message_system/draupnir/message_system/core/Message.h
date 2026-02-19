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

#include "draupnir/message_system/concepts/MessageTraitConcept.h"
#include "draupnir/message_system/core/MessageFields.h"

namespace Draupnir::Messages
{

/*! @class Message draupnir/core/Message.h
 *  @ingroup MessageSystem
 *  @brief This class represents some message from the App about event happened. */

class Message final
{
    Q_DISABLE_COPY(Message);
public:
    /*! @brief Conveniance alias to the @ref Draupnir::Messages::MessageField enum. */
    using Field = MessageField;

    /*! @brief Conveniance alias to the @ref Draupnir::Messages::MessageFields flag_enum. */
    using Fields = MessageFields;

    /*! @brief Static template method to create @ref Message objects from specified `MessageTrait`.
     *  @tparam MessageTrait trait representing a @ref Message to be created.
     *  @param text text of a @ref Message.
     * @note Memory is allocated by using new operator. Caller to be responsible for propper memory handling. */
    template<MessageTraitConcept MessageTrait>
    static Message* fromTrait(const QString& text) {
        return new Message{MessageTrait::type, MessageTrait::icon(), MessageTrait::displayName(), text};
    }

    /*! @brief Static template method to create @ref Message objects from specified `MessageTrait`.
     *  @tparam MessageTrait trait representing a @ref Message to be created.
     *  @param brief brief of a Message.
     *  @param text text of a Message.
     * @note Memory is allocated by using new operator. Caller to be responsible for propper memory handling. */
    template<MessageTraitConcept MessageTrait>
    static Message* fromTrait(const QString& brief, const QString& text) {
        return new Message{MessageTrait::type, MessageTrait::icon(), brief, text};
    }

    /*! @brief Returns `QString` with specified fields of the @ref Message object. */
    QString getViewString(const MessageFields& fields) const;

    /*! @brief Returns type of this @ref Message object. */
    uint64_t type() const { return m_type; };

    /*! @brief Returns brief description of this @ref Message object. */
    QString brief() const { return m_brief; }

    /*! @brief Returns text of this @ref Message object. */
    QString what() const { return m_what; };

    /*! @brief Returns `QDateTime` when this @ref Message object was created. */
    QDateTime dateTime() const { return m_dateTime; }

    /*! @brief This method returns an `QIcon` for the type of this @ref Message. */
    const QIcon& icon() const { return m_icon; }

private:
    Message(const uint64_t newType, const QIcon& icon, const QString& brief, const QString& what);
    Message(const uint64_t newType, const QIcon& icon, QString&& brief, QString&& what);

    const uint64_t m_type;
    const QIcon& m_icon;
    const QString m_brief;
    const QString m_what;
    const QDateTime m_dateTime;
    mutable QString m_cachedView;
    mutable MessageFields m_cachedFields;
};

inline QDebug operator<< (QDebug dbg, Message* messagePtr)
{
    dbg.nospace() << qUtf8Printable(QString("Message(type=%1; brief=%2; what=%3)")
                   .arg(QString::number(messagePtr->type(),2))
                   .arg(messagePtr->brief())
                   .arg(messagePtr->what()));

    return dbg.maybeSpace();
}

typedef QList<Message*> MessageList;

}; // namespace Draupnir::Messages

Q_DECLARE_METATYPE(Draupnir::Messages::Message*);

#endif // MESSAGE_H
