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
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"

namespace Draupnir::Ui {

/*! @class MenuTemplateTest tests/ui_bricks/unit/MenuTemplateTest/MenuTemplate.cpp
 *  @brief Test class for the @ref Draupnir::Ui::MenuTemplate.
 * @todo Extend this test. */

class MenuTemplateTest final : public QObject
{
    Q_OBJECT
public:
    MenuTemplateTest() = default;
    ~MenuTemplateTest() = default;

    MenuTemplate<
        FileNewEntry,
        FileOpenEntry,
        SeparatorEntry,
        ExitApplicationEntry
    > simpleFlatMenu;

private slots:
    void test_initialization() {
        QCOMPARE(simpleFlatMenu.entriesCount(), 4);
        QVERIFY(simpleFlatMenu.contains<FileNewEntry>());
        QVERIFY(simpleFlatMenu.contains<FileOpenEntry>());
        QVERIFY(simpleFlatMenu.contains<SeparatorEntry>());
        QVERIFY(simpleFlatMenu.contains<ExitApplicationEntry>());
        QVERIFY(!simpleFlatMenu.contains<FileSaveEntry>());
    }
};

} // namespace Draupnir::Ui

QTEST_MAIN(Draupnir::Ui::MenuTemplateTest)

#include "MenuTemplateTest.moc"
