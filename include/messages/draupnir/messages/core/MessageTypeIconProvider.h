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

#ifndef MESSAGETYPEICONPROVIDER_H
#define MESSAGETYPEICONPROVIDER_H

#include <QIcon>

#include "draupnir/messages/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageTypeIconProvider draupnir/messages/core/MessageTypeIconProvider.h
 *  @ingroup Messages
 *  @brief Extensible provider for resolving icons associated with message types.
 *  @details Provides default icons based on the message level. Derived classes may override @ref getIcon to provide specific
 *           icons for individual message types.
 *
 * @warning A `QApplication` instance must exist before default Qt style icons are requested.
 *
 * @todo Documentation: Write a page explaining how and when to inherit from this class. */

class MessageTypeIconProvider
{
public:
    MessageTypeIconProvider() = default;
    virtual ~MessageTypeIconProvider() = default;

    /*! @brief Returns the icon associated with a message type.
     *  @param messageType Message type whose icon should be resolved.
     *  @return Reference to the corresponding icon. Returns an empty icon when no icon is available.
     *  @details The default implementation selects an icon according to the message level. Derived classes may override
     *           this method to provide custom message-type-specific icons. */
    [[nodiscard]] virtual const QIcon& getIcon(const MessageType& messageType) const;

private:
    /*! @brief Returns the shared empty icon.
     *  @return Reference to an empty `QIcon` instance. */
    static const QIcon& _noIcon();

    /*! @brief Returns the default icon for debug messages.
     *  @return Reference to the default debug icon. */
    static const QIcon& _defaultDebugIcon();

    /*! @brief Returns the default icon for informational messages.
     *  @return Reference to the default information icon. */
    static const QIcon& _defaultInfoIcon();

    /*! @brief Returns the default icon for warning messages.
     *  @return Reference to the default warning icon. */
    static const QIcon& _defaultWarningIcon();

    /*! @brief Returns the default icon for error messages.
     *  @return Reference to the default error icon. */
    static const QIcon& _defaultErrorIcon();
};

} // namespace Draupnir::Messages

#endif // MESSAGETYPEICONPROVIDER_H
