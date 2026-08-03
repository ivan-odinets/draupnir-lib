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

#ifndef MESSAGECATEGORIESSERIALIZERTEMPLATE_H
#define MESSAGECATEGORIESSERIALIZERTEMPLATE_H

#include "draupnir/messages/categories/MessageCategoriesSerializerInterface.h"

#include "draupnir/messages/concepts/MessageCategoryTraitConcept.h"
#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"

namespace Draupnir::Messages
{

/*! @class MessageCategoriesSerializerTemplate draupnir/messages/categories/MessageCategoriesSerializerTemplate.h
 *  @ingroup Messages
 *  @brief Serializer for message category masks defined by a compile-time collection of category traits.
 *  @tparam CategoryTraits Traits describing the supported message categories.
 *
 *  @details Converts @ref Draupnir::Messages::MessageCategories masks between their runtime representation and textual configuration
 *           strings.
 *
 *           Individual category values are serialized using the configuration tokens provided by `CategoryTraits...`. The predefined
 *           @ref Draupnir::Messages::MessageCategories::None and @ref Draupnir::Messages::MessageCategories::All masks are handled
 *           using their dedicated configuration tokens. */

template<MessageCategoryTraitConcept... CategoryTraits>
class MessageCategoriesSerializerTemplate final : public MessageCategoriesSerializerInterface
{
public:
    /*! @brief Serializer used for individual message category values. */
    using SingleCategorySerializer = Draupnir::Settings::FlagSerializerTemplate<MessageCategory, CategoryTraits...>;

    /*! @brief Serializer used for complete message category masks.
     *  @details Combines the individual category serializer with dedicated representations for the empty and complete category masks. */
    using Serializer = Draupnir::Settings::FlagsMaskSerializerTemplate<
        MessageCategories, SingleCategorySerializer,
        Settings::FlagsMaskWrapperTemplate<
            []() { return MessageCategories::None; }, []() { return MessageCategories::noneConfigKey(); }
        >,
        Settings::FlagsMaskWrapperTemplate<
            []() { return MessageCategories::All; }, []() { return MessageCategories::allConfigKey(); }
        >
    >;

    /*! @brief Virtual destructor. */
    ~MessageCategoriesSerializerTemplate() final = default;

    /*! @brief Deserializes a message category mask from a configuration string.
     *  @param configString Configuration string to deserialize.
     *  @return Deserialized message category mask, or `std::nullopt` if the string is invalid or contains unsupported category tokens. */
    [[nodiscard]] std::optional<Draupnir::Messages::MessageCategories> fromConfigString(const QString& configString) const final {
        return Serializer::fromConfigString(configString);
    }

    /*! @brief Serializes a message category mask to its configuration string.
     *  @param mask Message category mask to serialize.
     *  @return Configuration string representing the specified mask. */
    [[nodiscard]] QString toConfigString(Draupnir::Messages::MessageCategories mask) const final {
        return Serializer::toConfigString(mask);
    }
};

} // namespace Draupnir::Messages

#endif // MESSAGECATEGORIESSERIALIZERTEMPLATE_H
