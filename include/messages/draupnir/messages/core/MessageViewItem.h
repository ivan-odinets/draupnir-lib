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

#include "draupnir/messages/core/Message.h"
#include "draupnir/messages/core/MessageViewItemFields.h"

namespace Draupnir::Messages
{

class MessageTypeIconProvider;

/*! @class MessageViewItem draupnir/messages/core/MessageViewItem.h
 *  @ingroup Messages
 *  @brief Presentation item wrapping a message for use in models and views.
 *
 *  @details Provides convenient access to message fields, formatted view text, and the icon associated with the message type.
 *
 * @warning An icon provider must be registered before calling @ref icon or @ref iconFor. */

class MessageViewItem final
{
public:
    /*! @brief Registers the icon provider and transfers its ownership.
     *  @param provider Provider to register. Must be dynamically allocated and must not be deleted by the caller after this
     *         call.
     * @pre `provider` must not be null.
     * @pre No icon provider may already be registered.
     * @warning This overload adopts ownership of the raw pointer. */
    static void registerIconProvider(MessageTypeIconProvider* provider);

    /*! @brief Registers the icon provider.
     *  @param provider Provider whose exclusive ownership is transferred to @ref Draupnir::Messages::MessageViewItem.
     * @pre `provider` must not be null.
     * @pre No icon provider may already be registered. */
    static void registerIconProvider(std::unique_ptr<MessageTypeIconProvider> provider);

    /*! @brief Constructs a presentation item for a message.
     *  @param message @ref Draupnir::Messages::MessagePtr to wrap.
     *  @pre `message` must not be null. */
    explicit MessageViewItem(MessagePtr message);
    ~MessageViewItem() = default;

    /*! @brief Returns the wrapped message.
     *  @return Shared pointer to the message. */
    [[nodiscard]] MessagePtr message() const { return p_message; }

    /*! @brief Returns type of @ref Draupnir::Messages::Message object, refered by this @ref MessageViewItem. */
    [[nodiscard]] MessageType type() const { return p_message->type(); };

    /*! @brief Returns brief description of @ref Draupnir::Messages::Message object, refered by this @ref MessageViewItem. */
    [[nodiscard]] QString brief() const { return p_message->brief(); }

    /*! @brief Returns text of @ref Draupnir::Messages::Message object, refered by this @ref MessageViewItem. */
    [[nodiscard]] QString what() const { return p_message->what(); };

    /*! @brief Returns `QDateTime` when @ref Draupnir::Messages::Message object, refered by this @ref MessageViewItem was
     *         created. */
    [[nodiscard]] QDateTime dateTime() const { return p_message->dateTime(); }

    /*! @brief Returns `QString` with specified fields of the @ref Draupnir::Messages::Message object, refered by this @ref
     *         MessageViewItem.
     *  @param fields Fields to include in the resulting string.
     *  @details Icon-related fields are ignored. The most recently requested textual representation is cached. */
    [[nodiscard]] QString getViewString(const MessageViewItemFields& fields) const;

    /*! @brief This method returns an `QIcon` for the type of @ref Draupnir::Messages::Message object, refered by this @ref
     *         MessageViewItem.  */
    [[nodiscard]] const QIcon& icon() const;

    /*! @brief This method returns an `QIcon` for the provided @ref Draupnir::Messages::MessageType. */
    [[nodiscard]] static const QIcon& iconFor(MessageType type);

private:
    static std::unique_ptr<MessageTypeIconProvider> p_iconProvider;

    MessagePtr p_message;
    mutable QString m_cachedView;
    mutable MessageViewItemFields m_cachedFields;
};

} // namespace Draupnir::Messages

#endif // MESSAGEVIEWITEM_H
