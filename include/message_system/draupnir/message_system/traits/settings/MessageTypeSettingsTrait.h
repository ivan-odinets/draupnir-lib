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

#ifndef MESSAGETYPESETTINGSTRAIT_H
#define MESSAGETYPESETTINGSTRAIT_H

#include "draupnir/settings_registry/utils/SettingTraitSerializer.h" // IWYU pragma: keep
#include "draupnir/message_system/core/Notification.h"

namespace Draupnir::Messages
{

/*! @struct MessageTypeSettingsTrait draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h
 *  @ingroup MessageSystem
 *  @brief This is a template representing setting trait for individual message trait. */

template<class MessageTrait>
struct MessageTypeSettingsTrait
{
    using Value = Notification::Type;

    /*! @brief Return the persistent key as a `QString`.
     * @note This method prepends notifications/ prefix to the value returned by the MessageTrait::settingsKey variable. */
    static QString key() { return QString{"notifications/"} + QString{MessageTrait::settingsKey}; }

    /*! @brief Return the compile-time default value. */
    static Value defaultValue() { return MessageTrait::defaultNotification; }
};

}; // namespace Draupnir::Messages

namespace Draupnir::Settings
{

/*! @class SettingTraitSerializer<Backend, Draupnir::Messages::MessageTypeSettingsTrait<MsgTrait>>
 *  @ingroup MessageSystem
 *  @brief Specialized serializer for message-type setting traits.
 *  @tparam Backend   The settings backend type (e.g. `QSettings`, or @ref Draupnir::Settings::AppSettings).
 *  @tparam MsgTrait  The message-trait type (e.g. @ref Draupnir::Messages::DebugMessageTrait, @ref Draupnir::Messages::InfoMessageTrait).
 *
 *  @details This specialization of `SettingTraitSerializer` defines how settings of message notifications represented by
 *           `Draupnir::Messages::MessageTypeSettingsTrait<MsgTrait>` are read from and written to a specific backend (such
 *           as `QSettings` or @ref Draupnir::Settings::AppSettings).
 *
 *           The serializer performs conversion between the backend-stored string representation and the strongly-typed
 *           `Draupnir::Messages::Notification::Type` enum used in the message system.
 *
 *           ### Behavior
 *           - get() — retrieves a value from the backend using the trait key. If the key does not exist, returns
 *             `SettingTrait::defaultValue()`.
 *           - If the stored string cannot be parsed to a valid message type, also returns the default value.
 *           - set() — writes the message type to the backend as-is.
 *
 * @todo Split this entity into seperate file. */

template<class Backend, class MsgTrait>
class SettingTraitSerializer<Backend,Draupnir::Messages::MessageTypeSettingsTrait<MsgTrait> >
{
public:
    using SettingTrait = Draupnir::Messages::MessageTypeSettingsTrait<MsgTrait>;
    using Value = typename SettingTrait::Value;

    /*! @brief Retrieves a message-type value from the backend.
     *  @param settings Pointer to the settings backend.
     *  @return Parsed `Draupnir::Messages::Notification::Type` value.
     *  @details If the key is not present in the backend, the default value is returned. If the stored string cannot be
     *           converted to a valid message type, the default value is returned instead. */
    inline static Draupnir::Messages::Notification::Type get(Backend* settings) {
        if (!settings->contains(SettingTrait::key())) {
            return SettingTrait::defaultValue();
        }

        const Draupnir::Messages::Notification::Type value =
                Draupnir::Messages::Notification::fromConfigString(settings->value(SettingTrait::key()).toString());
        return (value != Draupnir::Messages::Notification::UnknownType) ?
                    value :
                    SettingTrait::defaultValue();
    }

    /*! @brief Persists a message-type value into the backend.
     *  @param backend Pointer to the settings backend.
     *  @param value `Draupnir::Messages::Notification::Type` value to store. */
    inline static void set(Backend* backend, const Value& value) {
        backend->setValue(SettingTrait::key(),Draupnir::Messages::Notification::toConfigString(value));
    }
};

}; // namespace Draupnir::Settings

#endif // MESSAGETYPESETTINGSTRAIT_H
