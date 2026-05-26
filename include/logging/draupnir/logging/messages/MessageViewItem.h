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

#ifndef MESSAGEVIEWITEM_H
#define MESSAGEVIEWITEM_H

#include "draupnir/logging/messages/Message.h"
#include "draupnir/logging/messages/MessageViewItemFields.h"

namespace Draupnir::Logging
{

class AbstractMessageViewIconProvider;

/*! @class MessageViewItem draupnir/logging/messages/MessageViewItem.h
 *  @ingroup Logging
 *  @brief View representation of the @ref Draupnir::Logging::Message objects. */

class MessageViewItem final
{
    Q_DISABLE_COPY(MessageViewItem);
public:
    static void registerIconProvider(AbstractMessageViewIconProvider* provider) {
        Q_ASSERT(provider);
        Q_ASSERT(p_iconProvider == nullptr);
        p_iconProvider = provider;
    }

    explicit MessageViewItem(Message* message);
    ~MessageViewItem() = default;

    const Message* message() const { return p_message; }

    /*! @brief Returns type of @ref Draupnir::Logging::Message object, refered by this @ref MessageViewItem. */
    MessageType type() const { return p_message->type(); };

    /*! @brief Returns brief description of @ref Draupnir::Logging::Message object, refered by this @ref MessageViewItem. */
    QString brief() const { return p_message->brief(); }

    /*! @brief Returns text of @ref Draupnir::Logging::Message object, refered by this @ref MessageViewItem. */
    QString what() const { return p_message->what(); };

    /*! @brief Returns `QDateTime` when @ref Draupnir::Logging::Message object, refered by this @ref MessageViewItem was
     *         created. */
    QDateTime dateTime() const { return p_message->dateTime(); }

    /*! @brief Returns `QString` with specified fields of the @ref Message object. */
    QString getViewString(const MessageViewItemFields& fields) const;

    /*! @brief This method returns an `QIcon` for the type of this @ref Message. */
    const QIcon& icon() const;

private:
    static AbstractMessageViewIconProvider* p_iconProvider;

    Message* p_message;
    mutable QString m_cachedView;
    mutable MessageViewItemFields m_cachedFields;
};

};

#endif // MESSAGEVIEWITEM_H
