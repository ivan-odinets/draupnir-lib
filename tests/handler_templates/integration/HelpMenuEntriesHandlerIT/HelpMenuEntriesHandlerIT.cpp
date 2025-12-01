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

#include <QtTest>
#include <QCoreApplication>

#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"
#include "draupnir/handlers/help_menu/HelpMenuEntriesHandler.h"

#include "draupnir/handlers/help_menu/AboutAppEntryHandler.h"
#include "draupnir/handlers/help_menu/AboutDraupnirLibEntryHandler.h"
#include "draupnir/handlers/help_menu/AboutQtEntryHandler.h"
#include "draupnir/handlers/help_menu/HelpEntryHandler.h"

#include <QDialog>

/*! @class HelpSource
 *  @brief This is a test HelpSource for the HelpMenuEntriesHandler.
 *
 * @todo Pur this class into some resonable place. */

class HelpSource
{
public:
    static QString aboutAppText() {
        return QString{"About These \"Awesome\" Test"};
    }

    static QDialog* createHelpDialog() {
        return new QDialog;
    }
};


/*! @class FileMenuEntriesHandlerIT
 *  @brief This is a test class for testing basic functionality of the FileMenuEntriesHandler
 *
 * @todo Implement this test to check successfull compilation. */

class HelpMenuEntriesHandlerIT : public QObject
{
    Q_OBJECT
public:
    using MyHelpMenu = Draupnir::Ui::MenuTemplate<
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::AboutQtMenuTrait,
        Draupnir::Ui::HelpEntryMenuTrait,
        Draupnir::Ui::AboutAppMenuTrait
    >;

    using MyHelpHandler = Draupnir::Handlers::HelpMenuEntriesHandler<
        HelpSource,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::AboutQtMenuTrait,
        Draupnir::Ui::HelpEntryMenuTrait,
        Draupnir::Ui::AboutAppMenuTrait
    >;

    HelpMenuEntriesHandlerIT() = default;
    ~HelpMenuEntriesHandlerIT() = default;

private slots:
    void test_connectionSignals() {
        MyHelpMenu* menu = new MyHelpMenu;
        MyHelpHandler* handler = new MyHelpHandler;

        handler->template connectActions<MyHelpMenu>(menu);

        delete menu;
        delete handler;
    }
};

QTEST_MAIN(HelpMenuEntriesHandlerIT)

#include "HelpMenuEntriesHandlerIT.moc"
