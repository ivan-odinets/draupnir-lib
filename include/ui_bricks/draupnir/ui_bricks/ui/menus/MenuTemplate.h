/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef MENUTEMPLATE_H
#define MENUTEMPLATE_H

#include <QMenu>

#include <QEvent>

#include "draupnir/ui_bricks/core/MenuEntriesContainer.h"

namespace Draupnir::Ui {

/*! @class MenuTemplate draupnir/ui_bricks/ui/menus/MenuTemplate.h
 *  @ingroup MenuTemplates
 *  @brief Strongly-typed, compile-time generic menu class for Qt applications.
 *  @tparam Entries... Variadic parameter pack of menu entry traits/classes, each describing a QMenu/QAction type.
 *
 *  @details MenuTemplate is a variadic-template class designed to automate and unify creation, translation, and access of complex
 *           menu structures in Qt. It aggregates menu entries (QMenu, QAction, or descendants) defined by Entry traits, manages
 *           their lifecycle, and provides strongly-typed API for convenient compile-time and runtime access. */

template<class... Entries>
class MenuTemplate final :
    public QMenu,
    public MenuEntriesContainer<Entries...>
{
public:
    /*! @brief Constructs a menu with a empty title and parent. Automatically populates itself with the entries described by `Entries...`.
     *  @param parent Optional parent widget. */
    explicit MenuTemplate(QWidget* parent = nullptr) :
        QMenu{parent},
        MenuEntriesContainer<Entries...>{}
    {
        MenuEntriesContainer<Entries...>::populateUiElement(this);
    }

    /*! @brief Constructs a menu with a given title and parent. Automatically populates itself with the entries described by `Entries...`.
     *  @param title The menu's title.
     *  @param parent Optional parent widget. */
    explicit MenuTemplate(const QString& title, QWidget* parent = nullptr) :
        QMenu{title, parent},
        MenuEntriesContainer<Entries...>{}
    {
        MenuEntriesContainer<Entries...>::populateUiElement(this);
    }

    /*! @brief Final trivial destructor. */
    ~MenuTemplate() final = default;

protected:
    /*! @brief Qt event handler, automatically retranslates all entry texts when language changes.
     *  @param event The event pointer. */
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange) {
            MenuEntriesContainer<Entries...>::retranslateEntries();
        }
        QMenu::changeEvent(event);
    }

private:
    friend class MenuTemplateTest;
};

} // namespace Draupnir::Ui

#endif // MENUTEMPLATE_H
