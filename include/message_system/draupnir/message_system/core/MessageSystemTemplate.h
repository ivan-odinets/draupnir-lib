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

#ifndef MESSAGESYSTEMTEMPLATE_H
#define MESSAGESYSTEMTEMPLATE_H

#include "draupnir/message_system/core/AbstractMessageSystem.h"
#include "draupnir/message_system/traits/messages/DefaultMessageTraits.h"
#include "draupnir/message_system/utils/MessageTraitsHelper.h"
#include "draupnir/message_system/traits/settings/MessageTypeSettingsTrait.h"

#include "draupnir/settings_registry/SettingsBundleTemplate.h"
#include "draupnir/settings_registry/SettingsBundleMerge.h"

namespace Draupnir::Messages
{

/*! @class MessageSystemTemplate draupnir/message_system/core/MessageSystemTemplate.h
 *  @brief Template-based class that binds together components of @ref MessageSystem.
 *  @tparam MessageTraits Variadic list of message traits defining custom message types.
 *
 *  @details This class provides compile-time registration of message types via variadic template parameters. It serves as the
 *           central access point for both message processing (@ref Draupnir::Messages::AbstractMessageHandler and @ref
 *           Draupnir::Messages::MessageHandlerTemplate) and message GUI (@ref Draupnir::Messages::AbstractMessageUiBuilder and
 *           @ref Draupnir::Messages::MessageUiBuilderTemplate).
 *
 *           Provided MessageTraits... are passed down to the nested objects which includes:
 *           - @ref Draupnir::Messages::MessageHandlerTemplate - stores notification policies and handles messages being logged
 *             through @ref Logger.
 *           - @ref Draupnir::Messages::MessageUiBuilderTemplate - creates widgets related to @ref MessageSystem and configures
 *             them.
 *
 *           In addition to user-specified `MessageTraits...` there are the following default message types present:
 *           - Debug (@ref Draupnir::Messages::DebugMessageTrait);
 *           - Info (@ref Draupnir::Messages::InfoMessageTrait);
 *           - Warning (@ref Draupnir::Messages::WarningMessageTrait);
 *           - Error (@ref Draupnir::Messages::ErrorMessageTrait);
 *
 *           To be registered as a `MessageTrait` custom message trait type must have the following:
 *           - `static constexpr @ref Draupnir::Messages::MessageType type` - member with unique @ref Draupnir::Messages::MessageType
 *             representing this message type. This value should be a 64-bit integer with only one bit set (flag). The simplest way
 *             to obtain these values is to use MessageType::nextType method.
 *           - `static constexpr const char* settingsKey` - member with C-style string explaining under what name this message type
 *             should be stored within settings;
 *           - `static constexpr @ref Draupnir::Messages::Notification::Type defaultNotification` - value representing how messages
 *             of this type will be displayed to user.
 *           - `static QString displayName()` - method returning the `QString` with user-friendly representation of this message type.
 *           - `static const QIcon& icon()` - method returning const reference to the `QIcon` to be used while displaying messages of
 *             this type to user. Can return reference to the empty icon.
 *
 * @todo Maybe add some kind of macro-based configuration if default message traits should be enabled? */

template<class... MessageTraits>
class MessageSystemTemplate final : public AbstractMessageSystem
{
    using SpecificMessageTraitsHelper = MessageTraitsHelper<
        DebugMessageTrait,
        InfoMessageTrait,
        WarningMessageTrait,
        ErrorMessageTrait,
        MessageTraits...
    >;
public:
    using MessageHandler = typename SpecificMessageTraitsHelper::MessageHandler;

    using MessageUiBuilder = typename SpecificMessageTraitsHelper::MessageUiBuilder;

    using SettingsBundle = Draupnir::Settings::bundle_merge_all_t<
        typename MessageHandler::SettingsBundle,
        typename MessageUiBuilder::SettingsBundle
    >;

    /*! @brief Default constructor. Configures @ref Draupnir::Messages::MessageUiBuilderTemplate.
     *  @details A static_assert will happen within this constructor if:
     *           - Classes within provided `MessageTraits...` have duplicated IDs;
     *           - These `MessageTraits...` have IDs where multiple bits are set. */
    MessageSystemTemplate() {
        static_assert(SpecificMessageTraitsHelper::hasUniqueIds(),
                "MessageTraits... parameter pack has non-unique IDs or these IDs are overlapping with ones for the default types.");

        static_assert(SpecificMessageTraitsHelper::hasFlags(),
                "MessageTraits... parameter pack has non-flag IDs");

        m_uiBuilder.setMessageHandlerTemplate(&m_handler);
    }

    /*! @brief Loads settings for this MessageSystemTemplate instantiation from the `SettingsSource` privided. */
    template<class SettingsSource>
    void loadSettings(SettingsSource* registry) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>(),
                "SettingsSource specified can not populate the SettingsBundle within this MessageSystemTemplate instantiation.");
        Q_ASSERT_X(registry, "MessageSystemTemolate<MessageTraits...>::loadSettings",
                   "SettingsSource pointer provided is nullptr.");

        m_handler.template loadSettings<SettingsSource>(registry);

        m_uiBuilder.template loadSettings<SettingsSource>(registry);
    }

    /*! @brief Returns pointer to @ref Draupnir::Messages::MessageHandlerTemplate object in form of pointer to the @ref
     *         Draupnir::Messages::MessageHandlerInterface. */
    AbstractMessageHandler* handlerInterface() { return &m_handler; }

    /*! @brief Returns pointer to @ref Draupnir::Messages::MessageHandlerTemplate object used within this instantiation of
     *         MessageSystemTemplate. */
    MessageHandler* handler() { return &m_handler; }

    /*! @brief Returns pointer to @ref Draupnir::Messages::MessageUiBuilderTemplate object in form of pointer to the @ref
     *         Draupnir::Messages::AbstractMessageUiBuilder. */
    AbstractMessageUiBuilder* uiBuilderInterface() { return &m_uiBuilder; }

    /*! @brief Returns pointer to @ref Draupnir::Messages::MessageUiBuilderTemplate object used within this instantiation
     *         of the MessageSystemTemplate. */
    MessageUiBuilder* uiBuilder() { return &m_uiBuilder; }

    /*! @brief Returns `true` if provided `MessageType` is known to this instantiation of the MessageSystemTemplate */
    bool isTypeKnown(MessageType type) { return staticIsTypeKnown(type); }

    /*! @brief Returns `true` if provided `MessageType` is known to this instantiation of the MessageSystemTemplate */
    static bool staticIsTypeKnown(MessageType type) {
        return SpecificMessageTraitsHelper::isTypeKnown(type);
    }

private:
    SettingsBundle m_settings;
    MessageHandler m_handler;
    MessageUiBuilder m_uiBuilder;
};

}; // namespace Draupnir::Messages

#endif // MESSAGESYSTEMTEMPLATE_H
