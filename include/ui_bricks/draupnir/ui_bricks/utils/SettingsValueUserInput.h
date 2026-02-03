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

#ifndef SETTINGSVALUEUSERINPUT_H
#define SETTINGSVALUEUSERINPUT_H

#include <QApplication>
#include <QInputDialog>

#include <optional>

namespace Draupnir::Handlers
{

template<class SettingTrait,class Value>
class SettingsValueUserInput;

template<class SettingTrait>
class SettingsValueUserInput<SettingTrait,QString>
{
public:
    static std::optional<QString> getValue(const QString& oldValue) {
        bool ok = false;
        const QString& result = QInputDialog::getText(
            qApp->activeWindow(),
            qApp->applicationName(),
            qApp->applicationName(),
            QLineEdit::Normal,
            oldValue,
            &ok
        );
        return (ok) ? std::optional<QString>{result} : std::nullopt;
    }
};

};

#endif // SETTINGSVALUEUSERINPUT_H
