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

#ifndef DYNAMICCUSTOMMENU_H
#define DYNAMICCUSTOMMENU_H

#include <QString>

class QWidget;

#include <functional>

namespace Draupnir::Ui {

/*! @class DynamicCustomMenu draupnir/ui_bricks/traits/menu_entries/DynamicCustomMenu.h
 *  @ingroup UiBricks
 *  @ingroup MenuTemplates
 *  @brief Trait for a menu entry whose custom menu is constructed via a dynamic user-supplied factory.
 *  @tparam displayNameImpl A callable returning a `QString` (used for the menu's title).
 *  @tparam CustomMenu      The custom menu class type (should match the factory).
 *
 * @note Users must set the menu creator function via setMenuCreator() before using createElement(). This means that
 *       this method MUST be called before creating any MenuTemplate / MenuBarTemplate objects using DynamicCustomMenu
 *       as a template arguments. Otherwise - `Q_ASSERT_X` will happen in debug.
 *
 * @todo Write reasonable documentation and example of tha usage of this template.
 * @todo Add a test to check this functionality. */

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
            qPrintable(QLatin1String("Menu creator is not set. CustomMenu=%1; decltype(this)=%2; displayName()=%3")
                       .arg(QString::fromUtf8(typeid(CustomMenu).name()))
                       .arg(QString::fromUtf8(typeid(DynamicCustomMenu<displayNameImpl,CustomMenu>).name()))
                       .arg(displayName())));
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

}; // namespace Draupnir::Ui

#endif // DYNAMICCUSTOMMENU_H
