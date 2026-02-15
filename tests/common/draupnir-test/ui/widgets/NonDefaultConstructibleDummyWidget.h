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

#ifndef NONDEFAULTCONSTRUCTIBLEDUMMYWIDGET_H
#define NONDEFAULTCONSTRUCTIBLEDUMMYWIDGET_H

#include <QWidget>

class NonDefaultConstructibleDummyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NonDefaultConstructibleDummyWidget(const QString& random, QWidget* parent = nullptr) :
        QWidget{}
    {
        Q_UNUSED(random);
    }
};

#endif // NONDEFAULTCONSTRUCTIBLEDUMMYWIDGET_H
