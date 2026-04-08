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

#include "draupnir/ui_bricks/core/MenuEntriesContainer.h"

#include <QCoreApplication>
#include <QtTest>

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/decoration/SeparatorEntry.h"
#include "draupnir/ui_bricks/traits/menu_entries/templates/MenuTemplateEntry.h"
#include "draupnir/ui_bricks/traits/menu_entries/submenus/FileMenuTemplate.h"
#include "draupnir/utils/template_adapters.h"

#include "draupnir-test/helpers/TypeHelpers.h"

namespace Draupnir::Ui {

/*! @class MenuEntriesContainerTest tests/ui_bricks/unit/MenuEntriesContainerTest/MenuEntriesContainerTest.cpp
 *  @brief Test class for the @ref Draupnir::Ui::MenuEntriesContainer.
 * @todo Move DummyUiElement to some better place. */

class MenuEntriesContainerTest final : public QObject
{
    Q_OBJECT
public:
    class DummyUiElement {
    public:
        void addAction(QAction* action) { actionsAdded.append(action); }
        void addMenu(QMenu* menu) { menusAdded.append(menu); }

        QList<QAction*> actionsAdded;
        QList<QMenu*> menusAdded;
    };

    MenuEntriesContainer<
        FileNewEntry, FileOpenEntry,
        RecentFileEntry
    > flatContainer;

    template<MenuEntryConcept Entry>
    struct IsActionEntry : std::is_same<QAction, typename Entry::Type> {};

    template<MenuEntryConcept Entry>
    struct IsMenuBasedEntry : std::is_base_of<QMenu, typename Entry::Type> {};

    using FileSubMenuEntry = FileMenuTemplate<
        FileOpenEntry, FileNewEntry
    >;
    using TemplateSubMenuEntry = MenuTemplateEntry<[]() { return QString{}; },
        FileSaveEntry, MenuTemplateEntry<[]() { return QString{}; },
            FileSaveEntry,FileNewEntry,
            MenuTemplateEntry<[]() { return QString{}; },FileSaveAsEntry>
        >
    >;
    using ComplexMenuContainer = MenuEntriesContainer<
        FileNewEntry,
        FileSubMenuEntry,
        TemplateSubMenuEntry,
        FileSaveEntry
    >;
    ComplexMenuContainer randomNestedContainer;

private slots:
    void test_initialization() {
        QCOMPARE(flatContainer.entriesCount(),3);
        QCOMPARE(flatContainer.getUiElement<0>(), flatContainer.getUiElement<FileNewEntry>());
        QCOMPARE(flatContainer.getUiElement<1>(), flatContainer.getUiElement<FileOpenEntry>());
        QCOMPARE(flatContainer.getUiElement<2>(), flatContainer.getUiElement<RecentFileEntry>());
    }

    void test_recursiveEntryCount() {
        QCOMPARE(randomNestedContainer.recursiveEntriesCount<FileNewEntry>(),3);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount<FileOpenEntry>(),1);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount<FileSaveEntry>(),3);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount<FileSaveAsEntry>(),1);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount<ExitApplicationEntry>(),0);

        QCOMPARE(randomNestedContainer.recursiveEntriesCount_v<FileNewEntry>,3);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount_v<FileOpenEntry>,1);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount_v<FileSaveEntry>,3);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount_v<FileSaveAsEntry>,1);
        QCOMPARE(randomNestedContainer.recursiveEntriesCount_v<ExitApplicationEntry>,0);
    }

    void test_getAllInstances() {
        auto allNewFilesTuple = randomNestedContainer.recursiveGetAllUiElements<FileNewEntry>();
        QCOMPARE(std::tuple_size<decltype(allNewFilesTuple)>(), randomNestedContainer.recursiveEntriesCount<FileNewEntry>());

        std::apply([](auto&... element) {
            auto compareOne = [](auto&& e) { QCOMPARE(e->text(), FileNewEntry::displayName()); };
            (..., compareOne(element));
        }, allNewFilesTuple);

        [[maybe_unused]] auto emptyTuple = randomNestedContainer.getAllUiElements<ExitApplicationEntry>();
        QCOMPARE(std::tuple_size<decltype(emptyTuple)>(),0);
        TYPE_COMPARE(decltype(emptyTuple),std::tuple<>);
    }

    void test_connectionViaOnMethod() {
        QAction* newAction = flatContainer.getUiElement<FileNewEntry>();
        QAction* openAction = flatContainer.getUiElement<FileOpenEntry>();

        int newClickCount = 0;
        flatContainer.on<FileNewEntry>([&newClickCount](){
            newClickCount++;
        });

        int openClickCount = 0;
        flatContainer.on<FileOpenEntry>([&openClickCount](){
            openClickCount++;
        });

        newAction->trigger();
        QCOMPARE(newClickCount, 1);
        QCOMPARE(openClickCount, 0);

        openAction->trigger();
        openAction->trigger();
        QCOMPARE(newClickCount, 1);
        QCOMPARE(openClickCount, 2);
    }

    void test_populateUiElement() {
        MenuEntriesContainer<
            FileNewEntry, FileOpenEntry, SeparatorEntry,
            RecentFileEntry
        > flatContainer;

        DummyUiElement dummyUi;
        flatContainer.populateUiElement(&dummyUi);

        QCOMPARE(dummyUi.actionsAdded.count(), 3);
        QCOMPARE(dummyUi.menusAdded.count(), 1);
    }

    void test_type_getUiElement_index() {
        { using actual_t = decltype(randomNestedContainer.getUiElement<0>());
          TYPE_COMPARE(actual_t, QAction*); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getUiElement<0>());
          TYPE_COMPARE(actual_t, const QAction*); }
    }

    void test_type_getUiElement_trait() {
        { using actual_t = decltype(randomNestedContainer.getUiElement<FileNewEntry>());
            TYPE_COMPARE(actual_t, QAction*); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getUiElement<FileNewEntry>());
          TYPE_COMPARE(actual_t, const QAction*); }
    }

    void test_type_getUiElement_template_tp() {
        { using actual_t = decltype(randomNestedContainer.getUiElement<FileMenuTemplate>());
          TYPE_COMPARE(actual_t,typename FileSubMenuEntry::Type*); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getUiElement<FileMenuTemplate>());
          TYPE_COMPARE(actual_t,const typename FileSubMenuEntry::Type*); }
    }

    void test_type_getUiElement_template_a1tp() {
        { using actual_t = decltype(randomNestedContainer.getUiElement<TemplateSubMenuEntry>());
          TYPE_COMPARE(actual_t,typename TemplateSubMenuEntry::Type*); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getUiElement<TemplateSubMenuEntry>());
          TYPE_COMPARE(actual_t,const typename TemplateSubMenuEntry::Type*); }
    }

    void test_type_getUiElementRecursive_trait() {
        { using actual_t = decltype(randomNestedContainer.getUiElementRecursive<FileSaveAsEntry>());
            TYPE_COMPARE(actual_t, QAction*); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getUiElementRecursive<FileSaveAsEntry>());
          TYPE_COMPARE(actual_t, const QAction*); }
    }

    void test_type_getAllUiElementsIf() {
        { using actual_t = decltype(randomNestedContainer.getAllUiElementsIf<IsActionEntry>());
          using expected_t = std::tuple<QAction*,QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getAllUiElementsIf<IsActionEntry>());
          using expected_t = std::tuple<const QAction*, const QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
    }

    void test_type_getAllUiElements() {
        { using actual_t = decltype(randomNestedContainer.getAllUiElements<FileNewEntry>());
          using expected_t = std::tuple<QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.getAllUiElements<FileNewEntry>());
          using expected_t = std::tuple<const QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
    }

    void test_type_recursiveGetAllUiElementsIf() {
        { using actual_t = decltype(randomNestedContainer.recursiveGetAllUiElementsIf<IsActionEntry>());
          using expected_t = std::tuple<QAction*,QAction*,QAction*,QAction*,QAction*,QAction*,QAction*,QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.recursiveGetAllUiElementsIf<IsActionEntry>());
          using expected_t = std::tuple<const QAction*, const QAction*, const QAction*, const QAction*,
                                        const QAction*, const QAction*, const QAction*, const QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
    }

    void test_type_recursiveGetAllUiElements() {
        { using actual_t = decltype(randomNestedContainer.recursiveGetAllUiElements<FileNewEntry>());
          using expected_t = std::tuple<QAction*, QAction*, QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
        { const auto& constRef = randomNestedContainer;
          using actual_t = decltype(constRef.recursiveGetAllUiElements<FileNewEntry>());
          using expected_t = std::tuple<const QAction*, const QAction*, const QAction*>;
          TYPE_COMPARE(actual_t, expected_t); }
    }
};

} // namespace Draupnir::UI

QTEST_MAIN(Draupnir::Ui::MenuEntriesContainerTest)

#include "MenuEntriesContainerTest.moc"
