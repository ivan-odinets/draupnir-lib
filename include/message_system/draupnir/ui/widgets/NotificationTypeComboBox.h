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

#include "draupnir/core/Notification.h"

namespace Draupnir::Messages
{

/*! @class NotificationTypeComboBox draupnir/ui/widgets/NotificationTypeComboBox.h
 *  @brief QComboBox widget for selecting Notification::Type values.
 *  @details Provides a drop-down list for choosing a notification type as defined in Notification::Type.
 *           Handles translation updates and allows programmatic selection and querying of the current value.
 *
 *           Usage:
 *           - Use NotificationTypeComboBox::setNotificationType() to programmatically select a value.
 *           - Use selectedNotificationType() to get the currently selected value.
 * @note Only supports notification types included in Notification::displayedValues.
 * @note When building without systray support (macro QT_NO_SYSTEMTRAYICON defined) entry responsible for
 *       Notification::Type::Systemtray will be absent.
 *
 * @todo Seperate this class into seperate file.
 * @todo Maybe make this class template-based? */

class NotificationTypeComboBox final : public QComboBox
{
    Q_OBJECT
public:
    /*! @brief Constructs the combo box and populates it with Notification::Type values.
     *  @param parent - parent widget. */
    explicit NotificationTypeComboBox(QWidget* parent = nullptr);

    /*! @brief Destructor. */
    ~NotificationTypeComboBox() final=default;

    /*! @brief Returns the Notification::Type currently selected in the combo box.
     *  @return Selected Notification::Type value. */
    Notification::Type selectedNotificationType() const;

signals:
    /*! @brief This signal is emitted when user selects new Notification::Type value. */
    void notificationTypeChanged(Notification::Type type);

public slots:
    /*! @brief Sets the specified Notification::Type as selected in the combo box.
     *  @param type Notification::Type to select. */
    void setNotificationType(Notification::Type type);

protected:
    /*! @brief Handles language change events and updates item texts.
     *  @param event - change event (should be QEvent::LanguageChange). */
    void changeEvent(QEvent* event) final;

    void _onCurrentIndexChanged(int index);

private:
    /*! @brief Updates the displayed text for all items to match the current language.
     *  @details Called during language changes. */
    void _retranslateUi();
};

}; // namespace Draupnir::Messages

#endif // NOTIFICATIONTYPECOMBOBOX_H
