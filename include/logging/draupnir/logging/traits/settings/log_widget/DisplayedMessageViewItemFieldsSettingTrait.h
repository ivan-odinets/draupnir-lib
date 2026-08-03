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

#ifndef DISPLAYEDMESSAGEVIEWITEMFIELDSSETTINGTRAIT_H
#define DISPLAYEDMESSAGEVIEWITEMFIELDSSETTINGTRAIT_H

#include "draupnir/messages/core/MessageViewItemFields.h"

namespace Draupnir::Logging::Settings
{

namespace LogWidget
{

/*! @struct DisplayedMessageViewItemFieldsSettingTrait draupnir/logging/traits/settings/log_widgets/DisplayedMessageViewItemFieldsSettingTrait.h
 *  @ingroup Logging
 *  @brief Setting trait for message fields displayed by a log widget.
 *
 *  @details Stores the @ref Draupnir::Messages::MessageViewItemFields mask that controls which parts of each message are displayed in the
 *           associated log widget.
 *
 *           By default, all available message fields are displayed. */

struct DisplayedMessageViewItemFieldsSettingTrait
{
    /*! @brief Underlying value type. */
    using Value = Draupnir::Messages::MessageViewItemFields;

    /*! @brief Return the persistent key as a `QString`. */
    [[nodiscard]] static QString key() { return QString{"log_widget/message_fields_shown"}; }

    /*! @brief Return the default value. */
    [[nodiscard]] static Draupnir::Messages::MessageViewItemFields defaultValue() {
        return Draupnir::Messages::MessageViewItemFields::All;
    }
};

} // namespace LogWidget

} // namespace Draupnir::Logging::Settings

#endif // DISPLAYEDMESSAGEVIEWITEMFIELDSSETTINGTRAIT_H
