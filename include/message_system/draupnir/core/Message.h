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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QDebug>
#include <QIcon>
#include <QList>

namespace Draupnir::Messages
{

/*! @class Message draupnir/core/Message.h
 *  @brief This class represents some Message from the App about some event happened. This includes errors, balance changes,
 *         etc. */

class Message
{
    Q_DISABLE_COPY(Message);
public:
    /*! @brief Returns type of this Message object. */
    uint64_t type() const { return m_type; };

    /*! @brief Returns brief description of this Message object. */
    QString brief() const { return m_brief; }

    /*! @brief Returns text of this Message object. */
    QString what() const { return m_what; };

    /*! @brief Returns QDateTime when this Message object was created. */
    QDateTime dateTime() const { return m_dateTime; }

    /*! @brief This method returns an QIcon for the type of this message. */
    const QIcon& icon() const { return m_icon; }

protected:

    Message(const uint64_t newType, const QIcon& icon, const QString& brief, const QString& what) :
        m_type{newType},
        m_icon{icon},
        m_brief{brief},
        m_what{what},
        m_dateTime{QDateTime::currentDateTime()}
    {}

    Message(const uint64_t newType, const QIcon& icon, QString&& brief, QString&& what) :
        m_type{newType},
        m_icon{icon},
        m_brief{std::move(brief)},
        m_what{std::move(what)},
        m_dateTime{QDateTime::currentDateTime()}
    {}

private:
    const uint64_t m_type;
    const QIcon& m_icon;
    const QString m_brief;
    const QString m_what;
    const QDateTime m_dateTime;
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

#endif // MESSAGE_H
