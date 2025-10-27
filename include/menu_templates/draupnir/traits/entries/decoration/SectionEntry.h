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

#ifndef SECTIONENTRY_H
#define SECTIONENTRY_H

#include <QAction>

namespace Draupnir::Menus {

/*! @class SectionEntry draupnir/traits/entries/decoration/SectionEntry.h
 *  @ingroup MenuTemplates
 *  @brief Trait for the named section separator.
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

}; // nameserver Derupnir::Menus

#endif // SECTIONENTRY_H
