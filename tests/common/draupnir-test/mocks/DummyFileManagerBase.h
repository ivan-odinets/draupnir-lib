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

#ifndef DUMMYFILEMANAGERBASE_H
#define DUMMYFILEMANAGERBASE_H

#include <QFileInfo>
#include <QString>
#include <QStringList>

/*! @class DummyFileManagerBase draupnir-test/mocks/DummyFileManagerBase.h
 *  @brief This is a base class for FileManager mocks. */

class DummyFileManagerBase
{
public:
    bool currentFileSaved_value;
    bool isCurrentFileSaved() const { return currentFileSaved_value; }

    bool hasNothingOpened_value;
    bool hasNothingOpened() const { return hasNothingOpened_value; }

    int closeCurrentFile_callCount = 0;
    void closeCurrentFile() { closeCurrentFile_callCount++;  }

    bool currentFileHasName_value;
    bool currentFileHasName() const { return currentFileHasName_value; }

    int saveCurrentFile_callCount = 0;
    void saveCurrentFile() { saveCurrentFile_callCount++; }

    QStringList saveCurrentFileAs_callArguments;
    int saveCurrentFileAs_callCount = 0;
    void saveCurrentFileAs(const QString& filePath) {
        saveCurrentFileAs_callArguments.append(filePath);
        saveCurrentFileAs_callCount++;
    }

    int newFile_callCount = 0;
    void newFile() { newFile_callCount++; }

    void openFile(const QFileInfo&) {};
};

#endif // DUMMYFILEMANAGERBASE_H
