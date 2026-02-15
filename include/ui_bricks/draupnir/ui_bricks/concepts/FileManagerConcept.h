/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef FILEMANAGERCONCEPT_H
#define FILEMANAGERCONCEPT_H

#include <QString>
#include <QFileInfo>

namespace Draupnir::Handlers
{

/*! @namespace FileManagerConcept draupnir/ui_bricks/concepts/FileManagerConcept.h
 *  @ingroup UiBricks
 *  @brief This is concept namespace. */

namespace FileManagerConcept
{
    template<class Manager>
    concept HasCloseCurrentFileMethod = requires(Manager manager) {
        { manager.closeCurrentFile() } -> std::same_as<void>;
    };

    template<class Manager>
    concept HasIsCurrentFileSavedMethod = requires(Manager manager) {
        { manager.isCurrentFileSaved() } -> std::same_as<bool>;
    };

    template<class Manager>
    concept HasHasNothingOpenedMethod = requires(Manager manager) {
        { manager.hasNothingOpened() } -> std::same_as<bool>;
    };

    template<class Manager>
    concept HasNewFileMethod = requires(Manager manager) {
        { manager.newFile() } -> std::same_as<void>;
    };

    template<class Manager>
    concept CanHaveMultipleFilesOpened =
        requires { { Manager::canHaveMultipleFilesOpened() } -> std::same_as<bool>; } &&
        Manager::canHaveMultipleFilesOpened();

    template<class Manager>
    concept HasOpenFileMethod = requires(Manager manager, QFileInfo i) {
        manager.openFile(i);
    };
};

};

#endif // FILEMANAGERCONCEPT_H
