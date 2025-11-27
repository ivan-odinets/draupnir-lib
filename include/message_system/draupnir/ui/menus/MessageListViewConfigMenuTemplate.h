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

#ifndef MESSAGELISTVIEWCONFIGMENUTEMPLATE_H
#define MESSAGELISTVIEWCONFIGMENUTEMPLATE_H

#include "draupnir/ui/menus/AbstractMessageListViewConfigMenu.h"

#include "draupnir/core/MessageTypeSelectorBase.h"    // IWYU pragma: keep

namespace Draupnir::Messages {

/*! @class MessageListViewConfigMenuTemplate draupnir/ui/menus/MessageListViewConfigMenuTemplate.h
 *  @ingroup MessageSystem
 *  @brief Template-based implementation of @ref AbstractMessageListViewConfigMenu with customizable message types.
 *
 *  @details This class implements template-dependant methods from the @ref Draupnir::Messages::AbstractMessageListViewConfigMenu. */

template<class... MessageTraits>
class MessageListViewConfigMenuTemplate final : public AbstractMessageListViewConfigMenu
{
public:
    /*! @brief Constructs menu and initializes actions for all provided message traits.
     *  @param parent Optional parent widget. */
    explicit MessageListViewConfigMenuTemplate(QWidget* parent = nullptr) :
        AbstractMessageListViewConfigMenu{parent}
    {
        // Add UiElements
        m_typeActionsContainer.forEachUiElement([this](QAction* action){
            AbstractMessageListViewConfigMenu::addAction(action);
        });
        m_typeActionsContainer.retranslateUiElements();
        // Set callbacks
        m_typeActionsContainer.onElementInteracted([this](MessageType type, bool isChecked) {
            emit messageTypeVisibilityChanged(type, isChecked);
        });
    }

    /*! @brief Trivial destructor. */
    ~MessageListViewConfigMenuTemplate() final = default;

    /*! @brief Sets all message type selections using a bitmask.
     *  @param config Bitmask of message types to mark as visible.
     * @note This method does not emit signals. */
    void setDisplayedMessageTypesMask(MessageType config) final {
        m_typeActionsContainer.setDisplayedMask(config);
    };

    /*! @brief Returns bitmask of currently selected message types.
     *  @return MessageType bitmask indicating visible types. */
    MessageType displayedMessageTypesMask() const final {
        return m_typeActionsContainer.displayedMask();
    }

    /*! @brief Sets visibility of a specific @ref MessageType.
     *  @param type Message type to update.
     *  @param isSelected Whether the message type should be marked as visible.
     * @note This method does not emit signals. */
    void setMessageTypeDisplayed(MessageType type, bool isSelected) final {
        m_typeActionsContainer.setFlagDisplayed(type, isSelected);
    }

    /*! @brief Returns whether specified @ref MessageType is marked as visible.
     *  @param type Message type to check.
     *  @return True if the corresponding QAction is checked. */
    bool isMessageTypeDisplayed(MessageType type) const final {
        return m_typeActionsContainer.isFlagDisplayed(type);
    };

protected:
    /*! @brief Updates text of all type-specific QAction items. Called on language change. */
    void retranslateMessageTypeActions() {
        m_typeActionsContainer.retranslateUiElements();
    }

private:
    friend class MessageListViewConfigMenuTemplateTest;

    MessageTypeSelectorBase<QAction,MessageTraits...> m_typeActionsContainer;
};

}; // namespace Draupnir::Messages

#endif // MESSAGELISTVIEWCONFIGMENUTEMPLATE_H
