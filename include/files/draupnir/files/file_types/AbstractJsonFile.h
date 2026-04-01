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

#ifndef ABSTRACTJSONFILE_H
#define ABSTRACTJSONFILE_H

#include "draupnir/files/file_types/AbstractTextFile.h"

#include <QJsonDocument>

namespace Draupnir::Files
{

/*! @class AbstractJsonFile draupnir/files/file_types/AbstractJsonFile.h
 *  @ingroup Files
 *  @brief This is a class. */

class AbstractJsonFile : public AbstractTextFile
{
public:
    explicit AbstractJsonFile(QObject* parent = nullptr) :
        AbstractTextFile{parent}
    {}
    ~AbstractJsonFile() override = default;
protected:
    std::expected<void,QString> dataProcessed(const QByteArray& bytes) override {
        QJsonParseError jsonError;
        QJsonDocument jsonDocument{QJsonDocument::fromJson(bytes,&jsonError)};

        if (jsonError.error != QJsonParseError::NoError) {
            return std::unexpected{jsonError.errorString()};
        }

        return jsonDocumentParsed(jsonDocument);
    }

    virtual std::expected<void,QString> jsonDocumentParsed(const QJsonDocument&) = 0;

    QByteArray currentData() const override {
        return currentJsonDocument().toJson();
    }

    virtual QJsonDocument currentJsonDocument() const = 0;
};

}; // namespace Draupnir::Files

#endif // ABSTRACTJSONFILE_H
