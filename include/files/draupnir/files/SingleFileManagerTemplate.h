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

#ifndef SINGLEFILEMANAGERTEMPLATE_H
#define SINGLEFILEMANAGERTEMPLATE_H

#include <QDebug>

#include "draupnir/files/file_types/AbstractFile.h"

/*! @class SingleFileManager
 *  @brief This is a class
 * @todo DocumentMe. */

namespace Draupnir::Files
{

class AbstractFileManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractFileManager(QObject* parent = nullptr) :
        QObject{parent}
    {}
    virtual ~AbstractFileManager() = default;

signals:
    void currentAbstractFileChanged(AbstractFile* file);
    void currentFileInfoChanged(const QFileInfo&);

private:
};

template<class FileClass>
class SingleFileManagerTemplate : public AbstractFileManager
{
public:
    /*! @brief Static constexpr method returning if this SingleFileManager implementation can open multiple files at once
     *         (false). It is required within handlers of file menu entires. */
    static constexpr bool canOpenMultipleFilesAtOnce() { return false; }

    /*! @brief Static constexpr method returning if this SingleFileManager implementation can have multiple files opened
     *         (false). It is required within handlers of file menu entires. */
    static constexpr bool canHaveMultipleFilesOpened() { return false; }

    /*! @brief Default constructor. Checks if the provided Implementation template argument is derived from this template
     *         class and connects some internall things.
     * @todo Add propper checks for method presense within Implementation AND FileClass. */
    SingleFileManagerTemplate() :
        p_currentFile{nullptr}
    {}

    /*! @brief Destructor. Deletes FileClass object. */
    ~SingleFileManagerTemplate() {
        if (p_currentFile)
            delete p_currentFile;
    }

    /*! @brief Creates the new FileClass object and sets it as a current file. If any FileClass object is opened - deletes
     *         it. */
    void newFile() {
        _setCurrentFile(new FileClass);
    }

    /*! @brief Opens file. */
    std::expected<void,QString> openFile(const QFileInfo& fileInfo) {
        FileClass* maybeNewFile = new FileClass;
        auto openResult = maybeNewFile->open(fileInfo);
        if (!openResult.has_value()) {
            delete maybeNewFile;
            return openResult;
        } else {
            _setCurrentFile(maybeNewFile);
            return {};
        }
    }

    /*! @brief This is a method. */
    void saveCurrentFile() {
        Q_ASSERT_X(p_currentFile, "SingleFileManager::saveCurrentFile",
            "This method should be called only if there is file opened.");
        Q_ASSERT_X(currentFileHasName(), "SingleFileManager::saveCurrentFile",
            "This method should be called only if there is file opened and this file has a name.");

        p_currentFile->save();
    }

    void saveCurrentFileAs(const QString& filePath) {
        saveCurrentFileAs(QFileInfo{filePath});
    }

    std::expected<void,QString> saveCurrentFileAs(const QFileInfo& fileInfo) {
        Q_ASSERT_X(p_currentFile, "SingleFileManager::saveCurrentFileAs",
            "This method should be called only if there is file opened.");
        return p_currentFile->saveAs(fileInfo);
    }

    /*! @brief This is a method. */
    void closeFile() {
        Q_ASSERT_X(p_currentFile, "SingleFileManager::closeCurrentFileAs",
            "This method should be called only if there is file opened.");

            _setCurrentFile(nullptr);
    }

    FileClass* currentFile() { return p_currentFile; }

    bool currentFileHasName() const {
        return (p_currentFile)
            ? !p_currentFile->isUntitled()
            : false;
    }

    bool hasUnsavedData() const {
        return (p_currentFile ? p_currentFile->hasUnsavedData() : false);
    }

    bool hasNothingOpened() const { return p_currentFile == nullptr; }

    /*! @brief If there is file opened and was saved (at least some time ago) - returns QFileInfo representing this file.
     *         If not - returns empty QFileInfo. */
    QFileInfo currentFileInfo() const {
        return p_currentFile ? p_currentFile->fileInfo() : QFileInfo{};
    }

private:
    FileClass* p_currentFile;

    /*! @brief This private method is used whenever the current file is changed (new file is created, file opened, etc. */
    void _setCurrentFile(FileClass* newCurrentFile) {
        if (p_currentFile != nullptr) {
            delete p_currentFile;
        }

        p_currentFile = newCurrentFile;
        emit currentAbstractFileChanged(p_currentFile);
    }
};

} // namespace Draupnir::Files

#endif // SINGLEFILEMANAGERTEMPLATE_H
