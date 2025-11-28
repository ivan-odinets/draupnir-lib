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

#ifndef LOGWIDGETSETTINGSTRAITS_H
#define LOGWIDGETSETTINGSTRAITS_H

#include "draupnir/message_system/core/Message.h"
#include "draupnir/message_system/core/MessageType.h"

/*! @file draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h
 *  @ingroup MessageSystem
 *  @brief This file contains setting traits for the @ref Draupnir::Messages::LogWidget. */

/*! @namespace Draupnir::Messages::Settings
 *  @ingroup MessageSystem
 *  @brief Namespace for settings-related things within the @ref MessageSystem module. */

namespace Draupnir::Messages::Settings
{

/*! @namespace Draupnir::Messages::Settings::LogWidget
 *  @ingroup MessageSystem
 *  @brief Namespace for settings traits of the @ref Draupnir::Messages::LogWidget widget. */

namespace LogWidget
{

/*! @struct IconSize draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h
 *  @ingroup MessageSystem
 *  @brief Setting trait for the size of icons within @ref Draupnir::Messages::LogWidget (displaying of the logged @ref
 *         Draupnir::Messages::Message objects is handled by the @ref Draupnir::Messages::MessageListView). */

struct IconSizeSetting
{
    /*! @brief Underlying value type. */
    using Value = QSize;

    /*! @brief Return the persistent key as a `QString`. */
    static QString key() { return QString{"log_widget/message_icon_size"}; }

    /*! @brief Return the default value. */
    static QSize defaultValue() { return QSize{64,64}; }
};

/*! @struct DisplayedMessageTypes draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h
 *  @ingroup MessageSystem
 *  @brief Setting trait for the displayed message types setting of the @ref Draupnir::Messages::LogWidget (displaying
 *         of the logged @ref Draupnir::Messages::Message objects is handled by the @ref Draupnir::Messages::MessageListView). */

struct DisplayedMessageTypesSetting
{
    /*! @brief Underlying value type. */
    using Value = uint64_t;

    /*! @brief Return the persistent key as a `QString`. */
    static QString key() { return QString{"log_widget/messages_shown"}; }

    /*! @brief Return the default value. */
    static auto defaultValue() { return MessageType::AllMessages; }
};

/*! @struct DisplayedMessageFields draupnir/message_system/traits/settings/LogWidgetSettingsTraits.h
 *  @ingroup MessageSystem
 *  @brief Setting trait for the fields of @ref Draupnir::Messages::Message objects which will be displayed to an end user
 *         within the @ref Draupnir::Messages::LogWidget (displaying of the logged @ref Draupnir::Messages::Message objects
 *         is handled by the @ref Draupnir::Messages::MessageListView).*/

struct DisplayedMessageFieldsSetting
{
    /*! @brief Underlying value type. */
    using Value = std::underlying_type_t<Message::Fields>;

    /*! @brief Return the persistent key as a `QString`. */
    static QString key() { return QString{"log_widget/meddsge_fields_shown"}; }

    /*! @brief Return the default value. */
    static auto defaultValue() { return Message::Fields::All; }
};

}; // namespace MessageListView

}; // namespace Draupnir::Messages::Settings

#endif // LOGWIDGETSETTINGSTRAITS_H
