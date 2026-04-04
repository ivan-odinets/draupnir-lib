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

#ifndef ABSTRACTFILEMANAGER_H
#define ABSTRACTFILEMANAGER_H

#include <QObject>
#include <QFileInfo>

#include "draupnir/files/file_types/AbstractFile.h"

namespace Draupnir::Files
{

/*! @class AbstractFileManager draupnir/files/managers/AbtsractFileManager.h
 *  @brief Base class for file manager implementations.
 *  @ingroup Files
 *  @details Provides common Qt signals for tracking the currently managed file and its file information. */

class AbstractFileManager : public QObject
{
    Q_OBJECT
public:
    /*! @brief Constructs the file manager object.
     *  @param parent Optional Qt parent object. */
    explicit AbstractFileManager(QObject* parent = nullptr) :
        QObject{parent}
    {}

    /*! @brief Virtual destructor. */
    virtual ~AbstractFileManager() override = default;

signals:
    /*! @brief Emitted when the currently managed file changes.
     *  @param file Pointer to the new current file, or `nullptr` if no file is open. */
    void currentAbstractFileChanged(Draupnir::Files::AbstractFile* file);

    /*! @brief Emitted when information about the current file changes.
     *  @param fileInfo New file information.
     * @todo Question: Is this signal usefull? */
    void currentFileInfoChanged(const QFileInfo&);
};

}; // namespace Draupnir::Files

#endif // ABSTRACTFILEMANAGER_H
