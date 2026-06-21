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
#include <QMenu>


#include "draupnir/ui_bricks/core/selectors/EnumFlagsMaskSelectorBase.h"

#include "draupnir/utils/flags.h"

#include "draupnir-test/helpers/TypeHelpers.h"

enum MyEnum
{
    UNUSED = 0b0000,
    One   = 0b0001,
    Two   = 0b0010,
    Three = 0b0100,
    Four  = 0b1000,
};

Q_DECLARE_METATYPE(MyEnum);

class MyEnumFlags : public draupnir::utils::enum_flags<MyEnum>
{
public:
    using draupnir::utils::enum_flags<MyEnum>::enum_flags;
    using draupnir::utils::enum_flags<MyEnum>::operator=;

    using enum_type = draupnir::utils::enum_flags<MyEnum>::enum_type;
    using integer = draupnir::utils::enum_flags<MyEnum>::integer;
    using flag_type = enum_type;


    static constexpr integer All         = MyEnum::One | MyEnum::Two | MyEnum::Three | MyEnum::Four;
    static constexpr integer OneThree    = MyEnum::One |               MyEnum::Three;
    static constexpr integer OneTwoThree = MyEnum::One | MyEnum::Two | MyEnum::Three;

    struct UiSelectorMetadata {
        static constexpr MyEnum displayedFlags[] = { MyEnum::One, MyEnum::Two, MyEnum::Three, MyEnum::Four };
        static constexpr integer displayedMaskPresets[] = { OneThree, OneTwoThree, All };
    };

    static QString toDisplayString(MyEnumFlags mask) {
        // Flags
        if (mask == MyEnum::One)
            return "One";
        if (mask == MyEnum::Two)
            return "Two";
        if (mask == MyEnum::Three)
            return "Three";
        if (mask == MyEnum::Four)
            return "Four";
        // Masks
        if (mask == OneThree)
            return "OneThree";
        if (mask == OneTwoThree)
            return "OneTwoThree";
        if (mask == All)
            return "All";
        Q_ASSERT(false);
        return "";
    }
};




class MyEnumFlagsMenu :
    public QMenu,
    public Draupnir::Ui::FlagsMaskSelectorBase<MyEnumFlagsMenu, QAction, MyEnumFlags,
        Draupnir::Ui::TemplateArgs::FlagValuesWrapper<>,
        Draupnir::Ui::TemplateArgs::MaskValuesWrapper<MyEnumFlags::All, MyEnumFlags::OneTwoThree, MyEnumFlags::OneThree>
    >
{
    Q_OBJECT
public:
    explicit MyEnumFlagsMenu(MyEnumFlags initialState = 0, QWidget* parent = nullptr) :
        QMenu{parent},
        Draupnir::Ui::FlagsMaskSelectorBase<MyEnumFlagsMenu, QAction, MyEnumFlags,
            Draupnir::Ui::TemplateArgs::FlagValuesWrapper<>,
            Draupnir::Ui::TemplateArgs::MaskValuesWrapper<MyEnumFlags::All, MyEnumFlags::OneTwoThree, MyEnumFlags::OneThree>
        >{initialState}
    {}

signals:
    void flagSelectionChanged(MyEnum flag, bool state);
};

class MyEnumFlagsWidget :
    public QWidget,
    public Draupnir::Ui::EnumFlagsMaskSelectorBase<MyEnumFlagsWidget, QCheckBox, MyEnumFlags>
{
    Q_OBJECT
public:
    explicit MyEnumFlagsWidget(MyEnumFlags initialState = 0, QWidget* parent = nullptr) :
        QWidget{parent},
        Draupnir::Ui::EnumFlagsMaskSelectorBase<MyEnumFlagsWidget, QCheckBox, MyEnumFlags>{initialState}
    {}

signals:
    void flagSelectionChanged(MyEnum flag, bool state);
};

namespace Draupnir::Ui
{

/*! @class EnumFlagsSelectorViewBaseTest tests/modules/ui_bricks/unit/EnumFlagsSelectorViewBaseTest/EnumFlagsSelectorViewBaseTest.cpp
 *  @ingroup UiBricksTests
 *  @brief Unit test for @ref Draupnir::UiBricks::FlagsMaskSelectorBase.
 * @todo Optional: Put MyEnum / MyEnumFlags into seperate shared file.
 * @todo Test case: Add testing of the following: retranslateUiElements method, getUiElement and related.
 * @todo Documentation: Extend the documentation for this test. */

class FlagsMaskSelectorBaseTest : public QObject
{
    Q_OBJECT
private:
    MyEnumFlagsMenu* menu = nullptr;
    MyEnumFlagsWidget* widget = nullptr;
    QSignalSpy* menuFlagSelectionChangedSignalSpy = nullptr;
    QSignalSpy* widgetFlagSelectionChangedSignalSpy = nullptr;

///@name Helper template methods
///@{

    template<class UiElement>
    static void clickOnElement(UiElement* element) {
        if constexpr (std::is_same_v<UiElement,QAction>)
            element->trigger();
        else
            element->click();
    }

    static void take_and_verify_first_flag_selection_changed_signal(QSignalSpy* spy, MyEnum value, bool state) {
        QVERIFY(spy->count() > 0);
        auto arguments = spy->takeFirst();
        QCOMPARE(arguments.count(), 2);
        QCOMPARE(arguments.at(0).value<MyEnum>(), value);
        QCOMPARE(arguments.at(1).toBool(), state);
    }

    static void take_and_verify_all_signals(QSignalSpy* spy, QMap<MyEnum,bool> acceptedArguments) {
        QVERIFY(spy->count() == acceptedArguments.count());
        for (auto it = spy->begin(); it != spy->end(); it++) {
            QCOMPARE(it->count(), 2);
            QVERIFY(acceptedArguments.contains(it->at(0).value<MyEnum>()));
            QCOMPARE(acceptedArguments[it->at(0).value<MyEnum>()], it->at(1).toBool());
            acceptedArguments.remove(it->at(0).value<MyEnum>());
        }
        spy->clear();
    }

    template<class TestedSubclass>
    static void verify_ui_elements_state(TestedSubclass* object, MyEnumFlags expectedMask) {
        // Check status of the flags ui elements
        QCOMPARE(object->template getFlagElement<MyEnum::One>()->isChecked(),    expectedMask.test_flag(MyEnum::One));
        QCOMPARE(object->template getFlagElement<MyEnum::Two>()->isChecked(),    expectedMask.test_flag(MyEnum::Two));
        QCOMPARE(object->template getFlagElement<MyEnum::Three>()->isChecked(),  expectedMask.test_flag(MyEnum::Three));
        QCOMPARE(object->template getFlagElement<MyEnum::Four>()->isChecked(),   expectedMask.test_flag(MyEnum::Four));
        // Check status of mask ui elements
        QCOMPARE(object->template getMaskElement<MyEnumFlags::OneThree>()->isChecked(),    expectedMask.is_superset(MyEnumFlags::OneThree));
        QCOMPARE(object->template getMaskElement<MyEnumFlags::OneTwoThree>()->isChecked(), expectedMask.is_superset(MyEnumFlags::OneTwoThree));
        QCOMPARE(object->template getMaskElement<MyEnumFlags::All>()->isChecked(),         expectedMask.is_superset(MyEnumFlags::All));
    }

    template<class TestedSubclass>
    static void verify_ui_elements_state(TestedSubclass* object) {
        verify_ui_elements_state(object, object->m_displayedFlags);
    }

    template<class TestedSubclass>
    static void check_initial_state(TestedSubclass* object) {
        verify_ui_elements_state(object, 0);
    }

///@}

///@name Template implementations of the test cases methods.
///@{
    template<class TestedSubclass>
    static void test_flags_and_mask_presense_impl() {
        // Flags
        QVERIFY(TestedSubclass::isFlagKnown(MyEnum::One));
        QVERIFY(TestedSubclass::isFlagKnown(MyEnum::Two));
        QVERIFY(TestedSubclass::isFlagKnown(MyEnum::Three));
        QVERIFY(TestedSubclass::isFlagKnown(MyEnum::Four));
        QVERIFY(!TestedSubclass::isFlagKnown(MyEnum::UNUSED));

        // Masks
        QVERIFY(TestedSubclass::isMaskKnown(MyEnumFlags::OneThree));
        QVERIFY(TestedSubclass::isMaskKnown(MyEnumFlags::OneTwoThree));
        QVERIFY(TestedSubclass::isMaskKnown(MyEnumFlags::All));
        QVERIFY(!TestedSubclass::isMaskKnown(MyEnum::Three|MyEnum::Two));
    }

    template<class TestedSubclass>
    static void test_constructor_impl() {
        auto empty = new TestedSubclass;
        QCOMPARE(empty->m_displayedFlags, 0);
        verify_ui_elements_state(empty);
        delete empty; empty = nullptr;

        auto oneFlag = new TestedSubclass{MyEnum::One};
        QCOMPARE(oneFlag->m_displayedFlags, MyEnum::One);
        verify_ui_elements_state(oneFlag);
        delete oneFlag; oneFlag = nullptr;

        auto oneThreeMask = new TestedSubclass{MyEnumFlags::OneThree};
        QCOMPARE(oneThreeMask->m_displayedFlags, MyEnumFlags::OneThree);
        verify_ui_elements_state(oneThreeMask);
        delete oneThreeMask; oneThreeMask = nullptr;

        auto allMask = new TestedSubclass{MyEnumFlags::All};
        QCOMPARE(allMask->m_displayedFlags, MyEnumFlags::All);
        verify_ui_elements_state(allMask, MyEnumFlags::All);
        delete allMask; allMask = nullptr;
    }

    template<class TestedSubclass>
    static void test_get_ui_element_impl(TestedSubclass* object) {
        // Check flag UiElement
        QCOMPARE(object->getUiElement(MyEnum::One),
                 object->template getFlagElement<MyEnum::One>());
        QCOMPARE(object->getUiElement(MyEnum::Two),
                 object->template getFlagElement<MyEnum::Two>());
        QCOMPARE(object->getUiElement(MyEnum::Three),
                 object->template getFlagElement<MyEnum::Three>());
        QCOMPARE(object->getUiElement(MyEnum::Four),
                 object->template getFlagElement<MyEnum::Four>());
        // Check mask UiElement
        QCOMPARE(object->getUiElement(MyEnumFlags::OneThree),
                 object->template getMaskElement<MyEnumFlags::OneThree>());
        QCOMPARE(object->getUiElement(MyEnumFlags::OneTwoThree),
                 object->template getMaskElement<MyEnumFlags::OneTwoThree>());
        QCOMPARE(object->getUiElement(MyEnumFlags::All),
                 object->template getMaskElement<MyEnumFlags::All>());

        // This will assert
        // object->getUiElement(MyEnum::UNUSED);
    }

    template<class TestedSubclass>
    static void test_display_mask_selected_impl(TestedSubclass* object, QSignalSpy* flagSelectionChangedSignalSpy) {
        check_initial_state(object);

        object->displayMaskSelected(MyEnumFlags::OneThree);
        QVERIFY(flagSelectionChangedSignalSpy->isEmpty());
        verify_ui_elements_state(object);

        object->displayMaskSelected(MyEnumFlags::OneTwoThree);
        QVERIFY(flagSelectionChangedSignalSpy->isEmpty());
        verify_ui_elements_state(object);

        object->displayMaskSelected(MyEnumFlags::All);
        QVERIFY(flagSelectionChangedSignalSpy->isEmpty());
        verify_ui_elements_state(object);

        object->displayMaskSelected(0);
        QVERIFY(flagSelectionChangedSignalSpy->isEmpty());
        verify_ui_elements_state(object);
    }

    template<class TestedSubclass>
    static void test_display_flag_selected_impl(TestedSubclass* object, QSignalSpy* flagSelectionChangedSignalSpy) {
        check_initial_state(object);

        // Display MyEnum::One selected
        object->displayFlagSelected(MyEnum::One, true);
        // No signals
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 0);
        verify_ui_elements_state(object);

        // Display MyEnum::Three selected
        object->displayFlagSelected(MyEnum::Three, true);
        // No signals
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 0);
        // UiElement status
        verify_ui_elements_state(object);

        // Display MyEnum::Three unselected
        object->displayFlagSelected(MyEnum::Three, false);
        // No signals
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 0);
        verify_ui_elements_state(object);
    }

    template<class TestedSubclass>
    static void test_is_flag_selected_impl(TestedSubclass* object) {
        check_initial_state(object);

        // Set selection
        object->displayMaskSelected(MyEnumFlags::All);
        verify_ui_elements_state(object);
        QVERIFY(object->isFlagSelected(MyEnum::One));
        QVERIFY(object->isFlagSelected(MyEnum::Two));
        QVERIFY(object->isFlagSelected(MyEnum::Three));
        QVERIFY(object->isFlagSelected(MyEnum::Four));

        // Set other selection
        object->displayFlagSelected(MyEnum::Three, false);
        verify_ui_elements_state(object);
        QVERIFY(object->isFlagSelected(MyEnum::One));
        QVERIFY(object->isFlagSelected(MyEnum::Two));
        QVERIFY(!object->isFlagSelected(MyEnum::Three));
        QVERIFY(object->isFlagSelected(MyEnum::Four));
    }

    template<class TestedSubclass>
    static void test_displayed_mask_impl(TestedSubclass* object) {
        check_initial_state(object);

        // Set selection
        object->displayMaskSelected(MyEnumFlags::All);
        verify_ui_elements_state(object);
        QCOMPARE(object->selectedMask(), MyEnumFlags::All);

        // Set other selection
        object->displayFlagSelected(MyEnum::Three, false);
        verify_ui_elements_state(object);
        QCOMPARE(object->selectedMask(), MyEnum::One|MyEnum::Two|MyEnum::Four);
    }

    template<class TestedSubclass>
    static void test_ui_triggering_impl(TestedSubclass* object, QSignalSpy* flagSelectionChangedSignalSpy) {
        check_initial_state(object);

        // Click on MyEnum::One
        clickOnElement(object->template getFlagElement<MyEnum::One>());
        // One signal with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 1);
        take_and_verify_first_flag_selection_changed_signal(flagSelectionChangedSignalSpy, MyEnum::One, true);
        // UiElement status
        verify_ui_elements_state(object);

        // Click on MyEnum::Three
        clickOnElement(object->template getFlagElement<MyEnum::Three>());
        // One signal with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 1);
        take_and_verify_first_flag_selection_changed_signal(flagSelectionChangedSignalSpy, MyEnum::Three, true);
        // UiElement status
        verify_ui_elements_state(object);

        // Click on MyEnum::Three again
        clickOnElement(object->template getFlagElement<MyEnum::Three>());
        // One signal with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 1);
        take_and_verify_first_flag_selection_changed_signal(flagSelectionChangedSignalSpy, MyEnum::Three, false);
        // UiElement status
        verify_ui_elements_state(object);

        // Click on MyEnumFlags::All
        clickOnElement(object->template getMaskElement<MyEnumFlags::All>());
        // Two signals with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 3);
        take_and_verify_all_signals(flagSelectionChangedSignalSpy, {
            { MyEnum::Two, true },
            { MyEnum::Three, true },
            { MyEnum::Four, true },
        });
        // UiElement status
        verify_ui_elements_state(object);

        // Click on MyEnumFlags::All again
        clickOnElement(object->template getMaskElement<MyEnumFlags::All>());
        // Four signals with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 4);
        take_and_verify_all_signals(flagSelectionChangedSignalSpy, {
            { MyEnum::One, false },
            { MyEnum::Two, false },
            { MyEnum::Three, false },
            { MyEnum::Four, false },
        });
        // UiElement status
        verify_ui_elements_state(object);

        // Click on MyEnum::OneTwoThree
        clickOnElement(object->template getMaskElement<MyEnumFlags::OneTwoThree>());
        // Three signals with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 3);
        take_and_verify_all_signals(flagSelectionChangedSignalSpy, {
            { MyEnum::One, true },
            { MyEnum::Two, true },
            { MyEnum::Three, true }
        });
        // UiElement status
        verify_ui_elements_state(object);

        // Click on MyEnum::OneThree
        clickOnElement(object->template getMaskElement<MyEnumFlags::OneThree>());
        // Three signals with expected arguments
        QTRY_COMPARE(flagSelectionChangedSignalSpy->count(), 2);
        take_and_verify_all_signals(flagSelectionChangedSignalSpy, {
            { MyEnum::One, false },
            { MyEnum::Three, false }
        });
        // UiElement status
        verify_ui_elements_state(object);
    }
///@}

private slots:
    void initTestCase() {
        MyEnumFlagsMenu* menu = new MyEnumFlagsMenu;
        // Taking in account all of the templated stuff above - just to be sure lets verify helpers.
        verify_ui_elements_state(menu);
        // Check status of the flags ui elements
        QCOMPARE(menu->template getFlagElement<MyEnum::One>()->isChecked(),   false);
        QCOMPARE(menu->template getFlagElement<MyEnum::Two>()->isChecked(),   false);
        QCOMPARE(menu->template getFlagElement<MyEnum::Three>()->isChecked(), false);
        // Check status of mask ui elements
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::OneThree>()->isChecked(),    false);
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::OneTwoThree>()->isChecked(), false);
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::All>()->isChecked(),         false);

        // Lets set manually some state
        menu->m_displayedFlags = MyEnumFlags{MyEnumFlags::OneThree};
        menu->template getFlagElement<MyEnum::One>()->setChecked(true);
        menu->template getFlagElement<MyEnum::Three>()->setChecked(true);
        menu->template getMaskElement<MyEnumFlags::OneThree>()->setChecked(true);

        // And see if everything is fine
        verify_ui_elements_state(menu);
        // Check status of the flags ui elements
        QCOMPARE(menu->template getFlagElement<MyEnum::One>()->isChecked(),   true);
        QCOMPARE(menu->template getFlagElement<MyEnum::Two>()->isChecked(),   false);
        QCOMPARE(menu->template getFlagElement<MyEnum::Three>()->isChecked(), true);
        QCOMPARE(menu->template getFlagElement<MyEnum::Four>()->isChecked(),  false);
        // Check status of mask ui elements
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::OneThree>()->isChecked(),    true);
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::OneTwoThree>()->isChecked(), false);
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::All>()->isChecked(),         false);

        // Lets set manually some other state
        menu->m_displayedFlags = MyEnumFlags{MyEnumFlags::OneThree | MyEnum::Four};
        menu->template getFlagElement<MyEnum::One>()->setChecked(true);
        menu->template getFlagElement<MyEnum::Three>()->setChecked(true);
        menu->template getFlagElement<MyEnum::Four>()->setChecked(true);
        menu->template getMaskElement<MyEnumFlags::OneThree>()->setChecked(true);

        // And see if everything is fine
        verify_ui_elements_state(menu);
        // Check status of the flags ui elements
        QCOMPARE(menu->template getFlagElement<MyEnum::One>()->isChecked(),   true);
        QCOMPARE(menu->template getFlagElement<MyEnum::Two>()->isChecked(),   false);
        QCOMPARE(menu->template getFlagElement<MyEnum::Three>()->isChecked(), true);
        QCOMPARE(menu->template getFlagElement<MyEnum::Four>()->isChecked(),  true);
        // Check status of mask ui elements
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::OneThree>()->isChecked(),    true);
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::OneTwoThree>()->isChecked(), false);
        QCOMPARE(menu->template getMaskElement<MyEnumFlags::All>()->isChecked(),         false);

        delete menu;

        qRegisterMetaType<MyEnum>("MyEnum");
    }

    void init() {
        menu = new MyEnumFlagsMenu;
        menuFlagSelectionChangedSignalSpy = new QSignalSpy{menu, &MyEnumFlagsMenu::flagSelectionChanged};

        widget = new MyEnumFlagsWidget;
        widgetFlagSelectionChangedSignalSpy = new QSignalSpy{widget, &MyEnumFlagsWidget::flagSelectionChanged};
    }

    void cleanup() {
        delete menu;
        menu = nullptr;
        delete menuFlagSelectionChangedSignalSpy;
        menuFlagSelectionChangedSignalSpy = nullptr;

        delete widget;
        widget = nullptr;
        delete widgetFlagSelectionChangedSignalSpy;
        widgetFlagSelectionChangedSignalSpy = nullptr;
    }

    void test_constructor() {
        // Test QMenu-based
        test_constructor_impl<MyEnumFlagsMenu>();
        // Test Widget-based
        test_constructor_impl<MyEnumFlagsWidget>();
    }

    void test_flags_and_mask_presense() {
        // Test QMenu-based
        test_flags_and_mask_presense_impl<MyEnumFlagsMenu>();
        // Test Widget-based
        test_flags_and_mask_presense_impl<MyEnumFlagsWidget>();
    }

    void test_get_ui_element() {
        // Test QMenu-based
        test_get_ui_element_impl<MyEnumFlagsMenu>(menu);
        // Test Widget-based
        test_get_ui_element_impl<MyEnumFlagsWidget>(widget);
    }

    void test_get_ui_element_variants_return_types() {
        // Test QMenu-based
        TYPE_COMPARE(decltype(menu->getUiElement(std::declval<MyEnumFlags>())), QAction*);
        TYPE_COMPARE(decltype(menu->template getFlagElement<MyEnum::One>()), QAction*);
        TYPE_COMPARE(decltype(menu->template getMaskElement<MyEnumFlags::All>()), QAction*);
        const auto* constMenu = menu;
        TYPE_COMPARE(decltype(constMenu->getUiElement(std::declval<MyEnumFlags>())), const QAction*);
        TYPE_COMPARE(decltype(constMenu->template getFlagElement<MyEnum::One>()), const QAction*);
        TYPE_COMPARE(decltype(constMenu->template getMaskElement<MyEnumFlags::All>()), const QAction*);
    }

    void test_display_mask_selected() {
        // Test QMenu-based
        test_display_mask_selected_impl<MyEnumFlagsMenu>(menu, menuFlagSelectionChangedSignalSpy);
        // Test Widget-based
        test_display_mask_selected_impl<MyEnumFlagsWidget>(widget, widgetFlagSelectionChangedSignalSpy);
    }

    void test_display_flag_selected() {
        // Test QMenu-based
        test_display_flag_selected_impl<MyEnumFlagsMenu>(menu, menuFlagSelectionChangedSignalSpy);
        // Test Widget-based
        test_display_flag_selected_impl<MyEnumFlagsWidget>(widget, widgetFlagSelectionChangedSignalSpy);
    }

    void test_is_flag_selected() {
        // // Test QMenu-based
        test_is_flag_selected_impl<MyEnumFlagsMenu>(menu);
        // Test Widget-based
        test_is_flag_selected_impl<MyEnumFlagsWidget>(widget);
    }

    void test_test_displayed_mask() {
        // Test QMenu-based
        test_displayed_mask_impl<MyEnumFlagsMenu>(menu);
        // Test Widget-based
        test_displayed_mask_impl<MyEnumFlagsWidget>(widget);
    }

    void test_ui_triggering() {
        // Test QMenu-based
        test_ui_triggering_impl<MyEnumFlagsMenu>(menu, menuFlagSelectionChangedSignalSpy);
        // Test Widget-based
        test_ui_triggering_impl<MyEnumFlagsWidget>(widget, widgetFlagSelectionChangedSignalSpy);
    }
};

}; // namespace Draupnir::Ui

QTEST_MAIN(Draupnir::Ui::FlagsMaskSelectorBaseTest)

#include "FlagsMaskSelectorBaseTest.moc"
