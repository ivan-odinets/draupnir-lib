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

#ifndef MESSAGECATEGORYREGISTRYTEMPLATE_H
#define MESSAGECATEGORYREGISTRYTEMPLATE_H

#include "draupnir/logging/messages/categories/AbstractMessageCategoryRegistry.h"

#include <QStringList>

#include "draupnir/logging/concepts/MessageCategoryTraitConcept.h"
#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"

namespace Draupnir::Logging
{

/*! @class MessageCategoryRegistryTemplate
 *  @ingroup Logging */

template<MessageCategoryTraitConcept... Traits>
class MessageCategoryRegistryTemplate final : public AbstractMessageCategoryRegistry
{
public:
    bool contains(MessageCategory id) const final { return _containsImpl<Traits...>(id); }

    using SingleCategorySerializer = ::Draupnir::Settings::FlagSerializerTemplate<
        MessageCategory,
        Traits...
    >;

    using Serializer = ::Draupnir::Settings::FlagsMaskSerializerTemplate<
        MessageCategories,
        SingleCategorySerializer,
        ::Draupnir::Settings::NoneFlagsMaskTemplate<MessageCategories>,
        ::Draupnir::Settings::FlagsMaskWrapperTemplate<
            []() { return MessageCategories::All; }, []() { return MessageCategories::allConfigKey(); }>
    >;

    std::optional<MessageCategories> fromConfigString(const QString &configString) const final {
        return Serializer::fromConfigString(configString);
    }

    QString toConfigString(MessageCategories mask) const final {
        return Serializer::toConfigString(mask);
    }

private:
    template<MessageCategoryTraitConcept First, MessageCategoryTraitConcept... Rest>
    static bool _containsImpl(MessageCategory id) {
        if (First::value() == id)
            return true;

        if constexpr (sizeof...(Rest) > 0)
            return _containsImpl<Rest...>(id);
        else
            return false;
    }
};

} // namespace Draupnir::Logging

#endif // MESSAGECATEGORYREGISTRYTEMPLATE_H
