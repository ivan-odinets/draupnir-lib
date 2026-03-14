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

/*! @class FileMenuEntriesHandlerIT
 *  @brief This is a test class for testing basic functionality of the FileMenuEntriesHandler */

#include "draupnir/ui_bricks/handlers/file_menu/FileMenuEntriesHandler.h"
#include "draupnir/utils/type_list.h"

#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"
#include "draupnir-test/mocks/MockSettingsTemplate.h"

class FileMenuEntriesHandlerIT : public QObject
{
    Q_OBJECT
public:
    using SettingsLst = draupnir::utils::type_list<
        Draupnir::Settings::LastUsedDirectorySetting
    >;

    typename SettingsLst::convert_to_t<
        MockSettingsTemplate
    > settings;

    typename SettingsLst::convert_to_t<
        Draupnir::Settings::SettingsRegistryTemplate
    > settingsRegistry;

   Draupnir::Handlers::FileMenuHandler<
       DummySingleFileManager,
       Draupnir::Ui::FileNewEntry,Draupnir::Ui::FileOpenEntry,Draupnir::Ui::FileSaveAsEntry
   > handler;

    FileMenuEntriesHandlerIT() {
       settingsRegistry.setBackend(&settings);
    }

private slots:
    void test_something() {};
};

QTEST_MAIN(FileMenuEntriesHandlerIT)

#include "FileMenuEntriesHandlerIT.moc"
