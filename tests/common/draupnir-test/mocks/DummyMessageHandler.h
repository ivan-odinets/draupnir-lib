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

#ifndef DUMMYMESSAGEHANDLER_H
#define DUMMYMESSAGEHANDLER_H

#include "draupnir/core/AbstractMessageHandler.h"

#include "draupnir/containers/fixed_map.h"

template<class... MessageTraits>
class DummyMessageHandler final : public Draupnir::Messages::AbstractMessageHandler
{
public:
    DummyMessageHandler() {
        if constexpr (sizeof...(MessageTraits) > 0) {
            initialize<MessageTraits...>();
        }
    }

    void setNotification(Draupnir::Messages::MessageType type, const Draupnir::Messages::Notification::Type notificationType) final {
        if constexpr (sizeof...(MessageTraits) > 0)
            m_dummyMap[type] = notificationType;
    }

    Draupnir::Messages::Notification::Type notification(uint64_t type) final {
        if constexpr (sizeof...(MessageTraits) > 0)
            return m_dummyMap[type];
        else
            return Draupnir::Messages::Notification::None;
    }

private:
    static constexpr Draupnir::Messages::MessageType types[] = { MessageTraits::type... };

    using MapOrInt = std::conditional_t<(sizeof...(MessageTraits) > 0),
        draupnir::containers::fixed_map<types,Draupnir::Messages::Notification::Type>,
        int>;
    MapOrInt m_dummyMap;

    template<class First, class... Rest, const bool isEnabled = (sizeof...(MessageTraits) > 0)>
    std::enable_if_t<isEnabled, void> initialize() {
        m_dummyMap[First::type] = First::defaultNotification;
        if constexpr(sizeof...(Rest) > 0)
            initialize<Rest...>();
    }
};

#endif // DUMMYMESSAGEHANDLER_H
