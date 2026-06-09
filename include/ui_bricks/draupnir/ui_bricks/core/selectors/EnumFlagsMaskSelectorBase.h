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

#ifndef ENUMFLAGSMASKSELECTORBASE_H
#define ENUMFLAGSMASKSELECTORBASE_H

#include <QDebug>

#include "draupnir/ui_bricks/concepts/DisplayableEnumFlagsConcept.h"
#include "draupnir/ui_bricks/utils/CheckableUiElementHelper.h"
#include "draupnir/utils/variadic_template_from_array.h"

namespace Draupnir::Ui
{

/*! @class EnumFlagsMaskSelectorBase draupnir/ui_bricks/core/selectors/EnumFlagsMaskSelectorBase.h
 *  @ingroup UiBricks
 *  @brief Template base class for generating checkable UI selectors for enum flags masks.
 *  @tparam Implementation CRTP implementation class. It is expected to provide `flagSelectionChanged(Enum, bool)`, typically
 *          as a Qt signal.
 *  @tparam UiElement Type of generated checkable UI element. Supported types are `QAction` and `QCheckBox`.
 *  @tparam _EnumFlags Displayable enum flags type used as the selector model.
 *
 *  @details `EnumFlagsMaskSelectorBase` builds a reusable selector for bitmask-like enum flags from compile-time display metadata
 *           provided by `_EnumFlags`.
 *
 *           The selector generates:
 *           - one checkable UI element for every value in `_EnumFlags::displayedIndividualValues`;
 *           - optional checkable UI elements for preset masks from `_EnumFlags::displayedMasks`.
 *
 *           Each generated element is connected to the selector state logic. Individual flag elements represent single enum values.
 *           Preset mask elements represent groups of flags and behave as group toggles: checking a preset mask adds its bits to the
 *           current mask, unchecking it removes its bits.
 *
 *           Programmatic state changes performed through @ref displayFlagSelected and @ref displayMaskSelected update the internal
 *           mask and synchronize generated UI elements, but do not emit user-interaction notifications. User-triggered interaction
 *           with generated UI elements updates the internal mask and notifies the derived implementation through
 *           `Implementation::flagSelectionChanged(Enum, bool)`.
 *  @see @ref enum_flags_mask_selector_usage
 *
 * @todo Important: Add support for creation of QComboBox-es with selections.
 * @todo Important: Fix potential memory leak when creating Ui elements.
 * @todo Question: Maybe add this auto&& self to the UiElement getters?
 * @todo Question: Maybe move tuple_cat_t to a separate file / location?
 * @todo Documentation: Extend usage page and put it into some reasonable place.
 * @todo User-Friendliness: Add validations for: Implementation (is derived? has methods?, _EnumFlags (Are static constexpr
 *       arrays containing reasonable values, e.g. no duplicates?).
 * @todo Tests: Add compile tests for this class. */

template<class Implementation, class UiElement, DisplayableEnumFlagsConcept _EnumFlags>
class EnumFlagsMaskSelectorBase
{
    static_assert(std::is_same_v<UiElement, QAction> || std::is_same_v<UiElement, QCheckBox>,
                  "Only QAction and QCheckBox are supported as UiElement");

    /*! @brief Entry wrapper for an individual enum flag UI element.
     *  @tparam Value Individual enum value taken from `EnumFlags::displayedIndividualValues`.
     *  @details This wrapper converts a compile-time enum value into a unique type. The resulting type is stored inside
     *           `m_uiElementsTuple` and contains the pointer to the UI element representing this flag.
     *
     *           For example, if `Value == MyEnum::One`, the generated tuple entry type is: `_FlagValueWrapper<MyEnum::One>`
     *           This allows retrieving and updating the corresponding UI element by type. */
    template<auto Value>
    struct _FlagValueWrapper {
        /*! @brief Type of the wrapped compile-time value. */
        using ValueType = std::remove_cvref_t<decltype(Value)>;

        /*! @brief Compile-time value represented by this entry. */
        static inline constexpr auto value = Value;

        /*! @brief Runtime UI element associated with @ref value. */
        UiElement* element = nullptr;
    };

    /*! @brief Entry wrapper for a preset mask UI element.
     *  @tparam Value Preset mask value taken from `EnumFlags::displayedMasks`.
     *  @details This wrapper converts a compile-time preset mask value into a unique type. The resulting type is stored inside
     *           `m_uiElementsTuple` and contains the pointer to the UI element representing this preset mask.
     *
     *           Unlike @ref _FlagValueWrapper, this entry represents a mask/group of flags, not a single individual flag. */
    template<auto Value>
    struct _MaskValueWrapper {
        using ValueType = std::remove_cvref_t<decltype(Value)>;
        static inline constexpr auto value = Value;
        UiElement* element = nullptr;
    };

    /*! @brief Builds an empty tuple for enum flags types without preset masks.
     *  @tparam Arg Displayable enum flags type.
     *  @details Primary template used when @p Arg does not provide `displayedMasks`. In this case the selector contains only
     *           individual flag entries, so the preset mask entries tuple is empty. */
    template<DisplayableEnumFlagsConcept Arg>
    struct _PresetMasksExtractor {
        /*! @brief Tuple type containing generated preset mask entries. */
        using type = std::tuple<>;
    };

    /*! @brief Builds preset mask entry tuple for enum flags types with displayed masks.
     *  @tparam Arg Displayable enum flags type providing `displayedMasks`.
     *  @details This specialization converts `Arg::displayedMasks` into a tuple of `_MaskValueWrapper<Mask>` entries. For
     *           example, if: `Arg::displayedMasks == { MaskA, MaskB }` then the resulting type is: std::tuple<_MaskValueWrapper<MaskA>,
     *           _MaskValueWrapper<MaskB>>`. */
    template<DisplayableEnumFlagsWithMasks Arg>
    struct _PresetMasksExtractor<Arg> {
        /*! @brief Tuple type containing generated preset mask entries. */
        using type = draupnir::utils::tuple_from_static_array_t<Arg::displayedMasks,_MaskValueWrapper>;
    };

    /*! @brief Convenience alias for concatenating tuple types.
     *  @tparam Tuples Tuple types to concatenate.
     *  @details Produces the type returned by `std::tuple_cat` for the provided tuple types without creating a runtime tuple
     *           object. For example: `tuple_cat_t<std::tuple<A, B>, std::tuple<C>> == std::tuple<A, B, C>`. */
    template<class... Tuples>
    using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

    /*! @brief Tuple type containing all generated selector entries.
     *  @details The tuple contains:
     *           - one `_FlagValueWrapper<Value>` entry for every value from `EnumFlags::displayedIndividualValues`;
     *           - zero or more `_MaskValueWrapper<Value>` entries generated from `EnumFlags::displayedMasks`, if
     *           the enum flags type provides them.
     *
     *           This tuple is the compile-time table used by the selector to create, access, update, and synchronize
     *           all runtime UI elements. */
    using _ElementsTuple = tuple_cat_t<
        draupnir::utils::tuple_from_static_array_t<_EnumFlags::displayedFlags, _FlagValueWrapper>,
        typename _PresetMasksExtractor<_EnumFlags>::type
    >;

public:
    /*! @brief Enum type represented by the selector. */
    using Enum = typename _EnumFlags::enum_type;

    /*! @brief Underlying integer type for Enum. */
    using Integer = typename _EnumFlags::integer;

    static_assert(std::is_same_v<std::underlying_type_t<Enum>,Integer>);

    /*! @brief Enum flags mask type represented by the selector. */
    using EnumFlags = _EnumFlags;

    /*! @brief Returns the UI element representing the specified flag or preset mask.
     *  @param key Flag or preset mask value to search for.
     *  @return Pointer to the UI element associated with `key`, or `nullptr` if no generated selector entry represents this value.
     *  @details This is a runtime lookup counterpart of `getFlagElement<Key>()` and `getMaskElement<Key>()`.
     *
     *           The lookup compares `key` with values represented by all generated selector entries:
     *           - individual flag entries;
     *           - optional preset mask entries.
     *
     *           Both individual enum values and preset mask values are normalized to `EnumFlags`, so the same lookup path can be
     *           used for both.
     * @todo User-Friendliness: In theory not all keys from the `Enum` can be included in this selector. Reasonable Q_ASSERT_X /
     *       Q_ASSERT here would be nice to have when argument is not fitting to any displayed combination of `Enum` keys. */
    [[nodiscard]] UiElement* getUiElement(EnumFlags key) { return _getUiElementImpl<0>(key); }

    /*! @brief Returns the UI element representing a specific individual enum flag.
     *  @tparam Key Individual enum value that must be present in `EnumFlags::displayedIndividualValues`.
     *  @return Pointer to the UI element associated with @p Key.
     *  @details The element is retrieved by the unique compile-time wrapper type generated for `Key`. If `Key` is not part
     *           of the generated selector entries, this method fails at compile time.
     * @todo User-Friendliness: Add reasonable static_assert to this method. Or requires statement? Depending on what produces
     *       better error output. */
    template<Enum Key>
    [[nodiscard]] UiElement* getFlagElement() {
        return std::get<_FlagValueWrapper<Key>>(m_uiElementsTuple).element;
    }

    /*! @brief Returns the UI element representing a specific preset mask.
     *  @tparam Key Preset mask value that must be present in `EnumFlags::displayedMasks`.
     *  @return Pointer to the UI element associated with `Key`.
     *  @details This method is available only when the enum flags type provides displayed preset masks. The element is retrieved
     *           by the unique compile-time wrapper type generated for `Key`.
     *
     *           If `Key` is not part of the generated preset mask entries, this method fails at compile time.
     * @todo User-Friendliness: Add reasonable static_assert to this method. Or requires statement? Depending on what produces
     *       better error output. */
    template<Integer Key>
    [[nodiscard]] UiElement* getMaskElement() requires(DisplayableEnumFlagsWithMasks<_EnumFlags>) {
        return std::get<_MaskValueWrapper<Key>>(m_uiElementsTuple).element;
    }

    /*! @brief Programmatically replaces the currently displayed flags mask.
     *  @param mask New mask to display.
     *  @details Updates the internal selected flags mask and synchronizes all generated UI elements with the new state. This
     *           method represents a programmatic state change and does not emit user-interaction signals from the derived
     *           implementation.
     * @todo User-Friendliness: As in theory EnumFlags mask can contain random data some reasonable Q_ASSERT_X / Q_ASSERT here
     *       would be nice to have. Things to cover: argument is not fitting to any possible combination of Enum keys; argument
     *       is not fitting to any displayed combination of Enum keys. */
    void displayMaskSelected(EnumFlags mask) {
        m_displayedEnumFlags = mask;
        _updateUiElements();
    }

    /*! @brief Programmatically changes the displayed state of one individual flag.
     *  @param flag Individual flag to update.
     *  @param isSelected New selected state for `flag`.
     *  @details Updates the internal selected flags mask, synchronizes the UI element representing `flag`, and then updates preset
     *           mask UI elements affected by the change.
     *
     *           This method represents a programmatic state change and does not emit user-interaction signals from the derived
     *           implementation.
     * @todo User-Friendliness: In theory not all keys from the `Enum` can be included in this selector. Reasonable Q_ASSERT_X /
     *       Q_ASSERT here would be nice to have when argument is not fitting to any displayed combination of `Enum` keys. */
    void displayFlagSelected(Enum flag, bool isSelected) {
        m_displayedEnumFlags.set_flag(flag, isSelected);
        getUiElement(flag)->setChecked(isSelected);
        _updateMaskUiElements();
    };

    /*! @brief Checks whether a specific individual flag is currently selected.
     *  @param flag Individual enum flag to check.
     *  @return `true` if `flag` is currently set in the displayed flags mask, `false` otherwise.
     * @todo User-Friendliness: In theory not all keys from the `Enum` can be included in this selector. Reasonable Q_ASSERT_X /
     *       Q_ASSERT here would be nice to have when argument is not fitting to any displayed combination of `Enum` keys. */
    [[nodiscard]] bool isFlagSelected(Enum flag) const { return m_displayedEnumFlags.test_flag(flag); }

    /*! @brief Returns the currently displayed flags mask. The returned mask reflects the current internal selector state,
     *         including both programmatic changes and user-triggered changes.
     *  @return Current selected flags mask represented by this selector. */
    [[nodiscard]] EnumFlags selectedMask() const { return m_displayedEnumFlags; }

protected:
    /*! @brief Constructs the selector and creates all generated UI elements.
     *  @param initialState Initial flags mask to be displayed by the selector.
     *  @details The constructor creates one checkable UI element for each compile-time selector entry stored in `m_uiElementsTuple`.
     *
     *           Generated entries include:
     *           - individual flag entries from `EnumFlags::displayedIndividualValues`;
     *           - optional preset mask entries from `EnumFlags::displayedMasks`.
     *
     *           Each created UI element is initialized with the display string provided by `EnumFlags::toDisplayString()` and
     *           connected to the corresponding user-interaction handler.
     *
     *           If `initialState` is not empty, all generated UI elements are synchronized with it after creation.
     *
     * @note Programmatic initialization does not emit derived-class user interaction signals. Only user-triggered UI element
     *       activation is expected to do that. */
    EnumFlagsMaskSelectorBase(EnumFlags initialState = 0) :
        m_displayedEnumFlags{initialState}
    {
        std::apply([this](auto&... element){
            ((
                 element.element = Ui::CheckableUiElementHelper<UiElement>::createConnectedUiElement(
                     _EnumFlags::toDisplayString(std::remove_cvref_t<decltype(element)>::value),
                     [this](bool checked){ _onUserInteraction(std::remove_cvref_t<decltype(element)>::value, checked); }
            )), ... );
        }, m_uiElementsTuple);

        if (initialState.any())
            _updateUiElements();
    };

    /*! @brief Updates text labels of all generated UI elements.
     *  @details Re-applies display strings from `EnumFlags::toDisplayString()` to all UI elements stored in `m_uiElementsTuple`.
     *
     *           This method is intended for Qt translation/retranslation flows where already-created UI elements need to update
     *           their visible text without changing the current selected flags mask. */
    void retranslateUiElements() {
        std::apply([this](auto&... element){
            ((
                 element.element->setText(_EnumFlags::toDisplayString(std::remove_cvref_t<decltype(element)>::value))
            ), ... );
        }, m_uiElementsTuple);
    };

private:
    friend class EnumFlagsSelectorViewBaseTest;

    _EnumFlags m_displayedEnumFlags;
    _ElementsTuple m_uiElementsTuple;

    /*! @brief Recursively searches generated selector entries for a matching value.
     *  @tparam Index Current tuple index used by the recursive lookup.
     *  @param key Normalized flag or preset mask value to search for.
     *  @return Pointer to the matching UI element, or `nullptr` if no matching entry exists.
     *  @details This method walks through `m_uiElementsTuple` at compile time using recursive template instantiation. Each entry
     *           value is normalized to `EnumFlags` before comparison, which allows the same implementation to handle both individual
     *           flag entries and preset mask entries. */
    template<std::size_t Index = 0>
    UiElement* _getUiElementImpl(EnumFlags key) {
        using TupleElement = std::tuple_element_t<Index, _ElementsTuple>;

        if (EnumFlags{TupleElement::value} == key)
            return std::get<Index>(m_uiElementsTuple).element;

        if constexpr (Index + 1 < std::tuple_size_v<_ElementsTuple>) {
            return _getUiElementImpl<Index+1>(key);
        } else {
            return nullptr;
        }
    }

    /*! @brief Handles user interaction with an individual flag UI element.
     *  @param value Individual enum flag represented by the activated UI element.
     *  @param checked New checked state of the UI element.
     *  @details Updates the internal displayed flags mask by setting or clearing `value` according to `checked`.
     *
     *           After the internal mask is updated, preset mask UI elements are synchronized with the new state.
     *           Individual flag UI elements are not updated here because the interacted element already has the
     *           correct checked state.
     *
     *           Finally, the derived implementation method `Implementation::flagSelectionChanged(value, checked)`
     *           is invoked. This method should be a Qt signal.
     * @note This handler is intended for user-triggered changes only. */
    void _onUserInteraction(EnumFlags::enum_type value, bool checked) {
        m_displayedEnumFlags.set_flag(value, checked);
        _updateMaskUiElements();
        static_cast<Implementation*>(this)->flagSelectionChanged(value, checked);
    }

    /*! @brief Handles user interaction with a preset mask UI element.
     *  @param value Preset flags mask represented by the activated UI element.
     *  @param checked New checked state of the UI element.
     *  @details If `checked` is `true`, all bits from `value` are added to the internal displayed flags mask. If `checked` is
     *           `false`, all bits from `value` are removed from the internal displayed flags mask.
     *
     *           After the mask is updated, all generated UI elements are synchronized. Individual flag elements whose state
     *           changed may notify the derived implementation through `Implementation::flagSelectionChanged(...)`.
     *
     *           Preset mask elements are updated according to the resulting mask state.
     *
     * @note Preset mask UI elements behave as group toggles: checking one adds its bits, unchecking one removes its bits. */
    void _onUserInteraction(EnumFlags value, bool checked) {
        m_displayedEnumFlags = checked ?
            m_displayedEnumFlags | value :
            m_displayedEnumFlags & ~value ;

        std::apply([this](auto&... element){
            ((
                 this->template _updateUiElementsOnMaskChange<std::remove_cvref_t<decltype(element)>>()
            ), ... );
        }, m_uiElementsTuple);
    }

///@name Mask UI synchronization helpers
///@{
    /*! @brief Updates a preset mask UI element according to the current selected flags mask.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskValueWrapper<Value>` entries.
     *  @details A preset mask UI element is checked when the current displayed flags mask is a superset of the mask represented
     *           by the entry.
     *
     *           For example:
     *           @code
     *           Element mask:  0b0011 | 0b1001
     *           Current mask:  0b0111 | 0b0111
     *           Checked:         true |  false
     *           @endcode */
    template<class TupleElement>
    void _updateMaskUiElement() requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement, _MaskValueWrapper>) {
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(m_displayedEnumFlags.is_superset(TupleElement::value));
    }

    /*! @brief Ignores individual flag UI elements during preset mask synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for
     *  @details `_updateMaskUiElements()` iterates over all generated selector entries. Individual flag entries are intentionally ignored
     *           by this overload, because this synchronization pass updates only preset mask UI elements. */
    template<class TupleElement>
    void _updateMaskUiElement() requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement, _FlagValueWrapper>) {}

    /*! @brief Updates checked state of all preset mask UI elements.
     *  @details Iterates over all generated selector entries stored in `m_uiElementsTuple` and updates only entries representing preset
     *           masks. Individual flag entries are ignored by the corresponding constrained overload of `_updateMaskUiElement()`. */
    void _updateMaskUiElements() {
        std::apply([this](auto&... element){
            ((
                 this->template _updateMaskUiElement<std::remove_cvref_t<decltype(element)>>()
                 ), ... );
        }, m_uiElementsTuple);
    }
///@}

///@name Flag UI synchronization helpers
///@{
    /*! @brief Ignores preset mask UI elements during individual flag synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskValueWrapper<Value>` entries.
     *  @details `_updateFlagsUiElements()` iterates over all generated selector entries. Preset mask entries are intentionally
     *           ignored by this overload, because this synchronization pass updates only individual flag UI elements. */
    template<class TupleElement>
    void _updateFlagUiElement() requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement,_MaskValueWrapper>) {}

    /*! @brief Updates an individual flag UI element according to the current selected flags mask.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagValueWrapper<Value>` entries.
     *  @details The UI element is checked when the current displayed flags mask contains the individual enum flag represented
     *           by the entry. */
    template<class TupleElement>
    void _updateFlagUiElement() requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement,_FlagValueWrapper>) {
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(m_displayedEnumFlags.test_flag(TupleElement::value));
    }

    /*! @brief Updates checked state of all individual flag UI elements.
     *  @details Iterates over all generated selector entries stored in `m_uiElementsTuple` and updates only entries representing
     *           individual enum flags. Preset mask entries are ignored by the corresponding constrained overload of
     *           `_updateFlagUiElement()`. */
    void _updateFlagsUiElements() {
        std::apply([this](auto&... element){
            ((
                 this->template _updateFlagUiElement<std::remove_cvref_t<decltype(element)>>()
                 ), ... );
        }, m_uiElementsTuple);
    }
///@}

///@name Full UI synchronization helpers
///@{
    /*! @brief Updates a preset mask UI element during full UI synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskValueWrapper<Value>` entries.
     *  @details Delegates to `_updateMaskUiElement()` so that preset mask entries are synchronized according to the
     *           current displayed flags mask. */
    template<class TupleElement> requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement, _MaskValueWrapper>)
    void _updateUiElement() { _updateMaskUiElement<TupleElement>(); }

    /*! @brief Updates an individual flag UI element during full UI synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagValueWrapper<Value>` entries.
     *  @details Delegates to `_updateFlagUiElement()` so that individual flag entries are synchronized according to the
     *  current displayed flags mask. */
    template<class TupleElement> requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement, _FlagValueWrapper>)
    void _updateUiElement() { _updateFlagUiElement<TupleElement>(); }

    /*! @brief Synchronizes checked state of all generated UI elements.
     *  @details Iterates over all selector entries stored in `m_uiElementsTuple` and updates each entry according to
     *           its kind:
     *           - `_FlagValueWrapper<Value>` entries are updated as individual flags;
     *           - `_MaskValueWrapper<Value>` entries are updated as preset masks.
     *
     *           This method is used after programmatic full-mask changes and during initialization from a non-empty
     *           initial mask. */
    void _updateUiElements() {
        std::apply([this](auto&... element){
            ((
                 this->template _updateUiElement<std::remove_cvref_t<decltype(element)>>()
                 ), ... );
        }, m_uiElementsTuple);
    }
///@}

///@name UI synchronization after preset mask interaction
///@{
    /*! @brief Synchronizes a preset mask UI element after a preset mask state change.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskValueWrapper<Value>` entries.
     *  @details Updates the checked state of the preset mask UI element according to the resulting displayed flags mask.
     *
     *           A preset mask UI element is checked when the current displayed flags mask is a superset of the mask
     *           represented by the entry.
     *
     * @note This overload does not emit `Implementation::flagSelectionChanged`, because preset mask entries do not
     *       represent individual flags. */
    template<class TupleElement> requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement, _MaskValueWrapper>)
    void _updateUiElementsOnMaskChange() {
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(m_displayedEnumFlags.is_superset(TupleElement::value));
    }

    /*! @brief Synchronizes an individual flag UI element after a preset mask state change.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagValueWrapper<Value>` entries.
     *  @details Compares the current checked state of the individual flag UI element with the state implied by the resulting
     *           displayed flags mask.
     *
     *           If the checked state changed, the UI element is updated and the derived implementation is notified through:
     *           `Implementation::flagSelectionChanged(flag, newState)`. This is used when a preset mask UI element is toggled,
     *           because one preset mask interaction may change multiple individual flags. */
    template<class TupleElement> requires (draupnir::utils::is_a1tp_instantiation_of_v<TupleElement, _FlagValueWrapper>)
    void _updateUiElementsOnMaskChange() {
        const bool currentState = std::get<TupleElement>(m_uiElementsTuple).element->isChecked();
        const bool newState = m_displayedEnumFlags.test_flag(TupleElement::value);

        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(newState);

        if (newState != currentState) {
            static_cast<Implementation*>(this)->flagSelectionChanged(TupleElement::value, newState);
        }
    }
///@}
};

}; // namespace Draupnir::UiBricks

/*! @page enum_flags_mask_selector_usage Enum Flags Mask Selector Usage
 *
 *  A typical flags type provides display metadata:
 *
 *  @code{.cpp}
 *  enum MyEnum {
 *      One   = 0b0001,
 *      Two   = 0b0010,
 *      Three = 0b0100,
 *      Four  = 0b1000
 *  };
 *
 *  class MyEnumFlags : public draupnir::utils::enum_flags<MyEnum>
 *  {
 *  public:
 *      using draupnir::utils::enum_flags<MyEnum>::enum_flags;
 *      using draupnir::utils::enum_flags<MyEnum>::operator=;
 *
 *      using enum_type = draupnir::utils::enum_flags<MyEnum>::enum_type;
 *      using integer = draupnir::utils::enum_flags<MyEnum>::integer;
 *
 *      static constexpr MyEnum displayedIndividualValues[] = {
 *          MyEnum::One,
 *          MyEnum::Two,
 *          MyEnum::Three,
 *          MyEnum::Four
 *      };
 *
 *      static constexpr integer OneThree = MyEnum::One | MyEnum::Three;
 *      static constexpr integer All = MyEnum::One | MyEnum::Two | MyEnum::Three | MyEnum::Four;
 *
 *      static constexpr integer displayedMasks[] = {
 *          OneThree,
 *          All
 *      };
 *
 *      static QString toDisplayString(MyEnumFlags value);
 *  };
 *  @endcode
 *
 *  A menu selector can then be built by inheriting from this base class:
 *
 *  @code{.cpp}
 *  class MyEnumFlagsMenu :
 *      public QMenu,
 *      public Draupnir::Ui::EnumFlagsMaskSelectorBase<MyEnumFlagsMenu, QAction, MyEnumFlags>
 *  {
 *      Q_OBJECT
 *
 *  public:
 *      explicit MyEnumFlagsMenu(MyEnumFlags initialState = 0, QWidget* parent = nullptr)
 *          : QMenu{parent}
 *          , EnumFlagsMaskSelectorBase{initialState}
 *      {
 *          addAction(getFlagElement<MyEnum::One>());
 *          addAction(getFlagElement<MyEnum::Two>());
 *          addAction(getFlagElement<MyEnum::Three>());
 *          addAction(getFlagElement<MyEnum::Four>());
 *
 *          addSeparator();
 *
 *          addAction(getMaskElement<MyEnumFlags::OneThree>());
 *          addAction(getMaskElement<MyEnumFlags::All>());
 *      }
 *
 *  signals:
 *      void flagSelectionChanged(MyEnum flag, bool selected);
 *  };
 *  @endcode
 *
 *  The same selector logic can be reused for widget-based UI by switching `UiElement`:
 *
 *  @code{.cpp}
 *  class MyEnumFlagsWidget :
 *      public QWidget,
 *      public Draupnir::Ui::EnumFlagsMaskSelectorBase<MyEnumFlagsWidget, QCheckBox, MyEnumFlags>
 *  {
 *      Q_OBJECT
 *
 *  public:
 *      explicit MyEnumFlagsWidget(MyEnumFlags initialState = 0, QWidget* parent = nullptr)
 *          : QWidget{parent}
 *          , EnumFlagsMaskSelectorBase{initialState}
 *      {
 *          auto* layout = new QVBoxLayout{this};
 *
 *          layout->addWidget(getFlagElement<MyEnum::One>());
 *          layout->addWidget(getFlagElement<MyEnum::Two>());
 *          layout->addWidget(getFlagElement<MyEnum::Three>());
 *          layout->addWidget(getFlagElement<MyEnum::Four>());
 *
 *          layout->addWidget(getMaskElement<MyEnumFlags::OneThree>());
 *          layout->addWidget(getMaskElement<MyEnumFlags::All>());
 *      }
 *
 *  signals:
 *      void flagSelectionChanged(MyEnum flag, bool selected);
 *  };
 *  @endcode
 *
 *  @section enum_flags_mask_selector_replaces What this replaces
 *
 *  Without this base class, each selector usually needs repetitive manual code:
 *
 *  @code{.cpp}
 *  oneAction = new QAction{"One", this};
 *  twoAction = new QAction{"Two", this};
 *  allAction = new QAction{"All", this};
 *
 *  oneAction->setCheckable(true);
 *  twoAction->setCheckable(true);
 *  allAction->setCheckable(true);
 *
 *  connect(oneAction, &QAction::triggered, this, [this](bool checked) {
 *      selectedMask.set_flag(MyEnum::One, checked);
 *      updateMaskActions();
 *      emit flagSelectionChanged(MyEnum::One, checked);
 *  });
 *
 *  connect(twoAction, &QAction::triggered, this, [this](bool checked) {
 *      selectedMask.set_flag(MyEnum::Two, checked);
 *      updateMaskActions();
 *      emit flagSelectionChanged(MyEnum::Two, checked);
 *  });
 *
 *  connect(allAction, &QAction::triggered, this, [this](bool checked) {
 *      selectedMask = checked ? (selectedMask | MyEnumFlags::All)
 *                             : (selectedMask & ~MyEnumFlags::All);
 *      updateFlagActionsAndEmitChanges();
 *      updateMaskActions();
 *  });
 *  @endcode
 *
 *  `EnumFlagsMaskSelectorBase` centralizes this boilerplate: element creation, signal connection,
 *  flag/mask synchronization, preset mask handling, and per-flag change notification. */

#endif // ENUMFLAGSMASKSELECTORBASE_H
