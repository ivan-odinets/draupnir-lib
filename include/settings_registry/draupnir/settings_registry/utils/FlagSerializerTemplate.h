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

#ifndef FLAGSERIALIZERTEMPLATE_H
#define FLAGSERIALIZERTEMPLATE_H

#include <QString>
#include <QStringList>

#include "draupnir/settings_registry/concepts/SettingsSerializationConcepts.h"

namespace Draupnir::Settings
{

/*! @struct FlagWrapperTemplate draupnir/settings_registry/utils/FlagWrapperTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Describes mapping between a single flag value and its config string representation.
 *  @tparam KeyProvider Flag value represented by this wrapper, or a nullary callable returning that value.
 *  @tparam ConfigStringProvider Nullary callable returning config string for the represented flag value.
 *
 *  @details This wrapper is intended to be used as metadata entry for @ref FlagSerializerTemplate.
 *           The represented flag value is exposed through @ref value(). If @p KeyProvider is callable,
 *           @ref value() returns the result of calling it; otherwise it returns @p KeyProvider directly.
 *
 *           The config representation is produced by calling @p ConfigStringProvider. */

template<auto KeyProvider, auto ConfigStringProvider>
struct FlagWrapperTemplate
{
    /*! @brief Returns flag value represented by this wrapper.
     *  @return Flag value produced either directly from @p KeyProvider or by calling @p KeyProvider. */
    static constexpr auto value() requires( requires { KeyProvider(); } ) {
        return KeyProvider();
    }

    /*! @brief Returns flag value represented by this wrapper.
     *  @return Flag value produced either directly from @p KeyProvider or by calling @p KeyProvider. */
    static constexpr auto value() requires( !requires { KeyProvider(); } ) {
        return KeyProvider;
    }

    /*! @brief Returns config string associated with @ref value.
     *  @return Stable config string used for settings serialization. */
    static QLatin1String configString() { return ConfigStringProvider(); }
};

/*! @class FlagSerializerTemplate draupnir/settings_registry/utils/FlagWrapperTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Serializes single flag values to and from config strings.
 *  @tparam FlagClass Type of single flag value handled by this serializer.
 *  @tparam FlagWrapperEntries Metadata entries describing supported flag values.
 *
 *  @details This serializer maps individual flag values to stable config strings using @p FlagWrapperEntries. It is intended
 *           for atomic flag values, not for arbitrary flag masks or flag combinations.
 *
 *           For flag masks, use a mask serializer that consumes this serializer as the source of supported atomic flags.
 * @todo Important: When switched to C++26 - add specialization for empty FlagWrapperEntries pack, which will use reflection.
 * @todo Tests: Add compilation tests for the FlagSerializerTemplate. */

template<draupnir::utils::enum_or_integer_like_concept FlagClass, SerializableValueWrapperConcept... FlagWrapperEntries>
class FlagSerializerTemplate
{
    static_assert(sizeof...(FlagWrapperEntries) > 0,
        "FlagSerializerTemplate requires at least one flag wrapper");

    template<class Callable>
    static inline constexpr bool callableWithAllWrappers_v =
        (requires(Callable& callable) {
            { callable.template operator()<FlagWrapperEntries>() } -> std::same_as<void>;
        } && ...);

public:
    /*! @var FlagSerializerTemplate::supportedFlags
     *  @brief List of flag values supported by this serializer.
     *  @details The order matches the order of @p _FlagWrapperEntries. This order is also suitable for deterministic mask
     *           serialization when flags are decomposed into individual tokens. */
    static inline constexpr FlagClass supportedFlags[] = { FlagWrapperEntries::value()... };

    /*! @brief Invokes callable for every flag wrapper entry.
     *  @tparam F Callable type.
     *  @param callable Callable object with templated call operator accepting wrapper type as template argument.
     *  @details The callable must support calls in the following form: `callable.template operator()<Wrapper>();` The wrappers
     *           are visited in the same order as @p _FlagWrapperEntries. */
    template<class F>
    static inline void forEachWrapper(F&& callable) {
        static_assert(callableWithAllWrappers_v<F>,
            "Callable passed to forEachWrapper must be invocable as callable.template operator()<Wrapper>() for every flag "\
            "wrapper and return void");
        (callable.template operator()<FlagWrapperEntries>(), ...);
    }

    /*! @brief Parses a single flag value from config string.
     *  @param string Config string to parse.
     *  @return Parsed flag value, or `std::nullopt` if the string is not recognized.
     * @todo Question: swicth from std::optional to std::expected with error message? */
    static std::optional<FlagClass> fromConfigString(const QString& string) {
        return _fromConfigStringImpl<FlagWrapperEntries...>(string);
    }

    /*! @brief Converts a single flag value to config string.
     *  @param value Flag value to serialize.
     *  @return Config string associated with @p value.
     *  @details Passing a value not described by @p FlagWrapperEntries is a programming error. */
    static QString toConfigString(FlagClass value) {
        return _toConfigStringImpl<FlagWrapperEntries...>(value);
    }

private:
    /*! @brief Recursive implementation for parsing config string using wrapper entries. */
    template<class First, class... Rest>
    static std::optional<FlagClass> _fromConfigStringImpl(const QString& string) {
        if (string == First::configString())
            return First::value();

        if constexpr (sizeof...(Rest) > 0)
            return _fromConfigStringImpl<Rest...>(string);
        else
            return std::nullopt;
    };

    /*! @brief Recursive implementation for converting flag value to config string using wrapper entries. */
    template<class First, class... Rest>
    static QString _toConfigStringImpl(FlagClass value) {
        if (value == First::value())
            return First::configString();

        if constexpr (sizeof...(Rest) > 0) {
            return _toConfigStringImpl<Rest...>(value);
        } else {
            Q_ASSERT(false);
            Q_UNREACHABLE();
            return "";
        }
    }
};

}; // namespace Draupnir::Settings

#endif // FLAGSERIALIZERTEMPLATE_H
