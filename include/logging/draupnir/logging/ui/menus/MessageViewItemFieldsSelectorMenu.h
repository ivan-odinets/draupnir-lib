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

#include "draupnir/logging/messages/MessageViewItemFields.h"

namespace Draupnir::Logging
{

/*! @class MessageViewItemFieldsSelectorMenu
 *  @ingroup Logging
 *  @brief This is a class.
 * @todo Documentation: Write reasonable documentation. */

class MessageViewItemFieldsSelectorMenu final :
    public QMenu,
    public Draupnir::Ui::EnumFlagsMaskSelectorBase<MessageViewItemFieldsSelectorMenu, QAction, MessageViewItemFields>
{
    Q_OBJECT
private:
    using _Base = Draupnir::Ui::EnumFlagsMaskSelectorBase<MessageViewItemFieldsSelectorMenu, QAction, MessageViewItemFields>;

public:
    MessageViewItemFieldsSelectorMenu(QWidget* parent = nullptr);
    MessageViewItemFieldsSelectorMenu(const QString& title, QWidget* parent = nullptr);
    ~MessageViewItemFieldsSelectorMenu() final = default;

signals:
    void flagSelectionChanged(MessageViewItemField::Value level, bool isChecked);

protected:
    void changeEvent(QEvent* event) final;

private:
    void _setupUi();
};

}; // namespace Draupnir::Logging

#endif // MESSAGEVIEWITEMFIELDSSELECTORMENU_H
