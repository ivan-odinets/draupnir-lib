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

#include "draupnir-test/helpers/FileTestHelpers.h"
#include "draupnir-test/mocks/DummyFiles.h"

/*! @class AbstractJsonFileTest tests/modules/files/unit/AbstractJsonFileTest.cpp
 *  @ingroup Files
 *  @ingroup Tests
 *  @brief Unit tests for @ref Draupnir::Files::AbstractJsonFile class. */

class AbstractJsonFileTest : public QObject
{
    Q_OBJECT
private:
    const QJsonDocument jsonDocument{QJsonObject{
        { "array", QJsonArray{{1,2,3}} },
        { "boolean", true },
        { "color", "gold" },
        { "null" , QJsonValue::Null },
        { "number", 123 },
        { "object", QJsonObject{ { "a", "b" }, { "c", "d" } } },
        { "string", "Hello World" }
    }};
    QString validJsonFilePath;

    const QByteArray invalidJsonData{QByteArray{
        "{"
            "\"array\": [1,2,3],"
            "\"boolean\" I Am Broken Json!: true,"
            "\"color\": \"gold\","
            "\"null\": null,"
            "\"number\": 123, "
            "\"object\": { \"a\": \"b\", \"c\": \"d\" }, "
            "\"string\": \"Hello World\""
        "}"
    }};
    QString invalidJsonFilePath;

    const QString filePathToWrite{"json_file_written.json"};

    DummyJsonFile* file = nullptr;

private slots:
    void initTestCase() {
        auto result = FileTestHelper::createTempFile(
            "valid_json.json", jsonDocument.toJson() );
        QVERIFY(result.has_value());
        validJsonFilePath = result.value();

        result = FileTestHelper::createTempFile(
            "invalid_json.json", invalidJsonData );
        QVERIFY(result.has_value());
        invalidJsonFilePath = result.value();
    };

    void init() { file = new DummyJsonFile; }
    void cleanup() { delete file; file = nullptr; }

    void test_file_open() {
        auto result = file->open(QFileInfo{validJsonFilePath});
        QCOMPARE(result.has_value(), true);
        QCOMPARE(file->jsonDocument, jsonDocument);
    }

    void test_file_open_invalid_file() {
        auto result = file->open(QFileInfo{invalidJsonFilePath});
        QCOMPARE(result.has_value(), false);
    }

    void test_file_save_as() {
        file->jsonDocument = jsonDocument;
        auto result = file->saveAs(filePathToWrite);
        QCOMPARE(result.has_value(), true);

        const QByteArray dataWritten{FileTestHelper::tempFileData(filePathToWrite).value()};
        QCOMPARE(dataWritten, jsonDocument.toJson());
    };
};

QTEST_MAIN(AbstractJsonFileTest)

#include "AbstractJsonFileTest.moc"
