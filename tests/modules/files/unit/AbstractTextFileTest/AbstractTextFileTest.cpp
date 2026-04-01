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

#include <QSignalSpy>

#include "draupnir-test/helpers/FileTestHelpers.h"
#include "draupnir-test/mocks/DummyFiles.h"

/*! @class AbstractTextFileTest tests/modules/files/unit/AbstractTextFileTest.cpp
 *  @ingroup Files
 *  @ingroup Tests
 *  @brief Unit tests for @ref Draupnir::Files::AbstractTextFile class. */

class AbstractTextFileTest : public QObject
{
    Q_OBJECT
private:
    const QByteArray fileData{"VeryRandomData"};
    const QByteArray anotherFileData{"Some other data"};

    QString unreachableFilePath;     /*!< @brief This file should be unreachable. */
    QString filePathToRead;          /*!< @brief This file will be used for reading. */
    QString anotherFilePathToRead;   /*!< @brief This file will be used for reading. */
    QString filePathToWrite;         /*!< @brief This file will be written. */
    QString fileToBeRemoved;         /*!< @brief This file will be removed during the test. */
    QString fileToBeEdited;          /*!< @brief This file will be edited during the test. */
    QString nonExistingFile;         /*!< @brief This file will be created using open method. */
    QString nonExistingFileInUnreachableDir;

///@name Test state things
///@{
    DummyTextFile* file                           = nullptr;
    QSignalSpy* fileInfoChangedSignalSpy          = nullptr;
    QSignalSpy* fileModifiedOnDisk                = nullptr;
    QSignalSpy* fileRemovedSignalSpy              = nullptr;
    QSignalSpy* unsavedDataStatusChangedSignalSpy = nullptr;
///@}

    /*! @brief Helper method which will set the unsaved state of the current `file` object and clear all signal spies. */
    void set_file_unsaved_state(bool state) {
        QVERIFY(file);
        file->triggerUnsavedStatusChange(state);
        unsavedDataStatusChangedSignalSpy->clear();
        QVERIFY(file->hasUnsavedData() == state);
    }

    /*! @brief Helper method which will open the file under specified path and clear all signal spies. */
    void set_file_opened(const QString& filePath) {
        QVERIFY(file);
        // Open the file.
        auto result = file->open(filePath);
        // Verify that expected things happened.
        QVERIFY(result.has_value() == true);
        QVERIFY(!file->isUntitled());
        QVERIFY(file->fileInfo().absoluteFilePath() == filePath);
        fileInfoChangedSignalSpy->clear();
        unsavedDataStatusChangedSignalSpy->clear();
    };

private slots:
    void initTestCase() {
        QVERIFY(FileTestHelper::tempDir().isValid());

        auto result = FileTestHelper::createTempFile( "unreadable_file.txt", {}, {} );
        QVERIFY(result.has_value());
        unreachableFilePath = result.value();

        result = FileTestHelper::createTempFile( "file_to_read.txt", fileData );
        QVERIFY(result.has_value());
        filePathToRead = result.value();

        result = FileTestHelper::createTempFile( "another_file_to_read.txt", anotherFileData);
        QVERIFY(result.has_value());
        anotherFilePathToRead = result.value();

        result = FileTestHelper::createTempFile( "file_to_write.txt" );
        QVERIFY(result);
        filePathToWrite = result.value();

        result = FileTestHelper::createTempFile( "to_be_removed.txt", {} );
        QVERIFY(result);
        fileToBeRemoved = result.value();

        result = FileTestHelper::createTempFile( "to_be_edited.txt", {} );
        QVERIFY(result);
        fileToBeEdited = result.value();

        result = FileTestHelper::getTempFilePath( "non_existing_file.txt" );
        QVERIFY(result);
        nonExistingFile = result.value();

        result = FileTestHelper::createTempDirectory("unreachable_dir",{});
        QVERIFY(result);
        nonExistingFileInUnreachableDir = QDir{result.value()}.filePath( "non_existing_file.txt" );
    }

    /*! @brief Will initialize the following fields with a freashly allocated objects: `DummyTextFile* file`, `QSignalSpy*
     *         fileInfoChangedSignalSpy`, `QSignalSpy* fileModifiedOnDisk`, `QSignalSpy* fileRemovedSignalSpy`, and
     *         `QSignalSpy* unsavedDataStatusChangedSignalSpy` */
    void init() {
        file = new DummyTextFile;

        fileInfoChangedSignalSpy          = new QSignalSpy{file, &DummyTextFile::fileInfoChanged};
        fileModifiedOnDisk                = new QSignalSpy{file, &DummyTextFile::fileChangedOnDisk};
        fileRemovedSignalSpy              = new QSignalSpy{file, &DummyTextFile::fileRemovedFromDisk};
        unsavedDataStatusChangedSignalSpy = new QSignalSpy{file, &DummyTextFile::unsavedDataStatusChanged};
    }

    /*! @brief Will delete the following fields: `DummyTextFile* file`, `QSignalSpy*  fileInfoChangedSignalSpy`, `QSignalSpy*
     *         fileModifiedOnDisk`, `QSignalSpy* fileRemovedSignalSpy`, and `QSignalSpy* unsavedDataStatusChangedSignalSpy` */
    void cleanup() {
        delete unsavedDataStatusChangedSignalSpy; unsavedDataStatusChangedSignalSpy = nullptr;
        delete fileRemovedSignalSpy;              fileRemovedSignalSpy = nullptr;
        delete fileModifiedOnDisk;                fileModifiedOnDisk = nullptr;
        delete fileInfoChangedSignalSpy;          fileInfoChangedSignalSpy = nullptr;
        delete file;                              file = nullptr;
    }

    void test_file_initial_state() {
        // Verify initial state of file object.
        QCOMPARE(file->isUntitled(), true);
        QCOMPARE(file->hasUnsavedData(), false);
        QCOMPARE(file->fileInfo().absoluteFilePath().isEmpty(), true);
    }

    void test_file_unsaved_status() {
        // Verify initial state.
        QVERIFY(file->hasUnsavedData() == false);

        // Trigger unsaved change once.
        file->triggerUnsavedStatusChange(true);
        // Signal unsavedDataStatusChanged should be emitted only once.
        QTRY_COMPARE(unsavedDataStatusChangedSignalSpy->count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().first().toBool(), true);
        unsavedDataStatusChangedSignalSpy->clear();
        QCOMPARE(file->hasUnsavedData(), true);

        // Trigger with the same value again.
        file->triggerUnsavedStatusChange(true);
        // No unsavedDataStatusChanged emission.
        QTRY_COMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
        QCOMPARE(file->hasUnsavedData(), true);

        // Trigger unsaved change once with different value.
        file->triggerUnsavedStatusChange(false);
        // Signal unsavedDataStatusChanged should be emitted only once.
        QTRY_COMPARE(unsavedDataStatusChangedSignalSpy->count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().first().toBool(), false);
        unsavedDataStatusChangedSignalSpy->clear();
        QCOMPARE(file->hasUnsavedData(), false);

        // Trigger with the same value again.
        file->triggerUnsavedStatusChange(false);
        // No unsavedDataStatusChanged emission.
        QTRY_COMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
        QCOMPARE(file->hasUnsavedData(), false);
    }

    void test_file_opening() {
        // Open file.
        auto result = file->open(filePathToRead);
        // Verify that expected things happened.
        QVERIFY(result);
        QCOMPARE(file->data, fileData);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().first().value<QFileInfo>().absoluteFilePath(), filePathToRead);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
        QCOMPARE(file->hasUnsavedData(), false);
    }

    void test_file_open_when_unsaved() {
        set_file_unsaved_state(true);

        // Open the file.
        auto result = file->open(QFileInfo{filePathToRead});
        // Verify that expected things happened.
        QCOMPARE(result.has_value(), true);
        QCOMPARE(file->data, fileData);
        // Opening of the file should trigger the unsavedStatusChanged signal as we have "modified" file data.
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().first().toBool(), false);
    }

    void test_file_open_when_another_file_is_opened() {
        set_file_opened(anotherFilePathToRead);

        // Open the file.
        auto result = file->open(QFileInfo{filePathToRead});
        // Verify that expected things happened.
        QCOMPARE(result.has_value(), true);
        QCOMPARE(file->data, fileData);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().first().value<QFileInfo>().absoluteFilePath(), filePathToRead);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
    }

    void test_opening_non_existing_file() {
        // Open the file.
        auto result = file->open(nonExistingFile);
        // Verify that expected things happened.
        QCOMPARE(result.has_value(), true);
        QCOMPARE(file->data, QByteArray{});
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().first().value<QFileInfo>().absoluteFilePath(), nonExistingFile);
        fileInfoChangedSignalSpy->clear();
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
        // We still have nothing on disk
        QCOMPARE(QFileInfo::exists(nonExistingFile),false);
    }

    void test_opening_non_existing_file_in_unreachable_directory() {
        // Try opening the file
        auto result = file->open(nonExistingFileInUnreachableDir);
        // Verify that expected things happened.
        QCOMPARE(result.has_value(), false);
        QCOMPARE(file->data, QByteArray{});
        QCOMPARE(fileInfoChangedSignalSpy->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
    }

    void test_file_open_failure() {
        auto result = file->open(QFileInfo{unreachableFilePath});
        // Check if only expected stuff happened
        QCOMPARE(result.has_value(), false);
        QCOMPARE(result.error().isEmpty(), false);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(),0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(),0);
    }

    void test_file_open_failure_when_another_file_is_opened() {
        // Open some normal file.
        set_file_opened(filePathToRead);

        // Now try opening unreachable path.
        auto result = file->open(unreachableFilePath);
        // Nothing should happen.
        QCOMPARE(file->data, fileData);
        QCOMPARE(result.has_value(), false);
        QCOMPARE(result.error().isEmpty(), false);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 0);
    }

    void test_file_save() {
        set_file_opened(filePathToWrite);
        set_file_unsaved_state(true);
        file->data = fileData;

        auto result = file->save();
        QCOMPARE(result.has_value(), true);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().first().toBool(), false);
        QCOMPARE(FileTestHelper::tempFileData(filePathToWrite), fileData);
    }

    void test_file_save_as_when_unsaved_and_untitled() {
        file->data = fileData;
        set_file_unsaved_state(true);

        // Ok, just to be sure - lets verify that no file exists.
        if (QFile::exists(filePathToWrite)) {
            QFile file{filePathToWrite};
            QVERIFY(file.remove());
            QVERIFY(!QFile::exists(filePathToWrite));
        }

        // Try saving file as
        auto result = file->saveAs(filePathToWrite);
        QCOMPARE(result.has_value(), true);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().count(), 1);
        QCOMPARE(fileInfoChangedSignalSpy->first().first().value<QFileInfo>().absoluteFilePath(), filePathToWrite);
        QCOMPARE(file->isUntitled(), false);
        QCOMPARE(fileRemovedSignalSpy->count(), 0);
        QCOMPARE(fileModifiedOnDisk->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().count(), 1);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->first().first().toBool(), false);
        unsavedDataStatusChangedSignalSpy->clear();
        QCOMPARE(file->hasUnsavedData(), false);

        // Verify state on the hard drive
        const QFileInfo writtenFileInfo{filePathToWrite};
        QVERIFY(writtenFileInfo.exists());
        QCOMPARE(writtenFileInfo.size(), fileData.size());
    }

    void test_untitled_file_save_as_failure() {
        file->data = fileData;
        QVERIFY(file->isUntitled());

        // Try writing untitled file.
        auto result = file->saveAs(unreachableFilePath);
        QCOMPARE(result.has_value(), false);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(),0);
        QCOMPARE(file->hasUnsavedData(), false);
        QCOMPARE(file->isUntitled(), true);
        QCOMPARE(file->data, fileData);
    }

    void test_titled_file_save_as_failure() {
        set_file_opened(filePathToRead);

        // Try writing titled file.
        auto result = file->saveAs(unreachableFilePath);
        QCOMPARE(result.has_value(), false);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(),0);
        QCOMPARE(file->hasUnsavedData(), false);
        QCOMPARE(file->isUntitled(), false);
        QCOMPARE(file->data, fileData);
    }

    void test_untitled_modified_file_save_as_failure() {
        file->data = fileData;
        set_file_unsaved_state(true);

        // Try writing titled file.
        auto result = file->saveAs(unreachableFilePath);
        QCOMPARE(result.has_value(), false);
        QTRY_COMPARE(fileInfoChangedSignalSpy->count(), 0);
        QCOMPARE(unsavedDataStatusChangedSignalSpy->count(),0);
        QCOMPARE(file->hasUnsavedData(), true);
        QCOMPARE(file->isUntitled(), true);
        QCOMPARE(file->data, fileData);
    }

    void test_file_external_edit() {
        QFile anotherWayToAccessFile{fileToBeEdited};
        QVERIFY(anotherWayToAccessFile.exists());

        set_file_opened(fileToBeEdited);
        // Edit file "externaly"
        QVERIFY(anotherWayToAccessFile.open(QIODevice::ReadWrite));
        QVERIFY(anotherWayToAccessFile.write(anotherFileData) == anotherFileData.size());
        QVERIFY(anotherWayToAccessFile.flush());
        anotherWayToAccessFile.close();

        // Check signals emission
        QTRY_COMPARE(fileModifiedOnDisk->count(), 1);
        QCOMPARE(fileModifiedOnDisk->first().count(), 0);
        QCOMPARE(fileRemovedSignalSpy->count(), 0);
    }

    void test_file_external_removal() {
        QFile anotherWayToAccessFile{fileToBeRemoved};
        QVERIFY(anotherWayToAccessFile.exists());
        set_file_opened(fileToBeRemoved);

        // Remove file
        QVERIFY(anotherWayToAccessFile.remove());
        QVERIFY(anotherWayToAccessFile.exists() == false);

        // Check signals
        QTRY_COMPARE(fileRemovedSignalSpy->count(),1);
        QCOMPARE(fileRemovedSignalSpy->first().count(), 0);
        QCOMPARE(fileModifiedOnDisk->count(), 0);
    }
};

QTEST_MAIN(AbstractTextFileTest)

#include "AbstractTextFileTest.moc"
