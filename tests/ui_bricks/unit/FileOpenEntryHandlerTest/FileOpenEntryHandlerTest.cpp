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

#include "draupnir/ui_bricks/handlers/file_menu/FileOpenEntryHandler.h"

#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummyMultipleFileManager.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

/*! @class FileNewEntryHandlerTest
 *  @brief This is a test class for testing basic functionality of the FileNewEntryHandler */

class FileOpenEntryHandlerTest final : public QObject
{
    Q_OBJECT
public:
    using MultipleFileContext = DummyFileContext<DummyMultipleFileManager>;
    using SingleFileContext = DummyFileContext<DummySingleFileManager>;

    using SingleFileOpenEntryHandler = Draupnir::Handlers::GenericMenuEntryHandler<
        SingleFileContext,Draupnir::Ui::FileOpenEntry
    >;
    using MultipleFileOpenEntryHandler = Draupnir::Handlers::GenericMenuEntryHandler<
        MultipleFileContext,Draupnir::Ui::FileOpenEntry
    >;

    SingleFileContext dummySingleContext;
    SingleFileOpenEntryHandler singleHandler{dummySingleContext};

    MultipleFileContext dummyMultipleContext;
    MultipleFileOpenEntryHandler multipleHandler{dummyMultipleContext};

private slots:
    void test_open_handler_with_multifile() {
        dummyMultipleContext.getOpenFileNames_result = QStringList{};
        dummyMultipleContext.updateLastUsedDirectory_value = QString{};

        // Trigger
        multipleHandler.onTriggered();

        // Check that nothing happened
        QCOMPARE(dummyMultipleContext.updateLastUsedDirectory_value, QString{});

    }
};

QTEST_MAIN(FileOpenEntryHandlerTest)

#include "FileOpenEntryHandlerTest.moc"
