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

#include "draupnir/ui_bricks/handlers/file_menu/FileCloseEntryHandler.h"

#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

/*! @class FileCloseEntryHandlerTest
 *  @brief This is a test class for testing basic functionality of the FileCloseEntryHandler. */

class FileCloseEntryHandlerTest : public QObject
{
    Q_OBJECT
public:
    using FileContext = DummyFileContext<DummySingleFileManager>;

    using FileCloseEntryHandler = Draupnir::Handlers::GenericMenuEntryHandler<
        FileContext,Draupnir::Ui::FileCloseEntry
    >;

    FileContext dummyContext;
    FileCloseEntryHandler handler{dummyContext};

private slots:
    void test_close_when_nothing_opened() {
        QVERIFY(dummyContext.onSaveFile_callCount == 0);

        // Simulate that we have nothing opened.
        dummyContext.fileManager_field.hasNothingOpened_value = true;
        // Trigger
        handler.onTriggered();

        // No save calls from CloseHandler
        QCOMPARE(dummyContext.onSaveFile_callCount, 0);
        // No close file calls from CloseHandler
        QCOMPARE(dummyContext.fileManager_field.closeCurrentFile_callCount, 0);
    }

    void test_close_saved_file() {
        QVERIFY(dummyContext.onSaveFile_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.closeCurrentFile_callCount == 0);

        // Simulate that we have something opened
        dummyContext.fileManager_field.hasNothingOpened_value = false;
        // And saved
        dummyContext.fileManager_field.currentFileSaved_value = true;
        // Trigger
        handler.onTriggered();

        // No save calls from CloseHandler
        QCOMPARE(dummyContext.onSaveFile_callCount, 0);
        // One close file call from CloseHandler
        QCOMPARE(dummyContext.fileManager_field.closeCurrentFile_callCount, 1);
        dummyContext.fileManager_field.closeCurrentFile_callCount = 0;
    }

    void test_close_unsaved_file() {
        QVERIFY(dummyContext.onSaveFile_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.closeCurrentFile_callCount == 0);

        // Simulate that we have something opened
        dummyContext.fileManager_field.hasNothingOpened_value = false;
        // And unsaved
        dummyContext.fileManager_field.currentFileSaved_value = false;
        // And "user" will cancel close dialog
        dummyContext.askUser_result = QMessageBox::Cancel;

        // Trigger
        handler.onTriggered();

        // No save calls from CloseHandler
        QCOMPARE(dummyContext.onSaveFile_callCount, 0);
        // No close file calls from CloseHandler
        QCOMPARE(dummyContext.fileManager_field.closeCurrentFile_callCount, 0);

        // Now "user" will press save button
        dummyContext.askUser_result = QMessageBox::Save;

        // Trigger
        handler.onTriggered();

        // One save call from CloseHandler
        QCOMPARE(dummyContext.onSaveFile_callCount, 1);
        dummyContext.onSaveFile_callCount = 0;
        // One close file call from CloseHandler
        QCOMPARE(dummyContext.fileManager_field.closeCurrentFile_callCount, 01);
        dummyContext.fileManager_field.closeCurrentFile_callCount = 0;
    }
};

QTEST_MAIN(FileCloseEntryHandlerTest)

#include "FileCloseEntryHandlerTest.moc"
