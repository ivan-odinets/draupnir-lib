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

#ifndef GENERALTRAITS_H
#define GENERALTRAITS_H

#include <QAction>

/*! @file draupnir-lib/templateMenus/entries/HelperEntries.h
 *  @brief Menu action and menu entry trait classes for the draupnir UI framework.
 *  @details This file contains trait classes describing some helper menu elements like separator and sections.
 *           All menu trait classes must provide:
 *           - using Type = QAction (or QMenu, or thier child classes)
 *           - static Type* createElement(QWidget* parent = nullptr)
 *           - static QString displayName() */

namespace Draupnir::Menus {

/*! @class SeparatorEntry draupnir-lib/templateMenus/entries/HelperEntries.h
 *  @brief Menu trait describing a separator entry. Provides a QAction marked as a separator. */

class SeparatorEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a separator QAction.
     *  @param parent Optional parent object.
     *  @return Newly created QAction pointer with separator property set. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{parent};
        result->setSeparator(true);
        return result;
    }

    /*! @brief Returns an empty display name for separator.
     *  @return Empty QString.
     * @todo Remove this method & update templated menus not to use it when we are dealing with the separator. */
    static QString displayName() {
        return QString{};
    }
};

/*! @class SectionEntry draupnir-lib/templateMenus/entries/HelperEntries.h
 *  @brief Menu trait describing a named section separator.
 *  @tparam displayNameImpl - a callable returning QString. */

template<auto displayNameImpl>
class SectionEntry
{
public:
    using Type = QAction;

    /*! @brief Creates a QAction with section name and separator property.
     *  @param parent Optional parent object.
     *  @return Newly created QAction pointer as a named section separator. */
    static QAction* createElement(QWidget* parent = nullptr) {
        QAction* result = new QAction{displayName(),parent};
        result->setSeparator(true);
        return result;
    }

    /*! @brief Returns display name for section (from displayNameImpl).
     *  @return Section title (QString). */
    static QString displayName() {
        return displayNameImpl();
    }
};

}; // namespace Draupnir::Menus

#endif // GENERALTRAITS_H
