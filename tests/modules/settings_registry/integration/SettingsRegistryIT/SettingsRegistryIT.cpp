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

#include "draupnir-test/mocks/SettingsBackendMockTemplate.h"
#include "draupnir-test/traits/settings/DoubleSettingTraits.h"
#include "draupnir-test/traits/settings/BoolSettingTraits.h"
#include "draupnir-test/traits/settings/WidgetIndexSettingTraits.h"
#include "draupnir-test/traits/settings/ComplexValueSettingTrait.h"

#include "draupnir/settings_registry/SettingsRegistryTemplate.h"

/*! @class SettingsRegistryIT tests/modules/settings_registry/integration/SettingsRegistryIT/SettingsRegistryIT.cpp
 *  @ingroup SettingsRegistryTests
 *  @brief This is a very basic test for @ref Draupnir::Settings::SettingsRegistryTemplate and related things.
 *
 * @todo Split this test into several: unit-test for SettingsRegistryTemplate, unit test for SettingsBundleTemplate and
 *       integration between SettingsRegistryTemplate and SettingsBundleTemplate.  */

class SettingsRegistryIT final : public QObject
{
    Q_OBJECT

public:
///@name Tested types
///@{
    using MockSettings = SettingsBackendMockTemplate<
        DoubleSettingTrait,
        BoolSettingTrait,
        ComplexValueSettingTrait
    >;

    using SettingsRegistry = Draupnir::Settings::SettingsRegistryTemplate<
        DoubleSettingTrait,
        BoolSettingTrait,
        ComplexValueSettingTrait
    >;
    using RandomPopulatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        DoubleSettingTrait
    >;
    using RandomUnpopulatableBundle = Draupnir::Settings::SettingsBundleTemplate<
        DoubleSettingTrait, WidgetIndexSettingTrait
    >;
///@}

///@name Required fields
///@{
    MockSettings mockedBackend;

    SettingsRegistry testedRegistry;
///@}

private slots:
    void initTestCase() {
        testedRegistry.setBackend(&mockedBackend);
    }

    /*! @brief Testing if the constexpt methods are correct for the specfic SettingsRegistryTemplate.
     * @todo Extend this test method. */
    void test_constexpr_methods() {
        // Contains
        QCOMPARE(decltype(testedRegistry)::template contains<DoubleSettingTrait>(), true);
        QCOMPARE(decltype(testedRegistry)::template contains<WidgetIndexSettingTrait>(), false);

        // If stuff is empry or not?
        QCOMPARE(Draupnir::Settings::SettingsRegistryTemplate<>::isEmpty(), true);
        QCOMPARE(decltype(testedRegistry)::isEmpty(), false);

        // If bundle can be populated from SettingsRegistryTemplate
        QCOMPARE(RandomPopulatableBundle::template canBeFullyPopulatedFrom<decltype(testedRegistry)>(), true);
        QCOMPARE(RandomUnpopulatableBundle::template canBeFullyPopulatedFrom<decltype(testedRegistry)>(), false);
    }

    /*! @brief Testing if the SettingsRegistryTemplate is correctly initialized and if backend is correctly loaded. */
    void test_initialization() {
        SettingsRegistry otherRegistry;
        // Test non-loaded SettingsRegistry
        QCOMPARE(otherRegistry.settings(), nullptr);
        QCOMPARE(otherRegistry.isLoaded(), false);
    }

    void test_backendLoading() {
        SettingsRegistry otherRegistry;
        MockSettings otherBackend;

        // Load the backend and see what happens
        otherRegistry.setBackend(&otherBackend);
        QCOMPARE(otherRegistry.settings(),static_cast<Draupnir::Settings::SettingsBackendInterface*>(&otherBackend));
        QVERIFY(otherRegistry.isLoaded());

        // By default we should have the default values.
        QCOMPARE(testedRegistry.get<BoolSettingTrait>(),
                 BoolSettingTrait::defaultValue());
        QCOMPARE(testedRegistry.get<DoubleSettingTrait>(),
                 DoubleSettingTrait::defaultValue());
    }

    void test_set_and_get() {
        double testDouble = M_E;

        // Verify that we don't have the test values
        QVERIFY(testedRegistry.template get<DoubleSettingTrait>() != testDouble);
        QVERIFY(mockedBackend.template getQVariant<DoubleSettingTrait>() != testDouble);

        // Set something
        testedRegistry.template set<DoubleSettingTrait>(testDouble);

        // Check if SettingsRegistry::get method is returning what expected
        QCOMPARE(testedRegistry.template get<DoubleSettingTrait>(), testDouble);

        // Check if values were indeed written to the backend
        QCOMPARE(mockedBackend.template getQVariant<DoubleSettingTrait>(), testDouble);
    }

    void test_bundle_functionality() {
        auto bundleByTrait = testedRegistry.template getSettingBundleForTraits<
            DoubleSettingTrait,BoolSettingTrait
        >();

        // Check if bundle reports the same as registry
        QCOMPARE(bundleByTrait.template get<BoolSettingTrait>(),
                 testedRegistry.template get<BoolSettingTrait>());
        QCOMPARE(bundleByTrait.template get<DoubleSettingTrait>(),
                 testedRegistry.template get<DoubleSettingTrait>());

        // Check if by any magic within bundle / registry wrong value is present
        QString testString = "Hello SettingsRegistry!";
        QVERIFY(bundleByTrait.template get<BoolSettingTrait>() != testString);
        QVERIFY(testedRegistry.template get<BoolSettingTrait>() != testString);

        // write to bundle and check again
        bundleByTrait.template set<BoolSettingTrait>(false);
        QCOMPARE(bundleByTrait.get<BoolSettingTrait>(),false);
        QCOMPARE(testedRegistry.template get<BoolSettingTrait>(), false);

        // Check if by any magic within bundle / resgistry wrong value is present
        double testDouble = M_PI * M_E;
        QVERIFY(bundleByTrait.template get<DoubleSettingTrait>() != testDouble);
        QVERIFY(testedRegistry.template get<DoubleSettingTrait>() != testDouble);

        // write to registry and check again
        testedRegistry.template set<DoubleSettingTrait>(testDouble);
        QCOMPARE(bundleByTrait.template get<DoubleSettingTrait>(), testDouble);
        QCOMPARE(testedRegistry.template get<DoubleSettingTrait>(), testDouble);
    }

    void test_complex_value_serialization() {
        using ValueSerializer = Draupnir::Settings::ValueSerializerTemplate<ComplexValue>;
        QCOMPARE(testedRegistry.template get<ComplexValueSettingTrait>(), ComplexValueSettingTrait::defaultValue());

        // Try writing something, and check if it is written as expected
        const ComplexValue value{42,42};
        const QVariant valueVariant{ValueSerializer::toQVariant(value)};
        testedRegistry.template set<ComplexValueSettingTrait>(value);

        QCOMPARE(testedRegistry.template get<ComplexValueSettingTrait>(), value);
        QCOMPARE(mockedBackend.getQVariant<ComplexValueSettingTrait>(), valueVariant);

    }
};

QTEST_MAIN(SettingsRegistryIT)

#include "SettingsRegistryIT.moc"
