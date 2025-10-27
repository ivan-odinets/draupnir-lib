/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MESSAGEVIEWCONFIGDIALOG_H
#define MESSAGEVIEWCONFIGDIALOG_H

#include <QDialog>

#include <QDebug>

class QCheckBox;
class QDialogButtonBox;
class QFormLayout;
class QLabel;
class QVBoxLayout;

#include "draupnir/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageViewConfigDialog draupnir/ui/windows/MessageViewConfigDialog.h
 *  @brief An dialog capable of displaying/editing settings of the MessageListView widget.
 *
 * @todo Write test for this class.
 * @todo Write extended docs for this class.
 * @todo Add support of selecting message parts to be displayed. */

class MessageViewConfigDialog : public QDialog
{
    Q_OBJECT
public:
    /*! @brief Constructor. Initializes internal UI elements. */
    explicit MessageViewConfigDialog(QWidget* parent = nullptr);

    /*! @brief Destructor. Trivial. */
    ~MessageViewConfigDialog() override = default;

    /*! @brief Virtual method implemented in MessageViewConfigDialogTemplate which marks if specified MessageType is selected.
     * @note No signals should be emitted. */
    virtual void displayTypeSelected(MessageType type, bool isSelected) = 0;

    /*! @brief Virtual method implemented in MessageViewConfigDialogTemplate which displays state of message filtering.
     * @note No signals should be emitted. */
    virtual void displayFilterConfig(MessageType config) = 0;

signals:
    /*! @brief This signal is emitted when user checks/unchecks QAction representing certain MessageType. */
    void messageTypeViewChanged(MessageType type, bool state);

protected:
    void addTypeVisibilityCheckBox(QCheckBox* checkBox);

private:
    QVBoxLayout* p_typeSelectorLayout;
    QDialogButtonBox* p_buttons;
};

}; // namespace Draupnir::Messages

#endif // MESSAGEVIEWCONFIGDIALOG_H
