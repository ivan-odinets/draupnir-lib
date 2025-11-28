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

#ifndef MESSAGETYPESSELECTORTRAIT_H
#define MESSAGETYPESSELECTORTRAIT_H

#include "draupnir/message_system/ui/widgets/AbstractMessageTypesSelectorWidget.h"

namespace Draupnir::Messages
{

/*! @struct MessageTypesSelectorTrait draupnir/message_system/traits/widets/MessageTypesSelectorTrait.h
 *  @ingroup MessageSystem
 *  @brief This struct represents a trait of widget capable of editing what types of the @ref Draupnir::Messages::Message objects
 *         will be displayed within the @ref Draupnir::Messages::LogWidget. */

struct MessageTypesSelectorTrait
{
    /*! @brief Type of a widget. */
    using Widget = AbstractMessageTypesSelectorWidget;

    /*! @brief Name of the tab within @ref Draupnir::Ui::FixedTabWidgetTemplate */
    static QString displayName() { return QObject::tr("Displayed Messages"); }
};

}; // namespace Draupnir::Messages

#endif // MESSAGETYPESSELECTORTRAIT_H
