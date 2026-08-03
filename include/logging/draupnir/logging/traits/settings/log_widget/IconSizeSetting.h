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

#ifndef ICONSIZESETTING_H
#define ICONSIZESETTING_H

#include <QSize>
#include <QString>

namespace Draupnir::Logging::Settings
{

namespace LogWidget
{

/*! @struct IconSizeSettingTrait draupnir/logging/traits/settings/log_widgets/IconSizeSettingTrait.h
 *  @ingroup Logging
 *  @brief Setting trait for the size of icons within @ref Draupnir::Logging::LogWidget (displaying of the logged @ref
 *         Draupnir::Logging::Message objects is handled by the @ref Draupnir::Logging::MessageListView). */

struct IconSizeSettingTrait
{
    /*! @brief Underlying value type. */
    using Value = QSize;

    /*! @brief Return the persistent key as a `QString`. */
    [[nodiscard]] static QString key() { return QString{"log_widget/message_icon_size"}; }

    /*! @brief Return the default value. */
    [[nodiscard]] static QSize defaultValue() { return QSize{64,64}; }
};

} // namespace LogWidget

} // namespace Draupnir::Logging::Settings

#endif // ICONSIZESETTING_H
