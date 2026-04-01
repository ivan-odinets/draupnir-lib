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

#ifndef SCOPEDFILETEMPLATE_H
#define SCOPEDFILETEMPLATE_H

#include "draupnir/files/file_types/AbstractFile.h"

namespace Draupnir::Files
{

class AbstractEnsuredFile : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEnsuredFile(AbstractFile* file, QObject* parent = nullptr) :
        QObject{parent},
        p_file{file}
    {}
    ~AbstractEnsuredFile() {
        // If we have sth unsaved - lets save it.
        if (p_file->hasUnsavedData())
            p_file->save();
        else if (!QFile::exists(p_file->fileInfo().absoluteFilePath())) {
            p_file->save();
        }
        delete p_file;
    }

    template<class Argument>
    auto open(const Argument& arg) {
        return p_file->open(arg);
    }

    AbstractFile* abstractFile() { return p_file; }

protected:
    AbstractFile* p_file;
};

template<class File>
class ScopedFileTemplate : public AbstractEnsuredFile
{
public:
    explicit ScopedFileTemplate(QObject* parent = nullptr) :
        AbstractEnsuredFile{new File,parent}
    {}

    File* file() { return p_file->to<File>(); }
};

}; // namespace Draupnir::Files

#endif // SCOPEDFILETEMPLATE_H
