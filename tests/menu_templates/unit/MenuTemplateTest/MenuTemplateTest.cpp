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

#include "draupnir/ui/menus/MenuTemplate.h"
#include "draupnir/traits/entries/FileMenuEntries.h"
#include "draupnir/traits/entries/decoration/SeparatorEntry.h"

namespace Draupnir::Menus {

/*! @class MenuTemplateTest tests/menu_templates/unit/MenuTemplateTest/MenuTemplateTest.cpp
 *  @brief Test class for the MenuTemplateTest.
 *
 * @note This class needs to be in namespace Draupnir::Menus in order to have access to internals of the MenuTemplateTest.
 *
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

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

    void test_connectionViaOnMethod() {
    }

    void test_populateUiElement() {
    }
};

} // namespace Draupnir::Menus

QTEST_MAIN(Draupnir::Menus::MenuTemplateTest)

#include "MenuTemplateTest.moc"
