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

#ifndef NOTIFICATIONTYPESSELECTORMENUTEMPLATE_H
#define NOTIFICATIONTYPESSELECTORMENUTEMPLATE_H

#include "draupnir/notifications/ui/menus/AbstractNotificationTypesSelectorMenu.h"
#include "draupnir/ui_bricks/core/selectors/FlagsMaskSelectorBase.h"

#include "draupnir/notifications/traits/NoNotificationTrait.h"

namespace Draupnir::Notifications
{

template<class... NotificationTypeTraits>
class NotificationTypesSelectorMenuTemplate :
    public AbstractNotificationTypesSelectorMenu,
    public Draupnir::Ui::FlagsMaskSelectorBase<
        NotificationTypesSelectorMenuTemplate<NotificationTypeTraits...>,
        QAction,
        NotificationTypes,
        Draupnir::Ui::TemplateArgs::FlagTraitsWrapper<NotificationTypeTraits...>,
        Draupnir::Ui::TemplateArgs::MaskTraitsWrapper<NoNotificationTrait>
    >
{
private:
    using _Base = Draupnir::Ui::FlagsMaskSelectorBase<
        NotificationTypesSelectorMenuTemplate<NotificationTypeTraits...>,
        QAction,
        NotificationTypes,
        Draupnir::Ui::TemplateArgs::FlagTraitsWrapper<NotificationTypeTraits...>,
        Draupnir::Ui::TemplateArgs::MaskTraitsWrapper<NoNotificationTrait>
    >;

public:
    NotificationTypesSelectorMenuTemplate(QWidget* parent = nullptr) :
        AbstractNotificationTypesSelectorMenu{parent},
        _Base{NoNotificationTrait::value()}
    { _setupUi(); }

    NotificationTypesSelectorMenuTemplate(const QString& title, QWidget* parent = nullptr) :
        AbstractNotificationTypesSelectorMenu{title, parent},
        _Base{NoNotificationTrait::value()}
    { _setupUi(); }

    ~NotificationTypesSelectorMenuTemplate() final = default;

protected:
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange)
            _Base::retranslateUiElements();

        // Although we are not inheriting directly from QMenu, but from AbstractNotificationTypesSelectorMenu
        // base class does not override changeEvent(QEvent*) method, so we can call QMenu::changeEvent directly
        QMenu::changeEvent(event);
    }

private:
    void _setupUi() {
        // NotificationType is an integer_wrapper-based type, and to pass it as NTTP we need to cast it to the
        // underlying integer explicitly. At least for now.
        (addAction(_Base::template getFlagElement<NotificationTypeTraits::value().value()>()), ... );
    };
};

}; // namespace Draupnir::Notifications

#endif // NOTIFICATIONTYPESSELECTORMENUTEMPLATE_H
