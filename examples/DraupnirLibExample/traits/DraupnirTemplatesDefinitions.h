/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef DRAUPNIRTEMPLATESDEFINITIONS_H
#define DRAUPNIRTEMPLATESDEFINITIONS_H

// Include things from DraupnirLib
#include "draupnir/UiBricks.h"         // IWYU pragma: keep
#include "draupnir/MessageSystem.h"    // IWYU pragma: keep
#include "draupnir/SettingsRegistry.h" // IWYU pragma: keep
#include "draupnir/message_system/traits/widgets/LogWidgetTrait.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutAppEntryHandler.h"             // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/help_menu/AboutDraupnirLibEntryHandler.h"     // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/help_menu/AboutQtEntryHandler.h"              // IWYU pragma: keep
#include "draupnir/ui_bricks/handlers/help_menu/HelpEntryHandler.h"                 // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/UseMenuBar.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/HelpMenuTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/SettingsMenuTemplate.h"

// Include things from the example
#include "core/ExampleHelpSource.h"
#include "traits/messages/MyMessageTrait.h"
#include "traits/settings/MySettingTraits.h"
#include "traits/widgets/ButtonsWidgetTrait.h"

struct ExampleAppConfig
{
    struct Core {
        using MessageSystem = Draupnir::Messages::MessageSystemTemplate<
            MyMessageTrait
        >;

        using ExtraSettingsBundle = Draupnir::Settings::SettingsBundleTemplate<
            DoubleSetting, IntSetting, StringSetting
        >;
    };

    using SettingsMenu = Draupnir::Ui::SettingsMenuTemplate<
        Draupnir::Ui::MinimizeOnCloseEntry,
        Draupnir::Ui::MinimizeToTrayEntry,
        Draupnir::Ui::MenuTemplateEntry<[]() { return QString{"Submenu"}; },
            SomeDoubleSettinMenuEntry,
            SomeIntSettinMenuEntry,
            SomeStringSettinMenuEntry
        >,
        Draupnir::Ui::SectionEntry<[]() { return QString{"Message Notifications"}; }>,
        Draupnir::Messages::MessageNotificationSettingsEntryTemplate<
            Draupnir::Messages::ErrorMessageTrait
        >,
        Draupnir::Messages::MultiMessageNotificationSettingsMenuEntryTemplate<
            Draupnir::Messages::DebugMessageTrait,
            Draupnir::Messages::InfoMessageTrait,
            Draupnir::Messages::WarningMessageTrait,
            Draupnir::Messages::ErrorMessageTrait,
            Draupnir::Messages::InfoMessageTrait,
            MyMessageTrait
        >
    >;

    using HelpMenu = Draupnir::Ui::HelpMenuTemplate<
        Draupnir::Ui::AboutAppMenuTrait,
        Draupnir::Ui::SeparatorEntry,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::AboutQtMenuTrait
    >;

    struct Ui {
        // We want to have our MenuBar
        using MenuBar = Draupnir::Ui::MenuBarTemplate<
            Draupnir::Ui::MenuTemplateEntry<[](){ return QObject::tr("My Menu"); },
                Draupnir::Ui::FileCloseEntry,
                Draupnir::Ui::ExitApplicationEntry
            >,
            SettingsMenu,
            HelpMenu
        >;

        struct MenuHandlers {
            using HelpSource = ExampleHelpSource;
        };

        using TrayIcon = Draupnir::Ui::TrayIcon;

        using MainWindow = Draupnir::Ui::MainWindowTemplate<
            Draupnir::Ui::MainWindow::ClosureConfirmation<>,
            Draupnir::Ui::MainWindow::UseMenuBar<
                MenuBar
            >,
            // Our CentralWidget
            Draupnir::Ui::MainWindow::UseCentralWidget<
                Draupnir::Ui::FixedCentralTabWidgetTemplate<
                    ButtonsWidgetTrait,
                    Draupnir::Messages::LogWidgetTrait
                >
            >,
            // Our window will have following behaviours
            Draupnir::Ui::MainWindow::MinimizableOnClose,
            Draupnir::Ui::MainWindow::MinimizableToTray,
            Draupnir::Ui::MainWindow::UseTrayIcon<>,
            Draupnir::Ui::MainWindow::RememberWindowSize
        >;
    };
};

#endif // DRAUPNIRTEMPLATESDEFINITIONS_H
