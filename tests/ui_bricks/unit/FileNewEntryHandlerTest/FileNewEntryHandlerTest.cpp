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

#include "draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h"

#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummyMultipleFileManager.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

/*! @class FileNewEntryHandlerTest
 *  @brief This is a test class for testing basic functionality of the FileNewEntryHandler */

class FileNewEntryHandlerTest final : public QObject
{
    Q_OBJECT
public:
    using MultipleFileContext = DummyFileContext<DummyMultipleFileManager>;
    using SingleFileContext = DummyFileContext<DummySingleFileManager>;

    using SingleFileNewEntryHandler = Draupnir::Handlers::GenericMenuEntryHandler<
        SingleFileContext,Draupnir::Ui::FileNewEntry
    >;
    using MultipleFileNewEntryHandler = Draupnir::Handlers::GenericMenuEntryHandler<
        MultipleFileContext,Draupnir::Ui::FileNewEntry
    >;

    SingleFileContext dummySingleContext;
    SingleFileNewEntryHandler singleHandler{dummySingleContext};

    MultipleFileContext dummyMultipleContext;
    MultipleFileNewEntryHandler multipleHandler{dummyMultipleContext};

private slots:
    void test_new_handler_with_multifile() {
        QVERIFY(dummyMultipleContext.fileManager_field.newFile_callCount == 0);

        // Trigger
        multipleHandler.onTriggered();

        // One call happened
        QCOMPARE(dummyMultipleContext.fileManager_field.newFile_callCount, 1);
        dummyMultipleContext.fileManager_field.newFile_callCount = 0;
    }

    void test_new_handler_singlefile_nothing_opened() {
        QVERIFY(dummySingleContext.fileManager_field.newFile_callCount == 0);
        dummySingleContext.fileManager_field.hasNothingOpened_value = true;

        // Trigger
        singleHandler.onTriggered();

        // One call happened
        QCOMPARE(dummySingleContext.fileManager_field.newFile_callCount, 1);
        dummySingleContext.fileManager_field.newFile_callCount = 0;
    }

    void test_new_handler_singlefile_sth_opened_and_saved() {
        QVERIFY(dummySingleContext.fileManager_field.newFile_callCount == 0);
        dummySingleContext.fileManager_field.hasNothingOpened_value = false;
        dummySingleContext.fileManager_field.currentFileSaved_value = true;

        // Assume that user will press Cancel button
        dummySingleContext.askUser_result = QMessageBox::Cancel;

        // Trigger
        singleHandler.onTriggered();

        // User deciced notto replace file - no calls.
        QCOMPARE(dummySingleContext.fileManager_field.newFile_callCount, 0);

        // Assume that user will press Cancel button
        dummySingleContext.askUser_result = QMessageBox::Yes;

        // Trigger
        singleHandler.onTriggered();

        // User deciced replace file - one newFile call.
        QCOMPARE(dummySingleContext.fileManager_field.newFile_callCount, 1);
        dummySingleContext.fileManager_field.newFile_callCount = 0;
    }

    void test_new_handler_singlefile_sth_opened_and_unsaved() {
        QVERIFY(dummySingleContext.fileManager_field.newFile_callCount == 0);
        dummySingleContext.fileManager_field.hasNothingOpened_value = false;
        dummySingleContext.fileManager_field.currentFileSaved_value = false;

        // Assume that user will press cancel
        dummySingleContext.askUser_result = QMessageBox::Cancel;

        // Trigger
        singleHandler.onTriggered();

        // Nothing happened
        QCOMPARE(dummySingleContext.fileManager_field.newFile_callCount, 0);
        QCOMPARE(dummySingleContext.onSaveFile_callCount, 0);

        // Assume that user will press discard now
        dummySingleContext.askUser_result = QMessageBox::Discard;

        // Trigger
        singleHandler.onTriggered();

        // Only newFile method was called
        QCOMPARE(dummySingleContext.fileManager_field.newFile_callCount, 1);
        dummySingleContext.fileManager_field.newFile_callCount = 0;
        QCOMPARE(dummySingleContext.onSaveFile_callCount, 0);

        // Assume that user will press Save now
        dummySingleContext.askUser_result = QMessageBox::Save;

        // Trigger
        singleHandler.onTriggered();

        // Both newFile and onSave methods were called
        QCOMPARE(dummySingleContext.fileManager_field.newFile_callCount, 1);
        dummySingleContext.fileManager_field.newFile_callCount = 0;
        QCOMPARE(dummySingleContext.onSaveFile_callCount, 1);
        dummySingleContext.onSaveFile_callCount = 0;
    }
};

QTEST_MAIN(FileNewEntryHandlerTest)

#include "FileNewEntryHandlerTest.moc"
