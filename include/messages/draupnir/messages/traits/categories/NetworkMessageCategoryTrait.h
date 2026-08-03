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

#ifndef NETWORKMESSAGECATEGORYTRAIT_H
#define NETWORKMESSAGECATEGORYTRAIT_H

#include <QObject>

#include "draupnir/messages/categories/MessageCategories.h"

namespace Draupnir::Messages
{

/*! @class NetworkMessageCategoryTrait draupnir/messages/traits/categories/NetworkMessageCategoryTrait.h
 *  @ingroup Messages
 *  @brief Trait describing the network message category.
 *
 *  @details Provides compile-time metadata for the network message category, including its identifier, configuration
 *           key, and translatable display name. */

class NetworkMessageCategoryTrait
{
public:
    /*! @brief Stable category identifier. */
    static constexpr MessageCategory value() { return MessageCategory::Network; }

    /*! @brief Returns the stable configuration key for this category. */
    static QLatin1String configString() { return QLatin1String{"network"}; }

    /*! @brief Returns the user-facing translated display name. */
    static QString displayName() { return QObject::tr("Network"); }
};

} // namespace Draupnir::Logging

#endif // NETWORKMESSAGECATEGORYTRAIT_H
