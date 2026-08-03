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

#ifndef MESSAGESYSTEMTYPEBUILDER_H
#define MESSAGESYSTEMTYPEBUILDER_H

#include "draupnir/message_system/core/MessageSystemTemplate.h"

namespace Draupnir::MessageSystem
{

/*! @class MessageSystemTypeBuilder draupnir/message_system/core/MessageSystemTypeBuilder.h
 *  @ingroup MessageSystem
 *  @brief This is a struct. */

class MessageSystemTypeBuilder
{
    template<
        class Categories,
        class MessageTypes,
        class NotificationChannels,
        class MessageIconProvider
        >
    struct MessageSystemTypeBuilderState
    {
        template<Draupnir::Messages::MessageCategoryTraitConcept... NewCategories>
        using withCategories = MessageSystemTypeBuilderState<
            MessageCategoryTraitContainer<NewCategories...>,
            MessageTypes,
            NotificationChannels,
            MessageIconProvider
            >;

        template<Draupnir::Messages::MessageTypeTraitConcept... NewTypes>
        using notifyFor = MessageSystemTypeBuilderState<
            Categories,
            Draupnir::MessageSystem::TemplateArgs::MessageTypeTraitsWrapper<NewTypes...>,
            NotificationChannels,
            MessageIconProvider
        >;

        template<Draupnir::Notifications::NotificationChannelConcept... NewChannels>
        using via = MessageSystemTypeBuilderState<
            Categories,
            MessageTypes,
            Draupnir::MessageSystem::TemplateArgs::NotificationChannelsWrapper<NewChannels...>,
            MessageIconProvider
        >;

        template<class NewMessageIconProvider>
        using withIconProvider = MessageSystemTypeBuilderState<
            Categories,
            MessageTypes,
            NotificationChannels,
            NewMessageIconProvider
        >;

        using type = MessageSystemTemplate<
            Categories,
            MessageNotificationsContainer<
                MessageTypes,
                NotificationChannels
            >,
            MessageIconProvider
        >;
    };

public:
    using start = MessageSystemTypeBuilderState<
        MessageCategoryTraitContainer<>,
        Draupnir::MessageSystem::TemplateArgs::MessageTypeTraitsWrapper<>,
        Draupnir::MessageSystem::TemplateArgs::NotificationChannelsWrapper<>,
        Messages::MessageTypeIconProvider
    >;
};

}; // namespace Draupnir::MessageSystem

#endif // MESSAGESYSTEMTYPEBUILDER_H
