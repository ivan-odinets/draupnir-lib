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

#include "draupnir/handlers/file_menu/FileSaveAsEntryHandler.h"

#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

/*! @class FileSaveAsEntryHandlerTest
 *  @brief This is a test class for testing basic functionality of the FileSaveAsEntryHandler. */

class FileSaveAsEntryHandlerTest final : public QObject
{
    Q_OBJECT
public:
    using FileContext = DummyFileContext<DummySingleFileManager>;

    using FileSaveAsEntryHandler = Draupnir::Handlers::GenericMenuEntryHandler<
        FileContext,Draupnir::Ui::FileSaveAsEntry
    >;

    FileContext dummyContext;
    FileSaveAsEntryHandler handler{dummyContext};

private slots:
    void test_close_when_nothing_opened() {
        QVERIFY(dummyContext.onSaveFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFileAs_callArguments.isEmpty());

        dummyContext.fileManager_field.hasNothingOpened_value = true;

        // Trigger
        handler.onTriggered();

        // Check that nothing changed
        QCOMPARE(dummyContext.onSaveFileAs_callCount, 0);
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFileAs_callCount, 0);
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFileAs_callArguments.isEmpty(), true);
    }

    void test_empty_save_file_name() {
        QVERIFY(dummyContext.onSaveFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFileAs_callArguments.isEmpty());

        dummyContext.fileManager_field.hasNothingOpened_value = false;
        dummyContext.getSaveFileName_result = QString{};

        // Trigger
        handler.onTriggered();

        // Check that nothing changed
        QCOMPARE(dummyContext.onSaveFileAs_callCount, 0);
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFileAs_callCount, 0);
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFileAs_callArguments.isEmpty(), true);
    }

    void test_reasonable_save_file_name() {
        QVERIFY(dummyContext.fileManager_field.saveCurrentFileAs_callCount == 0);
        QVERIFY(dummyContext.fileManager_field.saveCurrentFileAs_callArguments.isEmpty());

        dummyContext.fileManager_field.hasNothingOpened_value = false;
        const QString dummyFileName{"filename.txt"};
        dummyContext.getSaveFileName_result = dummyFileName;

        // Trigger
        handler.onTriggered();

        // Check that nothing changed
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFileAs_callCount, 1);
        QCOMPARE(dummyContext.fileManager_field.saveCurrentFileAs_callArguments.first(), dummyFileName);
        dummyContext.fileManager_field.saveCurrentFileAs_callCount = 0;
        dummyContext.fileManager_field.saveCurrentFileAs_callArguments.clear();
    }
};

QTEST_MAIN(FileSaveAsEntryHandlerTest)

#include "FileSaveAsEntryHandlerTest.moc"
