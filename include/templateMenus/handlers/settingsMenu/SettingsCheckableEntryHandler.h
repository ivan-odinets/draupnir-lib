#ifndef SETTINGSCHECKABLEENTRYHANDLER_H
#define SETTINGSCHECKABLEENTRYHANDLER_H

#include "../AbstractHandlers.h"

namespace Draupnir::Menus
{

template<class SettingsContext,class MenuEntry>
class SettingsCheckableEntryHandler :
    public CheckableActionHandler<SettingsCheckableEntryHandler<SettingsContext,MenuEntry>>
{
public:
    SettingsCheckableEntryHandler(SettingsContext& context) :
        m_context{context}
    {}

    void onSettingLoaded(bool state) {
        CheckableActionHandler<SettingsCheckableEntryHandler<SettingsContext,MenuEntry>>::action()->setChecked(state);
    }

    void onTriggered(bool state) {
        m_context.template set<MenuEntry>(state);
    }

private:
    SettingsContext& m_context;
};

};

#endif // SETTINGSCHECKABLEENTRYHANDLER_H
