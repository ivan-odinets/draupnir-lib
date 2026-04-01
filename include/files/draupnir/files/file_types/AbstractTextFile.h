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

#ifndef ABSTRACTTEXTFILE_H
#define ABSTRACTTEXTFILE_H

#include "draupnir/files/file_types/AbstractFile.h"

namespace Draupnir::Files
{

/*! @class AbstractTextFile draupnir/files/file_types/AbstractTextFile.h
 *  @ingroup Files
 *  @brief This is a class. */

class AbstractTextFile : public AbstractFile
{
public:
    explicit AbstractTextFile(QObject* parent = nullptr) :
        AbstractFile{parent}
    {}
    ~AbstractTextFile() override = default;

protected:
    QIODevice::OpenMode extraFlags() const override { return QIODevice::Text; }

    virtual std::expected<void,QString> dataProcessed(const QByteArray& bytes) override = 0;
    virtual QByteArray currentData() const override = 0;
};

}; // namespace Draupnir::Files

#endif // ABSTRACTTEXTFILE_H
