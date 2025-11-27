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

#ifndef ABSTRACTMESSAGELISTVIEWCONFIGMENU_H
#define ABSTRACTMESSAGELISTVIEWCONFIGMENU_H

#include <QMenu>
#include <QObject>

#include "draupnir/core/MessageFieldsSelectorBase.h"
#include "draupnir/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class AbstractMessageListViewConfigMenu draupnir/ui/menus/AbstractMessageListViewConfigMenu.h
 *  @ingroup MessageSystem
 *  @brief Abstract part of the menu for configuring how @ref Draupnir::Messages::Message objects will be displayed within
 *         the @ref Draupnir::Messages::MessageListView widget.
 *
 *  @details This menu allows users to select which parts and types of @ref Draupnir::Messages::Message objects
 *           should be shown in the @ref Draupnir::Messages::MessageListView widget.
 *           The class is split into:
 *           - @ref Draupnir::Messages::AbstractMessageListViewConfigMenu — Qt interface layer
 *           - @ref Draupnir::Messages::MessageListViewConfigMenuTemplate — implementation of trait-dependent logic. */

class AbstractMessageListViewConfigMenu : public QMenu
{
    Q_OBJECT
public:
    /*! @brief Trivial destructor. */
    ~AbstractMessageListViewConfigMenu() override = default;

    /*! @brief Updates check state of `QAction` elements representing @ref Draupnir::Messages::Message::Fields.
     *  @param mask Bitmask of fields to be shown. If equal to @ref Draupnir::Messages::Message::Fields::All, all field
     *         checkboxes will be marked.
     *  @details This function sets internal field mask and updates all field-related QAction-s accordingly.
     * @note No signals are emitted after calling this method. */
    void setDisplayedMessageFieldsMask(const std::underlying_type_t<Message::Fields> mask) {
        m_messageFieldsContainer.setDisplayedMask(mask);
    }

    /*! @brief This method returns mask of the parts of the @ref Draupnir::Messages::Message object which are marked as
     *         displayed within the @ref Draupnir::Messages::MessageListView widget. */
    std::underlying_type_t<Message::Fields> displayedMessageFieldsMask() const {
        return m_messageFieldsContainer.displayedMask();
    }

    /*! @brief Sets visibility flag of specific @ref Draupnir::Messages::Message::Fields element.
     *  @param field Field to modify.
     *  @param isShown Whether the field should be marked as visible.
     * @note No signals are emitted after calling this method. */
    void setMessageFieldDisplayed(Message::Fields field, bool isShown) {
        m_messageFieldsContainer.setFlagDisplayed(field, isShown);
    }

    /*! @brief Returns whether specific @ref Draupnir::Messages::Message::Fields is marked as visible.
     *  @param field Field to query. */
    bool isMessageFieldDisplayed(Message::Fields field) const {
        return m_messageFieldsContainer.isFlagDisplayed(field);
    }

    /*! @brief Updates all message type entries based on the given filter bitmask. This method is implemented in @ref
     *         Draupnir::Messages::MessageListViewConfigMenuTemplate.
     *  @param config Bitmask of message types to mark as visible.
     * @note No signals should be emitted. */
    virtual void setDisplayedMessageTypesMask(MessageType config) = 0;

    /*! @brief Returns current visibility mask for @ref Draupnir::Messages::MessageType entries. This method is implemented
     *         in @ref Draupnir::Messages::MessageListViewConfigMenuTemplate.
     * @note No signals should be emitted. */
    virtual MessageType displayedMessageTypesMask() const = 0;

    /*! @brief Marks a specific @ref Draupnir::Messages::MessageType as displayed/hidden. This method is implemented in
     *         @ref Draupnir::Messages::MessageListViewConfigMenuTemplate.
     *  @param type Message type to configure.
     *  @param isSelected True to mark as visible.
     * @note No signals should be emitted. */
    virtual void setMessageTypeDisplayed(MessageType type, bool isSelected) = 0;

    /*! @brief Returns whether the specified @ref Draupnir::Messages::MessageType is currently selected for display. This
     *         method is implemented in @ref Draupnir::Messages::MessageListViewConfigMenuTemplate. */
    virtual bool isMessageTypeDisplayed(MessageType type) const = 0;

signals:
    /*! @brief Emitted when user toggles field visibility via QAction.
     *  @param field The field whose visibility changed.
     *  @param isVisible Whether the field is now visible. */
    void messageFieldVisibilityChanged(Draupnir::Messages::Message::Fields field, bool isVisible);

    /*! @brief Emitted when user toggles visibility of a message type via QAction.
     *  @param messageType The message type being toggled.
     *  @param isVisible New visibility state. */
    void messageTypeVisibilityChanged(Draupnir::Messages::MessageType messageType, bool isVisible);

protected:
    /*! @brief Default constructor. Declared protected, as this class supposed to be inherited from. */
    explicit AbstractMessageListViewConfigMenu(QWidget* parent = nullptr);

protected:
    /*! @brief Handles dynamic retranslation when the application language changes. Updates all visible labels and translated
     *         strings in the UI.
     *  @param event The event being handled.   */
    void changeEvent(QEvent* event) final;

    /*! @brief Adds a QAction representing specific @ref Draupnir::Messages::MessageType to the menu. This is used by the
     *         @ref Draupnir::Messages::MessageListViewConfigMenuTemplate. */
    void addMessageTypeAction(QAction* action);

    /*! @brief Called during language change to update translated text of message-type actions. This method is implemented
     *         within the @ref Draupnir::Messages::MessageListViewConfigMenuTemplate. */
    virtual void retranslateMessageTypeActions() = 0;

private:
    friend class MessageListViewConfigMenuTemplateTest;

    QAction* w_messageFiledsSection;
    MessageFieldsSelectorBase<QAction> m_messageFieldsContainer;

    QAction* w_messageTypesSection;

    void _retranslateUi();
};

}; // namespace Draupnir::Messages

#endif // ABSTRACTMESSAGELISTVIEWCONFIGMENU_H
