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

#ifndef RESOURCEHELPER_H
#define RESOURCEHELPER_H

#include <QDebug>
#include <QFileInfo>
#include <QIcon>

namespace Draupnir::Resources
{

/*! @class ResourceHelper draupnir/utils/ResourceHelper.h
 *  @ingroup DraupnirResources
 *  @brief This singltone class is used to provide easy and centralized access to resources of the DrupnirLib
 *
 * @todo Allow usage of this class or at least provide some similar class for end-user (to simplify loading
 *       and using resources. */

class ResourceHelper
{
public:
    /*! @brief Access point. */
    static ResourceHelper& get() {
        static ResourceHelper theOne;
        return theOne;
    }

    /*! @brief Returns icon of the draupnir-lib. To be used within About draupnir lib action and about draupnir lib dialog. */
    const QIcon& draupnirIcon() const { return m_draupnirIcon; }

    /*! @brief Returns localized string containing html-formatted text within the About Draupnir Lib dialog. */
    QString aboutDraupnirLibText() const;

private:
    ResourceHelper();

    static QIcon _readIcon(const QString& name);
    static int constexpr sizes[] = {16, 24, 32, 64, 128, 256, 512 };

    const QIcon m_draupnirIcon;
};

};

#endif // RESOURCEHELPER_H
