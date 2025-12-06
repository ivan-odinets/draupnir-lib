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

#ifndef DUMMYFILECONTEXT_H
#define DUMMYFILECONTEXT_H

#include <QString>
#include <QMessageBox>

/*! @class DummyFileContext draupnir-tests/mocks/DummyFileContext.h
 *  @brief This is a DummyFileContext class for @ref HandlerTemplates tests. */

template<class Manager>
class DummyFileContext
{
public:
    using FileManager = Manager;

    FileManager fileManager_field;
    FileManager* fileManager() { return &fileManager_field; }

    static inline int askUser_result = 0;
    static int askUser(const QString&, const QString&, QMessageBox::StandardButtons) {
        return askUser_result;
    }

    int onSaveFile_callCount = 0;
    void onSaveFile() { onSaveFile_callCount++; }

    int onSaveFileAs_callCount = 0;
    void onSaveFileAs() { onSaveFileAs_callCount++; }

    QString getSaveFileName_result;
    QString getSaveFileName() const { return getSaveFileName_result; }
};

#endif // DUMMYFILECONTEXT_H
