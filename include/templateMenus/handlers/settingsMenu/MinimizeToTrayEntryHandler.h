#ifndef MINIMIZETOTRAYENTRYHANDLER_H
#define MINIMIZETOTRAYENTRYHANDLER_H

#include "SettingsCheckableEntryHandler.h"

namespace Draupnir::Menus
{

template<class SettingsContext>
class GenericMenuEntryHandler<SettingsContext,MinimizeToTrayEntry> :
    public SettingsCheckableEntryHandler<SettingsContext,MinimizeToTrayEntry>
{
public:
    GenericMenuEntryHandler(SettingsContext& context) :
        SettingsCheckableEntryHandler<SettingsContext,MinimizeToTrayEntry>{context}
    {}
};

};

#endif // MINIMIZETOTRAYENTRYHANDLER_H
