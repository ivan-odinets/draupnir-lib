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

#ifndef MESSAGECATEGORIESSELECTORMENUTEMPLATE_H
#define MESSAGECATEGORIESSELECTORMENUTEMPLATE_H

#include "draupnir/logging/ui/menus/AbstractMessageCategoriesSelectorMenu.h"
#include "draupnir/ui_bricks/core/selectors/FlagsMaskSelectorBase.h"

#include <QEvent>

namespace Draupnir::Logging
{

template<class... MessageCatrgoryTraits>
class MessageCategoriesSelectorMenuTemplate :
    public AbstractMessageCategoriesSelectorMenu,
    public Draupnir::Ui::FlagsMaskSelectorBase<
        MessageCategoriesSelectorMenuTemplate<MessageCatrgoryTraits...>,
        QAction,
        MessageCategories,
        Draupnir::Ui::TemplateArgs::FlagTraitsWrapper<MessageCatrgoryTraits...>
    >
{
private:
    using _Base = Draupnir::Ui::FlagsMaskSelectorBase<
        MessageCategoriesSelectorMenuTemplate<MessageCatrgoryTraits...>,
        QAction,
        MessageCategories,
        Draupnir::Ui::TemplateArgs::FlagTraitsWrapper<MessageCatrgoryTraits...>
    >;

public:
    MessageCategoriesSelectorMenuTemplate(QWidget* parent = nullptr) :
        AbstractMessageCategoriesSelectorMenu{parent}
    {
        _setupUi();
    }

    MessageCategoriesSelectorMenuTemplate(const QString& title, QWidget* parent = nullptr) :
        AbstractMessageCategoriesSelectorMenu{title, parent}
    {
        _setupUi();
    }

    ~MessageCategoriesSelectorMenuTemplate() final = default;

protected:
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange)
            _Base::retranslateUiElements();

        // Although we are not inheriting directly from QMenu, but from AbstractMessageCategoriesSelectorMenu
        // base class does not override changeEvent(QEvent*) method, so we can call QMenu::changeEvent directly
        QMenu::changeEvent(event);
    }

private:
    void _setupUi() {
        // MessageCategory is an integer_wrapper-based type, and to pass it as NTTP we need to cast it to the
        // underlying integer explicitly. At least for now.
        (addAction(_Base::template getFlagElement<MessageCatrgoryTraits::value().value()>()), ... );
    };
};

}; // namespace Draupnir::Logging

#endif // MESSAGECATEGORIESSELECTORMENUTEMPLATE_H
