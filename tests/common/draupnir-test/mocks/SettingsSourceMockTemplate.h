#ifndef SETTINGSSOURCEMOCKTEMPLATE_H
#define SETTINGSSOURCEMOCKTEMPLATE_H

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/settings_registry/concepts/SettingsBundleConcept.h"
#include "draupnir/settings_registry/core/SettingTemplate.h"
#include "draupnir/settings_registry/utils/SettingTraitSerializer.h"
#include "draupnir/utils/type_presense.h"

#include "draupnir-test/mocks/SettingsBackendMockTemplate.h"

namespace Draupnir::Settings
{

/*! @class SettingsSourceMockTemplate draupnir/tests/mocks/SettingsSourceMockTemplate.h
 *  @ingroup TestHelpers
 *  @brief Class to mock @ref Draupnir::Settings::SettingsRegistryTemplate and @ref Draupnir::Settings::SettingsBundleTemplate
 *         instantiations.
 * @todo Refractor this class and @ref SettingsBackendMockTemplate. */

template<Draupnir::Settings::SettingTraitConcept... SettingTraits>
class SettingsSourceMockTemplate
{
    using Backend = SettingsBackendMockTemplate<SettingTraits...>;

    using AbstractSettingsTuple = std::tuple<Draupnir::Settings::SettingTemplate<SettingTraits>...>;

public:
    template<Draupnir::Settings::SettingTraitConcept Trait>
    static constexpr bool contains() {
        return draupnir::utils::is_type_in_tuple_v<Draupnir::Settings::SettingTemplate<Trait>,AbstractSettingsTuple>;
    }

    void loadSettings() {
        _loadSettingsImpl();
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
    inline void _loadSettingsImpl() {
        std::get<Index>(m_settings).value = Draupnir::Settings::SettingTraitSerializer<
            Backend, typename _TraitForIndex<Index>::type
        >::get(&m_backend);

        if constexpr (Index + 1 < std::tuple_size_v<AbstractSettingsTuple>)
            _loadSettingsImpl<Index+1>();
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

}; // namespace Draupnir::Settings

#endif // SETTINGSSOURCEMOCKTEMPLATE_H
