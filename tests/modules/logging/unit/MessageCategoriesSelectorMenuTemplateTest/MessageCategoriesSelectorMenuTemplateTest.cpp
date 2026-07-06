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

#include "draupnir/logging/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/logging/traits/categories/NetworkMessageCategoryTrait.h"
#include "draupnir/logging/ui/menus/MessageCategoriesSelectorMenuTemplate.h"

using namespace Draupnir::Logging;

/*! @class MessageCategoriesSelectorMenuTemplateTest tests/modules/message_system/unit/MessageCategoriesSelectorMenuTemplateTest.cpp
 *  @ingroup LoggingTests */

class MessageCategoriesSelectorMenuTemplateTest final : public QObject
{
    Q_OBJECT
private:
    using MessageCategoriesSelectorMenu = MessageCategoriesSelectorMenuTemplate<
        DefaultMessageCategoryTrait, NetworkMessageCategoryTrait
    >;

    MessageCategoriesSelectorMenu* menu = nullptr;
    QSignalSpy* flagSelectionChangedSpy = nullptr;

private slots:
    void initTestCase() {
        qRegisterMetaType<MessageCategory>("MessageCategory");
    }
    void init() {
        menu = new MessageCategoriesSelectorMenu;
        flagSelectionChangedSpy = new QSignalSpy{menu, &AbstractMessageCategoriesSelectorMenu::flagSelectionChanged};
    }

    void cleanup() {
        delete flagSelectionChangedSpy; flagSelectionChangedSpy = nullptr;
        delete menu; menu = nullptr;
    }

    void test_initial_state() {
        QCOMPARE(menu->getUiElement<DefaultMessageCategoryTrait>()->text(),
                 DefaultMessageCategoryTrait::displayName());
        QCOMPARE(menu->getUiElement<NetworkMessageCategoryTrait>()->text(),
                 NetworkMessageCategoryTrait::displayName());
    }

    void test_ui_triggering() {
        // Prepeare
        menu->displayMaskSelected(DefaultMessageCategoryTrait::value());
        // Get action
        QAction* action = menu->getUiElement<DefaultMessageCategoryTrait>();
        // Trigger
        action->trigger();
        // Check signal
        QTRY_COMPARE(flagSelectionChangedSpy->count(), 1);
        auto arguments = flagSelectionChangedSpy->takeFirst();
        QCOMPARE(arguments.count(), 2);
        QCOMPARE(arguments.value(0).value<MessageCategory>(), DefaultMessageCategoryTrait::value());
        QCOMPARE(arguments.value(1).toBool(), false);
    }
};

QTEST_MAIN(MessageCategoriesSelectorMenuTemplateTest)

#include "MessageCategoriesSelectorMenuTemplateTest.moc"
