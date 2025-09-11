#ifndef MINIMIZEONCLOSEENTRYHANDLER_H
#define MINIMIZEONCLOSEENTRYHANDLER_H

#include "SettingsCheckableEntryHandler.h"

namespace Draupnir::Menus
{

template<class SettingsContext>
class GenericMenuEntryHandler<SettingsContext,MinimizeOnCloseEntry> :
    public SettingsCheckableEntryHandler<SettingsContext,MinimizeOnCloseEntry>
{
public:
    GenericMenuEntryHandler(SettingsContext& context) :
        SettingsCheckableEntryHandler<SettingsContext,MinimizeOnCloseEntry>{context}
    {}
};

}; // namespace Draupnir::Menus

#endif // MINIMIZEONCLOSEENTRYHANDLER_H
