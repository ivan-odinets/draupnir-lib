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

#ifndef ABSTRACTMESSAGETYPESSELECTORWIDGET_H
#define ABSTRACTMESSAGETYPESSELECTORWIDGET_H

#include <QWidget>

#include <QEvent>
#include <QVBoxLayout>

#include "draupnir/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class AbstractMessageTypesSelectorWidget draupnir/ui/widgets/AbstractMessageTypesSelectorWidget.h
 *  @ingroup MessageSystem
 *  @brief This widget provides interface allowing user to select various MessageType instances.
 *
 * @todo Extend and improve the documentation for this class. */

class AbstractMessageTypesSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Updates all message type entries based on the given filter bitmask. This method is implemented in @ref
     *         Draupnir::Messages::MessageTypesSelectorWidgetTemplate.
     *  @param config Bitmask of message types to mark as visible.
     * @note No signals should be emitted. */
    virtual void setDisplayedMessageTypesMask(MessageType config) = 0;

    /*! @brief Returns current visibility mask for @ref Draupnir::Messages::MessageType entries. This method is implemented
     *         in @ref Draupnir::Messages::MessageTypesSelectorWidgetTemplate.
     * @note No signals should be emitted. */
    virtual MessageType displayedMessageTypesMask() const = 0;

    /*! @brief Marks a specific @ref Draupnir::Messages::MessageType as displayed/hidden. This method is implemented in
     *         @ref Draupnir::Messages::MessageTypesSelectorWidgetTemplate.
     *  @param type Message type to configure.
     *  @param isSelected True to mark as visible.
     * @note No signals should be emitted. */
    virtual void setMessageTypeDisplayed(MessageType type, bool isSelected) = 0;

    /*! @brief Returns whether the specified @ref Draupnir::Messages::MessageType is currently selected for display. This
     *         method is implemented in @ref Draupnir::Messages::MessageTypesSelectorWidgetTemplate. */
    virtual bool isMessageTypeDisplayed(MessageType type) const = 0;

signals:
    /*! @brief Emitted when user toggles visibility of a message type via QCheckBox.
     *  @param messageType The message type being toggled.
     *  @param isVisible New visibility state. */
    void messageTypeVisibilityChanged(Draupnir::Messages::MessageType messageType, bool isVisible);

protected:
    explicit AbstractMessageTypesSelectorWidget(QWidget* parent = nullptr) :
        QWidget{parent},
        p_mainLayout{new QVBoxLayout}
    {
        setLayout(p_mainLayout);
    }

    void changeEvent(QEvent* event) override {
        if (event->type() == QEvent::LanguageChange) {
            retranslateMessageTypeWidgets();
        }

        QWidget::changeEvent(event);
    }

    virtual void retranslateMessageTypeWidgets() = 0;

    QVBoxLayout* mainLayout() { return p_mainLayout; }

private:
    QVBoxLayout* p_mainLayout;
};

}; // namespace Draupnir::Messages

#endif // ABSTRACTMESSAGETYPESSELECTORWIDGET_H
