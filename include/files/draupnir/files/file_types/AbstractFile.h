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

#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <QObject>

#include <expected>

#include <QFileInfo>
#include <QFileSystemWatcher>

namespace Draupnir::Files
{

/*! @class AbstractFile draupnir/files/file_types/AbstractFile.h
 *  @ingroup Files
 *  @brief Abstract runtime base class for file objects used within Draupnir file management infrastructure.
 *  @details This class provides the common QObject-based interface for file-like objects, including:
 *           - opening and saving files;
 *           - tracking the currently associated QFileInfo;
 *           - tracking unsaved in-memory changes;
 *           - emitting signals when file metadata or save state changes.
 *
 *           The save() method saves the current file and is only valid for non-untitled files. */

class AbstractFile : public QObject
{
    Q_OBJECT
public:
    explicit AbstractFile(QObject* parent = nullptr);
    virtual ~AbstractFile() = default;

    std::expected<void,QString> open(const QString& filePath);
    std::expected<void,QString> open(const QFileInfo& fileInfo);

    std::expected<void,QString> save();

    std::expected<void,QString> saveAs(const QString& filePath);
    std::expected<void,QString> saveAs(const QFileInfo& fileInfo);

    /*! @brief Returns true if the data within this @ref AbstractFile was not saved. */
    bool hasUnsavedData() const { return m_hasUnsavedData; }

    /*! @brief Returns QFileInfo representing this @ref AbstractFile. */
    QFileInfo fileInfo() const { return m_currentFileInfo; }

    /*! @brief This is a method. */
    bool isUntitled() const { return m_currentFileInfo.absoluteFilePath().isEmpty(); }

    template<class Derived>
    Derived* to() {
        static_assert(std::is_base_of_v<AbstractFile,Derived>);
        Q_ASSERT(dynamic_cast<Derived*>(this));
        return static_cast<Derived*>(this);
    }

signals:
    void fileChangedOnDisk();
    void fileRemovedFromDisk();
    void unsavedDataStatusChanged(bool status);
    void fileInfoChanged(const QFileInfo& fileInfo);

protected:
    void setUnsavedData(bool state);
    void setCurrentFileInfo(const QFileInfo& fileInfo);

    virtual QIODevice::OpenMode extraFlags() const { return {}; }
    virtual std::expected<void,QString> dataProcessed(const QByteArray& bytes) = 0;
    virtual QByteArray currentData() const = 0;

private:
    std::expected<void,QString> _fileDataLoaded(const QFileInfo& fileInfo);
    void _onFileChanged(const QString& filePath);

    bool m_hasUnsavedData;
    QFileInfo m_currentFileInfo;
    QFileSystemWatcher m_filesystemWatcher;
};

};

#endif // ABSTRACTFILE_H
