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

#ifndef CLOSURECONFIRMATION_H
#define CLOSURECONFIRMATION_H

#include <concepts>

#include <QApplication>
#include <QMessageBox>

namespace Draupnir::Ui::MainWindow
{

template<class Candidate>
concept CanConfirmWindowClosure = requires(Candidate c) {
    { c.canWindowBeClosed() } -> std::same_as<bool>;
};

class DefaultClosureConfirmer
{
public:
    bool canWindowBeClosed() {
        QMessageBox messageBox{qApp->activeWindow()};
        messageBox.setWindowTitle(qApp->applicationName());
        messageBox.setText(QObject::tr("Are you sure want to continue?"));
        messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        return (messageBox.exec() == QMessageBox::Ok);
    }
};

template<CanConfirmWindowClosure Confirmer = DefaultClosureConfirmer>
struct ClosureConfirmation
{
    Confirmer state;
};

};

#endif // CLOSURECONFIRMATION_H
