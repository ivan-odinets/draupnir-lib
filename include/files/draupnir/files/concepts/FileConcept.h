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

#ifndef FILECONCEPT_H
#define FILECONCEPT_H

#include <expected>

#include <QByteArray>
#include <QFileInfo>

namespace Draupnir::Files
{

namespace File
{

template<class Candidate>
concept HasDataProcessed = requires(Candidate& object, QByteArray bytes) {
    { object.dataProcessed(bytes) } -> std::same_as<std::expected<void,QString>>;
};

template<class Candidate>
concept HasCurrentData = requires(Candidate& object) {
    { object.currentData() } -> std::same_as<QByteArray>;
};

template<class Candidate>
concept HasExtraFlags = requires {
    { Candidate::extraFlags() } -> std::same_as<QIODevice::OpenMode>;
};

template<class Candidate>
concept HasOpen = requires(Candidate& object, QString path, QFileInfo fileInfo) {
    { object.open(path) }  -> std::same_as<std::expected<void,QString>>;
    { object.open(fileInfo) }  -> std::same_as<std::expected<void,QString>>;
};

template<class Candidate>
concept HasSave = requires(Candidate& object) {
    { object.save() } -> std::same_as<std::expected<void,QString>>;
};

};

template<class Candidate>
concept CanBeOpenedAndSaved =
    File::HasOpen<Candidate> &&
    File::HasSave<Candidate>;

}; // namespace Draupnir::Files

#endif // FILECONCEPT_H
