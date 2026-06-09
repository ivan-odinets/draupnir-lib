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

#include <QtTest>
#include <QCoreApplication>

#include "draupnir/logging/ui/menus/MessageViewItemFieldsSelectorMenu.h"

namespace Draupnir::Logging
{

/*! @class MessageViewItemFieldsSelectorMenu tests/modules/logging/unit/MessageViewItemFieldsSelectorMenuTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageLevelsSelectorMenu class. */

class MessageViewItemFieldsSelectorMenuTest final : public QObject
{
    Q_OBJECT
private:
    MessageViewItemFieldsSelectorMenu* menu = nullptr;

private slots:
    void init() { menu = new MessageViewItemFieldsSelectorMenu; }
    void cleanup() { delete menu; menu = nullptr; }

    void test_initial_state() {
        QList<QAction*> actions = menu->actions();
        // Lets check that we have exactly what we expect: Brief, What, DateTime, Icon
        QVERIFY(actions.contains(menu->getFlagElement<MessageViewItemField::Brief>()));
        actions.removeAll(menu->getFlagElement<MessageViewItemField::Brief>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageViewItemField::What>()));
        actions.removeAll(menu->getFlagElement<MessageViewItemField::What>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageViewItemField::DateTime>()));
        actions.removeAll(menu->getFlagElement<MessageViewItemField::DateTime>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageViewItemField::Icon>()));
        actions.removeAll(menu->getFlagElement<MessageViewItemField::Icon>());

        // And by default everything should be selected
        QCOMPARE(menu->selectedMask(), MessageViewItemFields::All);
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Logging::MessageViewItemFieldsSelectorMenuTest)

#include "MessageViewItemFieldsSelectorMenuTest.moc"
