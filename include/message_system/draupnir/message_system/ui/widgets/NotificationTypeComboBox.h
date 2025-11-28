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

#ifndef NOTIFICATIONTYPECOMBOBOX_H
#define NOTIFICATIONTYPECOMBOBOX_H

#include <QComboBox>

#include "draupnir/message_system/core/Notification.h"

namespace Draupnir::Messages
{

/*! @class NotificationTypeComboBox draupnir/message_system/ui/widgets/NotificationTypeComboBox.h
 *  @ingroup MessageSystem
 *  @brief `QComboBox`-based widget for selecting @ref Draupnir::Messages::Notification::Type values.
 *
 *  @details Provides a drop-down list for choosing a notification type as defined in @ref Draupnir::Messages::Notification::Type.
 *           Handles translation updates and allows programmatic selection and querying of the current value.
 *
 *           Usage:
 *           - Use NotificationTypeComboBox::setNotificationType() to programmatically select a value.
 *           - Use selectedNotificationType() to get the currently selected value.
 *
 * @note Only supports notification types included in Notification::displayedValues.
 *
 * @todo Refractor logic behind the handling of user selections (QComboBox::activated signal), and custom selections (
 *       setNotificationType).
 * @todo Try to find a reasonable way to make this class CTRP-derived / Macros-based thing for enums. */

class NotificationTypeComboBox final : public QComboBox
{
    Q_OBJECT
public:
    /*! @brief Constructs the combo box and populates it with Notification::Type values.
     *  @param parent - parent widget. */
    explicit NotificationTypeComboBox(QWidget* parent = nullptr);

    /*! @brief Destructor. */
    ~NotificationTypeComboBox() final = default;

    /*! @brief Returns the Notification::Type currently selected in the combo box.
     *  @return Selected Notification::Type value. */
    Notification::Type notificationType() const { return m_currentValue; }

    /*! @brief Sets the specified Notification::Type as selected in the combo box **and** emits signal notificationTypeSelected.
     *  @param type Notification::Type to select.
     * @note Emission of the notificationTypeSelected signal is done through the `QComboBox::activated` signal. */
    void setNotificationTypeSelected(Notification::Type type);

signals:
    /*! @brief This signal is emitted when user selects new @ref Draupnir::Messages::Notification::Type value.
     * @note This signal is **not** emitted when setting this value programmatically. */
    void notificationTypeSelected(Draupnir::Messages::Notification::Type type);

public slots:
    /*! @brief Sets the specified Notification::Type as selected in the combo box.
     *  @param type Notification::Type to select.
     * @note No signals are emitted while calling this method. */
    void setNotificationType(Notification::Type type);

protected:
    /*! @brief Handles language change events and updates item texts.
     *  @param event - change event (should be QEvent::LanguageChange). */
    void changeEvent(QEvent* event) final;

private slots:
    void _onActivated(int index);

private:
    /*! @brief Updates the displayed text for all items to match the current language. Called during language changes. */
    void _retranslateUi();

    int _indexOf(Notification::Type type);

    Notification::Type m_currentValue;
};

}; // namespace Draupnir::Messages

#endif // NOTIFICATIONTYPECOMBOBOX_H
