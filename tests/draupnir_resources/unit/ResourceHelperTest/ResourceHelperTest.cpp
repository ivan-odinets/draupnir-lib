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

#include "draupnir/utils/ResourceHelper.h"

/*! @class ResourceHelperTest tests/draupnir_resources/unit/ResourceHelperTest/ResourceHelperTest.cpp
 *  @brief This test class tests basic functionality of the ResourceHelper class. */

class ResourceHelperTest : public QObject
{
    Q_OBJECT

public:
    ResourceHelperTest()
        {}

    ~ResourceHelperTest() = default;

private slots:
    void test_draupnir_icon() {
        const QIcon icon = Draupnir::Resources::ResourceHelper::get().draupnirIcon();

        // If we have an icon
        QVERIFY(!icon.isNull());

        // If we have valid sizes
        QList<QSize> sizes = icon.availableSizes();
        QVERIFY(sizes.contains(QSize(16,16)));
        QVERIFY(sizes.contains(QSize(24,24)));
        QVERIFY(sizes.contains(QSize(24,24)));
        QVERIFY(sizes.contains(QSize(32,32)));
        QVERIFY(sizes.contains(QSize(64,64)));
        QVERIFY(sizes.contains(QSize(128,128)));
        QVERIFY(sizes.contains(QSize(256,256)));
        QVERIFY(sizes.contains(QSize(512,512)));
    }

    void test_about_text() {
        QVERIFY(!Draupnir::Resources::ResourceHelper::get().aboutDraupnirLibText().isEmpty());
    }

};

QTEST_MAIN(ResourceHelperTest)

#include "ResourceHelperTest.moc"
