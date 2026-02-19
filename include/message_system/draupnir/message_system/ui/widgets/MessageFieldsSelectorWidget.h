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

#ifndef MESSAGEFIELDSSELECTORWIDGET_H
#define MESSAGEFIELDSSELECTORWIDGET_H

#include <QWidget>

#include "draupnir/message_system/core/MessageFieldsSelectorBase.h"

namespace Draupnir::Messages
{

/*! @class MessageFieldsSelectorWidget draupnir/message_system/ui/widgets/MessageFieldsSelectorWidget.h
 *  @ingroup MessageSystem
 *  @brief Widget for selecting which parts of a @ref Draupnir::Messages::Message should be displayed.
 *
 *  @details This widget provides a group of checkboxes allowing the user to enable or disable the visibility of individual
 *           @ref Draupnir::Messages::Message::Fields within the @ref Draupnir::Messages::MessageListView component.
 *
 *           The widget internally uses a template helper class, @ref Draupnir::Messages::MessageFieldsSelectorBase, to manage
 *           state, callbacks and logic.
 *
 * @note Calling setter methods does not emit signals. Signals are only emitted on direct user interaction. */

class MessageFieldsSelectorWidget final : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Adds nested `QCheckBox` elements to the widget from the MessageFieldsSelectorBase. */
    explicit MessageFieldsSelectorWidget(QWidget* parent = nullptr);

    /*! @brief Final trivial destructor. */
    ~MessageFieldsSelectorWidget() final = default;

    /*! @brief Updates check state of the nested `QCheckBox` items representing @ref Draupnir::Messages::Message::Fields.
     *  @param mask Bitmask of fields to be shown. If equal to @ref Draupnir::Messages::Message::Fields::All, all field
     *         checkboxes will be marked.
     * @note No signals are emitted after calling this method. */
    void setDisplayedMessageFieldsMask(const MessageFields mask) {
        m_messageFieldsContainer.setDisplayedMask(mask);
    }

    /*! @brief This method returns mask of the parts of the @ref Draupnir::Messages::Message object which are marked as
     *         displayed within the @ref Draupnir::Messages::MessageListView widget. */
    MessageFields displayedMessageFieldsMask() const {
        return m_messageFieldsContainer.displayedMask();
    }

    /*! @brief Sets visibility flag of specific @ref Draupnir::Messages::Message::Fields element.
     *  @param field Field to modify.
     *  @param isShown Whether the field should be marked as visible.
     * @note No signals are emitted after calling this method. */
    void setMessageFieldDisplayed(MessageField field, bool isShown) {
        m_messageFieldsContainer.setFlagDisplayed(field, isShown);
    }

    /*! @brief Returns whether specific @ref Draupnir::Messages::Message::Fields is marked as visible.
     *  @param field Field to query.
     *  @return `true` if field is visible. */
    bool isMessageFieldDisplayed(MessageField field) const {
        return m_messageFieldsContainer.isFlagDisplayed(field);
    }

signals:
    /*! @brief Emitted when user toggles field visibility via QAction.
     *  @param field The field whose visibility changed.
     *  @param isVisible Whether the field is now visible. */
    void messageFieldVisibilityChanged(Draupnir::Messages::MessageField field, bool isVisible);

protected:
    /*! @brief Handles retranslation of the UI Elements. */
    void changeEvent(QEvent* event) final;

private:
    friend class MessageFieldsSelectorWidgetTest;

    MessageFieldsSelectorBase<QCheckBox> m_messageFieldsContainer;
};

};

#endif // MESSAGEFIELDSSELECTORWIDGET_H
