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
#include "draupnir/files/ScopedFileTemplate.h"

/*! @class ScopedFileTemplateTest tests/modules/files/unit/ScopedFileTemplateTest.cpp
 *  @ingroup Files
 *  @ingroup Tests
 *  @brief Unit tests for @ref Draupnir::Files::ScopedFileTemplate class. */

class ScopedFileTemplateTest : public QObject
{
    Q_OBJECT
private:
    const QByteArray fileData{"VeryRandomData"};
    const QByteArray anotherFileData{"Some other data"};

    QString filePathToRead;          /*!< @brief This file will be used for reading. */
    QString filePathToRestore;       /*!< @brief This file will be removed and than restored. */
    QString filePathToWrite;         /*!< @brief This file will be used for writing. */

///@name Test state things
///@{
    Draupnir::Files::ScopedFileTemplate<DummyTextFile>* file = nullptr;
///@}

private slots:
    void initTestCase() {
        QVERIFY(FileTestHelper::tempDir().isValid());

        auto result = FileTestHelper::createTempFile("file_to_read.txt", fileData );
        QVERIFY(result.has_value());
        filePathToRead = result.value();

        result = FileTestHelper::createTempFile("file_to_be_restored.txt", fileData );
        QVERIFY(result.has_value());
        filePathToRestore = result.value();

        result = FileTestHelper::getTempFilePath("file_to_write.txt");
        QVERIFY(result);
        filePathToWrite = result.value();
    }

    void init() { file = new Draupnir::Files::ScopedFileTemplate<DummyTextFile>; }
    void cleanup() { delete file; file = nullptr; }

    void test_scoped_file_initial_state() {
        // Verify initial state of file object.
        QCOMPARE(file->abstractFile() != nullptr, true);
        QCOMPARE(file->file() != nullptr, true);
        QCOMPARE(file->file()->isUntitled(), true);
        QCOMPARE(file->file()->fileInfo().absoluteFilePath().isEmpty(), true);
    }

    void test_scoped_file_open() {
        auto result = file->open(filePathToRead);
        QCOMPARE(result.has_value(), true);
        QCOMPARE(file->file()->data, fileData);
    }

    void test_scoped_file_save_on_delete() {
        auto result = file->open(filePathToRestore);
        QVERIFY(result.has_value() == true);

        // Change file data and remove file from disk
        file->file()->data = anotherFileData;
        QVERIFY(QFile::remove(filePathToRestore));
        QVERIFY(!QFile::exists(filePathToRestore));
        // Delete scoped file
        delete file; file = nullptr;

        // We should have what we have written
        QCOMPARE(QFile::exists(filePathToRestore), true);
        QCOMPARE(FileTestHelper::tempFileData(filePathToRestore), anotherFileData);
    }
};

QTEST_MAIN(ScopedFileTemplateTest)

#include "ScopedFileTemplateTest.moc"
