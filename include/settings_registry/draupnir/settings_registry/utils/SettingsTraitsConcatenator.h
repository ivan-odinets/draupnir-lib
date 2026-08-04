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

#ifndef SETTINGSTRAITSCONCATENATOR_H
#define SETTINGSTRAITSCONCATENATOR_H

#include "draupnir/settings_registry/core/SettingsRegistryTemplate.h"
#include "draupnir/utils/tuple_like_merge.h"

namespace Draupnir::Settings
{

/*! @class SettingsTraitsConcatenator draupnir/settings_registry/utils/SettingsTraitsConcatenator.h
 *  @brief Utility class that normalizes a heterogeneous pack of types into settings-related bundles and registries.
 *
 *  @details This class accepts an arbitrary pack of types `Things...`, where each element can be:
 *           - A type that exposes a nested `T::SettingsBundle` which is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate;
 *           - A direct instantiation of @ref Draupnir::Settings::SettingsBundleTemplate;
 *           - A valid setting trait (as determined by @ref Draupnir::Settings::SettingTraitConcept );
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
    /*! @brief Normalizes arbitrary type `T` into a settings bundle type. This template performs the following conversion rules:
     *         - If `T` exposes a nested `T::SettingsBundle` that is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate
     *           - that type is used as the result;
     *         - If `T` itself is an instantiation of @ref Draupnir::Settings::SettingsBundleTemplate — `T` is used as the result;
     *         - If `T` is a valid setting trait (according to @ref SettingTraitConcept) - a single-trait bundle `SettingsBundleTemplate<T>`
     *           is used as the result;
     *         - Otherwise, an empty @ref Draupnir::Settings::SettingsBundleTemplate is used.
     *
     *         These rules allow callers to pass traits, handlers, existing bundles, or unrelated types, and always obtain a well-formed
     *         bundle type as the output. */
    template<
        class T,
        bool = HasNestedSettingsBundle<T>,
        bool = draupnir::utils::is_instantiation_of_v<T,SettingsBundleTemplate>,
        bool = SettingTraitConcept<T>
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

    /*! @brief Specialization: `T` is some random thing. */
    template<class T>
    struct convertToBundle<T, false, false, false> { using type = SettingsBundleTemplate<>; };

public:
    /*! @brief Converts all `Things...` into settings bundles and merges them into a single container.
     *  @tparam OutputContainer A "tuple-like" template that will be used as the final container type. It is expected to have the form
     *          `template<class...> class OutputContainer`.
     *
     *  @details Internally, each `Things` element is transformed via @ref convertToBundle into a concrete `SettingsBundleTemplate<...>`
     *           (possibly empty). All these bundles are then merged together using @ref draupnir::utils::tuple_like_merge_t, with @ref
     *           SettingsBundleTemplate acting as the "input tuple-like" container and `OutputContainer` as the resulting container type.
     *
     *           This allows the same normalized settings information to be materialized either as a `SettingsBundleTemplate`, a
     *           `SettingsRegistryTemplate`, or any other compatible container. */
    template<template <class...> class OutputContainer>
    using to = draupnir::utils::tuple_like_merge_t<
        SettingsBundleTemplate,
        OutputContainer,
        typename convertToBundle<Things>::type...
    >;

    /*! @brief Merges all detected settings bundles and traits into a single settings registry. */
    using toSettingsRegistry = to<SettingsRegistryTemplate>;

    /*! @brief Merges all detected settings bundles and traits into a single settings bundle. */
    using toSettingsBundle = to<SettingsBundleTemplate>;
};

};

#endif // SETTINGSTRAITSCONCATENATOR_H
