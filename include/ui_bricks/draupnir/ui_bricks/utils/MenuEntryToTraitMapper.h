#ifndef MENUENTRYTOTRAITMAPPER_H
#define MENUENTRYTOTRAITMAPPER_H

#include "draupnir/settings_registry/traits/settings/main_window/MinimizeOnCloseSetting.h"
#include "draupnir/settings_registry/traits/settings/main_window/MinimizeToTraySetting.h"
#include "draupnir/settings_registry/traits/settings/main_window/StartHiddenSetting.h"

#include "draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h"

#define DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(MenuEntry,SettingTrait) \
    template<> \
    struct MapMenuEntry<MenuEntry> { \
        using MappedEntry = MenuEntry; \
        using ToTrait = SettingTrait; \
    };


template<class Entry>
struct MapMenuEntry
{
    using MappedEntry = Entry;
    using ToTrait = void;
};

DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(Draupnir::Ui::StartHiddenMenuEntry,  Draupnir::Settings::MainWindow::StartHiddenSetting);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(Draupnir::Ui::MinimizeToTrayEntry,   Draupnir::Settings::MainWindow::MinimizeToTraySetting);
DRAUPNIR_DEFINE_SETTINGTRAIT_MAPPING(Draupnir::Ui::MinimizeOnCloseEntry,  Draupnir::Settings::MainWindow::MinimizeOnCloseSetting);

#endif // MENUENTRYTOTRAITMAPPER_H
