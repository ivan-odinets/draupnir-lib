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

#ifndef MESSAGESYSTEMCONFIGDIALOGSETTINGS_H
#define MESSAGESYSTEMCONFIGDIALOGSETTINGS_H

#include <QString>

namespace Draupnir::Messages
{

namespace MessageSystemConfigDialogSettings
{

struct WidgetIndexSetting
{
    /*! @brief Underlying value type. */
    using Value = int;

    /*! @brief Return the persistent storage key ("message_system_config_dialog/active_widget_index"). */
    static QString key() { return "message_system_config_dialog/active_widget_index"; }

    /*! @brief Return the default value - 0. */
    static int defaultValue() { return 0; }
};

}; // namespace MessageSystemConfigDialogSettings

}; // namespace Draupnir::Messages

#endif // MESSAGESYSTEMCONFIGDIALOGSETTINGS_H
