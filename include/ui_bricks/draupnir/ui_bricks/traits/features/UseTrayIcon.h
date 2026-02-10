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

#ifndef USETRAYICON_H
#define USETRAYICON_H

#include <QSystemTrayIcon>


namespace Draupnir::Ui::MainWindow
{

template<class Candidate>
concept IsQSystemTrayBased =
    std::is_base_of_v<QSystemTrayIcon,Candidate>;

/*! @struct UseTrayIcon draupnir/ui_bricks/traits/features/UseTrayIcon.h
 *  @ingroup UiBricks
 *  @brief Feature that adds system tray icon support to @ref Draupnir::Ui::MainWindowTemplate.
 *  @tparam IconClass   Type used to represent the tray icon. By default this is `QSystemTrayIcon`, but any compatible type can be
 *                      used as long as it behaves like a `QSystemTrayIcon`-based tray icon (e.g. provides the `activated` signal,
 *                      etc.).
 *
 *  @details This feature is a stateful feature (declares `hasState_v = true`) and therefore its instance is stored inside @ref
 *           Draupnir::Ui::MainWindowTemplate’s internal `_FeaturesStateTuple`. */

template<IsQSystemTrayBased IconClass = QSystemTrayIcon>
struct UseTrayIcon
{
    /*! @brief Alias for the tray icon type used by this feature. */
    using TrayIcon = IconClass;

    /*! @brief Pointer to the tray icon instance associated with the main window.
     *  @details This pointer is used by MainWindowTemplate to: hide or show the tray icon; connect to tray-icon signals; customize
     *           tray behavior per window.
     *
     *           The lifetime of the pointed-to object is managed by the caller; the feature itself does not own the icon. */
    TrayIcon* state { nullptr };
};

}; // namespace Draupnir::Ui::MainWindow

#endif // USETRAYICON_H
