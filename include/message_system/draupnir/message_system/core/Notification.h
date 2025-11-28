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

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QCoreApplication>

#include "draupnir/utils/advance_enum.h"

/*! @class Notification draupnir/message_system/core/Notification.h
 *  @ingroup MessageSystem
 *  @brief Encapsulates all supported notification types in the application.
 *
 *  @details Provides a typed enum for describing where and how notifications should appear (e.g., tray, message box), and
 *           utility functions for conversion between enum values and string representations for config and UI.
 * @note When having QT_NO_SYSTEMTRAYICON macro enabled - tray notification type will be not available.
 *
 * @todo Add support of the telegram notifications (using QtTelegramBot)
 * @todo Add a way so that user can add its own notification types, or at least disable ones being already present. */

namespace Draupnir::Messages
{

class Notification
{
    Q_DECLARE_TR_FUNCTIONS(Notification);
public:
    /*! @brief Supported types of notifications. */
    enum Type {
        None,                /*!< @brief None: No notification; silently logged. */
        MessageBoxType,      /*!< @brief MessageBoxType: Show notification as QMessageBox. */
#ifndef QT_NO_SYSTEMTRAYICON
        Systemtray,          /*!< @brief Systemtray: Show notification in system tray.
                              *   @note This enum value is present only if building without QT_NO_SYSTEMTRAYICON macro */
#endif //QT_NO_SYSTEMTRAYICON
        UnknownType          /*!< @brief Fallback for unrecognized/invalid types. */
    };

    /*! @brief List of notification types intended for display in UI selection elements (e.g., combo box).
     *  @details Only includes types relevant for user selection; does not include UnknownType. */
    static constexpr Type displayedValues[] = {
        None, MessageBoxType,
#ifndef QT_NO_SYSTEMTRAYICON
        Systemtray
#endif //QT_NO_SYSTEMTRAYICON
    };

    /*! @brief List of all supported notification types (except UnknownType). */
    static constexpr Type notificationTypes[] = {
        None, MessageBoxType,
#ifndef QT_NO_SYSTEMTRAYICON
        Systemtray
#endif //QT_NO_SYSTEMTRAYICON
    };

    /*! @brief Converts a config QString into a Notification::Type.
     *  @param QString - string representation of notification type (e.g., "none", "msg", "tray").
     *  @return Notification::Type corresponding to the given string, or UnknownType if invalid.
     *  @details Recognizes specific string keys for each notification type. Returns UnknownType if conversion is not possible. */
    static Type fromConfigString(const QString& string);

    /*! @brief Converts a Notification::Type to its config QString representation.
     *  @param type - Notification::Type value to convert.
     *  @return QString key for this type (e.g., "none", "msg", "tray").
     *  @details Provides a string suitable for serialization/config file usage.
     * @note If Notification::Type::UnknownType is passed here - Q_ASSERT will happen. */
    static QString toConfigString(Type type);

    /*! @brief Returns a user-friendly string for display in UI based on Notification::Type.
     *  @param type - Notification::Type value to display.
     *  @return Translated QString for UI display.
     *  @details Provides a localized, human-readable name for each supported notification type.
     * @note If Notification::Type::UnknownType is passed here - Q_ASSERT will happen. */
    static QString toDisplayString(Type type);

private:
    typedef draupnir::utils::enum_values<Notification::Type,
            Notification::None,
            Notification::MessageBoxType,
#ifndef QT_NO_SYSTEMTRAYICON
            Notification::Systemtray,
#endif //QT_NO_SYSTEMTRAYICON
            Notification::UnknownType
    > Type_values;

    friend Type& operator++(Notification::Type& type,int);
};

/*! @brief Increments a Notification::Type value (postfix operator++).
 *  @param type - reference to Notification::Type to increment.
 *  @return Reference to the updated Notification::Type.
 *  @details Advances the enum to the next value in Type_values; wraps around at the end. */
inline Notification::Type& operator++(Notification::Type& type,int)
{
    Notification::Type_values::advance(type);
    return type;
}

}; // namespace Draupnir::Messages

Q_DECLARE_METATYPE(Draupnir::Messages::Notification::Type);

#endif // NOTIFICATION_H
