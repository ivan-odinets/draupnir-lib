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

#include <QtTest>
#include <QCoreApplication>

#include "draupnir/ui_bricks/ui/menus/RecentFilesMenu.h"

namespace Draupnir::Ui {

/*! @class RecentFilesMenuTest tests/modules/ui_bricks/unit/RecentFilesMenuTest/RecentFilesMenuTest.cpp
 *  @ingroup UiBricks
 *  @ingroup Tests
 *  @brief Unit test for @ref Draupnir::Ui::RecentFilesMenu.
 * @note This class needs to be in namespace Draupnir::Ui in order to have access to internals of the RecentFilesMenu. */

class RecentFilesMenuTest final : public QObject
{
    Q_OBJECT
private:
    static inline const QFileInfo firstFile{QFileInfo("some_file.txt")};
    static inline const QFileInfo secondFile{QFileInfo("some_other_file.txt")};
    static inline const QList<QFileInfo> randomFiles{ firstFile, secondFile };

    RecentFilesMenu* recentFiles = nullptr;

private slots:
    void init() {
        recentFiles = new RecentFilesMenu();
    }

    void cleanup() {
        delete recentFiles; recentFiles = nullptr;
    }

    void test_initialization() {
        QVERIFY(recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFiles->actions().count(),2);
        QVERIFY(recentFiles->m_recentFiles.isEmpty());
    }

    void test_addRecentAction() {
        // Test adding files one by one
        RecentFilesMenu* recentFiles = new RecentFilesMenu();
        recentFiles->addRecentAction(firstFile);
        recentFiles->addRecentAction(secondFile);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFiles->actions().count(),4);
        QCOMPARE(recentFiles->w_recentFilesActions->actions().count(),2);
        QCOMPARE(recentFiles->m_recentFiles.count(), 2);
    }

    void test_loadRecentFiles() {
        recentFiles->loadRecentFiles(randomFiles);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFiles->actions().count(),4);
        QCOMPARE(recentFiles->w_recentFilesActions->actions().count(),2);
        QCOMPARE(recentFiles->m_recentFiles.count(), 2);
    }

    void test_hasFileAction() {
        recentFiles->addRecentAction(firstFile);

        QCOMPARE(recentFiles->hasFileAction(firstFile), true);
        QCOMPARE(recentFiles->hasFileAction(secondFile), false);
    }

    void test_getFileAction() {
        recentFiles->addRecentAction(firstFile);
        QVERIFY(recentFiles->getFileAction(firstFile) != nullptr);
        QCOMPARE(recentFiles->getFileAction(secondFile), nullptr);
    }

    void test_updateRecentAction() {
        recentFiles->addRecentAction(firstFile);
        QAction* fileAction = recentFiles->getFileAction(firstFile);
        QCOMPARE(fileAction->text(), firstFile.fileName());

        recentFiles->updateRecentAction(firstFile,secondFile);
        QCOMPARE(fileAction->text(), secondFile.fileName());
    }

    void test_reset() {
        recentFiles->loadRecentFiles(randomFiles);
        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());

        recentFiles->reset();
        QVERIFY(recentFiles->w_recentFilesActions->actions().isEmpty());
    }

    void test_removeRecentAction() {
        recentFiles->loadRecentFiles(randomFiles);
        QPointer<QAction> firstAction{recentFiles->getFileAction(firstFile)};
        QVERIFY(firstAction.isNull() == false);
        QVERIFY(recentFiles->hasFileAction(firstFile));
        QVERIFY(recentFiles->hasFileAction(secondFile));

        recentFiles->removeRecentAction(firstAction.data());
        QTRY_VERIFY(firstAction.isNull());
        QCOMPARE(recentFiles->hasFileAction(firstFile), false);
        QCOMPARE(recentFiles->hasFileAction(secondFile), true);
    }

    void test_clearButtonClick() {
        recentFiles->loadRecentFiles(randomFiles);
        QSignalSpy recentFilesMenuClearedSpy(recentFiles, &RecentFilesMenu::recentFilesMenuCleared);

        QVERIFY(!recentFiles->w_recentFilesActions->actions().isEmpty());
        recentFiles->w_clearRecentAction->trigger();

        QVERIFY(recentFiles->w_recentFilesActions->actions().isEmpty());
        QCOMPARE(recentFilesMenuClearedSpy.count(),1);
        QCOMPARE(recentFilesMenuClearedSpy.first().count(),0);
    }

    void test_fileSelection() {
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

} // namespace Draupnir::Ui

QTEST_MAIN(Draupnir::Ui::RecentFilesMenuTest)

#include "RecentFilesMenuTest.moc"
