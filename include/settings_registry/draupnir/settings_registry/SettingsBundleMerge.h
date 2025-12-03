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

#ifndef SETTINGSBUNDLEMERGE_H
#define SETTINGSBUNDLEMERGE_H

#include <type_traits>

#include "draupnir/settings_registry/SettingsBundleTemplate.h"

/*! @file draupnir/settings_resgistry/SettingsBundleMerge.h
 *  @ingroup SettingsRegistry
 *  @brief This file contains the SettingsBundleMerge utility capable of merging multiple SettingsBundleTemplate types
 *         into one without possible duplicates.
 *
 * @todo Add test for utils within this file. */

namespace Draupnir::Settings
{

/*! @brief Trait to detect presence of nested ::SettingBundle type. */
template<class, class = std::void_t<>>
struct has_settingBundle : std::false_type {};

/*! @brief Specialization: true if T defines nested type ::SettingBundle. */
template<class T>
struct has_settingBundle<T, std::void_t<typename T::SettingsBundle>> : std::true_type {};

/*! @brief Convenience variable template for has_settingBundle. */
template<class T>
inline constexpr bool has_settingBundle_v = has_settingBundle<T>::value;

/*! @brief Extracts ::Settings bundle type from a handler if present, otherwise returns an empty bundle. */
template<class T, bool = has_settingBundle_v<T>>
struct extract_settings { using type = Settings::SettingsBundleTemplate<>; };

/*! @brief Specialization: returns T::SettingsBundle if available. */
template<class T>
struct extract_settings<T, true> { using type = typename T::SettingsBundle; };

/*! @brief Convenience alias for extract_settings. */
template<class T>
using extract_settings_t = typename extract_settings<T>::type;

/*! @brief Append a single trait T into a SettingsBundle if it is not already present. */
template<class Bundle, class T>
struct bundle_push_unique;

/*! @brief Implementation: appends T only if not already in Ts... */
template<class... Ts, class T>
struct bundle_push_unique<Settings::SettingsBundleTemplate<Ts...>, T> {
    using type = std::conditional_t<
        (std::is_same_v<T, Ts> || ...),
        Settings::SettingsBundleTemplate<Ts...>,
        Settings::SettingsBundleTemplate<Ts..., T>
    >;
};

/*! @brief Convenience alias for bundle_push_unique. */
template<class Bundle, class T>
using bundle_push_unique_t = typename bundle_push_unique<Bundle, T>::type;

/*! @brief Append a parameter pack of traits Us... into a SettingsBundle, skipping duplicates. */
template<class Bundle, class... Us>
struct bundle_push_all_unique;

/*! @brief Base case: no traits to push. */
template<class Bundle>
struct bundle_push_all_unique<Bundle> { using type = Bundle; };

/*! @brief Recursive case: push first trait U, then recurse on Us... */
template<class Bundle, class U, class... Us>
struct bundle_push_all_unique<Bundle, U, Us...> {
    using next = bundle_push_unique_t<Bundle, U>;
    using type = typename bundle_push_all_unique<next, Us...>::type;
};

/*! @brief Merge two SettingsBundle types into one, removing duplicates. */
template<class B1, class B2>
struct bundle_merge_unique;

/*! @brief Implementation: merge SettingsBundle<Ts...> and SettingsBundle<Us...>. */
template<class... Ts, class... Us>
struct bundle_merge_unique<Settings::SettingsBundleTemplate<Ts...>, Settings::SettingsBundleTemplate<Us...>> {
    using type = typename bundle_push_all_unique<Settings::SettingsBundleTemplate<Ts...>, Us...>::type;
};

/*! @brief Convenience alias for bundle_merge_unique. */
template<class B1, class B2>
using bundle_merge_unique_t = typename bundle_merge_unique<B1, B2>::type;

/*! @brief Merge an arbitrary number of SettingsBundle types into one, removing duplicates. */
template<class... Bundles>
struct bundle_merge_all;

/*! @brief Base case: empty merge yields empty bundle. */
template<>
struct bundle_merge_all<> { using type = Settings::SettingsBundleTemplate<>; };

/*! @brief Single bundle: returned as is. */
template<class B>
struct bundle_merge_all<B> { using type = B; };

/*! @brief Recursive case: merge first two, then recurse on the rest. */
template<class B1, class B2, class... Rest>
struct bundle_merge_all<B1, B2, Rest...> {
    using type = typename bundle_merge_all<bundle_merge_unique_t<B1, B2>, Rest...>::type;
};

/*! @brief Convenience alias for bundle_merge_all. */
template<class... Bundles>
using bundle_merge_all_t = typename bundle_merge_all<Bundles...>::type;

/*! @brief Collects settings from a list of handler types by extracting and merging their SettingBundle definitions.
 *
 *  Example:
 *  @code
 *  using Bundle = collect_settings_t<
 *      FileOpenEntryHandler,
 *      FileSaveEntryHandler,
 *      FileRecentEntryHandler
 *  >;
 *  @endcode */
template<class... Handlers>
using collect_settings_t =
    bundle_merge_all_t< extract_settings_t<Handlers>... >;

};

#endif // SETTINGSBUNDLEMERGE_H
