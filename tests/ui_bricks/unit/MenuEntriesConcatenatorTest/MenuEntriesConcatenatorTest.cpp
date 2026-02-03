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

#include <QtTest>
#include <QCoreApplication>

#include "draupnir/ui_bricks/utils/MenuEntriesConcatenator.h"

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/SettingsMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/SettingsMenuTemplate.h"
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SectionEntry.h"

#include "draupnir-test/helpers/TypeHelpers.h"

namespace Draupnir::Ui {

/*! @class MenuEntriesConcatenatorTest tests/ui_bricks/unit/MenuEntriesConcatenatorTest/MenuEntriesConcatenatorTest.cpp
 *  @brief Test class for the MenuEntriesConcatenator. */

class MenuEntriesConcatenatorTest final : public QObject
{
    Q_OBJECT
public:
    MenuEntriesConcatenatorTest() = default;
    ~MenuEntriesConcatenatorTest() = default;

private slots:
    void test_all_entries() {
        // Things... - is empty
        TYPE_COMPARE(typename MenuEntriesConcatenator<>::AllEntries,
                     draupnir::utils::type_list<>);

        // Things... - is pack of Entries
        using FirstResult = typename MenuEntriesConcatenator<FileNewEntry,SeparatorEntry,FileOpenEntry>::AllEntries;
        using FirstExpected = typename draupnir::utils::type_list<FileNewEntry,SeparatorEntry,FileOpenEntry>;
        TYPE_COMPARE(FirstResult,FirstExpected);

        // Things... - contain one MenuTemplateEntry
        using SecondResult = typename MenuEntriesConcatenator<
            FileNewEntry,SettingsMenuTemplate<FileOpenEntry,FileSaveAsEntry>,SeparatorEntry>::AllEntries;
        using SecondExpected = typename draupnir::utils::type_list<
            FileNewEntry,FileOpenEntry,FileSaveAsEntry,SeparatorEntry
        >;
        TYPE_COMPARE(SecondResult,SecondExpected);

        // Things... contain MenuTemplate itself
        using ThirdResult = typename MenuEntriesConcatenator<
            FileNewEntry,MenuTemplate<FileOpenEntry,FileSaveAsEntry,SectionEntry<fileMenuName>>,SeparatorEntry>::AllEntries;
        using ThirdExpected = typename draupnir::utils::type_list<
            FileNewEntry,FileOpenEntry,FileSaveAsEntry,SectionEntry<fileMenuName>,SeparatorEntry
        >;
        TYPE_COMPARE(ThirdResult,ThirdExpected);

        // Things... contain various entries and recursion
        using FourthResult = typename MenuEntriesConcatenator<
            FileNewEntry,
            SettingsMenuTemplate<FileOpenEntry>,
            MenuTemplateEntry<fileMenuName,FileSaveEntry,FileSaveAsEntry,
                SettingsMenuTemplate<RecentFileEntry,ExitApplicationEntry,
                    MenuTemplate<OpenSettingsEntry,MinimizeOnCloseEntry>
                >
            >
        >::AllEntries;
        using FourthExpected = draupnir::utils::type_list<
            FileNewEntry,FileOpenEntry,FileSaveEntry,FileSaveAsEntry,RecentFileEntry,ExitApplicationEntry,OpenSettingsEntry,MinimizeOnCloseEntry
        >;
        TYPE_COMPARE(FourthResult,FourthExpected);

        // When uncommented - this will produce static_assert
        //using Fail = MenuEntriesConcatenator<FileNewEntry,int>::AllEntries;
    }

    void test_unique_entries() {
        using Result = typename MenuEntriesConcatenator<
            FileNewEntry, FileOpenEntry, MenuTemplate<FileOpenEntry,FileNewEntry>,SettingsMenuTemplate<FileNewEntry>
        >::UniqueEntries;
        using Expected = typename draupnir::utils::type_list<
            FileNewEntry, FileOpenEntry
        >;
        TYPE_COMPARE(Result,Expected);
    }

    void test_removing_decoration_entries() {
        using Result = typename MenuEntriesConcatenator<
            FileNewEntry,SettingsMenuTemplate<FileOpenEntry,SectionEntry<fileMenuName>,FileSaveAsEntry>,SeparatorEntry>::RemovedDecorationEntries;
        using Expected = typename draupnir::utils::type_list<
            FileNewEntry,FileOpenEntry,FileSaveAsEntry
        >;
        TYPE_COMPARE(Result,Expected);
    }
};

} // namespace Draupnir::UI

QTEST_MAIN(Draupnir::Ui::MenuEntriesConcatenatorTest)

#include "MenuEntriesConcatenatorTest.moc"
