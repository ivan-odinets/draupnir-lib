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

#ifndef MESSAGELEVELSSELECTORMENU_H
#define MESSAGELEVELSSELECTORMENU_H

#include <QMenu>

#include "draupnir/ui_bricks/core/selectors/EnumFlagsMaskSelectorBase.h"

#include "draupnir/messages/core/MessageLevels.h"

namespace Draupnir::Messages
{

/*! @class MessageLevelsSelectorMenu draupnir/messages/ui/menus/MessageLevelsSelectorMenu.h
 *  @ingroup Messages
 *  @brief Menu for selecting message severity levels.
 *
 *  @details Provides one checkable action for each supported @ref Draupnir::Messages::MessageLevel::Value and manages their
 *           combined @ref Draupnir::Messages::MessageLevels mask.
 *
 * @see Draupnir::Messages::MessageLevel
 * @see Draupnir::Messages::MessageLevels
 * @see Draupnir::Ui::EnumFlagsMaskSelectorBase */

class MessageLevelsSelectorMenu final :
    public QMenu,
    public Draupnir::Ui::EnumFlagsMaskSelectorBase<MessageLevelsSelectorMenu, QAction, MessageLevels>
{
    Q_OBJECT
private:
    using _Base = Draupnir::Ui::EnumFlagsMaskSelectorBase<MessageLevelsSelectorMenu, QAction, MessageLevels>;

public:
    /*! @brief Constructs a message level selector menu.
     *  @param parent Optional pointer to the parent `QWidget`. */
    MessageLevelsSelectorMenu(QWidget* parent = nullptr);

    /*! @brief Constructs a message level selector menu with a title.
     *  @param title Menu title.
     *  @param parent Parent widget. */
    MessageLevelsSelectorMenu(const QString& title, QWidget* parent = nullptr);
    ~MessageLevelsSelectorMenu() final = default;

signals:
    /*! @brief Emitted when a message level selection state changes.
     *  @param level Message level whose state changed.
     *  @param isChecked New selection state. */
    void flagSelectionChanged(MessageLevel::Value level, bool isChecked);

protected:
    /*! @brief Handles menu language changes.
     *  @param event Change event received by the menu. */
    void changeEvent(QEvent* event) final;

private:
    /*! @brief Creates and configures the message level actions. */
    void _setupUi();
};

} // namespace Draupnir::Messages

#endif // MESSAGELEVELSSELECTORMENU_H
