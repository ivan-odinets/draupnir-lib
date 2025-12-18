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

#ifndef TRAYICONSUPPORTED_H
#define TRAYICONSUPPORTED_H

#include <QSystemTrayIcon>


namespace Draupnir::Ui::MainWindow
{

/*! @class TrayIconSupported draupnir/ui_bricks/traits/features/TrayIconSupported.h
 *  @ingroup UiBricks
 *  @brief Feature that adds system tray icon support to @ref Draupnir::Ui::MainWindowTemplate.
 *  @tparam IconClass   Type used to represent the tray icon. By default this is `QSystemTrayIcon`, but any compatible type can be
 *                      used as long as it behaves like a `QSystemTrayIcon`-based tray icon (e.g. provides the `activated` signal,
 *                      etc.).
 *
 *  @details This feature is a stateful feature (declares `hasState_v = true`) and therefore its instance is stored inside @ref
 *           Draupnir::Ui::MainWindowTemplate’s internal `FeaturesStateTuple`.
 *
 *           Example usage with MainWindowTemplate:
 *           @code
 *           using MainWindowBase = Draupnir::Ui::MainWindowTemplate<
 *              Draupnir::Ui::MainWindow::TrayIconSupported<>,
 *              Draupnir::Ui::MainWindow::MinimizableToTray,
 *              Draupnir::Ui::MainWindow::RememberWindowSize
 *           >;
 *
 *           // Somewhere in initialization code:
 *           QSystemTrayIcon* tray = new QSystemTrayIcon{QIcon(":/icons/app"), mainWindow};
 *           mainWindow->setTrayIcon(tray);
 *           @endcode */

template<class IconClass = QSystemTrayIcon>
class TrayIconSupported
{
public:
    /*! @brief Marks this feature as stateful.
     *  @details Because `hasState_v` is `true`, @ref Draupnir::Ui::MainWindowTemplate will store an instance of @ref TrayIconSupported
     *           inside its `FeaturesStateTuple` and use it to keep track of the tray icon pointer. */
    static inline constexpr bool hasState_v = true;

    /*! @brief Alias for the tray icon type used by this feature. */
    using TrayIconClass = IconClass;

    /*! @brief Pointer to the tray icon instance associated with the main window.
     *  @details This pointer is typically set by @ref MainWindowTemplate::setTrayIcon() and may be used later to:
     *           - hide or show the tray icon;
     *           - connect to tray-icon signals;
     *           - customize tray behavior per window.
     *
     *           The lifetime of the pointed-to object is managed by the caller; the feature itself does not own the icon. */
    TrayIconClass* icon { nullptr };
};

}; // namespace Draupnir::Ui::MainWindow

#endif // TRAYICONSUPPORTED_H
