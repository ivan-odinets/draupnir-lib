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

// Mocks
#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"

#include "draupnir/ui_bricks/handlers/file_menu/FileExitEntryHandler.h"
#include "draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h"
#include "draupnir/ui_bricks/handlers/file_menu/FileOpenEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutDraupnirLibEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutQtEntryHandler.h"

// Tested things
#include "draupnir/ui_bricks/handlers/templates/MenuEntryHandlerContainer.h"

namespace Draupnir::Handlers
{

/*! @class MenuEntryHandlerContainerTest
 *  @ingroup UiBricks
 *  @ingroup Tests
 *  @brief Unit test for @ref Draupnir::Handlers::MenuEntryHandlerContainer
 * @todo Reduce amount of main code classes being used within this test.
 * @todo Extend this test to check other things: are actions connected? are settings loaded? */

class MenuEntryHandlerContainerTest : public QObject
{
    Q_OBJECT
private:
    class PseudoEmptyContext {
        [[maybe_unused]] int m_someString;
    };

    using HandlerWithoutContext = Draupnir::Handlers::MenuEntryHandlerContainer<
        PseudoEmptyContext, Draupnir::Handlers::GenericMenuEntryHandlerTemplate,
        Draupnir::Ui::ExitApplicationEntry,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::AboutQtMenuTrait
    >;

    using SomeContext = DummyFileContext<DummySingleFileManager>;
    using HandlerWithSomeContext = Draupnir::Handlers::MenuEntryHandlerContainer<
        SomeContext, Draupnir::Handlers::GenericMenuEntryHandlerTemplate,
        Draupnir::Ui::ExitApplicationEntry,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::FileNewEntry,
        Draupnir::Ui::AboutQtMenuTrait
    >;

private slots:
    void test_handler_without_context_object() {
        HandlerWithoutContext handlerWithouContextObject;
        QVERIFY(handlerWithouContextObject.p_handlerTuple == nullptr);
        handlerWithouContextObject.initialize();
        QVERIFY(handlerWithouContextObject.p_handlerTuple != nullptr);
    }

    void test_handler_with_context_object_for_some_handlers() {
        SomeContext context;
        HandlerWithSomeContext handlerWithSomeContext;
        QVERIFY(handlerWithSomeContext.p_handlerTuple == nullptr);
        handlerWithSomeContext.initialize(&context);
        QVERIFY(handlerWithSomeContext.p_handlerTuple != nullptr);
    }
};

}; // namespace Draupnir::Handlers

QTEST_MAIN(Draupnir::Handlers::MenuEntryHandlerContainerTest)

#include "MenuEntryHandlerContainerTest.moc"
