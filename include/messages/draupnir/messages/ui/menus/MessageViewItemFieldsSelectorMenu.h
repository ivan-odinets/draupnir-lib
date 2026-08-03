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

#ifndef MESSAGEVIEWITEMFIELDSSELECTORMENU_H
#define MESSAGEVIEWITEMFIELDSSELECTORMENU_H

#include <QMenu>

#include "draupnir/ui_bricks/core/selectors/EnumFlagsMaskSelectorBase.h"

#include "draupnir/messages/core/MessageViewItemFields.h"

namespace Draupnir::Messages
{

/*! @class MessageViewItemFieldsSelectorMenu draupnir/messages/ui/menus/MessageViewItemFieldsSelectorMenu.h
 *  @ingroup Messages
 *  @brief Menu for selecting visible message view-item fields.
 *
 *  @details Provides checkable actions for the flags represented by @ref Draupnir::Messages::MessageViewItemFields.
 *
 *           Changes to individual field selections are reported through @ref flagSelectionChanged.
 * @see Draupnir::Ui::EnumFlagsMaskSelectorBase */

class MessageViewItemFieldsSelectorMenu final :
    public QMenu,
    public Draupnir::Ui::EnumFlagsMaskSelectorBase<MessageViewItemFieldsSelectorMenu, QAction, MessageViewItemFields>
{
    Q_OBJECT
private:
    /*! @brief Base selector implementation used by this menu. */
    using _Base = Draupnir::Ui::EnumFlagsMaskSelectorBase<MessageViewItemFieldsSelectorMenu, QAction, MessageViewItemFields>;

public:
    /*! @brief Constructs an untitled field-selector menu.
     *  @param parent Optional parent widget. */
    MessageViewItemFieldsSelectorMenu(QWidget* parent = nullptr);

    /*! @brief Constructs a field-selector menu with the specified title.
     *  @param title Menu title.
     *  @param parent Optional parent widget. */
    MessageViewItemFieldsSelectorMenu(const QString& title, QWidget* parent = nullptr);

    /*! @brief Destroys the selector menu. */
    ~MessageViewItemFieldsSelectorMenu() final = default;

signals:
    /*! @brief Emitted when the selection state of a field changes.
     *  @param field Field whose selection state changed.
     *  @param isChecked New selection state. */
    void flagSelectionChanged(MessageViewItemField::Value field, bool isChecked);

protected:
    /*! @brief Handles dynamic retranslation of the menu.
     *  @param event Event being processed. */
    void changeEvent(QEvent* event) final;

private:
    /*! @brief Creates and configures the menu actions. */
    void _setupUi();
};

} // namespace Draupnir::Messages

#endif // MESSAGEVIEWITEMFIELDSSELECTORMENU_H
