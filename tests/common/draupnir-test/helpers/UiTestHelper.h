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

#ifndef UITESTHELPER_H
#define UITESTHELPER_H

#include <QApplication>
#include <QInputDialog>
#include <QTimer>

class UiTestHelper
{
public:
    template<class Widget, class Task>
    static void scheduleForActiveModalWidget(Task&& task, int delay = 200) {
        QTimer::singleShot(delay, [task = std::forward<Task>(task)]() {
            auto* widget = qobject_cast<Widget*>(QApplication::activeModalWidget());
            QVERIFY(widget);
            task(widget);
        });
    };

    template<class Value>
    static void scheduleQInputDialogUserInput(const Value& newValue, bool acceptDialog, int delay = 200) {
        scheduleForActiveModalWidget<QInputDialog>([newValue,acceptDialog](QInputDialog* dialog){
            if constexpr (std::is_floating_point_v<Value>) {
                dialog->setDoubleValue(newValue);
            } else if constexpr (std::is_integral_v<Value>) {
                dialog->setIntValue(newValue);
            } else if constexpr (std::is_same_v<QString,Value>) {
                dialog->setTextValue(newValue);
            } else if constexpr (std::is_same_v<std::string,Value>) {
                dialog->setTextValue(QString::fromStdString(newValue));
            }

            if (acceptDialog)
                dialog->accept();
            else
                dialog->reject();
            }, delay);
    }
};

#endif // UITESTHELPER_H
