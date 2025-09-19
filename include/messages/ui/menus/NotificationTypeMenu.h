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

#ifndef NOTIFICATIONTYPEMENU_H
#define NOTIFICATIONTYPEMENU_H

#include <QMenu>

#include "Notification.h"

/*! @class NotificationTypeMenu draupnir-lib/src/messages/NotificationTypeMenu.h
 *  @brief Menu widget for selecting a Notification::Type at runtime.
 *  @details Provides a QMenu-based UI for choosing the desired notification type (e.g., message box, tray). Automatically
 *           adapts available options depending on build settings (QT_NO_SYSTEMTRAYICON). Emits a signal when the user
 *           selects a new notification type.
 *
 *           Usage:
 *           - Use NotificationTypeMenu::setNotificationType() to programmatically select a type.
 *           - Listen for NotificationTypeMenu::notificationTypeChanged() to handle user selection.
 *           - Call NotificationTypeMenu::selectedNotificationType() to query the currently selected type.
 *
 * @note Only supports notification types included in Notification::displayedValues.
 * @note When building without systray support (macro QT_NO_SYSTEMTRAYICON defined) menu entry responsible for
 *       Notification::Type::Systemtray will be absent. */

class NotificationTypeMenu final : public QMenu
{
    Q_OBJECT
public:
    /*! @brief Default constructor.
     *  @param parent - parent widget. */
    explicit NotificationTypeMenu(QWidget *parent = nullptr);

    /*! @brief Constructor with menu title.
     *  @param title - menu title.
     *  @param parent - parent widget. */
    explicit NotificationTypeMenu(const QString& title, QWidget* parent = nullptr);

    /*! @brief Destructor. */
    ~NotificationTypeMenu() final = default;

    /*! @brief Sets the selected Notification::Type in the menu.
     *  @details Marks the specified type as checked. Does not emit signals.
     *  @param newStatus - Notification::Type to mark as selected. */
    void setNotificationType(Notification::Type newStatus);

    /*! @brief Returns the currently selected Notification::Type.
     *  @return The selected Notification::Type, or Notification::UnknownType if none is selected. */
    Notification::Type selectedNotificationType() const;

signals:
    /*! @brief Emitted when the user changes the notification type via the menu.
     *  @param notify - the newly selected Notification::Type. */
    void notificationTypeChanged(Notification::Type notify);

protected:
    /*! @brief Handles dynamic language changes and updates menu text.
     *  @param event - change event (should be QEvent::LanguageChange). */
    void changeEvent(QEvent* event) final;

private slots:
    /*! @brief Slot triggered when a notification type action is selected.
     *  @param action - the QAction corresponding to the selected notification type. */
    void _notificationActionSelected(QAction* action);

private:
    /*! @brief Sets up menu actions and associates them with notification types.
     *  @details Called from constructors to initialize the menu. */
    void _setupUi();

    /*! @brief Updates the menu action text to match the current language.
     *  @details Called on construction and during language changes. */
    void _retranslateUi();

    QActionGroup* w_notificationActionGroup;
};

#endif // NOTIFICATIONTYPEMENU_H
