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

#ifndef MESSAGECATEGORIESSERIALIZER_H
#define MESSAGECATEGORIESSERIALIZER_H

#include "draupnir/settings_registry/utils/ValueSerializerTemplate.h"

#include "draupnir/messages/categories/MessageCategoriesSerializerInterface.h"

namespace Draupnir::Settings
{

/*! @class ValueSerializerTemplate
 *  @headerfile draupnir/messages/utils/MessageCategoriesSerializer.h
 *  @ingroup Messages
 *  @brief Specialization for serializing message category masks.
 *
 *  @details Converts @ref Draupnir::Messages::MessageCategories values between their runtime representation and a textual configuration
 *           representation.
 *
 *           The actual conversion is delegated to a registered @ref Draupnir::Messages::MessageCategoriesSerializerInterface implementation.
 *           An implementation must be registered before calling @ref toQVariant or @ref fromQVariant. */

template<>
class ValueSerializerTemplate<Draupnir::Messages::MessageCategories>
{
public:
    /*! @brief Owning pointer to the registered serializer implementation. */
    using ImplementationPtr = std::unique_ptr<Draupnir::Messages::MessageCategoriesSerializerInterface>;

    /*! @brief MessageCategories mask handled by this serializer. */
    using Value = Draupnir::Messages::MessageCategories;

    /*! @brief Registers a message category serializer implementation.
     *  @param implementation Serializer implementation to register. Must not be null.
     *  @details Ownership of `implementation` is transferred to this specialization.
     *  @pre No serializer implementation has previously been registered. */
    static void registerImplementation(Draupnir::Messages::MessageCategoriesSerializerInterface* implementation) {
        Q_ASSERT(implementation);
        Q_ASSERT(p_implementation == nullptr);
        p_implementation = ImplementationPtr(implementation);
    }

    /*! @brief Registers a message category serializer implementation.
     *  @param implementation Owning pointer to the serializer implementation to register. Must not be null.
     *  @details Ownership of the implementation is transferred to this specialization.
     * @pre No serializer implementation has previously been registered. */
    static void registerImplementation(ImplementationPtr implementation) {
        Q_ASSERT(implementation);
        Q_ASSERT(p_implementation == nullptr);
        p_implementation = std::move(implementation);
    }

    /*! @brief Serializes a message category mask to a `QVariant`.
     *  @param value Message category mask to serialize.
     *  @return `QVariant` containing its textual configuration representation.
     * @pre A serializer implementation has been registered. */
    [[nodiscard]] static QVariant toQVariant(const Draupnir::Messages::MessageCategories& value) {
        Q_ASSERT(p_implementation);
        return p_implementation->toConfigString(value);
    }

    /*! @brief Deserializes a message category mask from a `QVariant`.
     *  @param value `QVariant` containing a textual configuration
     *         representation.
     *  @return Deserialized message category mask, or `std::nullopt` if `value` cannot be converted to `QString` or its contents cannot
     *          be deserialized by the registered implementation.
     * @pre A serializer implementation has been registered. */
    [[nodiscard]] static std::optional<Draupnir::Messages::MessageCategories> fromQVariant(const QVariant& value) {
        Q_ASSERT(p_implementation);
        if (!value.canConvert<QString>())
            return std::nullopt;

        const QString valueString = value.toString();
        return p_implementation->fromConfigString(valueString);
    }
private:
    static inline ImplementationPtr p_implementation = nullptr;
};

using MessageCategoriesSerializer = ValueSerializerTemplate<Draupnir::Messages::MessageCategories>;

} // namespace Draupnir::Settings

#endif // MESSAGECATEGORIESSERIALIZER_H
