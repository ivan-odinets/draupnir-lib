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

#include "draupnir/messages/ui/menus/AbstractMessageCategoriesSelectorMenu.h"
#include "draupnir/ui_bricks/core/selectors/FlagsMaskSelectorBase.h"

#include <QEvent>

#include "draupnir/messages/concepts/MessageCategoryTraitConcept.h"

namespace Draupnir::Messages
{

/*! @class MessageCategoriesSelectorMenuTemplate draupnir/messages/ui/menus/MessageCategoriesSelectorMenuTemplate.h
 *  @ingroup Messages
 *  @brief Menu for selecting a compile-time set of message categories.
 *  @tparam MessageCategoryTraits Traits describing the categories exposed by the menu.
 *
 *  @details Creates one checkable action for every supplied category trait and manages their combined mask.
 *
 * @see Draupnir::Messages::MessageCategories
 * @see Draupnir::Ui::FlagsMaskSelectorBase */

template<MessageCategoryTraitConcept... MessageCatrgoryTraits>
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
    /*! @brief Constructs an empty category selector menu.
     *  @param parent Optional pointer to the parent `QWidget`. */
    MessageCategoriesSelectorMenuTemplate(QWidget* parent = nullptr) :
        AbstractMessageCategoriesSelectorMenu{parent}
    {
        _setupUi();
    }

    /*! @brief Constructs a category selector menu with a title.
     *  @param title Menu title.
     *  @param parent Optional pointer to the parent `QWidget`. */
    MessageCategoriesSelectorMenuTemplate(const QString& title, QWidget* parent = nullptr) :
        AbstractMessageCategoriesSelectorMenu{title, parent}
    {
        _setupUi();
    }

    ~MessageCategoriesSelectorMenuTemplate() final = default;

protected:
    /*! @brief Handles menu language changes.
     *  @param event Change event received by the menu. */
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange)
            _Base::retranslateUiElements();

        // Although we are not inheriting directly from QMenu, but from AbstractMessageCategoriesSelectorMenu
        // base class does not override changeEvent(QEvent*) method, so we can call QMenu::changeEvent directly
        QMenu::changeEvent(event);
    }

private:
    /*! @brief Creates actions for all configured message categories. */
    void _setupUi() {
        // MessageCategory is an integer_wrapper-based type, and to pass it as NTTP we need to cast it to the
        // underlying integer explicitly. At least for now.
        (addAction(_Base::template getFlagElement<MessageCatrgoryTraits::value().value()>()), ... );
    };
};

}; // namespace Draupnir::Messages

#endif // MESSAGECATEGORIESSELECTORMENUTEMPLATE_H
