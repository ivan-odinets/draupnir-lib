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

#ifndef FLAGSMASKSERIALIZERTEMPLATE_H
#define FLAGSMASKSERIALIZERTEMPLATE_H

#include "draupnir/settings_registry/utils/FlagSerializerTemplate.h"

namespace Draupnir::Settings
{

/*! @struct FlagsMaskWrapperTemplate draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Describes mapping between a preset flags mask and its config string representation.
 *  @tparam MaskProvider Preset mask value represented by this wrapper, or a nullary callable returning that value.
 *  @tparam ConfigStringProvider Nullary callable returning config string for the represented mask.
 *
 *  @details This wrapper is intended to be used as metadata entry for @ref FlagsMaskSerializerTemplate. The represented
 *           mask value is exposed through @ref value(). If @p MaskProvider is callable, @ref value() returns the result
 *           of calling it; otherwise it returns @p MaskProvider directly.
 *
 *           Preset mask wrappers are useful for named mask aliases such as `none`, `all`, or other commonly used flag
 *           combinations. The config representation is produced by calling @p ConfigStringProvider. */

template<auto MaskProvider, auto ConfigStringProvider>
struct FlagsMaskWrapperTemplate
{
public:
    /*! @brief Returns mask value represented by this wrapper.
     *  @return Mask value produced either directly from @p MaskProvider or by calling @p MaskProvider. */
    static constexpr auto value() requires( requires { MaskProvider(); } ){
        return MaskProvider();
    }

    /*! @brief Returns mask value represented by this wrapper.
     *  @return Mask value produced either directly from @p MaskProvider or by calling @p MaskProvider. */
    static constexpr auto value() requires( !requires { MaskProvider(); } ){
        return MaskProvider;
    }

    /*! @brief Returns config string associated with @ref value().
     *  @return Stable Latin-1 config string used for settings serialization. */
    static QLatin1String configString() { return ConfigStringProvider(); }
};

/*! @struct NoneFlagsMaskTemplate draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Provides default config representation for an empty flags mask.
 *  @tparam Flags Flags mask type represented by this wrapper.
 *
 *  @details This wrapper maps an empty mask, constructed as `Flags{0}`, to the stable config token `"none"`. It is intended
 *           to be used as a preset mask entry for @ref FlagsMaskSerializerTemplate when explicit serialization of an empty
 *           flags mask is needed. */

template<class Flags>
struct NoneFlagsMaskTemplate
{
    static inline constexpr Flags value() { return Flags{0}; }
    static QLatin1String configString() { return QLatin1String{"none"}; }
};

/*! @class FlagsMaskSerializerTemplate draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h
 *  @ingroup SettingsRegistry
 *  @brief Serializes flags masks to and from config strings.
 *  @tparam Flags Flags mask type handled by this serializer.
 *  @tparam SingleFlagSerializer Serializer used for individual flag values.
 *  @tparam PresetMaskWrapperEntries Metadata entries describing named preset masks.
 *
 *  @details This serializer supports two config string forms:
 *           - a single token representing either a preset mask or a single flag;
 *           - a comma-separated list of individual flag tokens.
 *
 *           Preset masks are checked before individual flag values. This allows aliases such as `none`, `all`, or other
 *           named flag combinations to have stable compact config representations.
 *
 *           Individual flag tokens are parsed and serialized using @p SingleFlagSerializer.
 * @todo Tests: Add compilation tests for the FlagsMaskSerializerTemplate. */

template<class Flags, class SingleFlagSerializer, class... PresetMaskWrapperEntries>
class FlagsMaskSerializerTemplate
{
    // Why not using concepts like here:
    // template<class Flags, class SingleFlagSerializer, SerializableValueWrapperConcept... PresetMaskWrapperEntries>
    // class FlagsMaskSerializerTemplate { ... } ?
    //
    // Reason to allow following use-case:
    // struct SomeFlags : public draupnir::utils::enum_flags<SomeEnum> {
    //     ...
    //     using Serializer = FlagsMaskSerializerTemplate<
    //         SomeFlags,
    //         FlagSerializerTemplate<SomeEnum, {enum value wrappers pack}... >,
    //         {mask wrappers pack}
    //     >;
    // };
    // Although, this is compilable even when using concept restriction within FlagsMaskSerializerTemplate definition - QtCreator
    // syntax check fails. And this seems to be fragile.
    //
    // At that point SomeFlags is still incomplete and constraining the preset wrapper pack directly in the template parameter list
    // may force early instantiation of wrapper value() expressions and make otherwise valid self-referential declarations fragile
    // for compilers/tools.
    //
    // If weired stuff begins to happens here - move these validations into the method bodies.
    static_assert((SerializableValueWrapperConcept<PresetMaskWrapperEntries> && ...),
        "All types within PresetMaskWrapperEntries... pack must satisfy SerializableValueWrapperConcept");

    // Same applies here.
    static_assert(draupnir::utils::flags_like_concept<Flags>,
        "Flags parameter must satisfy draupnir::utils::flags_like_concept");

    // And here...
    static_assert(ValueSerializableBy<SingleFlagSerializer, typename Flags::flag_type>,
        "SingleFlagSerializer must satisfy ValueSerializableBy concept for the individual flag type (\"Flags::flag_type\"");

    static consteval Flags _evaluateAllKnownFlags() {
        Flags result{0};
        for (auto flag : SingleFlagSerializer::supportedFlags)
            result.set_flag(flag, true);
        return result;
    }

    static constexpr Flags _allKnownFlags = _evaluateAllKnownFlags();

public:
    /*! @brief Parses flags mask from config string.
     *  @param string Config string to parse.
     *  @return Parsed flags mask, or `std::nullopt` if the string is empty or contains unknown tokens.
     *  @details A single-token string is first matched against preset mask wrappers. If no preset mask matches, the token
     *           is parsed as an individual flag using @p SingleFlagSerializer.
     *
     *           A multi-token string is interpreted as a comma-separated list of individual flags. Every token must be
     *           recognized by @p SingleFlagSerializer; otherwise parsing fails. */
    [[nodiscard]] static std::optional<Flags> fromConfigString(const QString& string) {
        if (string.isEmpty())
            return std::nullopt;

        // Split into tokens
        const QStringList parts = string.split(',');
        // If we have only one token
        if (parts.count() == 1)
            return _fromSingleToken<PresetMaskWrapperEntries...>(string);

        // If we have many tokens, lets go through them
        Flags result{0};

        for (const QString& part : parts) {
            auto maybeFlag = SingleFlagSerializer::fromConfigString(part);
            if (!maybeFlag)
                return std::nullopt;
            result.set_flag(maybeFlag.value(), true);
        }
        return std::optional<Flags>{result};
    };

    /*! @brief Converts flags mask to config string.
     *  @param flags Flags mask to serialize.
     *  @return Config string representing @p flags.
     *  @details Preset masks are checked first. If @p flags exactly matches a preset mask, the corresponding preset token
     *           is returned. Otherwise the mask is serialized as comma-separated individual flag tokens using @p SingleFlagSerializer. */
    [[nodiscard]] static QString toConfigString(const Flags& flags) {
        // First lets check if we have one of preset things
        std::optional<QString> maybePresetMaskString = _knownMaskToConfigString<PresetMaskWrapperEntries...>(flags);
        if (maybePresetMaskString)
            return maybePresetMaskString.value();

        QStringList resultParts;
        SingleFlagSerializer::forEachWrapper([&resultParts, flags]<SerializableValueWrapperConcept Wrapper>(){
            if (flags.test_flag(Wrapper::value()))
                resultParts.append(Wrapper::configString());
        });
        return resultParts.join(',');
    };

private:
    /*! @brief Parses a single config token as either preset mask or individual flag.
     *  @tparam First First preset mask wrapper to check.
     *  @tparam Rest Remaining preset mask wrappers.
     *  @param string Single config token to parse.
     *  @return Parsed flags mask, or `std::nullopt` if the token is not recognized.
     *  @details Preset mask wrappers are checked in declaration order. If none of them matches, parsing is delegated to
     *           @p SingleFlagSerializer. */
    template<SerializableValueWrapperConcept First, SerializableValueWrapperConcept... Rest>
    static std::optional<Flags> _fromSingleToken(const QString& string) {
        if (First::configString() == string)
            return Flags{First::value()};

        if constexpr (sizeof...(Rest) > 0) {
            return _fromSingleToken<Rest...>(string);
        } else {
            const auto maybeFlag = SingleFlagSerializer::fromConfigString(string);
            return (maybeFlag) ?
                std::optional<Flags>(maybeFlag.value()) :
                std::nullopt;
        }
    }

    /*! @brief Converts flags mask to preset config string if it matches a preset mask.
     *  @tparam First First preset mask wrapper to check.
     *  @tparam Rest Remaining preset mask wrappers.
     *  @param mask Flags mask to match against preset masks.
     *  @return Preset config string if @p mask matches one of the preset masks; otherwise `std::nullopt`.
     *  @details Preset mask wrappers are checked in declaration order. The first exact match wins. */
    template<SerializableValueWrapperConcept First, SerializableValueWrapperConcept... Rest>
    static std::optional<QString> _knownMaskToConfigString(Flags mask) {
        if (mask == First::value())
            return First::configString();

        if constexpr (sizeof...(Rest) > 0)
            return _knownMaskToConfigString<Rest...>(mask);
        else
            return std::nullopt;
    }
};

}; // namespace Draupnir::Settings

#endif // FLAGSMASKSERIALIZERTEMPLATE_H
