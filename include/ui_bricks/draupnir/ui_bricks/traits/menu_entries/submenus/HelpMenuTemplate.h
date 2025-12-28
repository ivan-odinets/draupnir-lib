#ifndef HELPMENUTEMPLATE_H
#define HELPMENUTEMPLATE_H

#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"

#include "draupnir/handlers/help_menu/HelpMenuEntriesHandler.h"

namespace Draupnir::Ui
{

template<class... Entries>
class HelpMenuTemplate
{
public:
    using Type = MenuTemplate<Entries...>;

    static Type* createElement(QWidget* parent = nullptr) {
        return new MenuTemplate<Entries...>{displayName(),parent};
    }

    /*! @brief Returns the display name for the menu template.
     *  @return Localized `QString`. */
    static QString displayName() {
        return QObject::tr("Help");
    }

    template<class HelpSource>
    using Handler = Draupnir::Handlers::HelpMenuEntriesHandler<HelpSource,Entries...>;
};

}; // namespace Draupnir::Ui

#endif // HELPMENUTEMPLATE_H
