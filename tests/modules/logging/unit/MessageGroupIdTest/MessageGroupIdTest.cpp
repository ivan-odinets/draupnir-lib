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

#include "draupnir/logging/core/MessageGroupId.h"

#include "draupnir-test/helpers/ConcurrentTestHelpers.h"

using namespace Draupnir::Logging;

/*! @class MessageGroupTest tests/modules/logging/unit/MessageGroupIdTest.cpp
 *  @ingroup LoggingTests
 *  @brief Unit test for @ref Draupnir::Logging::MessageGroupId class.
 * @todo Question: Maybe add testing of concurrent usage of `MessageGroupId` */

class MessageGroupIdTest final : public QObject
{
    Q_OBJECT
private slots:
    void test_generate_unique_group() {
        constexpr int num_tries = 5;
        QList<MessageGroupId> groups;

        // Test basic uniquness of the groups generated
        for (int i = 0; i < num_tries; i ++) {
            const auto group = MessageGroupId::generate();
            QVERIFY2(groups.contains(group) == false,
                     QString("i = %1, group repeated.").arg(i).toStdString().c_str());
            groups.append(group);
        }
    }

    /*! @brief This test case will never fail. Just print when MessageGroupId will generate repeating id. */
    void test_generate_unique_group_limits() {
        constexpr int num_tries = 1000;
        QList<MessageGroupId> groups;

        // Test basic uniquness of the groups generated
        for (int i = 0; i < num_tries; i ++) {
            const MessageGroupId group = MessageGroupId::generate();
            if (groups.contains(group)) {
                qInfo() << "At the iteration "<<i<<" MessageGroup::generateUniqueGroup returned duplicated id.";
                break;
            }
            groups.append(group);
        }
    }

    void test_concurrent_generate_unique_group_usage() {
        // If something wrong will happen - we should fail.
        ConcurrentTestHelpers::performSpamCalls(20, 5000, [](){
            auto _ = MessageGroupId::generate();
        });
    }
};

QTEST_MAIN(MessageGroupIdTest)

#include "MessageGroupIdTest.moc"
