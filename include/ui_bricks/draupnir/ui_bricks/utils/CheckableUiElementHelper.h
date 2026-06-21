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
 *  @details This class is used within template-based containers (e.g. @ref Draupnir::Logging::MessageFieldsSelectorBase)
 *           to have an abstract interface for connecting slots to a `QAction` and `QCheckBox` user-triggered signals.
 * @todo Feature: Allow creation QAction / QCheckBox / ... with parent.
 * @todo Feature: Add possibility of conenction to different signals from "checkable" elements.
 * @todo Feature: Add support of QAction-derived and QCheckBox-derived widgets.
 * @todo Question: Do we need here support of QGroupBox?
 * @todo Tests: Add tests for this class. */

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
            static_assert(!std::is_same_v<UiElement, UiElement>,
                "Only QAction or QCheckBox are supported as UiElement.");
            return 0xDEADBEFF;
        }
    }();

    /*! @brief Helper method to create a new checkable UI element.
     *  @return A new checkable UiElement*. */
    static UiElement* createUiElement() {
        UiElement* result = new UiElement{};
        // QCheckBox is checkable by default. For QAction we need to call
        if constexpr (std::is_same_v<UiElement, QAction>)
            result->setCheckable(true);
        return result;
    }

    /*! @brief Helper method to create a new checkable UI element with specified text / label.
     *  @param text Text to be shown on the newly created element
     *  @return A new checkable UiElement*. */
    static UiElement* createUiElement(const QString& text) {
        UiElement* result = createUiElement();
        // Call either QAction::setText ot QCheckBox::setText method
        result->setText(text);
        return result;
    }

    /*! @brief Helper method to conenct provided checkable UI element to a specified callable.
     *  @param callable Slot to call on toggled/triggered(bool).
     *  @return A new checkable UiElement*. */
    template<typename F> requires(std::invocable<F,bool>)
    static auto connectElement(UiElement* element, F&& callable) {
        return QObject::connect(element, singalAddress, std::forward<F>(callable));
    }

    /*! @brief Helper method to create and connect a new checkable UI element.
     *  @param callable Slot to call on toggled/triggered(bool).
     *  @return A new UiElement* properly connected and checkable. */
    template<typename F> requires(std::invocable<F,bool>)
    static UiElement* createConnectedUiElement(F&& callable) {
        UiElement* result = createUiElement();
        connectElement(result, std::forward<F>(callable));
        return result;
    }

    /*! @brief Helper method to create and connect a new checkable UI element with specified text / label.
     *  @param text Text to be shown on the newly created element
     *  @param callable Slot to call on toggled/triggered(bool).
     *  @return A new UiElement* properly connected and checkable. */
    template<typename F> requires(std::invocable<F,bool>)
    static UiElement* createConnectedUiElement(const QString& text, F&& callable) {
        UiElement* result = createUiElement(text);
        connectElement(result, std::forward<F>(callable));
        return result;
    }
};

}; // namespace Draupnir::Ui

#endif // CHECKABLEUIELEMENTHELPER_H
