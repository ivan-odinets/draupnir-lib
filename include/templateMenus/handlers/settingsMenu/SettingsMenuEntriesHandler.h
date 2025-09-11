#ifndef SETTINGSMENUENTRIESHANDLER_H
#define SETTINGSMENUENTRIESHANDLER_H

#include "../GenericMenuEntriesHandler.h"
#include "../../entries/HelperEntries.h"
#include "SettingsEntriesHandlerContext.h"

namespace Draupnir::Menus
{

template<class Registry, class... Entries>
class SettingsMenuEntriesHandler :
        public GenericMenuEntriesHandler<SettingsEntriesHandlerContext<Registry>,Entries...>
{
    template<class T>
    struct is_CheckableEntry : std::false_type {};

    template<auto F>
    struct is_CheckableEntry<CheckableEntry<F>> : std::true_type {};

    template<class T>
    static constexpr bool is_CheckableEntry_v = is_CheckableEntry<T>::value;

public:
    void setRegistry(Registry* registry) {
        GenericMenuEntriesHandler<SettingsEntriesHandlerContext<Registry>,Entries...>::context().setRegistry(registry);

        _displayRegistryState<Entries...>(registry);
    }

private:
    template<class First, class... Rest>
    void _displayRegistryState(Registry* registry) {
        // Check if First entry handled by this SettingsMenuEntriesHandler is handled by the SettingsRegistry.
        if constexpr (Registry::template containsSettingForMenuEntry<First>()) {
            // If yes, check if this Entry is checkable
            if constexpr (is_CheckableEntry_v<First>) {
                auto& handler = GenericMenuEntriesHandler<SettingsEntriesHandlerContext<Registry>,Entries...>::template getHandler<First>();
                handler.onSettingLoaded(registry->template getSettingForMenuEntry<First>());
            }
        }

        if constexpr (sizeof...(Rest) > 0)
            _displayRegistryState<Rest...>(registry);
    }
};

};

#endif // SETTINGSMENUENTRIESHANDLER_H
