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

#ifndef SUBMENUENTRIES_H
#define SUBMENUENTRIES_H

#include <QString>

class QWidget;

#include <functional>

namespace Draupnir::Menus {

/*! @class CustomMenuEntryTrait draupnir-lib/templateMenus/entries/SubmenuEntries.h
 *  @brief Trait for describing a menu entry that uses a custom menu class. Provides static creation of the custom menu,
 *         passing displayName() and parent as arguments.
 *  @tparam displayNameImpl A callable returning a QString (used for the menu's title).
 *  @tparam CustomMenu The custom menu class type (should have a constructor taking QString). */

template<auto displayNameImpl, class CustomMenu>
class CustomMenuEntryTrait
{
public:
    using Type = CustomMenu;

    /*! @brief Creates a new instance of the custom menu with the display name.
     *  @param parent Optional parent widget.
     *  @return Pointer to newly created CustomMenu. */
    static CustomMenu* createElement(QWidget* parent = nullptr) {
        return new CustomMenu{displayName(), parent};
    }

    /*! @brief Returns the display name for this entry.
     *  @return Localized QString. */
    static QString displayName() {
        return displayNameImpl();
    }
};

/*! @class DynamicCustomMenu draupnir-lib/templateMenus/entries/SubmenuEntries.h
 *  @brief Trait for a menu entry whose custom menu is constructed via a dynamic user-supplied factory.
 *  @tparam displayNameImpl A callable returning a QString (used for the menu's title).
 *  @tparam CustomMenu      The custom menu class type (should match the factory).
 *
 * @note Users must set the menu creator function via setMenuCreator() before using createElement(). This means that
 *       this method MUST be called before creating any MenuTemplate / MenuBarTemplate objects using DynamicCustomMenu
 *       as a template arguments. Otherwise - Q_ASSERT_X will happen in debug. */

template<auto displayNameImpl, class CustomMenu>
class DynamicCustomMenu
{
public:
    using Type = CustomMenu;

    /*! @brief Creates a new custom menu instance using the registered creator function.
     *  @param parent Optional parent widget.
     *  @return Pointer to newly created CustomMenu.
     * @warning If m_creator is not set, this will Q_ASSERT_X in debug and crash in release (nullptr dereference)! */
    static CustomMenu* createElement(QWidget* parent = nullptr) {
        Q_ASSERT_X(m_creator,"DynamicCustomMenu::createElement",
            qPrintable(QLatin1String("Menu creator is not set. CustomMenu=%1; displayName()=%2")
                       .arg(QString::fromUtf8(typeid(CustomMenu).name())).arg(displayName())));
        return m_creator(displayName(),parent);
    }

    /*! @brief Returns the display name for this entry.
     *  @return Localized QString. */
    static QString displayName() {
        return displayNameImpl();
    }

    /*! @brief Sets the factory function to be used for menu creation.
     *  @param creator A callable accepting QString and QWidget* and returning a CustomMenu*.
     * @note This method MUST be called before creating any MenuTemplate / MenuBarTemplate objects using this trait as
     *       one of the template arguments. */
    static void setMenuCreator(std::function<CustomMenu*(const QString&,QWidget*)> creator) {
        m_creator = creator;
    }

private:
    inline static std::function<CustomMenu*(const QString&,QWidget*)> m_creator;
};

template<class... Entries>
class MenuTemplate;

/*! @class MenuTemplateEntry draupnir-lib/templateMenus/entries/SubmenuEntries.h
 *  @brief Trait for a nested sub-menu described by a MenuTemplate with compile-time entries. Provides static creation of
 *         the menu template (as a QMenu subclass) with the given entries.
 *  @tparam displayNameFunction Callable returning a QString (used as menu title).
 *  @tparam Entries...  Variadic trait parameter pack describing menu entries. */

template<auto displayNameFunction, class... Entries>
class MenuTemplateEntry
{
public:
    using Type = MenuTemplate<Entries...>;

    /*! @brief Creates a new MenuTemplate instance (as QMenu) with the given entries and title.
     *  @param parent Optional parent widget.
     *  @return Pointer to the newly created MenuTemplate<Entries...>. */
    static MenuTemplate<Entries...>* createElement(QWidget* parent = nullptr) {
        return new MenuTemplate<Entries...>{displayName(),parent};
    }

    /*! @brief Returns the display name for the menu template.
     *  @return Localized QString. */
    static QString displayName() {
        return displayNameFunction();
    }
};

}; // namespace Draupnir::Menus

#endif // SUBMENUENTRIES_H
