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

#include "draupnir/utils/ResourceHelper.h"

#include <QDebug>

namespace Draupnir::Resources
{

ResourceHelper::ResourceHelper() :
    m_draupnirIcon{_readIcon("draupnir-icon-no-code.png")}
{}

QString ResourceHelper::aboutDraupnirLibText() const
{
    return QObject::tr(
        "<h2>About draupnir-lib</h2>"
        "<p>This application uses draupnir-lib version " DRAUPNIR_LIB_VERSION "</p>"
        "<p>draupnir-lib is a modular C++/Qt library designed to simplify the development of structured applications. It is not "
        "(yet) fully polished out-of-the-box solution, but more like growing internal library, somewhere between early alpha and alpha."
        "<p>draupnir-lib can be downloaded from <a href=\"https://github.com/ivan-odinets/draupnir-lib\">github</a>.</p>"
        "<p>Special thanks to the ChatGPT (4o, 5, 4.1 and others) for writing documentation, fixing stupid (and not only stupid) bugs "
        "and for explaining template-related compiler errors.</p>"
    );
}

QIcon ResourceHelper::_readIcon(const QString& name)
{
    QIcon result;

    for (const int size : sizes) {
        QString fileName = QString(":/draupnir/icons/%1x%1/%2").arg(size).arg(name);
        if (QFileInfo::exists(fileName)) {
            result.addFile(fileName);
        }
    }

    return result;
}

};
