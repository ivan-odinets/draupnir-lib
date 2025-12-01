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
#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"

namespace Draupnir::Ui {

/*! @class MenuTemplateTest tests/ui_bricks/unit/MenuTemplateTest/MenuTemplate.cpp
 *  @brief Test class for the MenuTemplate.
 *
 * @todo Refractor this test so that it will have better readability. */

class MenuTemplateTest final : public QObject
{
    Q_OBJECT
public:
    MenuTemplateTest() = default;
    ~MenuTemplateTest() = default;

private slots:
    void test_initialization() {
        MenuTemplate<
            FileNewEntry,
            FileOpenEntry,
            SeparatorEntry,
            ExitApplicationEntry
        > testMenu;

        QCOMPARE(testMenu.count(), 4);
        QVERIFY(testMenu.contains<FileNewEntry>());
        QVERIFY(testMenu.contains<FileOpenEntry>());
        QVERIFY(testMenu.contains<SeparatorEntry>());
        QVERIFY(testMenu.contains<ExitApplicationEntry>());
        QVERIFY(!testMenu.contains<FileSaveEntry>());
    }
};

} // namespace Draupnir::Ui

QTEST_MAIN(Draupnir::Ui::MenuTemplateTest)

#include "MenuTemplateTest.moc"
