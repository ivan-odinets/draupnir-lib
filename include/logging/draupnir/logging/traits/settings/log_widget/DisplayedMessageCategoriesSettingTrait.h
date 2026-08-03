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

#ifndef DISPLAYEDMESSAGECATEGORIESSETTINGTRAIT_H
#define DISPLAYEDMESSAGECATEGORIESSETTINGTRAIT_H

#include "draupnir/messages/categories/MessageCategories.h"

namespace Draupnir::Logging::Settings
{

namespace LogWidget
{

/*! @struct DisplayedMessageCategoriesSettingTrait draupnir/logging/traits/settings/log_widgets/DisplayedMessageCategoriesSettingTrait.h
 *  @ingroup Logging
 *  @brief Setting trait for message categories displayed by a log widget.
 *
 *  @details Stores the @ref Draupnir::Messages::MessageCategories mask used to filter messages displayed by the associated log widget.
 *
 *           By default, messages from all categories are displayed. */

struct DisplayedMessageCategoriesSettingTrait
{
    /*! @brief Message category mask stored by this setting. */
    using Value = Draupnir::Messages::MessageCategories;

    /*! @brief Returns the persistent configuration key.
     *  @return Configuration key used to store the displayed category mask. */
    [[nodiscard]] static QString key() { return QString{"log_widget/message_categories_shown"}; }

    /*! @brief Returns the default displayed category mask.
     *  @return Mask representing all message categories. */
    [[nodiscard]] static Draupnir::Messages::MessageCategories defaultValue() {
        return Draupnir::Messages::MessageCategories::All;
    }
};

} // namespace LogWidget

} // namespace Draupnir::Logging::Settings

#endif // DISPLAYEDMESSAGECATEGORIESSETTINGTRAIT_H
