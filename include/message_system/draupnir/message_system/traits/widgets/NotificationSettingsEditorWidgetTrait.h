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

#ifndef NOTIFICATIONSETTINGSEDITORWIDGETTRAIT_H
#define NOTIFICATIONSETTINGSEDITORWIDGETTRAIT_H

#include "draupnir/message_system/ui/widgets/AbstractNotificationSettingsWidget.h"

namespace Draupnir::Messages
{

/*! @struct NotificationSettingsEditorTrait draupnir/message_system/traits/widgets/NotificationSettingsEditorTrait.h
 *  @ingroup MessageSystem
 *  @brief This struct represents a trait of widget capable of editing notification settings for different types of the
 *         @ref Draupnir::Messages::Message objects which are handled by the @ref Draupnir::Messages::AbstractMessageSystem /
 *         @ref Draupnir::Messages::MessageSystemTemplate. */

struct NotificationSettingsEditorTrait
{
    /*! @brief Type of a widget. */
    using Widget = AbstractNotificationSettingsWidget;

    /*! @brief Name of the tab within @ref Draupnir::Ui::FixedTabWidgetTemplate */
    static QString displayName() { return QObject::tr("Notifications"); }
};

}; // namespace Drauopnir::Messages

#endif // NOTIFICATIONSETTINGSEDITORWIDGETTRAIT_H
