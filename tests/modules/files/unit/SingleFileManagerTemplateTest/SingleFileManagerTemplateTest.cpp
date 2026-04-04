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

#include "draupnir/files/managers/SingleFileManagerTemplate.h"

#include "draupnir-test/helpers/FileTestHelpers.h"
#include "draupnir-test/mocks/DummyFiles.h"

/*! @class SingleFileManagerTemplateTest tests/modules/files/unit/SingleFileManagerTemplateTest.cpp
 *  @ingroup Files
 *  @ingroup Tests
 *  @brief Unit tests for @ref Draupnir::Files::SingleFileManagerTemplate class. */

class SingleFileManagerTemplateTest : public QObject
{
    Q_OBJECT
private:
    const QByteArray firstFileData{"I am The File!."};
    QString firstFilePath;        /*!< @brief This file will be used for reading. */

    const QByteArray secondFileData{"VIF - Very Importat File"};
    QString secondFilePath;       /*!< @brief This file will be used for reading. */

    QString nonExistingFile;      /*!< @brief This file will be created during testing. */

    Draupnir::Files::SingleFileManagerTemplate<DummyTextFile>* manager = nullptr;
    QSignalSpy* abstractCurrentFileChangedSignalSpy = nullptr;
    QSignalSpy* currentFileInfoChanged = nullptr;

    void set_file_opened(const QString& filePath) {
        QVERIFY(manager);
        auto result = manager->openFile(filePath);
        QVERIFY(result);
        abstractCurrentFileChangedSignalSpy->clear();
    }

private slots:
    void initTestCase() {
        QVERIFY(FileTestHelper::tempDir().isValid());

        auto result = FileTestHelper::createTempFile("first_file.txt", firstFileData);
        QVERIFY(result.has_value());
        firstFilePath = result.value();

        result = FileTestHelper::createTempFile("second_file.txt", secondFileData);
        QVERIFY(result.has_value());
        secondFilePath = result.value();

        result = FileTestHelper::getTempFilePath("non_existing_file.json");
        QVERIFY(result.has_value());
        nonExistingFile = result.value();
    }

    void init() {
        manager = new Draupnir::Files::SingleFileManagerTemplate<DummyTextFile>;
        abstractCurrentFileChangedSignalSpy =
            new QSignalSpy(manager, &Draupnir::Files::AbstractFileManager::currentAbstractFileChanged);
        currentFileInfoChanged =
            new QSignalSpy(manager, &Draupnir::Files::AbstractFileManager::currentFileInfoChanged);
    }

    void cleanup() {
        delete abstractCurrentFileChangedSignalSpy; abstractCurrentFileChangedSignalSpy = nullptr;
        delete manager; manager = nullptr;
    }

    void test_initial_state() {
        QVERIFY(manager->currentFile() == nullptr);
        QVERIFY(manager->hasNothingOpened() == true);
    }

    void test_new_file() {
        manager->newFile();
        QVERIFY(manager->currentFile() != nullptr);
        QVERIFY(manager->hasNothingOpened() == false);
        QTRY_COMPARE(abstractCurrentFileChangedSignalSpy->count(), 1);
        QCOMPARE(abstractCurrentFileChangedSignalSpy->first().count(), 1);
        auto* emittedFile = abstractCurrentFileChangedSignalSpy->first().first().value<Draupnir::Files::AbstractFile*>();
        QCOMPARE(emittedFile, manager->currentFile());
    }

    void test_opening_file() {
        auto openResult = manager->openFile(firstFilePath);
        QVERIFY(manager->currentFile() != nullptr);
        QVERIFY(manager->hasNothingOpened() == false);
        QTRY_COMPARE(abstractCurrentFileChangedSignalSpy->count(), 1);
        QCOMPARE(abstractCurrentFileChangedSignalSpy->first().count(), 1);
        auto* emittedFile = abstractCurrentFileChangedSignalSpy->first().first().value<Draupnir::Files::AbstractFile*>();
        QCOMPARE(emittedFile, manager->currentFile());
    }

    void test_opening_file_when_sth_alreay_opened() {
        set_file_opened(firstFilePath);
        auto* firstFile = manager->currentFile();

        manager->openFile(secondFilePath);
        QVERIFY(manager->currentFile() != nullptr);
        QVERIFY(manager->currentFile() != firstFile);
        QTRY_COMPARE(abstractCurrentFileChangedSignalSpy->count(), 1);
        QCOMPARE(abstractCurrentFileChangedSignalSpy->first().count(), 1);
        auto* emittedFile = abstractCurrentFileChangedSignalSpy->first().first().value<Draupnir::Files::AbstractFile*>();
        QCOMPARE(emittedFile, manager->currentFile());
    }

    void test_untitled_file_save_as() {
        // Verify that test file is not existing
        QVERIFY(!QFile::exists(nonExistingFile));
        // Create new file
        manager->newFile();
        QVERIFY(manager->currentFile() != nullptr);
        abstractCurrentFileChangedSignalSpy->clear();
        currentFileInfoChanged->clear();

        // Modify file
        manager->currentFile()->data = firstFileData;
        // And save file
        manager->saveCurrentFileAs(nonExistingFile);

        // And finally - check if expected thing happened
        auto result = FileTestHelper::tempFileData(nonExistingFile);
        QVERIFY(result);
        QCOMPARE(result.value(), firstFileData);

        QVERIFY(QFile::remove(nonExistingFile));
    }

    void test_close_file() {
        manager->newFile();
        abstractCurrentFileChangedSignalSpy->clear();

        manager->closeFile();
        QCOMPARE(manager->currentFile(), nullptr);
        QCOMPARE(manager->hasNothingOpened(), true);
        QCOMPARE(abstractCurrentFileChangedSignalSpy->count(), 1);
        QCOMPARE(abstractCurrentFileChangedSignalSpy->first().count(), 1);
        auto* emittedFile = abstractCurrentFileChangedSignalSpy->first().first().value<Draupnir::Files::AbstractFile*>();
        QCOMPARE(emittedFile, nullptr);
    }
};

QTEST_MAIN(SingleFileManagerTemplateTest)

#include "SingleFileManagerTemplateTest.moc"
