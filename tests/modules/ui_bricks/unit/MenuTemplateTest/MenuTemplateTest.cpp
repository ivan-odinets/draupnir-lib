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

#include "draupnir/ui_bricks/ui/menus/MenuTemplate.h"

#include <QCoreApplication>
#include <QtTest>

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"
#include "draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h"

#include "draupnir-test/helpers/TypeHelpers.h"

namespace Draupnir::Ui {

/*! @class MenuTemplateTest tests/modules/ui_bricks/unit/MenuTemplateTest/MenuTemplate.cpp
 *  @ingroup UiBricks
 *  @ingroup Tests
 *  @brief Unit test for @ref Draupnir::Ui::MenuTemplate class. */

class MenuTemplateTest final : public QObject
{
    Q_OBJECT
private:
    using Menu = MenuTemplate<
        FileNewEntry,
        FileOpenEntry,
        SeparatorEntry,
        RecentFileEntry,
        MenuTemplateEntry<[]() { return QString{"Help!"}; },
            FileNewEntry,
            AboutAppMenuTrait,
            AboutQtMenuTrait
        >
    >;
    Menu* menu;

private slots:
    void init() {
        menu = new Menu;
    }

    void cleanup() {
        delete menu; menu = nullptr;
    }

    void test_count() {
        QCOMPARE(menu->entriesCount(), 5);

        QCOMPARE(menu->recursiveEntriesCount<AboutAppMenuTrait>(), 1);
        QCOMPARE(menu->recursiveEntriesCount<AboutAppMenuTrait>(), 1);
        QCOMPARE(menu->recursiveEntriesCount<FileNewEntry>(), 2);
        QCOMPARE(menu->recursiveEntriesCount<ExitApplicationEntry>(), 0);
    }

    void test_if_all_entries_properly_initialized() {
        QVERIFY(menu->getUiElement<FileNewEntry>() != nullptr);
        TYPE_COMPARE(decltype(menu->getUiElement<FileNewEntry>()),QAction*);
        QVERIFY(menu->getUiElement<FileOpenEntry>() != nullptr);
        TYPE_COMPARE(decltype(menu->getUiElement<FileOpenEntry>()),QAction*);
        QVERIFY(menu->getUiElementRecursive<RecentFileEntry>() != nullptr);
        TYPE_COMPARE(decltype(menu->getUiElement<RecentFileEntry>()),RecentFilesMenu*);

        QVERIFY(menu->getUiElementRecursive<AboutAppMenuTrait>() != nullptr);
        TYPE_COMPARE(decltype(menu->getUiElementRecursive<AboutAppMenuTrait>()),QAction*);
    }

    void test_contains() {
        QVERIFY(menu->contains<FileNewEntry>());
        QVERIFY(menu->contains<FileOpenEntry>());
        QVERIFY(menu->contains<SeparatorEntry>());

        QCOMPARE(menu->contains<AboutAppMenuTrait>(), false);
        QCOMPARE(menu->recursiveContains<AboutAppMenuTrait>(), true);
    }
};

} // namespace Draupnir::Ui

QTEST_MAIN(Draupnir::Ui::MenuTemplateTest)

#include "MenuTemplateTest.moc"
