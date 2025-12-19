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


#ifndef ABSTRACTMESSAGESYSTEM_H
#define ABSTRACTMESSAGESYSTEM_H

#include "draupnir/message_system/core/MessageType.h"

namespace Draupnir::Messages
{

class AbstractMessageHandler;
class AbstractMessageUiBuilder;
class LogWidget;
class NotificationTypeMenu;

/*! @class AbstractMessageSystem
 *  @ingroup MessageSystem
 *  @brief Abstract interface providing unified access to the logic and UI components of the @ref Draupnir::Messages::MessageSystemTemplate.
 *
 *  @details This class defines the API that is implemented by the @ref Draupnir::Messages::MessageSystemTemplate. It acts as a façade
 *           through which client code may interact with the message-processing logic and UI-building subsystem.
 *
 *           Concrete implementation of this interface is @ref Draupnir::Messages::MessageSystemTemplate, which performs compile‑time
 *           registration of message types and binds together: a message handler (@ref Draupnir::Messages::AbstractMessageHandler / @ref
 *           Draupnir::Messages::MessageHandlerTemplate), and a UI builder (@ref Draupnir::Messages::AbstractMessageUiBuilder / @ref
 *           Draupnir::Messages::MessageUiBuilderTemplate).
 *
 *           The interface provides:
 *           - @ref AbstractMessageSystem::handlerInterface() — access to the message-processing backend,
 *           - @ref AbstractMessageSystem::uiBuilderInterface() — access to the GUI-construction utilities,
 *           - @ref isTypeKnown() — runtime check whether a message type is supported by the current @ref Draupnir::Messages::MessageSystemTemplate
 *             instantiation.
 *
 *           This abstraction allows client code (UI, logging, tools) to work with any MessageSystem implementation without depending on
 *           template parameters.*/

class AbstractMessageSystem
{
public:
    /*! @brief Returns pointer to the message handler as a @ref Draupnir::Messages::AbstractMessageHandler. */
    virtual AbstractMessageHandler* handlerInterface() = 0;

    /*! @brief Returns pointer to the ui builder as a @ref Draupnir::Messages::AbstractMessageUiBuilder. */
    virtual AbstractMessageUiBuilder* uiBuilderInterface() = 0;

    /*! @brief Checks whether the specified message type is supported by this MessageSystem.
     *  @param type The @ref Draupnir::Messages::MessageType to check.
     *  @return `true` if this message system recognizes the type; otherwise `false`. */
    virtual bool isTypeKnown(MessageType type) = 0;

    /*! @brief Configures specified @ref Draupnir::Messages::LogWidget instance to be used within this @ref AbstractMessageSystem. */
    virtual void configureLogWidget(LogWidget* logWidget) = 0;
};

}; // namespace Draupnir::Messages

#endif // ABSTRACTMESSAGESYSTEM_H
