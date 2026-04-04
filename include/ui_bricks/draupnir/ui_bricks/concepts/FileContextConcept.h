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

#ifndef FILECONTEXTCONCEPT_H
#define FILECONTEXTCONCEPT_H

#include <QMessageBox>
#include <QString>

namespace Draupnir::Handlers
{

/*! @namespace FileContext draupnir/ui_bricks/concepts/FileContextConcept.h
 *  @brief Helper concepts for validating file-related context and file manager types.
 *  @ingroup UiBricks
 *
 *  @details Contains compile-time checks used to verify:
 *           - presence of a file manager type in a context;
 *           - presence of an accessor returning the file manager;
 *           - presence of a helper method for asking the user;
 *           - presence of save/save-as API in a file manager. */

namespace FileContext
{


/*! @concept HasFileManagerType
 *  @brief Checks whether a context defines a nested `FileManager` type.*/
template<class Context>
concept HasFileManagerType = requires { typename Context::FileManager; };

/*! @concept HasFileManagerMethod
 *  @brief Checks whether a context provides a `fileManager()` method. The method must return a pointer to `Context::FileManager`. */
template<class Context>
concept HasFileManagerMethod = requires (Context context) {
    { context.fileManager() } -> std::same_as<typename Context::FileManager*>;
};

/*! @concept HasAskUserMethod
 *  @brief Checks whether a context provides a static `askUser(...)` method. This method is expected to show a dialog to the user
 *         and return an integer compatible with a `QMessageBox` result. */
template<class Context>
concept HasAskUserMethod = requires {
    { Context::askUser(std::declval<QString>(), std::declval<QString>(), std::declval<QMessageBox::StandardButtons>()) }
    -> std::same_as<int>;
};

};

}; // namespace Draupnir::Handlers

#endif // FILECONTEXTCONCEPT_H
