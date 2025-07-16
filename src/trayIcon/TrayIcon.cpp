/*
 **********************************************************************************************************************
 *
 * This file is part of the crypto-stalker project.
 *
 * Copyright (c) 2025 Ivan Odinets <i_odinets@protonmail.com>
 *
 * crypto-stalker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * crypto-stalker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with crypto-stalker. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "TrayIcon.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMenu>

TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon{parent},
    w_trayMenu{new QMenu{QString(),nullptr}}
{
    if (!QSystemTrayIcon::isSystemTrayAvailable() || QSystemTrayIcon::supportsMessages()) {
        qWarning() << "System tray is not avaialble.";
    }

    QSystemTrayIcon::setIcon(qApp->windowIcon());

    w_exitTrayAction = w_trayMenu->addAction(QString());
    w_exitTrayAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(w_exitTrayAction, &QAction::triggered, qApp, &QApplication::exit);
    QSystemTrayIcon::setContextMenu(w_trayMenu);

    _retranslateUi();
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

bool TrayIcon::event(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        _retranslateUi();

    return QSystemTrayIcon::event(event);
}

void TrayIcon::_retranslateUi()
{
    w_exitTrayAction->setText(tr("Exit"));
}
