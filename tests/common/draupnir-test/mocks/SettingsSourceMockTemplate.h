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

#ifndef SETTINGSSOURCEMOCKTEMPLATE_H
#define SETTINGSSOURCEMOCKTEMPLATE_H

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/settings_registry/concepts/SettingsBundleConcept.h"
#include "draupnir/settings_registry/core/SettingTemplate.h"
#include "draupnir/settings_registry/utils/SettingTraitSerializer.h"
#include "draupnir/utils/type_presence.h"

#include "draupnir-test/mocks/SettingsBackendMockTemplate.h"

namespace Draupnir::Settings
{

/*! @class SettingsSourceMockTemplate draupnir/tests/mocks/SettingsSourceMockTemplate.h
 *  @ingroup TestHelpers
 *  @brief Class to mock @ref Draupnir::Settings::SettingsRegistryTemplate and @ref Draupnir::Settings::SettingsBundleTemplate
 *         instantiations.
 * @todo Refractor this class and @ref SettingsBackendMockTemplate. */

template<class... SettingTraits>
class SettingsSourceMockTemplate
{
    using Backend = SettingsBackendMockTemplate<SettingTraits...>;

    using AbstractSettingsTuple = std::tuple<Draupnir::Settings::SettingTemplate<SettingTraits>...>;

public:
    /*! @brief Returns `true` if the specified SettingTrait is known within this @ref Draupnir::Settings::SettingsSourceMockTemplate. */
    template<Draupnir::Settings::SettingTraitConcept Trait>
    static constexpr bool contains() {
        return draupnir::utils::is_type_in_tuple_v<Draupnir::Settings::SettingTemplate<Trait>,AbstractSettingsTuple>;
    }

    /*! @brief Resets state of the @ref RegistryMockTemplate to the default state. This includes resetting internally stored instances
     *         of the @ref Draupnir::Settings::SettingTemplate instantiations with the default values of the corresponding `SettingTraits...`
     *         traits and writing the same values in the internal @ref Draupnir::Settings::SettingsBackendMockTemplate. */
    void reset() {
        _resetImpl();
    }

    Backend* backend() { return &m_backend; }

    template<Draupnir::Settings::SettingsBundleConcept Bundle>
    Bundle getSettingsBundle() {
        static_assert(Bundle::template canBeFullyPopulatedFrom<SettingsSourceMockTemplate<SettingTraits...>>(),
            "Requested Bundle can not be fully populated by this SettingsSourceMock<SettingTraits...> instance.");

        Bundle result{&m_backend};
        _populateSettingBundle<Bundle>(result);
        return result;
    }

    /*! @brief Gets the value of a specific setting.
     *  @tparam SettingTrait Trait present in the registry.
     *  @return const reference to the setting's stored value. */
    template<Draupnir::Settings::SettingTraitConcept SettingTrait>
    const typename SettingTrait::Value& get() const {
        static_assert(contains<SettingTrait>(),
                      "SettingTrait specified is not registered within this SettingsRegistry.");
        return std::get<Draupnir::Settings::SettingTemplate<SettingTrait>>(m_settings).value;
    }

    /*! @brief Sets and persists a new value for a specific setting.
     *  @tparam SettingTrait Trait present in the registry.
     *  @param value New value to store and persist. */
    template<Draupnir::Settings::SettingTraitConcept SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        static_assert(contains<SettingTrait>(),
                      "SettingTrait specified is not registered within this SettingsRegistry.");
        std::get<Draupnir::Settings::SettingTemplate<SettingTrait>>(m_settings).value = value;
        Draupnir::Settings::SettingTraitSerializer<Backend,SettingTrait>::set(&m_backend, value);
    }

private:
    Backend m_backend;
    AbstractSettingsTuple m_settings;

    template<std::size_t Index>
    struct _TraitForIndex {
        using type = typename std::tuple_element_t<Index,AbstractSettingsTuple>::Trait;
    };

    template<std::size_t Index = 0>
    inline void _resetImpl() {
        using CurrentTrait = _TraitForIndex<Index>::type;

        std::get<Index>(m_settings).value = CurrentTrait::defaultValue();
        Draupnir::Settings::SettingTraitSerializer<
            Backend, CurrentTrait
        >::set(&m_backend, CurrentTrait::defaultValue());

        if constexpr (Index + 1 < std::tuple_size_v<AbstractSettingsTuple>)
            _resetImpl<Index+1>();
    }

    template<class Bundle,std::size_t Index = 0>
    inline void _populateSettingBundle(Bundle& bundle) {
        using Trait = typename _TraitForIndex<Index>::type;

        if constexpr (Bundle::template contains<Trait>()) {
            bundle.registerSetting(&std::get<Draupnir::Settings::SettingTemplate<Trait>>(m_settings));
        }

        if constexpr (Index + 1 < std::tuple_size_v<AbstractSettingsTuple>)
            _populateSettingBundle<Bundle,Index+1>(bundle);
    }
};

} // namespace Draupnir::Settings

#endif // SETTINGSSOURCEMOCKTEMPLATE_H
