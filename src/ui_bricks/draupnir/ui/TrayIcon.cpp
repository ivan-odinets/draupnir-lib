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

#include "draupnir/ui_bricks/ui/TrayIcon.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMenu>

namespace Draupnir::Ui
{

TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon{parent},
    w_trayMenu{new QMenu{QString(),nullptr}}
{
    if (!QSystemTrayIcon::isSystemTrayAvailable() || QSystemTrayIcon::supportsMessages()) {
        qWarning() << "System tray is not avaialble.";
    }

    QSystemTrayIcon::setIcon(qApp->windowIcon());
    QSystemTrayIcon::setContextMenu(w_trayMenu);
}

TrayIcon::~TrayIcon()
{
    w_trayMenu->deleteLater();
}

void TrayIcon::addAction(QAction* action)
{
    w_trayMenu->addAction(action);
}

void TrayIcon::addMenu(QMenu* menu)
{
    w_trayMenu->addMenu(menu);
}

};
