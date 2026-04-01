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

#ifndef FILETESTHELPERS_H
#define FILETESTHELPERS_H

#include <optional>

#include <QDebug>
#include <QTemporaryDir>
#include <QFileDevice>
#include <QtTest>

#include <expected>

/*! @class FileTestHelper draupnir-test/helpers/FileTestHelper.h
 *  @ingroup Tests
 *  @brief Static helper class for testing file-related classes and methods.
 * @todo Migrate from std::optional to std::expected. */

class FileTestHelper
{
public:
    static constexpr auto allReadWrite =
        QFileDevice::ReadUser  | QFileDevice::WriteUser |
        QFileDevice::ReadGroup | QFileDevice::WriteGroup |
        QFileDevice::ReadOther | QFileDevice::WriteOther;

    static QTemporaryDir& tempDir() {
        static QTemporaryDir dir;
        if (!dir.isValid()) {
            qCritical() << "Directory: "<<dir.path()<<" is not valid.";
        }

        return dir;
    }

    static std::optional<QString> getTempFilePath(const QString& fileName) {
        return (tempDir().isValid()) ?
            std::optional<QString>{tempDir().filePath(fileName)} :
            std::nullopt;
    }

    static std::optional<QString> createTempFile(const QString& name, const QByteArray& data = QByteArray{}, QFileDevice::Permissions permissions = allReadWrite) {
        auto maybePath = getTempFilePath(name);
        if (!maybePath.has_value())
            return std::nullopt;

        const QString path = maybePath.value();
        QFile file{path};
        if (!file.open(QIODevice::WriteOnly)) {
            qCritical() << "Error opening file: "<<path<<" for writing. "
                        << file.errorString();
            return std::nullopt;
        }

        if (!data.isEmpty()) {
            const qint64 bytesWritten = file.write(data);
            if (bytesWritten != data.size()) {
                qCritical() << "Amount of bytes written: "<<bytesWritten
                            << " but it should be: "<<data.size();
                return std::nullopt;
            }
        }

        if (!file.setPermissions(permissions)) {
            qCritical() << "Error setting permissions for: "<<path
                        << ". Reason: "<<file.errorString();
        };
        if (!QFile::exists(path)) {
            return std::nullopt;
        }
        return path;
    };

    static std::optional<QString> createTempDirectory(const QString& directoryName, QFileDevice::Permissions permissions = allReadWrite) {
        const QString resultingPath = tempDir().filePath(directoryName);
        if (!QDir{}.mkpath(resultingPath)) {
            qCritical() << "Can not create directory: "<<resultingPath;
            return std::nullopt;
        }

        if (!QFile::setPermissions(resultingPath, permissions)) {
            qCritical() << "Can not set permissions for: "<<resultingPath;
            return std::nullopt;
        }
        return std::optional<QString>{resultingPath};
    }

    static std::optional<QByteArray> tempFileData(const QString& filePath) {
        QFile file{filePath};
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << "Error opening file: "<<filePath<<" for reading. "
                        << file.errorString();
            return std::nullopt;
        }
        return std::optional<QByteArray>{file.readAll()};
    };
};

#endif // FILETESTHELPERS_H
