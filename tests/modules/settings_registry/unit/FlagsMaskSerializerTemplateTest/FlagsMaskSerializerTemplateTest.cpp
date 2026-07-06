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

#include "draupnir/settings_registry/utils/FlagsMaskSerializerTemplate.h"
#include "draupnir/utils/flags.h"

#include <QLatin1Literal>

/*! @enum MyImportantEnum tests/modules/settings_registry/unit/FlagsMaskSerializerTemplateTest/FlagsMaskSerializerTemplateTest.cpp
 *  @ingroup SettingsRegistryTests
 *  @brief Enum to be serialized / deserialized. */

enum MyImportantEnum
{
    One   = 0b00000001,
    Two   = 0b00000010,
    Three = 0b00000100,
    Four  = 0b00001000,
    Five  = 0b00010000
};

/*! @typedef MyImportantEnumFlags tests/modules/settings_registry/unit/FlagsMaskSerializerTemplateTest/FlagsMaskSerializerTemplateTest.cpp
 *  @ingroup SettingsRegistryTests
 *  @brief Enum flags to be serialized / deserialized. */

using MyImportantEnumFlags = draupnir::utils::enum_flags<MyImportantEnum>;

///@name Predefined masks
///@{

static constexpr MyImportantEnumFlags none{0};
static constexpr MyImportantEnumFlags oneTwo{
    MyImportantEnum::One |
    MyImportantEnum::Two
};
static constexpr MyImportantEnumFlags oneThree{
    MyImportantEnum::One |
    MyImportantEnum::Three
};

static constexpr MyImportantEnumFlags all{
    MyImportantEnum::One   |
    MyImportantEnum::Two   |
    MyImportantEnum::Three |
    MyImportantEnum::Four  |
    MyImportantEnum::Five
};

///@}

///@name Valid tokens for enum keys
///@{

static QLatin1String oneToken()   { return QLatin1String{"One"}; }
static QLatin1String twoToken()   { return QLatin1String{"Two"}; }
static QLatin1String threeToken() { return QLatin1String{"Three"}; }
static QLatin1String fourToken()  { return QLatin1String{"Four"}; }
static QLatin1String fiveToken()  { return QLatin1String{"Five"}; }

///@}

///@name Valid tokens for predefined masks
///@{

static QLatin1String oneThreeMaskToken() { return QLatin1String{"one_three"}; };
static QLatin1String allMaskToken()      { return QLatin1String{"all"}; };

///@}

///@name Valid enum token combinations
///@{

static QString oneTwoToken() { return QStringList{oneToken(),twoToken()}.join(','); }
static QString twoOneToken() { return QStringList{twoToken(),oneToken()}.join(','); }
static QString oneThreeStraightforward() {
    return QStringList{oneToken(),threeToken()}.join(',');
}
static QString threeOneStraightforward() {
    return QStringList{threeToken(), oneToken()}.join(',');
}
static QString allTokenStraightforward() {
    return QStringList{oneToken(),twoToken(),threeToken(),fourToken(),fiveToken()}.join(',');
};

///@}

///@name Invalid strings
///@{

static QLatin1String invalidToken() { return QLatin1String{"Invalid"}; }
static QLatin1String emptyToken() { return QLatin1String{""}; }
static QString allAndOneThreeMasks() {
    return QStringList{allMaskToken(), oneThreeMaskToken()}.join(',');
}
static QString allAndOne() {
    return QStringList{allMaskToken(), oneToken()}.join(',');
}
static QLatin1String commaOnly() { return QLatin1String{","}; }

static QString leadingComma()  { return QString{',' + oneToken() }; }
static QString trailingComma() { return QString{twoToken() + ','}; }
static QString doubleComma()   { return QString{oneToken() + ",," + twoToken()}; }

///@}

/*! @class FlagsMaskSerializerTemplateTest tests/modules/settings_registry/unit/FlagsMaskSerializerTemplateTest/FlagsMaskSerializerTemplateTest.cpp
 *  @ingroup SettingsRegistryTests
 *  @brief Unit test for @ref Draupnir::Settings::FlagsMaskSerializerTemplate. */

class FlagsMaskSerializerTemplateTest final : public QObject
{
    Q_OBJECT
private:
    using ExplicitFlagSerializer = Draupnir::Settings::FlagSerializerTemplate<
        MyImportantEnum,
        Draupnir::Settings::FlagWrapperTemplate<MyImportantEnum::One,   []() { return oneToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<MyImportantEnum::Two,   []() { return twoToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<MyImportantEnum::Three, []() { return threeToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<MyImportantEnum::Four,  []() { return fourToken(); }>,
        Draupnir::Settings::FlagWrapperTemplate<MyImportantEnum::Five,  []() { return fiveToken(); }>
    >;
    using ExplicitFlagsMaskSerializer = Draupnir::Settings::FlagsMaskSerializerTemplate<
        MyImportantEnumFlags,
        ExplicitFlagSerializer,
        Draupnir::Settings::NoneFlagsMaskTemplate<MyImportantEnumFlags>,
        Draupnir::Settings::FlagsMaskWrapperTemplate<oneThree.value(),     []() { return oneThreeMaskToken(); }>,
        Draupnir::Settings::FlagsMaskWrapperTemplate<[]() { return all; }, []() { return allMaskToken(); }>
    >;

private slots:
    void test_explicit_enum_flag_serializer_to_config_string() {
        QCOMPARE(ExplicitFlagSerializer::toConfigString(MyImportantEnum::One), oneToken());
        QCOMPARE(ExplicitFlagSerializer::toConfigString(MyImportantEnum::Two), twoToken());
        QCOMPARE(ExplicitFlagSerializer::toConfigString(MyImportantEnum::Three), threeToken());
        QCOMPARE(ExplicitFlagSerializer::toConfigString(MyImportantEnum::Four), fourToken());
        QCOMPARE(ExplicitFlagSerializer::toConfigString(MyImportantEnum::Five), fiveToken());

        // This will Q_ASSERT when uncommented
        // QCOMPARE(ExplicitFlagSerializer::toConfigString(static_cast<MyImportantEnum>(42)), fiveToken());
    }

    void test_explicit_enum_flag_serializer_from_config_string() {
        // Valid strings
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(oneToken()).value(), MyImportantEnum::One);
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(twoToken()).value(), MyImportantEnum::Two);
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(threeToken()).value(), MyImportantEnum::Three);
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(fourToken()).value(), MyImportantEnum::Four);
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(fiveToken()).value(), MyImportantEnum::Five);

        // Invalid string
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(invalidToken()).has_value(), false);
        QCOMPARE(ExplicitFlagSerializer::fromConfigString(emptyToken()).has_value(), false);
    }

    void test_explicit_from_enum_flags_serializer_to_config_string() {
        // Single flags should work the same as for single flag serializer
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(MyImportantEnum::One), oneToken());
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(MyImportantEnum::Two), twoToken());
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(MyImportantEnum::Three), threeToken());
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(MyImportantEnum::Four), fourToken());
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(MyImportantEnum::Five), fiveToken());

        // Multiple flags should work like this
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(none), Draupnir::Settings::NoneFlagsMaskTemplate<MyImportantEnumFlags>::configString());
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(all), allMaskToken());
        QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(oneThree), oneThreeMaskToken());

        const QStringList configStringParts =
            ExplicitFlagsMaskSerializer::toConfigString(oneTwo).split(',');
        QCOMPARE(configStringParts.count(), 2);
        QVERIFY(configStringParts.contains(oneToken()));
        QVERIFY(configStringParts.contains(twoToken()));

        // This will Q_ASSERT when uncommented
        // QCOMPARE(ExplicitFlagsMaskSerializer::toConfigString(static_cast<MyImportantEnum>(42)), fiveToken());
    }

    void test_explicit_from_enum_flags_serializer_from_config_string() {
        // Single flags should work the same as for single flag serializer
        // Valid strings
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(oneToken()).value(), MyImportantEnum::One);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(twoToken()).value(), MyImportantEnum::Two);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(threeToken()).value(), MyImportantEnum::Three);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(fourToken()).value(), MyImportantEnum::Four);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(fiveToken()).value(), MyImportantEnum::Five);

        // Multiple flags should work like this
        // Valid strings
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(Draupnir::Settings::NoneFlagsMaskTemplate<MyImportantEnumFlags>::configString()).value(), none);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(oneThreeMaskToken()).value(), oneThree);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(oneThreeStraightforward()).value(), oneThree);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(threeOneStraightforward()).value(), oneThree);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(allMaskToken()).value(), all);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(allTokenStraightforward()).value(), all);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(oneTwoToken()).value(), oneTwo);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(twoOneToken()).value(), oneTwo);

        // Invalid strings
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(invalidToken()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(emptyToken()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(allAndOne()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(allAndOneThreeMasks()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(commaOnly()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(leadingComma()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(trailingComma()).has_value(), false);
        QCOMPARE(ExplicitFlagsMaskSerializer::fromConfigString(doubleComma()).has_value(), false);
    }
};

QTEST_MAIN(FlagsMaskSerializerTemplateTest)

#include "FlagsMaskSerializerTemplateTest.moc"
