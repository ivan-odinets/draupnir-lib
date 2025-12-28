#ifndef SETTINGSMENUTEMPLATE_H
#define SETTINGSMENUTEMPLATE_H

#include "draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h"

#include "draupnir/handlers/settings_menu/SettingsMenuEntriesHandler.h"

namespace Draupnir::Ui
{

template<class... Entries>
class SettingsMenuTemplate
{
public:
    using Type = MenuTemplate<Entries...>;

    static Type* createElement(QWidget* parent = nullptr) {
        return new MenuTemplate<Entries...>{displayName(),parent};
    }

    /*! @brief Returns the display name for the menu template.
     *  @return Localized `QString`. */
    static QString displayName() {
            return QObject::tr("Settings");
    }

    template<class SettingsRegistry>
    using Handler = Draupnir::Handlers::SettingsMenuEntriesHandler<SettingsRegistry,Entries...>;
};

}; // namespace Draupnir::Ui

#endif // SETTINGSMENUTEMPLATE_H
