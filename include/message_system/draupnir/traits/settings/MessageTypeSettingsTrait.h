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

#include "draupnir/utils/SettingTraitSerializer.h"
#include "draupnir/core/Notification.h"

namespace Draupnir::Messages
{

/*! @struct MessageTypeSettingsTrait draupnir/traits/settings/MessageTypeSettingsTrait.h
 *  @brief This template allows creation of setting traits for each message type. */

template<class MsgType>
struct MessageTypeSettingsTrait
{
    using Entry = void;
    using Value = Notification::Type;

    /*! @brief Return the persistent key as a QString. */
    static QString key() { return QString{"notifications/"} + QString{MsgType::settingsKey}; }

    /*! @brief Return the compile-time default value. */
    static Value defaultValue() { return MsgType::defaultNotification; }
};

}; // namespace Draupnir::Messages

namespace Draupnir::Settings
{

template<class Backend, class MsgTrait>
class SettingTraitSerializer<Backend,Draupnir::Messages::MessageTypeSettingsTrait<MsgTrait> >
{
public:
    using SettingTrait = Draupnir::Messages::MessageTypeSettingsTrait<MsgTrait>;
    using Value = typename SettingTrait::Value;

    inline static Draupnir::Messages::Notification::Type get(Backend* settings) {
        if (!settings->contains(SettingTrait::key())) {
            return SettingTrait::defaultValue();
        }

        const Draupnir::Messages::Notification::Type value = Draupnir::Messages::Notification::fromConfigString(settings->value(SettingTrait::key()).toString());
        return (value != Draupnir::Messages::Notification::UnknownType) ?
                    value :
                    SettingTrait::defaultValue();
    }

    inline static void set(Backend* backend, const Value& value) {
        backend->setValue(SettingTrait::key(),value);
    }
};

}; // namespace Draupnir::Settings

#endif // MESSAGETYPESETTINGSTRAIT_H
