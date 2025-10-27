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

#ifndef DEFAULTMESSAGETRAITS_H
#define DEFAULTMESSAGETRAITS_H

#include <QIcon>

#include "draupnir/core/MessageType.h"
#include "draupnir/core/Notification.h"

namespace Draupnir::Messages
{

/*! @file DefaultMessageTraits.h draupnir/traits/messages/DefaultMessageTraits.h
 *  @ingroup MessageSystem
 *  @brief Contains default message trait definitions used by the logging and messaging subsystem.
 *
 *  @details This file defines a set of default traits used to classify and style messages in the system. Each trait encapsulates
 *           compile-time metadata such as type, settings keys, and icon rendering, and is used to instantiate strongly-typed
 *           Message objects.
 * @todo Split this file into several files. */

/*! @class DebugMessageTrait draupnir/traits/messages/DefaultMessageTraits.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for debug-level messages.
 *
 *  @details This trait is intended for internal diagnostic messages. */

class DebugMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Debug};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within Draupnir::Messages::MessageTypeSettingsTrait template to this string a notifications/ prefix will
     *       be prepended. Actual storage key: notifications/debug. */
    static constexpr const char* settingsKey = "debug";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Debug"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon();
};

/*! @class InfoMessageTrait draupnir/traits/messages/DefaultMessageTraits.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for informational messages.
 *
 *  @details Messages using this trait are intended to inform the user of successful or neutral operations. */

class InfoMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Info};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within Draupnir::Messages::MessageTypeSettingsTrait template to this string a notifications/ prefix will
     *       be prepended. Actual storage key: notifications/info. */
    static constexpr const char* settingsKey = "info";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Info"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon();
};

/*! @class WarningMessageTrait draupnir/traits/messages/DefaultMessageTraits.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for warning messages.
 *
 *  @details Messages with this trait indicate that something might be wrong, but is not necessarily an error. */

class WarningMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Warning};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within Draupnir::Messages::MessageTypeSettingsTrait template to this string a notifications/ prefix will
     *       be prepended. Actual storage key: notifications/warning. */
    static constexpr const char* settingsKey = "warning";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Warning"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon();
};

/*! @class ErrorMessageTrait draupnir/traits/messages/DefaultMessageTraits.h
 *  @ingroup MessageSystem
 *  @brief Defines metadata for error messages.
 *
 *  @details This trait represents critical issues or failures in the application. */

class ErrorMessageTrait
{
public:
    /*! @brief Unique bit-flag identifier for this trait. */
    static constexpr MessageType type{MessageType::Error};

    /*! @brief Key used for accessing settings related to this message type.
     * @note Within Draupnir::Messages::MessageTypeSettingsTrait template to this string a notifications/ prefix will
     *       be prepended. Actual storage key: notifications/error. */
    static constexpr const char* settingsKey = "error";

    /*! @brief Default notification level associated with this message type. */
    static constexpr Notification::Type defaultNotification = Notification::None;

    /*! @brief Display name used in GUI elements. */
    static QString displayName() { return QObject::tr("Error"); }

    /*! @brief Returns the icon associated with this message type. */
    static const QIcon& icon();
};

}; // namespace Draupnir::Messages

#endif // DEFAULTMESSAGETRAITS_H
