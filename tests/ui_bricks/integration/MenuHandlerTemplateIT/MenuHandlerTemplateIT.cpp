/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
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
#include "draupnir-test/mocks/MockSettingsTemplate.h"
#include "draupnir-test/mocks/DummyFileContext.h"
#include "draupnir-test/mocks/DummySingleFileManager.h"

#include "draupnir-test/helpers/TypeHelpers.h"

// SettingsRegistry
#include "draupnir/SettingsRegistry.h"

#include "draupnir/ui_bricks/traits/menu_entries/FileMenuEntries.h"
#include "draupnir/ui_bricks/traits/menu_entries/HelpMenuEntries.h"

#include "draupnir/ui_bricks/handlers/file_menu/FileExitEntryHandler.h"
#include "draupnir/ui_bricks/handlers/file_menu/FileNewEntryHandler.h"
#include "draupnir/ui_bricks/handlers/file_menu/FileOpenEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutDraupnirLibEntryHandler.h"
#include "draupnir/ui_bricks/handlers/help_menu/AboutQtEntryHandler.h"

// Tested things
#include "draupnir/ui_bricks/handlers/templates/MenuHandlerTemplate.h"

/*! @class MenuHandlerTemplateIT */

class MenuHandlerTemplateIT : public QObject
{
    Q_OBJECT

public:
    class PseudoEmptyContext {
        QString m_someString;
        int m_someInt;
    };

    using HandlerWithoutContext = Draupnir::Handlers::MenuHandlerTemplate<
        PseudoEmptyContext, Draupnir::Handlers::GenericMenuEntryHandlerTemplate,
        Draupnir::Ui::ExitApplicationEntry,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::AboutQtMenuTrait
    >;

    using ExpectedTupleFor_HandlerWithoutContext = std::tuple<
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<PseudoEmptyContext,Draupnir::Ui::ExitApplicationEntry>,
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<PseudoEmptyContext,Draupnir::Ui::AboutDraupnirLibMenuTrait>,
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<PseudoEmptyContext,Draupnir::Ui::AboutQtMenuTrait>
    >;

    using HandlerWithSomeContext = Draupnir::Handlers::MenuHandlerTemplate<
        DummyFileContext<DummySingleFileManager>, Draupnir::Handlers::GenericMenuEntryHandlerTemplate,
        Draupnir::Ui::ExitApplicationEntry,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::FileNewEntry,
        Draupnir::Ui::AboutQtMenuTrait
    >;

    using ExpectedTupleFor_HandlerWithSomeContext = std::tuple<
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<DummyFileContext<DummySingleFileManager>,Draupnir::Ui::ExitApplicationEntry>,
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<DummyFileContext<DummySingleFileManager>,Draupnir::Ui::AboutDraupnirLibMenuTrait>,
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<DummyFileContext<DummySingleFileManager>,Draupnir::Ui::FileNewEntry>,
        Draupnir::Handlers::GenericMenuEntryHandlerTemplate<DummyFileContext<DummySingleFileManager>,Draupnir::Ui::AboutQtMenuTrait>
    >;

    using HandlerWithAutodeductedSettings = Draupnir::Handlers::MenuHandlerTemplate<
        DummyFileContext<DummySingleFileManager>, Draupnir::Handlers::GenericMenuEntryHandlerTemplate,
        Draupnir::Ui::FileOpenEntry,
        Draupnir::Ui::ExitApplicationEntry,
        Draupnir::Ui::AboutDraupnirLibMenuTrait,
        Draupnir::Ui::AboutQtMenuTrait
    >;

private slots:

    void test_handler_without_context_object() {

        HandlerWithoutContext handlerWithouContextObject;

        qInfo() << "Expected Handler size = " << sizeof(ExpectedTupleFor_HandlerWithoutContext);
        qInfo() << "Actual Handler size   = " << sizeof(HandlerWithoutContext);

        // Uncommenting line below will result in compile-time error
        // handlerWithouContextObject.context();
    }

    void test_handler_with_context_object_for_some_handlers() {

        HandlerWithSomeContext handlerWithSomeContext;

        qInfo() << "Actual Handler size   = " << sizeof(handlerWithSomeContext);
        qInfo() << "Expected Handler size = " << sizeof(ExpectedTupleFor_HandlerWithSomeContext) +
                                                     sizeof(DummyFileContext<DummySingleFileManager>);

        // //
        // handlerWithSomeContext.context().fileManager();
    }

    void test_handler_with_autodeducted_settings() {

        HandlerWithAutodeductedSettings handler;

        using DeductedContext = std::remove_reference_t<decltype(handler.context())>;
        QVERIFY((draupnir::utils::is_t1a1_template_base_of_v<Draupnir::Settings::OptionalSettingsBundle,DeductedContext>));
    }
};

QTEST_MAIN(MenuHandlerTemplateIT)

#include "MenuHandlerTemplateIT.moc"
