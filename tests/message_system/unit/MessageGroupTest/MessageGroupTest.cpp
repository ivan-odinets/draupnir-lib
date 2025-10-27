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

#include "draupnir/MessageGroup.h"

namespace Draupnir::Messages
{

/*! @class MessageGroupTest tests/message_system/unit/MessageGroupTest.cpp
 *  @brief This test class tests MessageGroup class.
 *
 * @todo Refractor this test so that it will have better readability.
 * @todo Add some script to execute this test in the context of CI. */

class MessageGroupTest : public QObject
{
    Q_OBJECT
public:
    MessageGroupTest() = default;
    ~MessageGroupTest() = default;

private slots:
    void test_generateUniqueGroup() {
        constexpr int num_tries = 5;
        QList<MessageGroup> groups;

        // Test basic uniquness of the groups generated
        for (int i = 0; i < num_tries; i ++) {
            const MessageGroup group = MessageGroup::generateUniqueGroup();
            QVERIFY2(groups.contains(group) == false,
                     QString("i = %1, group repeated.").arg(i).toStdString().c_str());
            groups.append(group);
        }
    }

    /*! @brief This test case will never fail. Just print when MessageGroup will generate repeating id. */
    void test_generateUniqueGroup_limits() {
        constexpr int num_tries = 1000;
        QList<MessageGroup> groups;

        // Test basic uniquness of the groups generated
        for (int i = 0; i < num_tries; i ++) {
            const MessageGroup group = MessageGroup::generateUniqueGroup();
            if (groups.contains(group)) {
                qInfo() << "At the iteration "<<i<<" MessageGroup::generateUniqueGroup returned duplicated id.";
                break;
            }
            groups.append(group);
        }
    }
};

}; // namespace Draupnir::MessageGroup

QTEST_MAIN(Draupnir::Messages::MessageGroupTest)

#include "MessageGroupTest.moc"
