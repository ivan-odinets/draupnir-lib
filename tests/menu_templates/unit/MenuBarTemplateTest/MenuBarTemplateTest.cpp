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

#include "draupnir/ui/menus/MenuBarTemplate.h"
#include "draupnir/traits/entries/FileMenuEntries.h"
#include "draupnir/traits/entries/decoration/SeparatorEntry.h"

namespace Draupnir::Menus {

/*! @class MenuBarTemplateTest tests/menu_templates/unit/MenuBarTemplateTest/MenuBarTemplateTest.cpp
 *  @brief Test class for the MenuBarTemplate.
 *
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

class MenuBarTemplateTest final : public QObject
{
    Q_OBJECT
public:
    MenuBarTemplateTest() = default;
    ~MenuBarTemplateTest() = default;

private slots:
    void test_initialization() {
        MenuBarTemplate<
            FileNewEntry,
            FileOpenEntry,
            SeparatorEntry,
            ExitApplicationEntry
        > testMenuBar;

        QCOMPARE(testMenuBar.count(), 4);
        QVERIFY(testMenuBar.contains<FileNewEntry>());
        QVERIFY(testMenuBar.contains<FileOpenEntry>());
        QVERIFY(testMenuBar.contains<SeparatorEntry>());
        QVERIFY(testMenuBar.contains<ExitApplicationEntry>());
        QVERIFY(!testMenuBar.contains<FileSaveEntry>());
    }
};

} // namespace Draupnir::Menus

QTEST_MAIN(Draupnir::Menus::MenuBarTemplateTest)

#include "MenuBarTemplateTest.moc"
