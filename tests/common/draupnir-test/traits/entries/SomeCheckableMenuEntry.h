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

#ifndef SOMECHECKABLEMENUENTRY_H
#define SOMECHECKABLEMENUENTRY_H

#include <QAction>

/*! @class SomeCheckableMenuEntry draupnir-test/traits/entries/SomeCheckableMenuEntry.h
 *  @brief This is a checkable test menu entry trait for MenuTemplate. */

class SomeCustomCheckableMenuEntry
{
public:
    using Type = QAction;

    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(), parent};
        result->setCheckable(true);
        return result;
    }

    static QString displayName() { return QObject::tr("Some Random CustomTrait"); }
};

#endif // SOMECHECKABLEMENUENTRY_H
