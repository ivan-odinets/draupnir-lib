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

#include "MessageSystemInterface.h"

#include "draupnir/traits/messages/DefaultMessageTraits.h"
#include "draupnir/utils/MessageTraitsHelper.h"
#include "draupnir/traits/settings/MessageTypeSettingsTrait.h"

#include "draupnir/SettingsBundleTemplate.h"
#include "draupnir/SettingsBundleMerge.h"

namespace Draupnir::Messages
{

/*! @class MessageSystemTemplate draupnir/MessageSystemTemplate.h
 *  @brief Template-based implementation of the MessageSystemInterface that defines message types at compile-time.
 *  @tparam MessageTraits Variadic list of message traits defining custom message types.
 *
 *  @details This class provides compile-time registration of message types via variadic template parameters. It serves as the
 *           central access point for both message processing (MessageHandler and MessageHandlerTemplate) and message GUI
 *           (MessageUiBuilder and MessageUiBuilderTemplate).
 *
 *           Provided MessageTraits... are passed down to the MessageHandlerTemplate and MessageUiBuilderTemplate, which operate on the
 *           same set of types. `MessageHandlerTemplate` stores notification policies while `MessageUiBuilderTemplate` creates widgets
 *           to view and configure them. This class wires both together so that the `Logger` can send messages through the handler and
 *           the resulting UI stays synchronized.
 *
 *           In addition to user-specified MessageTraits this class has the following default message types:
 *           - Debug (DebugMessageTrait);
 *           - Info (InfoMessageTrait);
 *           - Warning (WarningMessageTrait);
 *           - Error (ErrorMessageTrait);
 *
 *           To be registered as a MessageTrait custom message class must have the following:
 *           - static constexpr MessageType type - member with unique MessageType representing this message type. This value should be
 *             a 64-bit integer with only one bit set (flag). The simplest way to obtain these values is to use MessageType::nextType
 *             method.
 *           - static constexpr const char* settingsKey - member with C-style string explaining under what name this message type should
 *             be stored within settings;
 *           - static constexpr Notification::Type defaultNotification - value from enum Notification::Type
 *           - static QString displayName() - method returning the QString with user-friendly representation of this message type.
 *           - static const QIcon& icon() - method returning const reference to the QIcon to be used while displaying messages of this
 *             type to user. Can return reference to the empty icon. */

template<class... MessageTraits>
class MessageSystemTemplate final : public Draupnir::Messages::MessageSystemInterface
{
    using SpecificMessageTraitsHelper = MessageTraitsHelper<
        DebugMessageTrait,
        InfoMessageTrait,
        WarningMessageTrait,
        ErrorMessageTrait,
        MessageTraits...
    >;

    using MessageUiBuilder = typename SpecificMessageTraitsHelper::MessageUiBuilder;

public:
    using MessageHandler = typename SpecificMessageTraitsHelper::MessageHandler;

    using SettingsBundle = Draupnir::Settings::bundle_merge_all_t<
        typename MessageHandler::SettingsBundle,
        typename MessageUiBuilder::SettingsBundle
    >;

    /*! @brief Default constructor. Configures MessageUiBuilderTemplate.
     *  @details A static_assert will happen within this constructor if:
     *           - Classes within provided MessageTraits... and predefined MessageTraits (e.g. DebugMessageTrait) have duplicated IDs;
     *           - These MessageTraits... have IDs where multiple bits are set. */
    MessageSystemTemplate() {
        static_assert(SpecificMessageTraitsHelper::hasUniqueIds(),
                "MessageTraits... parameter pack has non-unique IDs or these IDs are overlapping with ones for the default types.");

        static_assert(SpecificMessageTraitsHelper::hasFlags(),
                "MessageTraits... parameter pack has non-flag IDs");

        m_uiBuilder.setMessageHandlerTemplate(&m_handler);
    }

    template<class SettingsRegistry>
    void loadSettings(SettingsRegistry* registry) {
        Q_ASSERT_X(registry, "MessageSystemTemolate<MessageTraits...>::loadSettings",
                   "SettingsRegistry pointer provided is nullptr.");

        m_handler.template loadSettings<SettingsRegistry>(registry);

        m_uiBuilder.template loadSettings<SettingsRegistry>(registry);
    }

    /*! @brief Returns pointer to MessageHandlerTemplate object in form of pointer to the MessageHandler interface. */
    MessageHandler* handler() final { return &m_handler; }

    /*! @brief Returns pointer to MessageUiBuilderTemplate object in form of pointer to the MessageUiBuilder interface. */
    MessageUiBuilder* uiBuilder() final { return &m_uiBuilder; }

    /*! @brief Returns true if provided MessageType is known to this MessageSystem. */
    bool isTypeKnown(MessageType type) final { return staticIsTypeKnown(type); }

    /*! @brief Returns true if provided type is known to this MessageSystem. */
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
