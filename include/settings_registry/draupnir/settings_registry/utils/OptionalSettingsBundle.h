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

#ifndef OPTIONALSETTINGSBUNDLE_H
#define OPTIONALSETTINGSBUNDLE_H

#include "draupnir/settings_registry/SettingsBundleTemplate.h"

namespace Draupnir::Settings
{

/*! @class OptionalSettingsBundle draupnir/settings_registry/utils/OptionalSettingsBundle.h
 *  @ingroup SettingsRegistry
 *  @brief Compile-time optional wrapper for a settings bundle.
 *  @tparam Bundle    Settings bundle type that satisfies @ref SettingsBundleConcept.
 *  @tparam isEnabled Compile-time flag that controls whether the wrapper provides
 *                    full settings access API or compiles down to an empty type.
 *
 *  @details This class template acts as a conditional holder for a settings bundle:
 *           - When `isEnabled` is `false`, the primary specialization provides an empty type with no data members and no
 *             behavior. In this case, inheriting from @ref OptionalSettingsBundle - will result in using empty base class,
 *             which may be optimized.
 *           - When `isEnabled` is `true`, the enabled specialization stores an instance of `Bundle` and exposes a small
 *             convenience API for loading and accessing settings contained in that bundle.
 *
 *           The enabled specialization offers the following operations:
 *           - `loadSettings(SettingsSource): Loads the bundle from a `SettingsSource`, which may be passed either by value
 *              /reference or as a pointer. At compile time, the call is guarded by a `static_assert` that checks if the
 *              Bundle can be populated from a `SettingsSource`.
 *           - `bundle()`: Returns a non-const reference to the underlying `Bundle` instance.
 *           - `get<SettingTrait>()`: Returns a const reference to the value associated with `SettingTrait`. The presence
 *              of the trait in the bundle is verified at compile time.
 *           - `set<SettingTrait>(...)`: Assigns a new value to the given `SettingTrait` within the stored bundle. As with
 *             `get`, the trait is checked to be a member of the bundle at compile time.
 *
 *           This wrapper is intended to be used in places where the existence of a particular settings bundle is controlled
 *           by compile-time configuration (e.g. feature flags, conditional subsystems) and you want to:
 *           - Pay zero runtime cost when the feature is disabled, and
 *           - Have convenient access to the bundle when it is enabled.  */

template<SettingsBundleConcept Bundle,bool isEnabled>
class OptionalSettingsBundle;

/*! @brief Disabled specialization of @ref OptionalSettingsBundle.
 *
 *  @details This specialization corresponds to `isEnabled = false` and represents a completely empty type. It does not store
 *           a `Bundle` instance and provides no member functions. Instantiating this specialization is effectively free in
 *           terms of runtime cost (subject to the usual rules for empty types and EBO). */

template<SettingsBundleConcept Bundle>
class OptionalSettingsBundle<Bundle,false>
{};

/*! @brief Enabled specialization of @ref OptionalSettingsBundle.
 *
 *  @details This specialization corresponds to `isEnabled` = `true` and stores an instance of `Bundle`. It exposes a small
 *           API for loading the bundle from a settings source and for querying or updating individual setting traits.
 *
 *           The `SettingsSource` used in @ref loadSettings may be provided either as a pointer or as a non-pointer type.
 *           In both cases, the implementation enforces at compile time that the bundle can be fully populated from the given
 *           source type. */

template<SettingsBundleConcept Bundle>
class OptionalSettingsBundle<Bundle,true>
{
public:
    /*! @brief Loads the underlying bundle from the given settings source.
     *  @tparam SettingsSource Type of the source object providing access to settings. May be a pointer or a non-pointer type.
     *  @param source The settings source instance (or pointer to it).
     *  @details At compile time this function verifies that `Bundle` can be fully populated from `SettingsSource`. If the
     *           check fails, compilation is aborted via `static_assert`. */
    template<class SettingsSource>
    void loadSettings(SettingsSource source) {
        if constexpr (std::is_pointer_v<SettingsSource>) {
            using SettingsSorucePointee = std::remove_pointer_t<SettingsSource>;
            static_assert(Bundle::template canBeFullyPopulatedFrom<SettingsSorucePointee>());
            m_settings = source->template getSettingsBundle<Bundle>();
        } else {
            static_assert(Bundle::template canBeFullyPopulatedFrom<SettingsSource>());
            m_settings = source.template getSettingsBundle<Bundle>();
        }
    }

    /*! @brief Provides direct access to the underlying settings bundle.
     *  @return A non-const reference to the stored `Bundle` instance. */
    auto& bundle() { return m_settings; }

    /*! @brief Retrieves the value of a specific setting trait from the bundle.
     *  @tparam SettingTrait A setting trait that must be contained in `Bundle`.
     *  @return A const reference to the value associated with `SettingTrait`.
     *  @details This function performs a compile-time check that `Bundle` contains the specified `SettingTrait`. If the trait
     *           is not part of the bundle, compilation fails with a `static_assert`. */
    template<class SettingTrait>
    const typename SettingTrait::Value& get() const {
        static_assert(Bundle::template contains<SettingTrait>());
        return m_settings.template get<SettingTrait>();
    }

    /*! @brief Updates the value of a specific setting trait in the bundle.
     *  @tparam SettingTrait A setting trait that must be contained in `Bundle`.
     *  @param value New value to assign to the trait.
     *  @details As with @ref get, this function enforces at compile time that `SettingTrait is a member of `Bundle`. The new
     *           value is then forwarded to the underlying bundle's `set` member. */
    template<class SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        static_assert(Bundle::template contains<SettingTrait>());
        m_settings.template set<SettingTrait>(value);
    }

private:
    Bundle m_settings;
};

/*! @class NonEmptySettingsBundle draupnir/settings_registry/utils/OptionalSettingsBundle.h
 *  @ingroup SettingsRegistry
 *  @brief Convenience class that enables an optional bundle only when it is non-empty.
 *  @tparam Bundle Settings bundle type that satisfies @ref SettingsBundleConcept.
 *  @details This helper class derives from @ref OptionalSettingsBundle and automatically enables or disables the underlying
 *           bundle wrapper based on the bundle's own compile-time property `isEmpty_v`.
 *
 *           More specifically, it is defined as:
 *           @code
 *           class NonEmptySettingsBundle<Bundle>
 *               : public OptionalSettingsBundle<Bundle, !Bundle::isEmpty_v>
 *           {};
 *           @endcode
 *
 *           In other words, @ref NonEmptySettingsBundle is a "compile-time opt-in" wrapper: it transparently disables itself
 *           for empty bundles, and behaves like a fully enabled @ref OptionalSettingsBundle otherwise. */

template<SettingsBundleConcept Bundle>
class NonEmptySettingsBundle
    : public OptionalSettingsBundle<Bundle,!Bundle::isEmpty_v>
{};

}; // namespace Draupnir::Settings

#endif // OPTIONALSETTINGSBUNDLE_H
