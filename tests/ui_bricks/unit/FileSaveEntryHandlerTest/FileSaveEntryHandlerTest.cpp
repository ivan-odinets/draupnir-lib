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

#include "draupnir/ui_bricks/handlers/file_menu/FileSaveEntryHandler.h"

#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

/*! @class FileSaveEntryHandlerTest
 *  @brief This is a test class for testing basic functionality of the FileSaveEntryHandler */

class FileSaveEntryHandlerTest final : public QObject
{
    Q_OBJECT
public:
    using FileContext = DummyFileContext<DummySingleFileManager>;

    using FileSaveEntryHandler = Draupnir::Handlers::GenericMenuEntryHandlerTemplate<
        FileContext,Draupnir::Ui::FileSaveEntry
    >;

    FileContext dummyContext;
    FileSaveEntryHandler handler{dummyContext};

private slots:
    void test_save_file_without_name() {
        QVERIFY(dummyContext.onSaveFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFile_callCount == 0);

        // Simulate that we don't have file name
        dummyContext.fileManager_field.currentFileHasName_value = false;
        // Trigger
        handler.onTriggered();

        // Check that saveCurrentFile() was not called
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFile_callCount, 0);

        // Check that onSaveFileAs() was called
        QCOMPARE(dummyContext.onSaveFileAs_callCount, 1);
        dummyContext.onSaveFileAs_callCount = 0;
    }

    void test_save_file_with_name() {
        QVERIFY(dummyContext.onSaveFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFile_callCount == 0);

        // Simulate that we don't have file name
        dummyContext.fileManager_field.currentFileHasName_value = false;
        // Trigger
        handler.onTriggered();

        // Check that onSaveFileAs was called
        QCOMPARE(dummyContext.onSaveFileAs_callCount, 1);
        dummyContext.onSaveFileAs_callCount = 0;
    }
};

QTEST_MAIN(FileSaveEntryHandlerTest)

#include "FileSaveEntryHandlerTest.moc"
