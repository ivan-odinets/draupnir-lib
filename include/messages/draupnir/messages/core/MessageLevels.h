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

namespace Draupnir::Messages
{

/*! @class MessageLevel draupnir/messages/core/MessageLevels.h
 *  @ingroup Messages
 *  @brief Provides the built-in message severity levels.
 *
 *  @details A message level describes the severity or importance of a @ref Draupnir::Messages::Message. It is combined with
 *           a @ref Draupnir::Messages::MessageCategory to form a complete @ref Draupnir::Messages::MessageType.
 *
 *           The class also provides conversion between severity levels, configuration tokens, and translated display names.
 *
 * @todo Tests: Add some unit tests for this class. */

class MessageLevel
{
public:
    MessageLevel() = delete;
    Q_DISABLE_COPY(MessageLevel)

    /*! @enum MessageLevel::Value
     *  @brief Built-in message severity levels.
     *  @details Each value occupies a separate bit so that multiple levels can be combined in a @ref Draupnir::Messages::MessageLevels
     *           mask. */
    enum Value : uint8_t {
        Debug   = 0b0000'0001, /*!< @brief Debug-level diagnostic message. */
        Info    = 0b0000'0010, /*!< @brief Informational message. */
        Warning = 0b0000'0100, /*!< @brief Warning message. */
        Error   = 0b0000'1000, /*!< @brief Error message. */
    };

    /*! @brief Returns the configuration token for @ref Debug.
     *  @return Latin-1 string containing `"debug"`. */
    static QLatin1String debugToken() { return QLatin1String{"debug"}; }

    /*! @brief Returns the configuration token for @ref Info.
     *  @return Latin-1 string containing `"info"`. */
    static QLatin1String infoToken() { return QLatin1String{"info"}; }

    /*! @brief Returns the configuration token for @ref Warning.
     *  @return Latin-1 string containing `"warning"`. */
    static QLatin1String warningToken() { return QLatin1String{"warning"}; }

    /*! @brief Returns the configuration token for @ref Error.
     *  @return Latin-1 string containing `"error"`. */
    static QLatin1String errorToken() { return QLatin1String{"error"}; }

    /*! @brief Serializer used to convert individual message levels to and from configuration tokens. */
    using Serializer = Draupnir::Settings::FlagSerializerTemplate<
        Value,
        Draupnir::Settings::FlagWrapperTemplate<Value::Debug,   []() { return debugToken();   }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Info,    []() { return infoToken();    }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Warning, []() { return warningToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<Value::Error,   []() { return errorToken();   }>
    >;

    /*! @brief Converts a message level to its configuration token.
     *  @param level Message level to serialize.
     *  @return Configuration string representing the supplied level. */
    [[nodiscard]] static QString toConfigString(MessageLevel::Value level) {
        return Serializer::toConfigString(level);
    }

    /*! @brief Parses a message level from a configuration string.
     *  @param string Configuration string to parse.
     *  @return Parsed message level, or `std::nullopt` when the string does not contain a recognized level token. */
    [[nodiscard]] static std::optional<MessageLevel::Value> fromConfigString(const QString& string) {
        return Serializer::fromConfigString(string);
    }

    /*! @brief Returns the translated display name for @ref Debug.
     *  @return Translated debug-level name. */
    static QString debugDisplayName();

    /*! @brief Returns the translated display name for @ref Info.
     *  @return Translated informational-level name. */
    static QString infoDisplayName();

    /*! @brief Returns the translated display name for @ref Warning.
     *  @return Translated warning-level name. */
    static QString warningDisplayName();

    /*! @brief Returns the translated display name for @ref Error.
     *  @return Translated error-level name. */
    static QString errorDisplayName();

    /*! @brief Converts a message level to a user-friendly display string.
     *  @param value Message level value.
     *  @return Translated display string for the given message level.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] static QString toDisplayName(Value value);
};

/*! @ingroup Messages
 *  @brief Advances a message level to the next built-in severity level.
 *  @param level Message level to advance.
 *  @return Value of @p level before it was advanced.
 *
 *  @details The operation follows the order `Debug`, `Info`, `Warning`, `Error`. The behavior when advancing past `Error`
 *           is determined by @ref draupnir::utils::enum_values. */

inline MessageLevel::Value operator++(MessageLevel::Value& level,int)
{
    const auto previous = level;
    draupnir::utils::enum_values<
        MessageLevel::Value,
        MessageLevel::Value::Debug,
        MessageLevel::Value::Info,
        MessageLevel::Value::Warning,
        MessageLevel::Value::Error
    >::advance(level);
    return previous;
}

/*! @class MessageLevels draupnir/messages/core/MessageLevels.h
 *  @brief Flag mask containing one or more message severity levels.
 *
 *  @details MessageLevels combines @ref Draupnir::Messages::MessageLevel::Value values into a single mask. It provides predefined
 *           masks and conversion to configuration and display strings.
 *
 * @todo Tests: Add unit tests for mask serialization, presets, and display name conversion. */

class MessageLevels final : public draupnir::utils::enum_flags<MessageLevel::Value>
{
    /*! @brief Underlying enum-flags implementation. */
    using _Base = draupnir::utils::enum_flags<MessageLevel::Value>;

public:
    using _Base::enum_flags;
    using _Base::operator=;

    /*! @brief Type used to represent an individual flag. */
    using flag_type = _Base::flag_type;

    /*! @brief Empty message-level mask. */
    static constexpr _Base::primitive_integer None = 0;

    /*! @brief Mask containing all built-in message levels. */
    static constexpr _Base::primitive_integer All =
        MessageLevel::Debug | MessageLevel::Info | MessageLevel::Warning | MessageLevel::Error;

    /*! @brief Returns the configuration token for the @ref All mask.
     *  @return Latin-1 string containing `"all"`. */
    static QLatin1String allToken() { return QLatin1String{"all"}; }

    /*! @brief Serializer used to convert message-level masks to and from configuration strings. */
    using Serializer = Draupnir::Settings::FlagsMaskSerializerTemplate<
        MessageLevels,
        MessageLevel::Serializer,
        Draupnir::Settings::NoneFlagsMaskTemplate<MessageLevels>,
        Draupnir::Settings::FlagsMaskWrapperTemplate<[]() { return All; }, []() { return allToken(); }>
    >;

    /*! @brief Parses a message-level mask from a configuration string.
     *  @param string Configuration string to parse.
     *  @return Parsed message-level mask, or `std::nullopt` when the string cannot be parsed. */
    [[nodiscard]] static std::optional<MessageLevels> fromConfigString(const QString& string) {
        return Serializer::fromConfigString(string);
    }

    /*! @brief Converts a message-level mask to a configuration string.
     *  @param mask Message-level mask to serialize.
     *  @return Configuration string representing the supplied mask. */
    [[nodiscard]] static QString toConfigString(MessageLevels mask) {
        return Serializer::toConfigString(mask);
    }

    /*! @brief Returns a translated display string for a message-level value or the complete mask.
     *  @param levels A mask containing exactly one message level, or @ref All.
     *  @return `"All"` for the complete mask, or the translated display name of the contained message level.
     *  @pre @p levels must contain exactly one message level or equal @ref All.
     * @todo Feature: Allow choosing behaviour for handling wrong arguments. Q_ASSERT / UB / fallback. For example by using
     *       DRAUPNIR_ENABLE_SAFE_RELEASE macro. */
    [[nodiscard]] static QString toDisplayName(MessageLevels levels) {
        if (levels == All)
            return QObject::tr("All");

        return MessageLevel::toDisplayName(static_cast<MessageLevel::Value>(levels.value()));
    }

    /*! @brief Metadata describing message-level options exposed by UI selectors.
     *  @details Lists the individual flags and predefined mask presets that should be displayed by generic
     *           settings UI components. */
    struct UiSelectorMetadata {
        /*! @brief Individual message levels displayed by UI selectors. */
        static constexpr _Base::enum_type displayedFlags[] = {
            MessageLevel::Debug, MessageLevel::Info, MessageLevel::Warning, MessageLevel::Error };

        /*! @brief Predefined message-level masks displayed by UI selectors. */
        static constexpr _Base::primitive_integer displayedMaskPresets[] = { All };
    };

};

} // namespace Draupnir::Messages

Q_DECLARE_METATYPE(Draupnir::Messages::MessageLevel::Value);
Q_DECLARE_METATYPE(Draupnir::Messages::MessageLevels);

#endif // MESSAGELEVELS_H
