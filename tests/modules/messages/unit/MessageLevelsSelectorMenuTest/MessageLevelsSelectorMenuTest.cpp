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

#include "draupnir/messages/ui/menus/MessageLevelsSelectorMenu.h"

using namespace Draupnir::Messages;

/*! @class MessageLevelsSelectorMenuTest tests/modules/messages/unit/MessageLevelsSelectorMenuTest.cpp
 *  @ingroup MessagesTests
 *  @brief Unit test for @ref Draupnir::Messages::MessageLevelsSelectorMenu class.
 * @note Most of the functionality of this class is covered by the unit test for the @ref Draupnir::Ui::EnumFlagsMaskSelectorBase */

class MessageLevelsSelectorMenuTest final : public QObject
{
    Q_OBJECT
private:
    MessageLevelsSelectorMenu* menu = nullptr;

private slots:
    void init() { menu = new MessageLevelsSelectorMenu; }
    void cleanup() { delete menu; menu = nullptr; }

    void test_initial_state() {
        QList<QAction*> actions = menu->actions();
        // Lets check that we have exactly what we expect: All, Debug, Info, Warning, Error
        QVERIFY(actions.contains(menu->getMaskElement<MessageLevels::All>()));
        actions.removeAll(menu->getMaskElement<MessageLevels::All>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageLevel::Debug>()));
        actions.removeAll(menu->getFlagElement<MessageLevel::Debug>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageLevel::Info>()));
        actions.removeAll(menu->getFlagElement<MessageLevel::Info>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageLevel::Warning>()));
        actions.removeAll(menu->getFlagElement<MessageLevel::Warning>());
        QVERIFY(actions.contains(menu->getFlagElement<MessageLevel::Error>()));
        actions.removeAll(menu->getFlagElement<MessageLevel::Error>());
        // And separator
        QCOMPARE(actions.count(), 1);
        QVERIFY(actions.first()->isSeparator());

        // And by default everything should be selected
        QCOMPARE(menu->selectedMask(), MessageLevels::All);
    }
};

QTEST_MAIN(MessageLevelsSelectorMenuTest)

#include "MessageLevelsSelectorMenuTest.moc"
