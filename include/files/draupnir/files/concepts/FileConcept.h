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

#include "draupnir/files/file_types/AbstractFile.h"

namespace Draupnir::Files
{

/*! @namespace File
 *  @brief Helper concepts for validating file-like types.
 *  @ingroup Files
 *
 *  @details Contains compile-time checks for types that provide file opening and saving functionality. */

namespace File
{

/*! @brief Checks whether a type provides file opening methods.
 *  @details A valid type must implement both overloads of `open(...)`: one accepting a file path and one accepting a
 *           `QFileInfo`.
 *
 *           Expected signatures:
 *           @code
 *           std::expected<void,QString> open(QString path);
 *           std::expected<void,QString> open(QFileInfo fileInfo);
 *           @endcode */

template<class Candidate>
concept HasOpen = requires(Candidate& object, QString path, QFileInfo fileInfo) {
    { object.open(path) }  -> std::same_as<std::expected<void,QString>>;
    { object.open(fileInfo) }  -> std::same_as<std::expected<void,QString>>;
};

/*! @brief Checks whether a type provides a `save()` method.
 *  @details Expected signature:
 *           @code
 *           std::expected<void,QString> save();
 *           @endcode */

template<class Candidate>
concept HasSave = requires(Candidate& object) {
    { object.save() } -> std::same_as<std::expected<void,QString>>;
};

}; // namespace Draupnir::Files::File

/*! @brief Checks whether a type supports both opening and saving. Combines the requirements of `File::HasOpen` and
 *  `File::HasSave`.*/

template<class Candidate>
concept CanBeOpenedAndSaved =
    File::HasOpen<Candidate> &&
    File::HasSave<Candidate>;

/*! @brief Checks whether a type is derived from `AbstractFile`. */

template<class Candidate>
concept AbstractFileBased =
    std::default_initializable<Candidate> &&
    std::derived_from<Candidate,AbstractFile>;

}; // namespace Draupnir::Files

#endif // FILECONCEPT_H
