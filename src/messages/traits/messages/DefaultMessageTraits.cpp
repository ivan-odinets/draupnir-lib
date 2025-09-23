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

#include "traits/messages/DefaultMessageTraits.h"

#include <QApplication>
#include <QMessageBox>
#include <QStyle>
#include <QSystemTrayIcon>

namespace Draupnir::Messages
{

const QIcon& DebugMessageTrait::icon()
{
    static const QIcon icon;
    return icon;
}

const QIcon& ErrorMessageTrait::icon()
{
    static const QIcon icon = qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical);
    return icon;
}

const QIcon& WarningMessageTrait::icon()
{
    static const QIcon icon = qApp->style()->standardIcon(QStyle::SP_MessageBoxWarning);
    return icon;
};

const QIcon& InfoMessageTrait::icon()
{
    static const QIcon icon = qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation);
    return icon;
};

}; // namespace Draupnir::Messages
