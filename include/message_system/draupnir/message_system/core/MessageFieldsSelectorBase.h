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

#ifndef MESSAGEFIELDSSELECTORBASE_H
#define MESSAGEFIELDSSELECTORBASE_H

#include "draupnir/containers/fixed_map.h"  // IWYU pragma: keep
#include "draupnir/message_system/core/MessageFields.h"
#include "draupnir/ui_bricks/utils/CheckableUiElementHelper.h"

namespace Draupnir::Messages
{

/*! @class MessageFieldsSelectorBase draupnir/message_system/core/MessageFieldsSelectorBase.h
 *  @ingroup MessageSystem
 *  @brief This class is used internally within the draupnir-lib. It will be (hopefully) refractored.
 *
 * @todo Refractor me together with @ref Draupnir::Messages::MessageTypeSelectorBase */

template<class UiElement>
class MessageFieldsSelectorBase
{
public:
    /*! @brief Constructs the selector logic and initializes all UI elements. Individual elements and the "All" selector are
     *         allocated and connected. At construction time, all fields are considered hidden. */
    MessageFieldsSelectorBase() :
        m_displayedMask{MessageField::None},
        w_showAllUiElement{nullptr}
    {
        static_assert(std::is_same_v<UiElement,QAction> || std::is_same_v<UiElement,QCheckBox>,
                "Only QAction and QCheckBox are supported as UiElement");

        // Handle individual checkers
        m_uiElementMap.for_each_pair([this](auto& pair){
            pair.second = Draupnir::Ui::CheckableUiElementHelper<UiElement>::createConnectedUiElement([this,pair](bool isChecked) {
                _handleUiElementUserInteraction(pair.first,isChecked);
            });
        });

        // All Action
        w_showAllUiElement = Draupnir::Ui::CheckableUiElementHelper<UiElement>::createConnectedUiElement([this](bool isChecked){
            _handleShowAllUserInteraction(isChecked);
        });
    }

    /*! @brief This method is used to set the callback function, which will be invoked when user will check / uncheck UiElement
     *         for certain flag. */
    void onElementInteracted(std::function<void(MessageField,bool)> callback) { m_callback = callback; }

    /*! @brief Sets the current mask of visible elements. All UI elements are updated to reflect the new state.
     *  @param mask New mask to set.
     * @note No callback called when calling this method. */
    void setDisplayedMask(MessageFields mask) {
        m_displayedMask = mask;

        m_uiElementMap.for_each_pair([this,mask](auto& pair){
            pair.second->setChecked(mask.testFlag(pair.first));
        });

        _updateShowAllAction();
    }

    /*! @brief Returns the current mask of displayed message fields. */
    MessageFields displayedMask() const { return m_displayedMask; }

    /*! @brief Sets visibility of an individual UiElement. Also updates "All" selector.
     *  @param field Field to update.
     *  @param isShown Whether the field should be visible. */
    void setFlagDisplayed(MessageField field, bool isShown) {
        m_displayedMask.setFlag(field,isShown);
        m_uiElementMap[field]->setChecked(isShown);
        _updateShowAllAction();
    }

    /*! @brief Returns true if the specified field is currently marked as displayed. */
    bool isFlagDisplayed(MessageField field) const { return m_displayedMask.testFlag(field); }

    /*! @brief Returns a pointer to the "All fields" UI element. */
    UiElement* showAllUiElement() { return w_showAllUiElement; }

    /*! @brief Returns a pointer to a UiElement for a specific Message::Fields entry. */
    UiElement* getUiElement(MessageField field) { return m_uiElementMap[field]; }

    /*! @brief Iterates over all field-related UI elements. Allows client code to add them into layouts or menus.
     *  @param callable Function to call with each DisplayUiElement*. */
    void forEachUiElement(std::function<void(UiElement*)> callable) {
        m_uiElementMap.for_each_value(callable);
    }

    /*! @brief Updates UI labels with the current translations. Should be called on `QEvent::LanguageChange`. */
    void retranslateUiElements() {
        w_showAllUiElement->setText(QObject::tr("All"));
        m_uiElementMap.for_each_pair([](auto& pair){
            pair.second->setText(fieldsDisplayString(pair.first));
        });
    }

private:
    friend class MessageListViewConfigMenuTemplateTest;

    /*! @brief Current field visibility mask. */
    MessageFields m_displayedMask;

    /*! @brief "All fields" selector UI element. */
    UiElement* w_showAllUiElement;

    /*! @brief Map from message field → UI element. */
    draupnir::containers::fixed_map<
        individualFieldsArray,
        UiElement*
    > m_uiElementMap;
    std::function<void(MessageField, bool)> m_callback;

    /*! @brief Internal slot handler for a single field UI element being toggled. */
    void _handleUiElementUserInteraction(MessageField field, bool isChecked) {
        m_displayedMask.setFlag(field,isChecked);

        Q_ASSERT(m_callback);
        _updateShowAllAction();
        m_callback(field,isChecked);
    }

    /*! @brief Internal slot handler for the "All" field toggler being changed. */
    void _handleShowAllUserInteraction(bool isChecked) {
        m_displayedMask = (isChecked) ?
            MessageField::All :
            MessageField::None;

        m_uiElementMap.for_each_pair([this,isChecked](auto& pair){
            if (isChecked != pair.second->isChecked()) {
                pair.second->setChecked(isChecked);
                Q_ASSERT(m_callback);
                m_callback(pair.first, isChecked);
            }
        });
    }

    /*! @brief Updates the "All" selector checkbox to match current state. Will be checked only if all individual fields
     *         are selected. */
    void _updateShowAllAction() {
        w_showAllUiElement->setChecked((m_displayedMask == MessageField::All));
    }
};

} // namespace Draupnir::Messages

#endif // MESSAGEFIELDSSELECTORBASE_H
