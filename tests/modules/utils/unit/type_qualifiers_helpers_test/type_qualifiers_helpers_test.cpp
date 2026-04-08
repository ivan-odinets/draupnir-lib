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

#include <QObject>

#include "draupnir-test/helpers/TypeHelpers.h"

#include "draupnir/utils/type_qualifiers_helpers.h"

/*! @class TypeQualifiersHelpersTest tests/modules/utils/unit/type_qualifiers_helpers_test/TypeQualifiersHelpersTest.cpp
 *  @brief Unit test for entities present within @ref draupnir/utils/type_qualifiers_helpers.h. */

class TypeQualifiersHelpersTest : public QObject
{
    Q_OBJECT
private:

private slots:
    void test_copy_const_from_const_type() {
        // Test cases when const is added
        { using actual_t = draupnir::utils::copy_const_from_t<const int, QString>;
          TYPE_COMPARE(actual_t, const QString); }
        { using actual_t = draupnir::utils::copy_const_from_t<const int&, QByteArray>;
          TYPE_COMPARE(actual_t, const QByteArray); }
        { using actual_t = draupnir::utils::copy_const_from_t<const volatile int, double>;
            TYPE_COMPARE(actual_t, const double); }
        { using actual_t = draupnir::utils::copy_const_from_t<volatile int* const, float>;
            TYPE_COMPARE(actual_t, const float); }
        { using actual_t = draupnir::utils::copy_const_from_t<const int&&, long>;
            TYPE_COMPARE(actual_t, const long); }

        // Test cases when const is still present
        { using actual_t = draupnir::utils::copy_const_from_t<const int, const QString>;
            TYPE_COMPARE(actual_t, const QString); }
        { using actual_t = draupnir::utils::copy_const_from_t<const int&, const QByteArray>;
            TYPE_COMPARE(actual_t, const QByteArray); }
        { using actual_t = draupnir::utils::copy_const_from_t<const volatile int, const double>;
            TYPE_COMPARE(actual_t, const double); }
        { using actual_t = draupnir::utils::copy_const_from_t<volatile int* const, const float>;
            TYPE_COMPARE(actual_t, const float); }
        { using actual_t = draupnir::utils::copy_const_from_t<const int&&, const long>;
            TYPE_COMPARE(actual_t, const long); }
    }

    void test_copy_const_from_non_const_type() {
        // Test cases when no const added
        { using actual_t = draupnir::utils::copy_const_from_t<int, QString>;
            TYPE_COMPARE(actual_t, QString); }
        { using actual_t = draupnir::utils::copy_const_from_t<int&, QByteArray>;
            TYPE_COMPARE(actual_t, QByteArray); }
        { using actual_t = draupnir::utils::copy_const_from_t<volatile int, double>;
            TYPE_COMPARE(actual_t, double); }
        { using actual_t = draupnir::utils::copy_const_from_t<volatile int*, float>;
            TYPE_COMPARE(actual_t, float); }
        { using actual_t = draupnir::utils::copy_const_from_t<int&&, long>;
            TYPE_COMPARE(actual_t, long); }

        // Test cases when no const is kept
        { using actual_t = draupnir::utils::copy_const_from_t<int, const QString>;
            TYPE_COMPARE(actual_t, QString); }
        { using actual_t = draupnir::utils::copy_const_from_t<int&, const QByteArray>;
            TYPE_COMPARE(actual_t, QByteArray); }
        { using actual_t = draupnir::utils::copy_const_from_t<volatile int, const double>;
            TYPE_COMPARE(actual_t, double); }
        { using actual_t = draupnir::utils::copy_const_from_t<volatile int*, const float>;
            TYPE_COMPARE(actual_t, float); }
        { using actual_t = draupnir::utils::copy_const_from_t<int&&, const long>;
            TYPE_COMPARE(actual_t, long); }
    }
};

QTEST_APPLESS_MAIN(TypeQualifiersHelpersTest)

#include "type_qualifiers_helpers_test.moc"
