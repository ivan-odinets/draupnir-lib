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

#ifndef MESSAGETYPESELECTORBASE_H
#define MESSAGETYPESELECTORBASE_H

#include "draupnir/containers/fixed_map.h"  // IWYU pragma: keep
#include "draupnir/message_system/core/MessageType.h"
#include "draupnir/ui_bricks/utils/CheckableUiElementHelper.h"

namespace Draupnir::Messages
{

/*! @class MessageTypeSelectorBase
 *  @ingroup MessageSystem
 *  @brief This class is used internally within the draupnir-lib. It will be (hopefully) refractored.
 *
 * @todo Refractor me together with @ref Draupnir::Messages::MessageFieldsSelectorBase. */

template<class UiElement, class... MessageTraits>
class MessageTypeSelectorBase
{
public:
    MessageTypeSelectorBase() {
        static_assert(std::is_same_v<UiElement,QAction> || std::is_same_v<UiElement,QCheckBox>,
                "Only QAction and QCheckBox are supported as DisplayUiElement");

        // Handle individual Field checkers
        m_uiElementMap.for_each_pair([this](auto& pair){
            pair.second = Draupnir::Ui::CheckableUiElementHelper<UiElement>::createConnectedUiElement([this,pair](bool isChecked) {
                m_callback(pair.first,isChecked);
            });
        });
    }

    /*! @brief This method is used to set the callback function, which will be invoked when user will check / uncheck UiElement
     *         for certain flag. */
    void onElementInteracted(std::function<void(MessageType,bool)> callback) { m_callback = callback; }

    /*! @brief Sets all message type selections using a bitmask.
     *  @param config Bitmask of message types to mark as visible. */
    void setDisplayedMask(MessageType config) {
        m_uiElementMap.for_each_pair([config](auto& pair){
            pair.second->setChecked(pair.first & config);
        });
    };

    /*! @brief Returns bitmask of currently selected message types.
     *  @return MessageType bitmask indicating visible types. */
    MessageType displayedMask() const {
        quint64 result = MessageType::None;
        m_uiElementMap.for_each_pair([&result](auto& pair) {
            if (pair.second->isChecked())
                result |= pair.first;
        });
        return result;
    }

    /*! @brief Sets visibility of a specific @ref MessageType.
     *  @param type Message type to update.
     *  @param isSelected Whether the message type should be marked as visible. */
    void setFlagDisplayed(MessageType type, bool isSelected) {
        m_uiElementMap[type]->setChecked(isSelected);
    }

    /*! @brief Returns whether specified @ref MessageType is marked as visible.
     *  @param type Message type to check.
     *  @return True if the corresponding UiElement is checked. */
    bool isFlagDisplayed(MessageType type) const {
        return m_uiElementMap[type]->isChecked();
    };

    UiElement* getUiElement(MessageType type) { return m_uiElementMap[type]; }

    void forEachUiElement(std::function<void(UiElement*)> callable) {
        m_uiElementMap.for_each_value(callable);
    }

    void retranslateUiElements() {
        _retranslateUiElementsImpl<MessageTraits...>();
    }

private:
    friend class MessageListViewConfigMenuTemplateTest;

    /*! @brief Static array of supported MessageType values (extracted from traits). */
    static constexpr MessageType supportedMessageIds[] = { MessageTraits::type... };

    /*! @brief Map of MessageType to their corresponding QAction. */
    draupnir::containers::fixed_map<
        supportedMessageIds,
        UiElement*
    > m_uiElementMap;

    std::function<void(MessageType,bool)> m_callback;

    template<class First, class... Rest>
    inline void _retranslateUiElementsImpl() {
        m_uiElementMap[First::type]->setText(
            First::displayName()
        );

        if constexpr (sizeof...(Rest) > 0)
            _retranslateUiElementsImpl<Rest...>();
    }

};

};

#endif // MESSAGETYPESELECTORBASE_H
