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

#ifndef CHECKABLEUIELEMENTHELPER_H
#define CHECKABLEUIELEMENTHELPER_H

#include <QAction>
#include <QCheckBox>

namespace Draupnir::Ui
{

/*! @class CheckableUiElementHelper draupnit/ui_bricks/utils/CheckableUiElementHelper.h
 *  @ingroup UiBricks
 *  @brief This is a helper class used to create checkable UI elements (`QAction` or `QCheckBox`) which are connected to
 *         a specific callback using a generic interface.
 *  @tparam UiElement class of the UI element to be created / handled. Should be either `QAction` or `QCheckBox`.
 *
 *  @details This class is used within template-based containers (e.g. @ref Draupnir::Messages::MessageFieldsSelectorBase)
 *           to have an abstract interface for connecting slots to a `QAction` and `QCheckBox` user-triggered signals. */

template<class UiElement>
class CheckableUiElementHelper
{
public:
    /*! @brief Deduces the correct signal to connect to for `QAction`/`QCheckBox`. */
    static inline constexpr auto singalAddress = [] {
        if constexpr (std::is_same_v<UiElement, QAction>) {
            return &QAction::triggered;
        } else if constexpr (std::is_same_v<UiElement, QCheckBox>) {
            return &QCheckBox::clicked;
        } else {
            static_assert(!std::is_same_v<UiElement,UiElement>,
                "Only QAction or QCheckBox are supported as UiElement.");
            return 0xDEADBEFF;
        }
    }();

    /*! @brief Helper method to create and connect a new checkable UI element.
     *  @param callable Slot to call on toggled/triggered(bool).
     *  @return A new DisplayUiElement* properly connected and checkable.
     * @todo Create a universal template-based analogue of this method. */
    template<typename F>
    static UiElement* createConnectedUiElement(/*std::function<void(bool)>*/F callable) {
        UiElement* result = new UiElement{};
        if constexpr (std::is_same_v<UiElement,QAction>)
            result->setCheckable(true);

        QObject::connect(result, singalAddress, callable);

        return result;
    }
};

}; // namespace Draupnir::Ui

#endif // CHECKABLEUIELEMENTHELPER_H
