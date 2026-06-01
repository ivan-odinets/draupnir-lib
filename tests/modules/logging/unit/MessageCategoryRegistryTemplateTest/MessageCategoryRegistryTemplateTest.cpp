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

#include <QtTest>
#include <QCoreApplication>

#include "draupnir/logging/messages/categories/MessageCategoryRegistryTemplate.h"
#include "draupnir/logging/traits/categories/DefaultMessageCategoryTrait.h"
#include "draupnir/logging/traits/categories/NetworkMessageCategoryTrait.h"

namespace Draupnir::Logging
{

/*! @class MessageCategoryRegistryTemplateTest tests/modules/logging/unit/MessageCategoryRegistryTemplateTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageCategoryRegistryTemplate class. */

class MessageCategoryRegistryTemplateTest final : public QObject
{
    Q_OBJECT
private:
    using MessageCategoryRegistry = MessageCategoryRegistryTemplate<
        DefaultMessageCategoryTrait,
        NetworkMessageCategoryTrait
    >;
    MessageCategoryRegistry* registry = nullptr;

private slots:
    void initTestCase() { registry = new MessageCategoryRegistry; }
    void cleanupTestCase() { delete registry; registry = nullptr; }

    void test_contains() {
        QCOMPARE(registry->contains(DefaultMessageCategoryTrait::id()), true);
        QCOMPARE(registry->contains(NetworkMessageCategoryTrait::id()), true);
        QCOMPARE(registry->contains(125), false);
    }
};

}; // namespace Draupnir::Logging::MessageCategoryRegistryTemplateTest

QTEST_MAIN(Draupnir::Logging::MessageCategoryRegistryTemplateTest)

#include "MessageCategoryRegistryTemplateTest.moc"
