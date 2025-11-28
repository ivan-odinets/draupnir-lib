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

#ifndef MESSAGETYPESSELECTORWIDGETTEMPLATE_H
#define MESSAGETYPESSELECTORWIDGETTEMPLATE_H

#include "draupnir/message_system/ui/widgets/AbstractMessageTypesSelectorWidget.h"

#include "draupnir/message_system/core/MessageTypeSelectorBase.h"    // IWYU pragma: keep

namespace Draupnir::Messages
{

/*! @class MessageTypesSelectorWidgetTemplate draupnir/ui/widgets/MessageTypesSelectorWidgetTemplate.h
 *  @ingroup MessageSystem
 *  @brief This widget implements interface defined in the @ref Draupnir::Messages::AbstractMessageTypesSelectorWidget
 *
 * @todo Extend and improve the documentation for this class. */

template<class... MessageTraits>
class MessageTypesSelectorWidgetTemplate final : public AbstractMessageTypesSelectorWidget
{
public:
    /*! @brief Constructs widget and initializes `QCheckBox` elements for all provided message traits.
     *  @param parent Optional parent widget. */
    explicit MessageTypesSelectorWidgetTemplate(QWidget* parent = nullptr) :
        AbstractMessageTypesSelectorWidget{parent}
    {
        m_typeCheckBoxContainer.forEachUiElement([this](QCheckBox* checkbox){
            mainLayout()->addWidget(checkbox);
        });

        m_typeCheckBoxContainer.onElementInteracted([this](MessageType type, bool isChecked) {
            emit messageTypeVisibilityChanged(type, isChecked);
        });

        m_typeCheckBoxContainer.retranslateUiElements();
    }

    /*! @brief Trivial destructor. */
    ~MessageTypesSelectorWidgetTemplate() final = default;

    /*! @brief Sets all message type selections using a bitmask.
     *  @param config Bitmask of message types to mark as visible.
     * @note This method does not emit signals. */
    void setDisplayedMessageTypesMask(MessageType config) final {
        m_typeCheckBoxContainer.setDisplayedMask(config);
    };

    /*! @brief Returns bitmask of currently selected message types.
     *  @return MessageType bitmask indicating visible types. */
    MessageType displayedMessageTypesMask() const final {
        return m_typeCheckBoxContainer.displayedMask();
    }

    /*! @brief Sets visibility of a specific @ref MessageType.
     *  @param type Message type to update.
     *  @param isSelected Whether the message type should be marked as visible.
     * @note This method does not emit signals. */
    void setMessageTypeDisplayed(MessageType type, bool isSelected) final {
        m_typeCheckBoxContainer.setFlagDisplayed(type, isSelected);
    }

    /*! @brief Returns whether specified @ref MessageType is marked as visible.
     *  @param type Message type to check.
     *  @return True if the corresponding QAction is checked. */
    bool isMessageTypeDisplayed(MessageType type) const final {
        return m_typeCheckBoxContainer.isFlagDisplayed(type);
    };

protected:
    /*! @brief Updates text of all type-specific `QCheckBox` widgets. Called on language change. */
    void retranslateMessageTypeWidgets() final {
        m_typeCheckBoxContainer.retranslateUiElements();
    }

private:
    friend class MessageTypesSelectorWidgetTemplateTest;

    MessageTypeSelectorBase<QCheckBox,MessageTraits...> m_typeCheckBoxContainer;
};

}; // namespace Draupnir::Messages

#endif // MESSAGETYPESSELECTORWIDGETTEMPLATE_H
