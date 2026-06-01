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

namespace Draupnir::Logging
{

/*! @class MessageCategoryRegistryTemplate
 *  @ingroup Logging */

template<MessageCategoryTraitConcept... Traits>
class MessageCategoryRegistryTemplate final : public AbstractMessageCategoryRegistry
{
public:
    bool contains(MessageCategory id) const final { return _containsImpl<Traits...>(id); }

    std::optional<MessageCategories> fromConfigString(const QString& configString) const final {
        if (configString.isEmpty())
            return std::nullopt;

        const QStringList parts = configString.split(',');
        const bool hasNone = parts.contains(MessageCategories::noneConfigKey());
        const bool hasAll  = parts.contains(MessageCategories::allConfigKey());

        if ((hasNone || hasAll) && parts.size() != 1)
            return std::nullopt;

        if (hasNone)
            return MessageCategories::None;
        if (hasAll)
            return MessageCategories::All;

        MessageCategories result{MessageCategories::None};
        for (const QString& part : parts) {
            auto maybeField = _fromConfigStringImpl<Traits...>(part);
            if (!maybeField) return std::nullopt;
            result.set_flag(maybeField.value(), true);
        }
        return std::optional<MessageCategories>{result};
    };

    QString toConfigString(MessageCategories mask) const final {
        if (mask == MessageCategories::None)
            return MessageCategories::noneConfigKey();
        if (mask == MessageCategories::All)
            return MessageCategories::allConfigKey();

        QStringList parts{};
        _toConfigStringImpl<Traits...>(mask, parts);
        return parts.join(',');
    };

private:
    template<MessageCategoryTraitConcept First, MessageCategoryTraitConcept... Rest>
    static bool _containsImpl(MessageCategory id) {
        if (First::id() == id)
            return true;

        if constexpr (sizeof...(Rest) > 0)
            return _containsImpl<Rest...>(id);
        else
            return false;
    }

    template<MessageCategoryTraitConcept First, MessageCategoryTraitConcept... Rest>
    static std::optional<MessageCategory> _fromConfigStringImpl(const QString& part) {
        if (First::configKey() == part)
            return First::id();

        if constexpr (sizeof...(Rest) > 0)
            return _fromConfigStringImpl<Rest...>(part);
        else
            return std::nullopt;
    }

    template<MessageCategoryTraitConcept First, MessageCategoryTraitConcept... Rest>
    static void _toConfigStringImpl(const MessageCategories mask, QStringList& out) {
        if (mask.test_flag(First::id()))
            out.append(First::configKey());

        if constexpr (sizeof...(Rest) > 0)
            _toConfigStringImpl<Rest...>(mask, out);
    }
};

};

#endif // MESSAGECATEGORYREGISTRYTEMPLATE_H
