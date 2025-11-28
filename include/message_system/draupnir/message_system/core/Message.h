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

#include "draupnir/utils/advance_enum.h"

namespace Draupnir::Messages
{

/*! @class Message draupnir/core/Message.h
 *  @ingroup MessageSystem
 *  @brief This class represents some message from the App about event happened.
 *
 * @todo Prettify fields-related code.
 * @todo Add Fields::toString / Fields::fromString or other settings conversion thingy. */

class Message final
{
    Q_DISABLE_COPY(Message);
public:
    /*! @brief This enum holds flag for fields of @ref Message objects, which can be displayed seperately. */
    enum Fields : uint8_t {
        None       = 0b00000000,  /*!< @brief None. */
        Brief      = 0b00000001,  /*!< @brief Brief summary of Message (Message::brief). */
        What       = 0b00000010,  /*!< @brief Details what happened (Message::what). */
        DateTime   = 0b00000100,  /*!< @brief DateTime of Message (Message::dateTime). */
        Icon       = 0b00001000,  /*!< @brief Icon of Message (Message::icon). */
        All        = Brief | What | DateTime | Icon,  /*!< @brief All of the above. */
    };

    /*! @brief This static constexpr array holds values from enum @ref Draupnir::Messages::Message::Fields, which are
     *         representing individual fields of the @ref Draupnir::Messages::Message objects. */
    static constexpr Fields individualFieldsArray[] = { Brief, What, DateTime, Icon };

    /*! @brief This is a static comverting Fields valur to user-friendly string. */
    static QString fieldsDisplayString(Fields value) {
        switch (value) {
        case None:
            return QObject::tr("None");
        case Brief:
            return QObject::tr("Brief");
        case What:
            return QObject::tr("What");
        case DateTime:
            return QObject::tr("Timestamp:");
        case Icon:
            return QObject::tr("Icon");
        case All:
            return QObject::tr("All");
        }
        Q_ASSERT(false);
        return QString{};
    }

    /*! @brief Static template method to create @ref Message objects from specified `MessageTrait`.
     *  @tparam MessageTrait trait representing a @ref Message to be created.
     *  @param text text of a @ref Message.
     * @note Memory is allocated by using new operator. Caller to be responsible for propper memory handling.
     * @todo static_assert to verify MessageTrait. */
    template<class MessageTrait>
    static Message* fromTrait(const QString& text) {
        return new Message{MessageTrait::type, MessageTrait::icon(), MessageTrait::displayName(), text};
    }

    /*! @brief Static template method to create @ref Message objects from specified `MessageTrait`.
     *  @tparam MessageTrait trait representing a @ref Message to be created.
     *  @param brief brief of a Message.
     *  @param text text of a Message.
     * @note Memory is allocated by using new operator. Caller to be responsible for propper memory handling.
     * @todo static_assert to verify MessageTrait. */
    template<class MessageTrait>
    static Message* fromTrait(const QString& brief, const QString& text) {
        return new Message{MessageTrait::type, MessageTrait::icon(), brief, text};
    }

    /*! @brief Returns `QString` with specified fields of the @ref Message object. */
    QString getViewString(const std::underlying_type_t<Fields> fields) const {
        static constexpr std::underlying_type_t<Message::Fields> notIconFields =
                Message::Brief | Message::What | Message::DateTime;
        // This method shoud return QString, which is independent of icon display status.
        // To get rid of anything but not Brief, What, DateTime - we use this.
        if ((fields & notIconFields) != m_cachedFields) {
            m_cachedFields = fields & notIconFields;
            m_cachedView.clear();
            if (fields & Message::Brief)
                m_cachedView += brief();
            if (fields & Message::What)
                m_cachedView += (m_cachedView.isEmpty() ? "" : "\n") + what();
            if (fields & Message::DateTime)
                m_cachedView += (m_cachedView.isEmpty() ? "" : "\n") + dateTime().toString();
        }
        return m_cachedView;
    }

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

    const uint64_t m_type;
    const QIcon& m_icon;
    const QString m_brief;
    const QString m_what;
    const QDateTime m_dateTime;
    mutable QString m_cachedView;
    mutable std::underlying_type_t<Fields> m_cachedFields;

    using FieldsValues = draupnir::utils::enum_values<
        Message::Fields,
        Message::Brief,
        Message::Icon,
        Message::All
    >;
    friend Message::Fields& operator++(Message::Fields& type,int);
};

inline Message::Fields& operator++(Message::Fields& type,int)
{
    Message::FieldsValues::advance(type);
    return type;
}

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

Q_DECLARE_METATYPE(Draupnir::Messages::Message::Fields);
Q_DECLARE_METATYPE(Draupnir::Messages::Message*);

#endif // MESSAGE_H
