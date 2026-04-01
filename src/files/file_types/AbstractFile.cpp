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

#include "draupnir/files/file_types/AbstractFile.h"

#include <QDebug>

namespace Draupnir::Files
{

AbstractFile::AbstractFile(QObject* parent) :
    QObject{parent},
    m_hasUnsavedData{false},
    m_currentFileInfo{QFileInfo{}}
{
    connect(&m_filesystemWatcher,&QFileSystemWatcher::fileChanged,
            this, &AbstractFile::_onFileChanged);
}

std::expected<void,QString> AbstractFile::open(const QString& filePath)
{
    return open(QFileInfo{filePath});
}

std::expected<void,QString> AbstractFile::open(const QFileInfo& fileInfo)
{
    if (!fileInfo.exists()) {
        const QFileInfo dirInfo{fileInfo.absolutePath()};
        const bool canCreateFile =
            dirInfo.exists() &&
            dirInfo.isDir() &&
            dirInfo.isWritable()
#ifdef Q_OS_UNIX
            && dirInfo.isExecutable()
#endif
            ;
        if (!canCreateFile) {
            return std::unexpected{tr("Can not create file in directory: ")+dirInfo.absoluteFilePath()};
        } else {
            setCurrentFileInfo(fileInfo);
            setUnsavedData(false);
            return {};
        }
    } else {
        return _fileDataLoaded(fileInfo);
    }
};

std::expected<void,QString> AbstractFile::save()
{
    Q_ASSERT(!isUntitled());
    return saveAs(fileInfo());
}

std::expected<void,QString> AbstractFile::saveAs(const QString& filePath)
{
    return saveAs(QFileInfo{filePath});
}

std::expected<void,QString> AbstractFile::saveAs(const QFileInfo& fileInfo)
{
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly | extraFlags()))
        return std::unexpected(QObject::tr("Error opening file %1.\r\n%2").arg(fileInfo.absoluteFilePath()).arg(file.errorString()));

    const QByteArray data = currentData();
    const qint64 bytesWritten = file.write(data);

    if (bytesWritten != data.size()) {
        return std::unexpected(QObject::tr("Error writing file %1.\r\n%2")
                                   .arg(fileInfo.absoluteFilePath()).arg(file.errorString()));
    }

    if (!file.flush()) {
        file.close();
        return std::unexpected(QObject::tr("Error flushing file %1.\r\n%2")
                                   .arg(fileInfo.absoluteFilePath()).arg(file.errorString()));
    }

    file.close();
    setCurrentFileInfo(fileInfo);
    setUnsavedData(false);
    return {};
};

void AbstractFile::setUnsavedData(bool state)
{
    if (m_hasUnsavedData == state)
        return;

    m_hasUnsavedData = state;
    emit unsavedDataStatusChanged(m_hasUnsavedData);
}

void AbstractFile::setCurrentFileInfo(const QFileInfo& fileInfo)
{
    if (m_currentFileInfo == fileInfo)
        return;

    if (!m_currentFileInfo.absoluteFilePath().isEmpty())
        m_filesystemWatcher.removePath(m_currentFileInfo.absoluteFilePath());

    m_currentFileInfo = fileInfo;
    m_filesystemWatcher.addPath(m_currentFileInfo.absoluteFilePath());
    emit fileInfoChanged(m_currentFileInfo);
}

std::expected<void,QString> AbstractFile::_fileDataLoaded(const QFileInfo& fileInfo)
{
    QFile file{fileInfo.absoluteFilePath()};
    if (!file.open(QIODevice::ReadOnly | extraFlags()))
        return std::unexpected(file.errorString());

    const QByteArray bytes = file.readAll();
    file.close();

    const auto result = dataProcessed(bytes);
    if (!result)
        return result;

    setCurrentFileInfo(fileInfo);
    setUnsavedData(false);
    return result;
}

void AbstractFile::_onFileChanged(const QString& filePath)
{
    if (!QFile::exists(filePath)) {
        emit fileRemovedFromDisk();
    } else {
        emit fileChangedOnDisk();
    }
}

}; // namespace Draupnir::Files
