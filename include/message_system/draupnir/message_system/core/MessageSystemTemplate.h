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

#ifndef MESSAGESYSTEMTEMPLATE_H
#define MESSAGESYSTEMTEMPLATE_H

#include "draupnir/message_system/core/MessageHandlerTemplate.h"
#include "draupnir/logging/core/Logger.h"
#include "draupnir/messages/concepts/MessageCategoryTraitConcept.h"
#include "draupnir/messages/core/MessageTypeIconProvider.h"
#include "draupnir/logging/ui/widgets/LogWidget.h"
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/messages/utils/MessageCategoriesSerializer.h"
#include "draupnir/notifications/utils/NotificationTypesSerializer.h"
#include "draupnir/notifications/core/NotificationTypesSerializerTemplate.h"
#include "draupnir/messages/core/MessageViewItem.h"

namespace Draupnir::MessageSystem
{

/*! @brief Type-pack wrapper for explicit categoriy trait types.
 *  @ingroup MessageSystem
 *  @tparam Traits Trait types describing displayed MessagaCategories entries. */

template<class... Traits>
struct CategoriesTraitsWrapper { DEFINE_COMPILE_TIME(CategoriesTraitsWrapper); };

template<Draupnir::Messages::MessageCategoryTraitConcept... MessageCategories>
class MessageCategoryTraitContainer {};

template<class,class>
class MessageNotificationsContainer;

template<
    Draupnir::Messages::MessageTypeTraitConcept... MessageTypes,
    Draupnir::Notifications::NotificationChannelConcept... NotificationTypes
>
class MessageNotificationsContainer<
    Draupnir::MessageSystem::TemplateArgs::MessageTypeTraitsWrapper<MessageTypes...>,
    Draupnir::MessageSystem::TemplateArgs::NotificationChannelsWrapper<NotificationTypes...>
> {};

template<class,class,class>
class MessageSystemTemplate;

template<
    Draupnir::Messages::MessageCategoryTraitConcept... MessageCategories,
    Draupnir::Messages::MessageTypeTraitConcept... MessageTypes,
    Draupnir::Notifications::NotificationChannelConcept... NotificationTypes,
    class MessageIconProvider/* = Draupnir::Messages::AbstractMessageViewIconProvider*/
>
class MessageSystemTemplate<
    MessageCategoryTraitContainer<MessageCategories...>,
    MessageNotificationsContainer<
        Draupnir::MessageSystem::TemplateArgs::MessageTypeTraitsWrapper<MessageTypes...>,
        Draupnir::MessageSystem::TemplateArgs::NotificationChannelsWrapper<NotificationTypes...>
    >,
    MessageIconProvider
>
{
    using NotificationTypesSerializer = Draupnir::Notifications::NotificationTypesSerializerTemplate<
        typename NotificationTypes::Trait...
    >;

    using MessageHandler = MessageHandlerTemplate<
        Draupnir::MessageSystem::TemplateArgs::MessageTypeTraitsWrapper<MessageTypes...>,
        Draupnir::MessageSystem::TemplateArgs::NotificationChannelsWrapper<NotificationTypes...>
    >;

public:
    using SettingsBundle = Draupnir::Settings::SettingsTraitsConcatenator<
        typename MessageHandler::SettingsBundle,
        typename Draupnir::Logging::LogWidget::SettingsBundle
    >::toSettingsBundle;

    MessageSystemTemplate() {
        Settings::NotificationTypesSerializer::registerImplementation(new NotificationTypesSerializer);
        Messages::MessageViewItem::registerIconProvider(new MessageIconProvider);
        Draupnir::Logging::Logger::get().setMessageReceiver(handler());
    }

    template<class SettingsSource>
    void loadSettings(SettingsSource* source) {
        m_messageHandler.template loadSettings<SettingsSource>(source);
    }

    [[nodiscard]] MessageHandler* handler() { return &m_messageHandler; }

private:
    MessageHandler m_messageHandler;
};

}; // Draupnir::MessageSystem

#endif // MESSAGESYSTEMTEMPLATE_H
