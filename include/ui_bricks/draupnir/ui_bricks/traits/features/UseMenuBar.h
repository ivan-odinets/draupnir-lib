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

#ifndef USEMENUBAR_H
#define USEMENUBAR_H

class QMenuBar;

#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"

namespace Draupnir::Ui::MainWindow
{

template<class Candidate>
concept IsQMenuBarBased =
    std::is_base_of_v<QMenuBar,Candidate>;

template<IsQMenuBarBased MenuBarImplementation>
struct UseMenuBar
{
    using SettingsBundle = typename Draupnir::Settings::SettingsTraitsConcatenator<MenuBarImplementation>::toSettingsBundle;

    using MenuBar = MenuBarImplementation;

    MenuBarImplementation* state{nullptr};
};

}; // namespace Draupnir::Ui

#endif // USEMENUBAR_H
