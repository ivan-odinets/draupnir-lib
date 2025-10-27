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

#include "draupnir/ui/menus/RecentFilesMenu.h"

namespace Draupnir::Menus {

/*! @class RecentFilesMenuTest tests/menu_templates/unit/RecentFilesMenuTest/RecentFilesMenuTest.cpp
 *  @brief Test class for the RecentFilesMenu.
 *
 * @note This class needs to be in namespace Draupnir::Menus in order to have access to internals of the RecentFilesMenu.
 *
 * @todo Add test for updateRecentAction method(s).
 * @todo Add test for removeRecentAction method.
 * @todo Add test for getFileAction method.
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

class RecentFilesMenuTest final : public QObject
{
    Q_OBJECT

public:
    QFileInfo firstFile;
    QFileInfo secondFile;
    QList<QFileInfo> randomFiles;

    RecentFilesMenuTest() :
        firstFile{QFileInfo("some_file.txt")},
        secondFile{QFileInfo("some_other_file.txt")},
        randomFiles{firstFile, secondFile}
    {
    }
    ~RecentFilesMenuTest() final = default;


private slots:
    void test_initialization() {
        RecentFilesMenu* recentFiles = new RecentFilesMenu();

        QVERIFY(recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFiles->actions().count(),2);
        QVERIFY(recentFiles->m_recentFiles.isEmpty());

        delete recentFiles;
    }

    void test_addRecentAction() {
        // Test adding files one
        RecentFilesMenu* recentFiles = new RecentFilesMenu();
        recentFiles->addRecentAction(firstFile);
        recentFiles->addRecentAction(secondFile);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFiles->actions().count(),4);
        QCOMPARE(recentFiles->w_recentFilesActions->actions().count(),2);
        QCOMPARE(recentFiles->m_recentFiles.count(), 2);

        delete recentFiles;
    }

    void test_loadRecentFiles() {
        RecentFilesMenu* recentFiles = new RecentFilesMenu();
        recentFiles->loadRecentFiles(randomFiles);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFiles->actions().count(),4);
        QCOMPARE(recentFiles->w_recentFilesActions->actions().count(),2);
        QCOMPARE(recentFiles->m_recentFiles.count(), 2);

        delete recentFiles;
    }

    void test_file_presense() {
        RecentFilesMenu* recentFiles = new RecentFilesMenu();
        recentFiles->addRecentAction(firstFile);

        QVERIFY(recentFiles->hasFileAction(firstFile));

        delete recentFiles;
    }

    void test_reset() {
        RecentFilesMenu* recentFiles = new RecentFilesMenu();
        recentFiles->loadRecentFiles(randomFiles);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());

        recentFiles->reset();
        QVERIFY(recentFiles->w_recentFilesActions->actions().isEmpty());
    }

    void test_clearButtonClick() {
        RecentFilesMenu* recentFiles = new RecentFilesMenu();
        recentFiles->loadRecentFiles(randomFiles);
        QSignalSpy recentFilesMenuClearedSpy(recentFiles, &RecentFilesMenu::recentFilesMenuCleared);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());
        recentFiles->w_clearRecentAction->trigger();

        QVERIFY(recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFilesMenuClearedSpy.count(),1);
        QCOMPARE(recentFilesMenuClearedSpy.first().count(),0);
    }

    void test_fileSelection() {
        RecentFilesMenu* recentFiles = new RecentFilesMenu;
        recentFiles->addRecentAction(firstFile);
        recentFiles->addRecentAction(secondFile);
        QSignalSpy recentFileSelectedSpy(recentFiles,&RecentFilesMenu::recentFileSelected);

        QAction* actionOne = recentFiles->getFileAction(firstFile);
        QVERIFY(actionOne != nullptr);
        actionOne->trigger();

        QCOMPARE(recentFileSelectedSpy.count(),1);
        QList<QVariant> arguments = recentFileSelectedSpy.at(0);
        QCOMPARE(arguments.count(),1);
        QCOMPARE(arguments.first().value<QFileInfo>(), firstFile);

        QAction* actionTwo = recentFiles->getFileAction(secondFile);
        QVERIFY(actionTwo != nullptr);
        actionTwo->trigger();

        QCOMPARE(recentFileSelectedSpy.count(),2);
        arguments = recentFileSelectedSpy.at(1);
        QCOMPARE(arguments.count(),1);
        QCOMPARE(arguments.first().value<QFileInfo>(), secondFile);
    }
};

} // namespace Draupnir::Menus

QTEST_MAIN(Draupnir::Menus::RecentFilesMenuTest)

#include "RecentFilesMenuTest.moc"
