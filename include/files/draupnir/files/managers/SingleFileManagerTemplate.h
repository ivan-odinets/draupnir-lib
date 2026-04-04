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

#include "draupnir/files/concepts/FileConcept.h"
#include "draupnir/files/managers/AbstractFileManager.h"

namespace Draupnir::Files
{

/*! @class SingleFileManagerTemplate draupnir/files/managers/SingleFileManagerTemplate.h
 *  @ingroup Files
 *  @brief File manager implementation that manages a single file object.
 *  @tparam FileClass File type managed by this class. Must satisfy @ref AbstractFileBased.
 *
 *  @details This manager owns at most one file instance at a time. Opening or creating a new file replaces the currently
 *           managed file.
 *
 *  @todo Remove canOpenMultipleFilesAtOnce() and canHaveMultipleFilesOpened(). File menu handlers may use draupnir::utils::is_template_base_of
 *        to distinguish between single-file and multi-file manager implementations. */

template<AbstractFileBased FileClass>
class SingleFileManagerTemplate : public AbstractFileManager
{
public:
    /*! @brief Returns whether this manager can open multiple files at once.
     *  @return Always `false`.
     * @note This method is used by file menu entry handlers. */
    static constexpr bool canOpenMultipleFilesAtOnce() { return false; }

    /*! @brief Returns whether this manager can keep multiple files open simultaneously.
     *  @return Always `false`.
     * @note This method is used by file menu entry handlers. */
    static constexpr bool canHaveMultipleFilesOpened() { return false; }

    /*! @brief Constructs an empty single-file manager. */
    explicit SingleFileManagerTemplate(QObject* parent = nullptr) :
        AbstractFileManager{parent},
        p_currentFile{nullptr}
    {}

    /*! @brief Destroys the manager and deletes the currently managed file, if any. */
    ~SingleFileManagerTemplate() {
        if (p_currentFile)
            delete p_currentFile;
    }

    /*! @brief Creates a new file object and makes it the current file. If another file is currently managed, it is deleted
     *         and replaced. */
    void newFile() {
        _setCurrentFile(new FileClass);
    }

    /*! @brief Opens a file and makes it the current file. If opening succeeds, the previously managed file is deleted and
     *         replaced. If opening fails, the current file remains unchanged.
     *  @param fileInfo Information about the file to open.
     *  @return Empty result on success or an error message on failure. */
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

    /*! @brief Saves the currently managed file. The current file must exist and must already have a file name
     *  @return Empty result on success or an error message on failure. */
    std::expected<void,QString> saveCurrentFile() {
        Q_ASSERT_X(p_currentFile, "SingleFileManagerTemplate<FileClass>::saveCurrentFile",
            "This method should be called only if there is file opened.");
        Q_ASSERT_X(currentFileHasName(), "SingleFileManagerTemplate<FileClass>::saveCurrentFile",
            "This method should be called only if there is file opened and this file has a name.");

        return p_currentFile->save();
    }

    /*! @brief Saves the current file under a new file path.
     *  @param filePath Target file path.
     *  @return Empty result on success or an error message on failure. */
    std::expected<void,QString> saveCurrentFileAs(const QString& filePath) {
        return saveCurrentFileAs(QFileInfo{filePath});
    }

    /*! @brief Saves the current file under a new file location. The current file must exist.
     *  @param fileInfo Target file information.
     *  @return Empty result on success or an error message on failure. */
    std::expected<void,QString> saveCurrentFileAs(const QFileInfo& fileInfo) {
        Q_ASSERT_X(p_currentFile, "SingleFileManagerTemplate<FileClass>::saveCurrentFileAs",
            "This method should be called only if there is file opened.");
        return p_currentFile->saveAs(fileInfo);
    }

    /*! @brief Closes the currently managed file. Deletes the current file object and clears the manager state. */
    void closeFile() {
        Q_ASSERT_X(p_currentFile, "SingleFileManagerTemplate<FileClass>::closeFile",
            "This method should be called only if there is file opened.");

        _setCurrentFile(nullptr);
    }

    /*! @brief Returns the currently managed file.
     *  @return Pointer to the current file, or `nullptr` if no file is open. */
    FileClass* currentFile() { return p_currentFile; }

    /*! @brief Returns whether the current file already has a file name.
     *  @return `true` if a file is open and is not untitled, otherwise `false`. */
    bool currentFileHasName() const {
        return (p_currentFile)
            ? !p_currentFile->isUntitled()
            : false;
    }

    /*! @brief Returns whether the current file contains unsaved data.
     *  @return `true` if a file is open and has unsaved changes, otherwise `false`. */
    bool hasUnsavedData() const {
        return (p_currentFile ? p_currentFile->hasUnsavedData() : false);
    }

    /*! @brief Returns whether no file is currently managed.
     *  @return `true` if no file is open, otherwise `false`. */
    bool hasNothingOpened() const { return p_currentFile == nullptr; }

    /*! @brief Returns file information for the current file.
     *  @return `QFileInfo` representing the current file, or an empty `QFileInfo` if no file is currently managed. */
    QFileInfo currentFileInfo() const {
        return p_currentFile ? p_currentFile->fileInfo() : QFileInfo{};
    }

private:
    FileClass* p_currentFile;

    /*! @brief Replaces the currently managed file.
     *  @param newCurrentFile New file object, or `nullptr` to clear the current file.
     *  @details Deletes the previously managed file, stores the new one, reconnects file-related signals, updates file info
     *           state, and emits corresponding manager signals. Passing the currently managed file object is considered a
     *           logic error. */
    void _setCurrentFile(FileClass* newCurrentFile) {
        Q_ASSERT(p_currentFile != newCurrentFile);
        const QFileInfo oldFileInfo = currentFileInfo();

        if (p_currentFile != nullptr) {
            delete p_currentFile;
            p_currentFile = nullptr;
        }
        p_currentFile = newCurrentFile;
        const QFileInfo newFileInfo = currentFileInfo();

        if (p_currentFile) {
            connect(p_currentFile, &AbstractFile::fileInfoChanged,
                    this, &AbstractFileManager::currentFileInfoChanged);
        }

        if (p_currentFile == nullptr)
            emit currentFileInfoChanged(QFileInfo{});
        else if (oldFileInfo.absoluteFilePath() != newFileInfo.absoluteFilePath())
            emit currentFileInfoChanged(newFileInfo);

        emit currentAbstractFileChanged(p_currentFile);
    }
};

} // namespace Draupnir::Files

#endif // SINGLEFILEMANAGERTEMPLATE_H
