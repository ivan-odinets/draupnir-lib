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

#ifndef EXAMPLEHELPSOURCE_H
#define EXAMPLEHELPSOURCE_H

#include <QMessageBox>

#include "draupnir/utils/ResourceHelper.h"

class ExampleHelpSource
{
public:
    static QString aboutAppText() {
        return QObject::tr("This is an example of draupnir-lib usage.");
    }

    static QDialog* createHelpDialog() {
        QMessageBox* msg = new QMessageBox;
        msg->setIconPixmap(Draupnir::Resources::ResourceHelper::get().draupnirIcon().pixmap(64,64));
        msg->setText(QObject::tr("This should be a help dialog. But its empty."));
        return msg;
    }
};

#endif // EXAMPLEHELPSOURCE_H
