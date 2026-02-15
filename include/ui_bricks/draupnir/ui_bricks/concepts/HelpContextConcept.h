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

#ifndef HELPCONTEXTCONCEPT_H
#define HELPCONTEXTCONCEPT_H

#include <concepts>

#include <QString>

class QDialog;

namespace Draupnir::Ui
{

namespace HelpContext
{

template<class Candidate>
concept HasAboutAppText = requires {
    { Candidate::aboutAppText() } -> std::convertible_to<QString>;
};

template<class Candidate>
concept HasCreateHelpDialog = requires {
    { Candidate::createHelpDialog() } -> std::convertible_to<QDialog*>;
};

}; // namespace Draupnir::Ui::HelpContext

}; // namespace Draupnir::Ui

#endif // HELPCONTEXTCONCEPT_H
