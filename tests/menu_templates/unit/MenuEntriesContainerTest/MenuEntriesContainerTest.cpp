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

#include "draupnir/core/MenuEntriesContainer.h"
#include "draupnir/traits/entries/FileMenuEntries.h"
#include "draupnir/traits/entries/decoration/SeparatorEntry.h"

namespace Draupnir::Menus {

/*! @class MenuEntriesContainerTest tests/menu_templates/unit/MenuEntriesContainerTest/MenuEntriesContainerTest.cpp
 *  @brief Test class for the MenuEntriesContainer.
 *
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

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

    MenuEntriesContainerTest() = default;
    ~MenuEntriesContainerTest() = default;

private slots:
    void test_initialization() {
        MenuEntriesContainer<
            FileNewEntry, FileOpenEntry,
            RecentFileEntry
        > randomContainer;

        QCOMPARE(randomContainer.count(),3);
        QCOMPARE(randomContainer.get<0>(), randomContainer.get<FileNewEntry>());
        QCOMPARE(randomContainer.get<1>(), randomContainer.get<FileOpenEntry>());
        QCOMPARE(randomContainer.get<2>(), randomContainer.get<RecentFileEntry>());
    }

    void test_connectionViaOnMethod() {
        MenuEntriesContainer<
            FileNewEntry, FileOpenEntry,
            RecentFileEntry
        > randomContainer;

        QAction* newAction = randomContainer.get<FileNewEntry>();
        QAction* openAction = randomContainer.get<FileOpenEntry>();

        int newClickCount = 0;
        randomContainer.on<FileNewEntry>([&newClickCount](){
            newClickCount++;
        });

        int openClickCount = 0;
        randomContainer.on<FileOpenEntry>([&openClickCount](){
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
        > randomContainer;

        DummyUiElement dummyUi;
        randomContainer.populateUiElement(&dummyUi);

        QCOMPARE(dummyUi.actionsAdded.count(), 3);
        QCOMPARE(dummyUi.menusAdded.count(), 1);
    }
};

} // namespace Draupnir::Menus

QTEST_MAIN(Draupnir::Menus::MenuEntriesContainerTest)

#include "MenuEntriesContainerTest.moc"
