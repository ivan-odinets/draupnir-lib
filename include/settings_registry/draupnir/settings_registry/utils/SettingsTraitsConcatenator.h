/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef SETTINGSTRAITSCONCATENATOR_H
#define SETTINGSTRAITSCONCATENATOR_H

#include <type_traits>

#include "draupnir/settings_registry/SettingsRegistryTemplate.h"
#include "draupnir/settings_registry/utils/SettingTraitValidator.h"

#include "draupnir/utils/template_detectors.h"

namespace Draupnir::Settings
{

/*! @class SettingsTraitsConcatenator draupnir/settings_registry/utils/SettingsTraitsConcatenator.h
 *  @brief Utility class that normalizes a heterogeneous pack of types into settings-related bundles and registries.
 *
 *  @details This class accepts an arbitrary pack of types @p Things..., where each element can be:
 *           - A type that exposes a nested `T::SettingsBundle` which is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate;
 *           - A direct instantiation of @ref Draupnir::Settings::SettingsBundleTemplate;
 *           - A valid setting trait (as determined by @ref Draupnir::Settings::SettingTraitValidator);
 *           - Or any other unrelated type;
 *
 *           For each such type `T`, the internal machinery converts it to an appropriate `SettingsBundleTemplate<...>` form
 *           (or an empty bundle), and then merges all resulting bundles into a single settings bundle and/or settings registry,
 *           depending on the requested output.
 *
 *           The main entry points are:
 *           - @ref to : generic conversion into an arbitrary "tuple-like" output container template;
 *           - @ref toSettingsRegistry : convenience alias that produces a unified @ref Draupnir::Settings::SettingsRegistryTemplate;
 *           - @ref toSettingsBundle : convenience alias that produces a unified @ref Draupnir::Settings::SettingsBundleTemplate. */

template<class... Things>
class SettingsTraitsConcatenator
{
private:
    /*! @brief Trait to detect presence of a nested `T::SettingsBundle` type. Primary template: assumes that @p T does not provide
     *         a nested `SettingsBundle` type. */
    template<class, class = std::void_t<>>
    struct has_settingsBundle : std::false_type {};

    /*! @brief Specialization: `true` if `T` defines nested type `T::SettingsBundle` and this type is an instantiation of @ref
     *         Draupnir::Settings::SettingsBundleTemplate. This ensures that we only treat `T::SettingsBundle` as a settings bundle
     *         if it has the expected form. */
    template<class T>
    struct has_settingsBundle<T, std::void_t<typename T::SettingsBundle>> :
        std::conditional_t<
            draupnir::utils::is_instantiation_of_v<typename T::SettingsBundle, Draupnir::Settings::SettingsBundleTemplate>,
            std::true_type,
            std::false_type
        > {};

    /*! @brief Convenience boolean variable template for @ref has_settingsBundle. */
    template<class T>
    static inline constexpr bool has_settingsBundle_v = has_settingsBundle<T>::value;

    /*! @brief Normalizes arbitrary type @p T into a settings bundle type. This template performs the following conversion rules:
     *         - If `T` exposes a nested `T::SettingsBundle` that is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate
     *           - that type is used as the result;
     *         - If `T` itself is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate — `T` is used as the result;
     *         - If `T` is a valid setting trait (according to @ref SettingTraitValidator) - a single-trait bundle `SettingsBundleTemplate<T>`
     *           is used as the result;
     *         - Otherwise, an empty @ref Draupnir::Settings::SettingsBundleTemplate is used.
     *
     *         These rules allow callers to pass traits, handlers, existing bundles, or unrelated types, and always obtain a well-formed
     *         bundle type as the output. */
    template<
        class T,
        bool = has_settingsBundle_v<T>,
        bool = draupnir::utils::is_instantiation_of_v<T,SettingsBundleTemplate>,
        bool = SettingTraitValidator::isValidSettingTrait<T>()
    >
    struct convertToBundle { using type = Settings::SettingsBundleTemplate<>; };

    /*! @brief Specialization: `T` exposes a nested `T::SettingsBundle` which is a valid bundle. Uses `T::SettingsBundle` directly as the
     *         resulting bundle type. */
    template<class T>
    struct convertToBundle<T, true, false, false> { using type = typename T::SettingsBundle; };

    /*! @brief Specialization: `T` itself is a @ref SettingsBundleTemplate instantiation. Uses `T` directly as the resulting bundle type. */
    template<class T>
    struct convertToBundle<T, false, true, false> { using type = T; };

    /*! @brief Specialization: `T` is a valid setting trait. Wraps the trait into a single-trait bundle @ref SettingsBundleTemplate. */
    template<class T>
    struct convertToBundle<T, false, false, true> { using type = SettingsBundleTemplate<T>; };

    /*! @brief Convenience alias for the result of @ref convertToBundle. */
    template<class T>
    using convertToBundle_t = typename convertToBundle<T>::type;

public:
    /*! @brief Converts all @p Things... into settings bundles and merges them into a single container.
     *  @tparam OutputContainer A "tuple-like" template that will be used as the final container type. It is expected to have the form
     *          `template<class...> class OutputContainer`.
     *
     *  @details Internally, each `Things` element is transformed via @ref convertToBundle into a concrete `SettingsBundleTemplate<...>`
     *           (possibly empty). All these bundles are then merged together using @ref draupnir::utils::tuple_like_merge_t, with @ref
     *           SettingsBundleTemplate acting as the "input tuple-like" container and @p OutputContainer as the resulting container type.
     *
     *           This allows the same normalized settings information to be materialized either as a `SettingsBundleTemplate`, a
     *           `SettingsRegistryTemplate`, or any other compatible container. */
    template<template <class...> class OutputContainer>
    using to = draupnir::utils::tuple_like_merge_t<
        SettingsBundleTemplate,
        OutputContainer,
        convertToBundle_t<Things>...
    >;

    /*! @brief Merges all detected settings bundles and traits into a single settings registry. */
    using toSettingsRegistry = to<SettingsRegistryTemplate>;

    /*! @brief Merges all detected settings bundles and traits into a single settings bundle. */
    using toSettingsBundle = to<SettingsBundleTemplate>;
};

};

#endif // SETTINGSTRAITSCONCATENATOR_H
