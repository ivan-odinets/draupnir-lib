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

#ifndef SETTINGSBACKENDCONCEPT_H
#define SETTINGSBACKENDCONCEPT_H

#include <QVariant>

namespace Draupnir::Settings
{

namespace SettingsBackend
{

template<class Candidate>
concept HasContainsMethod = requires(Candidate& obj, QString key) {
    { obj.contains(key) } -> std::same_as<bool>;
};

template<class Candidate>
concept HasSetValueMethod = requires(Candidate& obj, QString key, QVariant value) {
    { obj.setValue(key,value) } -> std::same_as<void>;
};

template<class Candidate>
concept HasValueMethod = requires(Candidate& obj, QString key) {
    { obj.value(key) } -> std::same_as<QVariant>;
};

}; // namespace Draupnir::Settings::SettingsBackend

template<class Candidate>
concept SettingsBackendConcept =
    SettingsBackend::HasContainsMethod<Candidate> &&
    SettingsBackend::HasValueMethod<Candidate> &&
    SettingsBackend::HasSetValueMethod<Candidate>;

}; // namespace Draupnir::Settings

#endif // SETTINGSBACKENDCONCEPT_H
