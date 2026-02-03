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

#include <concepts>

#include <QMessageBox>
#include <QString>

namespace Draupnir::Handlers
{

namespace FileContext
{

template<class Context>
concept HasFileManagerType = requires { typename Context::FileManager; };

template<class Context>
concept HasFileManagerMethod = requires (Context context) {
    { context.fileManager() } -> std::same_as<typename Context::FileManager*>;
};

template<class Context>
concept HasAskUserMethod = requires {
    { Context::askUser(std::declval<QString>(), std::declval<QString>(), std::declval<QMessageBox::StandardButtons>()) }
    -> std::same_as<int>;
};

template<class Manager>
concept HasOnSaveFileMethod = requires(Manager manager) {
    { manager.onSaveFile() } -> std::same_as<void>;
};

template<class Manager>
concept HasOnSaveFileAsMethod = requires(Manager manager) {
    { manager.onSaveFileAs() } -> std::same_as<void>;
};

};

}; // namespace Draupnir::Handlers

#endif // FILECONTEXTCONCEPT_H
