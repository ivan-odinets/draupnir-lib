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

#ifndef MESSAGECATEGORIES_H
#define MESSAGECATEGORIES_H

#include <QMetaType>

#include "draupnir/utils/integer_wrapper.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Logging
{

/*! @class MessageCategory draupnir/logging/messages/MessageCategories.h
 *  @ingroup Logging
 *  @brief Represents a log message category identifier.
 *
 *  @details Message categories are used in combination with @ref Draupnir::Logging::MessageLevel to form a complete @ref
 *           Draupnir::Logging::MessageType. Categories are represented as bit-like integer identifiers, allowing them
 *           to be combined into category masks where needed. */

class MessageCategory : public draupnir::utils::integer_wrapper<quint64, MessageCategory>
{
    using _Base = draupnir::utils::integer_wrapper<quint64, MessageCategory>;

public:
    /*! @enum MessageCategory::Value
     *  @brief Built-in message category values. */
    enum Value {
        /*! @brief Default message category used when no custom category is specified. */
        Default = 0b1,
        /*! @brief First value reserved for user-defined message categories. */
        FirstCustomCategory = 0b10,        
    };

    using draupnir::utils::integer_wrapper<quint64, MessageCategory>::integer_wrapper;

    using draupnir::utils::integer_wrapper<quint64, MessageCategory>::operator=;

    /*! @brief Returns the next available message category ID.
     *  @param prevType The previous MessageCategory.
     *  @return A new MessageCategory with the value shifted left by 1. */
    static constexpr MessageCategory nextType(MessageCategory::integer_wrapper prevType) {
        return MessageCategory{prevType << 1};
    }
};

class MessageCategories final : public draupnir::utils::flags<MessageCategory>
{
    using _Base = draupnir::utils::flags<MessageCategory>;

public:
    using _Base::flags;

    using _Base::operator=;

    static constexpr _Base::integer None = 0;

    static constexpr _Base::integer All =
        std::numeric_limits<MessageCategory::underlying_type>::max();
};

} // Draupnir::Logging

Q_DECLARE_METATYPE(Draupnir::Logging::MessageCategory);
Q_DECLARE_METATYPE(Draupnir::Logging::MessageCategories);

#endif // MESSAGECATEGORIES_H
