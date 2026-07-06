/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef LOGWIDGETSETTINGSTRAITS_H
#define LOGWIDGETSETTINGSTRAITS_H

#include <QSize>

#include "draupnir/logging/messages/categories/MessageCategories.h"
#include "draupnir/logging/messages/MessageLevels.h"
#include "draupnir/logging/messages/MessageViewItemFields.h"

/*! @file draupnir/logging/traits/settings/LogWidgetSettingsTraits.h
 *  @ingroup Logging
 *  @brief This file contains setting traits for the @ref Draupnir::Logging::LogWidget.
 * @todo Split this file into more suitable files. */

/*! @namespace Draupnir::Logging::Settings
 *  @ingroup Logging
 *  @brief Namespace for settings-related things within the @ref Logging module. */

namespace Draupnir::Logging::Settings
{

/*! @namespace Draupnir::Logging::Settings::LogWidget
 *  @ingroup Logging
 *  @brief Namespace for settings traits of the @ref Draupnir::Logging::LogWidget widget. */

namespace LogWidget
{

/*! @struct IconSize draupnir/logging/traits/settings/LogWidgetSettingsTraits.h
 *  @ingroup Logging
 *  @brief Setting trait for the size of icons within @ref Draupnir::Logging::LogWidget (displaying of the logged @ref
 *         Draupnir::Logging::Message objects is handled by the @ref Draupnir::Logging::MessageListView). */

struct IconSizeSetting
{
    /*! @brief Underlying value type. */
    using Value = QSize;

    /*! @brief Return the persistent key as a `QString`. */
    static QString key() { return QString{"log_widget/message_icon_size"}; }

    /*! @brief Return the default value. */
    static QSize defaultValue() { return QSize{64,64}; }
};

struct DisplayedMessageViewItemFieldsSetting
{
    /*! @brief Underlying value type. */
    using Value = MessageViewItemFields;

    /*! @brief Return the persistent key as a `QString`. */
    static QString key() { return QString{"log_widget/meddsge_fields_shown"}; }

    /*! @brief Return the default value. */
    static MessageViewItemFields defaultValue() { return MessageViewItemFields::All; }
};

struct DisplayedMessageLevelsSetting
{
    /*! @brief Underlying value type. */
    using Value = MessageLevels;

    /*! @brief Return the persistent key as a `QString`. */
    static QString key() { return QString{"log_widget/message_levels_shown"}; }

    /*! @brief Return the default value. */
    static MessageLevels defaultValue() { return MessageLevels::All; }
};

struct DisplayedMessageCategoriesSetting
{
    using Value = MessageCategories;

    static QString key() { return QString{"log_widget/message_categories_shown"}; }

    static MessageCategories defaultValue() { return MessageCategories::All; }
};

}; // namespace MessageListView

}; // namespace Draupnir::Messages::Settings

#include "draupnir/settings_registry/utils/ValueSerializerTemplate.h"

#include "draupnir/logging/messages/categories/MessageCategoriesSerializerInterface.h"

namespace Draupnir::Settings
{

template<>
class ValueSerializerTemplate<Draupnir::Logging::MessageCategories>
{
public:
    static void setMessageCategoriesSerializerInterface(Draupnir::Logging::MessageCategoriesSerializerInterface* interface) {
        p_serializer = interface;
    }

    static QVariant toQVariant(const Draupnir::Logging::MessageCategories& value) {
        Q_ASSERT(p_serializer);
        return p_serializer->toConfigString(value);
    }

    static std::optional<Draupnir::Logging::MessageCategories> fromQVariant(const QVariant& value) {
        Q_ASSERT(p_serializer);
        if (!value.canConvert<QString>())
            return std::nullopt;

        const QString valueString = value.toString();
        return p_serializer->fromConfigString(valueString);
    }
private:
    static inline Draupnir::Logging::MessageCategoriesSerializerInterface* p_serializer = nullptr;
};

};

#endif // LOGWIDGETSETTINGSTRAITS_H
