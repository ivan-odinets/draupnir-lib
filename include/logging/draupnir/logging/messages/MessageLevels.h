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

#ifndef MESSAGELEVELS_H
#define MESSAGELEVELS_H

#include <QObject>

#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"
#include "draupnir/utils/advance_enum.h"
#include "draupnir/utils/flags.h"

namespace Draupnir::Logging
{

/*! @class MessageLevel draupnir/logging/messages/MessageLevels.h
 *  @ingroup Logging
 *  @brief Provides built-in log message severity levels.
 *
 *  @details Message levels describe the severity or importance of a @ref Draupnir::Logging::Message. A message level is used
 *           together with @ref Draupnir::Logging::MessageCategory to form a complete @ref Draupnir::Logging::MessageType.
 * @todo Documentation: Write reasonable documentation for this class.
 * @todo Tests: Add some unit tests for this class. */

class MessageLevel
{
public:
    MessageLevel() = delete;
    Q_DISABLE_COPY(MessageLevel)

    /*! @enum MessageLevel::Value
     *  @brief Built-in message severity levels. */
    enum Value : uint8_t {
        Debug   = 0b0000'0001, /*!< @brief Debug-level diagnostic message. */
        Info    = 0b0000'0010, /*!< @brief Informational message. */
        Warning = 0b0000'0100, /*!< @brief Warning message. */
        Error   = 0b0000'1000, /*!< @brief Error message. */
    };

    static QLatin1String debugToken() { return QLatin1String{"debug"}; }
    static QLatin1String infoToken() { return QLatin1String{"info"}; }
    static QLatin1String warningToken() { return QLatin1String{"warning"}; }
    static QLatin1String errorToken() { return QLatin1String{"error"}; }

    using Serializer = Draupnir::Settings::FlagSerializerTemplate<
        Value,
        Draupnir::Settings::FlagWrapperTemplate<Value::Debug, []() { return debugToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Info, []() { return infoToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Warning, []() { return warningToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Error, []() { return errorToken(); }>
    >;

    static QString toConfigString(MessageLevel::Value level) {
        return Serializer::toConfigString(level);
    }

    static std::optional<MessageLevel::Value> fromConfigString(const QString& string) {
        return Serializer::fromConfigString(string);
    }

    static inline QString debugDisplayName();
    static inline QString infoDisplayName();
    static inline QString warningDisplayName();
    static inline QString errorDisplayName();

    /*! @brief Converts a message level to a user-friendly display string.
     *  @param value Message level value.
     *  @return Translated display string for the given message level. */
    static QString toDisplayName(Value value);
};

/*! @ingroup Logging
 *  @brief Advances a @ref Draupnir::Logging::MessageLevel::Value to the next message level. */

inline MessageLevel::Value& operator++(MessageLevel::Value& type,int)
{
    draupnir::utils::enum_values<
        MessageLevel::Value,
        MessageLevel::Value::Debug,
        MessageLevel::Value::Info,
        MessageLevel::Value::Warning,
        MessageLevel::Value::Error
    >::advance(type);
    return type;
}

/*! @ingroup Logging
 *  @brief Flag set of @ref Draupnir::Logging::MessageLevel::Value values.
 * @todo Documentation: Write reasonable documentation for this class.
 * @todo Tests: Add some unit tests for this class. */

class MessageLevels final : public draupnir::utils::enum_flags<MessageLevel::Value>
{
    using _Base = draupnir::utils::enum_flags<MessageLevel::Value>;

public:
    using _Base::enum_flags;
    using _Base::operator=;

    using flag_type = _Base::flag_type;

    static constexpr _Base::integer None = 0;
    static constexpr _Base::integer All =
        MessageLevel::Debug | MessageLevel::Info | MessageLevel::Warning | MessageLevel::Error;

    static QLatin1String allToken() { return QLatin1String{"all"}; }

    using Serializer = Draupnir::Settings::FlagsMaskSerializerTemplate<
        MessageLevels,
        MessageLevel::Serializer,
        Draupnir::Settings::NoneFlagsMaskTemplate<MessageLevels>,
        Draupnir::Settings::FlagsMaskWrapperTemplate<[]() { return All; }, []() { return allToken(); }>
    >;

    static std::optional<MessageLevels> fromConfigString(const QString& string) {
        return Serializer::fromConfigString(string);
    }

    static QString toConfigString(MessageLevels mask) {
        return Serializer::toConfigString(mask);
    }

    static QString toDisplayName(MessageLevels levels) {
        if (levels == All)
            return QObject::tr("All");

        return MessageLevel::toDisplayName(static_cast<MessageLevel::Value>(levels.value()));
    };

    struct UiSelectorMetadata {
        static constexpr _Base::enum_type displayedFlags[] = {
                                                              MessageLevel::Debug, MessageLevel::Info, MessageLevel::Warning, MessageLevel::Error };
        static constexpr _Base::integer displayedMaskPresets[] = { All };
    };

};

}; // namespace Draupnir::Logging

Q_DECLARE_METATYPE(Draupnir::Logging::MessageLevel::Value);
Q_DECLARE_METATYPE(Draupnir::Logging::MessageLevels);

#endif // MESSAGELEVELS_H
