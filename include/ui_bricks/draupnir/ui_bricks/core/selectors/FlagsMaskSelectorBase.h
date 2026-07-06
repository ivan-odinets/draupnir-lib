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

#ifndef FLAGSMASKSELECTORBASE_H
#define FLAGSMASKSELECTORBASE_H

#include <QDebug>

#include "draupnir/ui_bricks/concepts/UiTraitConcepts.h"
#include "draupnir/ui_bricks/utils/CheckableUiElementHelper.h"
#include "draupnir/utils/class_marcos.h"
#include "draupnir/utils/template_detectors.h"
#include "draupnir/utils/tuple_like_merge.h"
#include "draupnir/utils/type_presense.h"
#include "draupnir/utils/type_qualifiers_helpers.h"
#include "draupnir/utils/variadic_template_from_array.h"
#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/concepts/flags_concepts.h"

namespace Draupnir::Ui
{

/*! @namespace Draupnir::Ui::TemplateArgs
 *  @ingroup UiBricks
 *  @brief Compile-time argument wrappers for UI template APIs. */

namespace TemplateArgs
{

/*! @brief Type-pack wrapper for explicit mask trait types.
 *  @ingroup UiBricks
 *  @tparam Traits Trait types describing displayed mask entries. */

template<class... Traits>
struct MaskTraitsWrapper { DEFINE_COMPILE_TIME(MaskTraitsWrapper); };

/*! @brief Type-pack wrapper for explicit flag trait types.
 *  @ingroup UiBricks
 *  @tparam Traits Trait types describing displayed individual flag entries. */

template<class... Traits>
struct FlagTraitsWrapper { DEFINE_COMPILE_TIME(FlagTraitsWrapper); };

/*! @brief Value-pack wrapper for explicit mask values.
 *  @ingroup UiBricks
 *  @tparam Values Compile-time mask values to be converted into mask traits. */

template<auto... Values>
struct MaskValuesWrapper { DEFINE_COMPILE_TIME(MaskValuesWrapper); };


/*! @brief Value-pack wrapper for explicit individual flag values.
 *  @ingroup UiBricks
 *  @tparam Values Compile-time flag values to be converted into flag traits. */

template<auto... Values>
struct FlagValuesWrapper { DEFINE_COMPILE_TIME(FlagValuesWrapper); };

}; // namespace TemplateArgs

/*! @class FlagsMaskSelectorBase draupnir/ui_bricks/core/selectors/FlagsMaskSelectorBase.h
 *  @ingroup UiBricks
 *  @brief CRTP base class for building checkable UI selectors for flags-like values.
 *  @tparam _Implementation CRTP implementation type. The implementation is expected to inherit from this class and may optionally
 *          provide notification methods such as `flagSelectionChanged(FlagType, bool)` and `maskChanged(Flags)`. If one or both of
 *          these methods is available within provided `_Implementation` - they will be invoked when either state of individual flag
 *          changed or when the whole mask has changed.
 *  @tparam _UiElement Type of generated checkable UI element. Currently supported types are `QAction` and `QCheckBox`.
 *  @tparam _Flags Flags-like value type used as the selector state. The type must satisfy `draupnir::utils::flags_like_concept`.
 *          If `_FlagTraitContainer` equals `TemplateArgs::FlagTraitsWrapper<>` - _Flags must provide metadata for individual flags.
 *          `_Flags` type may provide metadata of preset masks as well, and it will be used when available and when `_MaskTraitContainer`
 *          equals `TemplateArgs::MaskTraitsWrapper<>`
 *  @tparam _FlagTraitContainer Compile-time source describing individual flag entries shown by the selector.
 *  @tparam _MaskTraitContainer Compile-time source describing optional preset mask entries shown by the selector.
 *
 *  @details `FlagsMaskSelectorBase` generates a group of checkable UI elements representing individual flags and, optionally, preset
 *           masks.
 *
 *           Individual flag entries represent single flag values. User interaction with such an entry sets or clears the corresponding
 *           flag in the internal selected mask.
 *
 *           Preset mask entries represent predefined combinations of flags. Checking a preset mask adds all bits from that mask to the
 *           current selection. Unchecking it removes those bits. After preset mask interaction, all generated UI elements are synchronized
 *           with the resulting selected mask.
 *
 *           The selector accepts metadata from two main sources:
 *           - from the `_Flags` type itself through `_Flags::UiSelectorMetadata`;
 *           - from explicit template argument wrappers passed to `_FlagTraitContainer` and `_MaskTraitContainer`.
 *
 *           All accepted inputs are normalized into a single internal compile-time tuple of selector entries. This tuple is the source of
 *           truth used for element creation, lookup, state synchronization, and compile-time availability checks.
 *
 * @note Programmatic state changes synchronize generated UI elements but are not intended to represent user interaction. User-triggered
 *       changes are handled through internally connected UI element callbacks.
 *
 * @todo Important: Add support for `QComboBox`-based selectors.
 * @todo User-friendliness: Extend validation of arguments: all flags are single-bit things, all masks are not equal to Flags AND
 *       can be composed from flags (?).
 * @todo Important: Clarify ownership/lifetime rules for generated UI elements and fix memory leak when creating Ui elements.
 * @todo Documentation: Extend usage page and put it into some reasonable place.
 * @todo Tests: Add compile tests for this class. */

template<
    class _Implementation,
    class _UiElement,
    draupnir::utils::flags_like_concept _Flags,
    class _FlagTraitContainer = TemplateArgs::FlagTraitsWrapper<>,
    class _MaskTraitContainer = TemplateArgs::MaskTraitsWrapper<>
    >
class FlagsMaskSelectorBase
{
private:
    /*! @brief Alias for the type of individual flag value. To be used internally. */
    using _FlagType = typename _Flags::flag_type;

    /*! @brief Alias for the underlying integer used to store individual flag and masks values. To be used internally. */
    using _UnderlyingInteger = typename _Flags::integer;

    /*! @brief Detects whether a candidate implementation provides flag selection notifications.
     *  @tparam Candidate Candidate CRTP implementation type.
     *  @details Evaluates to `true` when `Candidate` provides a callable method with the following signature: `void
     *           flagSelectionChanged(_FlagType flag, bool checked);`. Used to call flag-selection notifications only when
     *           the implementation explicitly supports them. */
    template<class Candidate>
    static constexpr bool _hasFlagSelectionChanged_v =
        requires(Candidate* candidate, _FlagType flag, bool checked) {
            { candidate->flagSelectionChanged(flag, checked) } -> std::same_as<void>;
        };

    /*! @brief Detects whether a candidate implementation provides mask change notifications.
     *  @tparam Candidate Candidate CRTP implementation type.
     *  @details Evaluates to `true` when `Candidate` provides a callable method with the
     *           following signature: `void maskChanged(_Flags mask);` Used to call preset-mask or selected-mask notifications only
     *           when the implementation explicitly supports them. */
    template<class Candidate>
    static constexpr bool _hasMaskChanged_v =
        requires(Candidate* candidate, _Flags mask) {
            { candidate->maskChanged(mask) } -> std::same_as<void>;
        };

    static_assert(std::is_same_v<_UiElement, QAction> || std::is_same_v<_UiElement, QCheckBox>,
        "Only QAction and QCheckBox are supported as UiElement.");


    /*! @brief Checks whether `_Flags` provides selector flag metadata. Evaluates to `true` when within `_Flags` exists nested
     *         `_Flags::UiSelectorMetadata::displayedFlags` and is a C-style array of `_FlagType` values. Used to detect whether
     *         individual flag entries can be taken from `_Flags` metadata. */
    static constexpr bool _flagsHasSelectorDisplayedFlags = requires {
        requires draupnir::utils::c_array_of_concept<
            decltype(_Flags::UiSelectorMetadata::displayedFlags),_FlagType>;
    };

    /*! @brief Checks whether `_Flags` provides selector preset mask metadata. Evaluates to `true` when within `_Flags` exists
     *         nested `_Flags::UiSelectorMetadata::displayedMaskPresets` and is a C-style array of `_UnderlyingInteger` values.
     *         Used to detect whether preset mask entries can be taken from `_Flags` metadata. */
    static constexpr bool _flagsHasSelectorDisplayedMaskPresets = requires {
        requires draupnir::utils::c_array_of_concept<
            decltype(_Flags::UiSelectorMetadata::displayedMaskPresets), _UnderlyingInteger>;
    };

    /*! @brief Internal selector entry for an individual flag trait.
     *  @tparam _Trait Trait type describing one displayed flag entry.
     *  @details Stores the trait type together with the runtime UI element created for that entry.
     *           The trait is expected to expose a compile-time `value()` member function and may optionally provide
     *           display metadata such as `displayString()`. */
    template<class _Trait>
    struct _FlagTraitWrapper {
        using Type = _Trait;
        _UiElement* element = nullptr;
    };

    /*! @brief Internal selector entry for a preset mask trait.
     *  @tparam _Trait Trait type describing one displayed preset mask entry.
     *  @details Stores the trait type together with the runtime UI element created for that entry.
     *           The trait is expected to expose a compile-time `value()` member function and may optionally provide display
     *           metadata such as `displayString()`. */
    template<class _Trait>
    struct _MaskTraitWrapper {
        using Type = _Trait;
        _UiElement* element = nullptr;
    };

    /*! @brief Minimal trait adapter for a compile-time value.
     *  @tparam Value Flag or preset mask value represented by this generated trait.
     *  @details Used when selector entries are provided as raw compile-time values instead of explicit
     *           trait types. The adapter exposes the value through the same `value()` member function expected from normal selector
     *           traits. */
    template<auto Value>
    struct _ValueWrapper {
        static constexpr inline auto value() { return Value; };
    };

    /*! @brief Normalizes selector flag input into internal flag selector entries.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam FlagsTraitContainer Compile-time container describing displayed flag entries.
     *  @details This helper converts all supported flag input forms into a tuple of
     *           `_FlagTraitWrapper<...>` entries used by the selector as part of its canonical compile-time entry table.
     *
     *           Supported input forms:
     *           - `TemplateArgs::FlagValuesWrapper<Values...>` for explicit flag values;
     *           - `TemplateArgs::FlagTraitsWrapper<Traits...>` for explicit flag traits;
     *           - an empty `FlagValuesWrapper<>` or `FlagTraitsWrapper<>` to use `Flags::UiSelectorMetadata::displayedFlags`. */
    template<class Flags, class FlagsTraitContainer>
    struct _FlagsTraitNormalizer;

    /*! @brief Normalizes explicitly provided flag values.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam FlagValues Explicit compile-time flag values to display.
     *  @details Each raw flag value is wrapped into `_ValueWrapper<Value>` and then into `_FlagTraitWrapper<...>`, producing
     *           the same internal entry shape as explicit trait-based input. */
    template<class Flags, auto... FlagValues> requires(sizeof...(FlagValues) > 0)
    struct _FlagsTraitNormalizer<Flags, TemplateArgs::FlagValuesWrapper<FlagValues...>> {
        using type = std::tuple<_FlagTraitWrapper<_ValueWrapper<FlagValues>>...>;
    };

    /*! @brief Normalizes explicitly provided flag traits.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam FlagTraits Explicit trait types describing displayed flag entries.
     *  @details Each provided trait type is wrapped into `_FlagTraitWrapper<Trait>`. The trait is expected to expose a compile-time
     *           compatible `value()` member function and may optionally provide additional display metadata. */
    template<class Flags, class... FlagTraits> requires(sizeof...(FlagTraits) > 0)
    struct _FlagsTraitNormalizer<Flags, TemplateArgs::FlagTraitsWrapper<FlagTraits...>> {
        using type = std::tuple<_FlagTraitWrapper<FlagTraits>...>;
    };

    /*! @brief Normalizes empty flag input by using default selector metadata from `Flags`.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam EmptyContainer Empty flag container type.
     *  @details Empty `FlagTraitsWrapper<>` and `FlagValuesWrapper<>` mean that individual flag entries should be taken from
     *           `Flags::UiSelectorMetadata::displayedFlags`. The metadata array is converted into a `FlagTraitsWrapper<_ValueWrapper<...>...>`
     *           and then normalized through the explicit-trait specialization. */
    template<class Flags, class EmptyContainer> requires(
        draupnir::utils::is_one_of_v<EmptyContainer, TemplateArgs::FlagTraitsWrapper<>, TemplateArgs::FlagValuesWrapper<>>
    )
    struct _FlagsTraitNormalizer<Flags, EmptyContainer> {
        using type = _FlagsTraitNormalizer<
            Flags,
            draupnir::utils::variadic_template_from_static_array_t<
                Flags::UiSelectorMetadata::displayedFlags,
                _ValueWrapper,
                TemplateArgs::FlagTraitsWrapper
                >
            >::type;
    };

    /*! @brief Normalizes selector preset mask input into internal mask selector entries.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam MaskTraitContainer Compile-time container describing displayed preset mask entries.
     *  @details This helper converts all supported preset mask input forms into a tuple of `_MaskTraitWrapper<...>` entries
     *           used by the selector as part of its canonical compile-time entry table.
     *
     *           Supported input forms:
     *           - `TemplateArgs::MaskTraitsWrapper<Traits...>` for explicit preset mask traits;
     *           - `TemplateArgs::MaskValuesWrapper<Values...>` for explicit preset mask values;
     *           - an empty `MaskTraitsWrapper<>` or `MaskValuesWrapper<>` to use `Flags::UiSelectorMetadata::displayedMaskPresets`
     *             when available;
     *           - an empty tuple when no default preset mask metadata is available. */
    template<class Flags, class MaskTraitContainer>
    struct _MaskTraitNormalizer;

    /*! @brief Normalizes explicitly provided preset mask traits.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam MaskTraits Explicit trait types describing displayed preset mask entries.
     *  @details Each provided trait type is wrapped into `_MaskTraitWrapper<Trait>`. The trait is expected to expose a compile-time
     *           `value` member and may optionally provide additional display metadata. */
    template<class Flags, class... MaskTraits> requires(sizeof...(MaskTraits) > 0)
    struct _MaskTraitNormalizer<Flags, TemplateArgs::MaskTraitsWrapper<MaskTraits...>> {
        using type = std::tuple<_MaskTraitWrapper<MaskTraits>...>;
    };

    /*! @brief Normalizes explicitly provided preset mask values.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam MaskValues Explicit compile-time preset mask values to display.
     *  @details Each raw preset mask value is wrapped into `_ValueWrapper<Value>` and then into `_MaskTraitWrapper<...>`, producing
     *           the same internal entry shape as explicit trait-based input. */
    template<class Flags, auto... MaskValues> requires(sizeof...(MaskValues) > 0)
    struct _MaskTraitNormalizer<Flags, TemplateArgs::MaskValuesWrapper<MaskValues...>> {
        using type = std::tuple<_MaskTraitWrapper<_ValueWrapper<MaskValues>>...>;
    };

    /*! @brief Normalizes empty preset mask input by using default selector metadata from `Flags`.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam EmptyContainer Empty preset mask container type.
     *  @details If as an Comtainer either `MaskTraitsWrapper<>` or `MaskValuesWrapper<>` are used - than preset mask entries are
     *           taken from `Flags::UiSelectorMetadata::displayedMaskPresets` when that metadata is available.
     *
     *           The metadata array is converted into a `MaskTraitsWrapper<_ValueWrapper<...>...>` and then normalized through the
     *           explicit-trait specialization. */
    template<class Flags, class EmptyContainer> requires(
        _flagsHasSelectorDisplayedMaskPresets &&
        draupnir::utils::is_one_of_v<EmptyContainer, TemplateArgs::MaskTraitsWrapper<>, TemplateArgs::MaskValuesWrapper<>>
    )
    struct _MaskTraitNormalizer<Flags, EmptyContainer> {
        using type = typename _MaskTraitNormalizer<
            Flags,
            draupnir::utils::variadic_template_from_static_array_t<
                Flags::UiSelectorMetadata::displayedMaskPresets,
                _ValueWrapper,
                TemplateArgs::MaskTraitsWrapper
                >
        >::type;
    };

    /*! @brief Normalizes empty preset mask input to an empty entry tuple.
     *  @tparam Flags Flags-like type used by the selector.
     *  @tparam EmptyContainer Empty preset mask container type.
     *  @details This cpecialization is used when no explicit preset mask entries are provided and `Flags` does not expose
     *           `UiSelectorMetadata::displayedMaskPresets`. In that case the selector is built without preset mask entries. */
    template<class Flags, class EmptyContainer> requires(
        !_flagsHasSelectorDisplayedMaskPresets &&
        draupnir::utils::is_one_of_v<EmptyContainer, TemplateArgs::MaskTraitsWrapper<>, TemplateArgs::MaskValuesWrapper<>>
    )
    struct _MaskTraitNormalizer<Flags, EmptyContainer> {
        using type = std::tuple<>;
    };

    /*! @brief Tuple type containing all generated selector entries.
     *  @details The tuple contains:
     *           - one `_FlagTraitWrapper<Trait>` entry for every displayed flag element;
     *           - zero or more `_MaskTraitWrapper<Value>` entries for every displayed mask element if any specified.
     *             provided.
     *
     *           This tuple is the compile-time table used by the selector to create, access, update, and synchronize
     *           all runtime UI elements. */
    using _ElementsTuple = typename draupnir::utils::tuple_like_concat_t<
        std::tuple,
        typename _FlagsTraitNormalizer<_Flags, _FlagTraitContainer>::type,
        typename _MaskTraitNormalizer<_Flags, _MaskTraitContainer>::type
    >;

    /*! @brief Type predicate used to detect preset mask selector entries. Defaults to `false` for all selector entry types.
     *         Specialized for `_MaskTraitWrapper<...>` below. Used together with tuple type helpers to detect whether the
     *         selector contains any displayed preset masks. */
    template<class T>
    struct _IsMaskEntry : std::false_type {};

    /*! @brief Marks `_MaskTraitWrapper<...>` entries as preset mask selector entries.
     *  @tparam Trait Trait type describing a displayed preset mask entry. */
    template<class Trait>
    struct _IsMaskEntry<_MaskTraitWrapper<Trait>> : std::true_type {};

    /*! @brief Indicates whether the selector has any displayed preset mask entries. Evaluates to `true` when `_ElementsTuple`
     *         contains at least one `_MaskTraitWrapper<...>` entry. Used to enable mask-specific API, lookup, and synchronization
     *         code only when preset masks are present. */
    static inline constexpr bool _hasAnyDisplayedMasks =
        draupnir::utils::tuple_contains_if_v<_IsMaskEntry, _ElementsTuple>;

public:
    /*! @brief Public alias for the current _Flags type. */
    using Flags = _Flags;

    /*! @brief Public alias for the individual flag type. */
    using FlagType = _FlagType;

    /*! @brief Checks whether the given individual flag is represented by this selector.
     *  @param flag Individual flag value to check.
     *  @return `true` when the selector has a displayed flag entry for `flag`. */
    static constexpr bool isFlagKnown(FlagType flag) {
        return _isFlagKnown<0>(flag);
    }

    /*! @brief Checks whether the given preset mask is represented by this selector.
     *  @param mask Preset mask value to check.
     *  @return `true` when the selector has a displayed preset mask entry for `mask`.
     * @note This method is available only when the selector contains at least one displayed preset mask. */
    static constexpr bool isMaskKnown(Flags mask) requires(_hasAnyDisplayedMasks) {
        return _isMaskKnown<0>(mask);
    }

    /*! @brief Returns the UI element representing the specified flag or preset mask. This is a runtime lookup counterpart
     *         of `getFlagElement<Key>()` and `getMaskElement<Key>()`.
     *  @param key Flag or preset mask value to search for.
     *  @return Pointer to the UI element associated with `key`.
     * @warning If the `key` provided does not correspond to any of the UI element - `Q_ASSERT` will happen in the debug build
     *          and `nullptr` will be returned in the release build. */
    template<class Self>
    [[nodiscard]] draupnir::utils::copy_const_from_t<Self, _UiElement>* getUiElement(this Self&& self, Flags key) {
        if constexpr (_hasAnyDisplayedMasks) {
            Q_ASSERT_X(isMaskKnown(key) || isFlagKnown(static_cast<FlagType>(key.value())), Q_FUNC_INFO,
                "Specified key is not known to this FlagsMaskSelectorBase.");
        } else {
            Q_ASSERT_X(isFlagKnown(static_cast<FlagType>(key.value())), Q_FUNC_INFO,
                "Specified key is not known to this FlagsMaskSelectorBase.");
        }
        return self.template _getUiElementImpl<Self, 0>(key);
    }

    template<class Trait, class Self>
    [[nodiscard]] draupnir::utils::copy_const_from_t<Self, _UiElement>* getUiElement(this Self&& self) {
        if constexpr (draupnir::utils::is_type_in_tuple_v<_FlagTraitWrapper<Trait>, _ElementsTuple>) {
            return std::get<_FlagTraitWrapper<Trait>>(self.m_uiElementsTuple).element;
        } else if constexpr (draupnir::utils::is_type_in_tuple_v<_MaskTraitWrapper<Trait>, _ElementsTuple>) {
            return std::get<_MaskTraitWrapper<Trait>>(self.m_uiElementsTuple).element;
        } else {
            static_assert(
                draupnir::utils::is_type_in_tuple_v<_FlagTraitWrapper<Trait>, _ElementsTuple> ||
                draupnir::utils::is_type_in_tuple_v<_MaskTraitWrapper<Trait>, _ElementsTuple>
            );
        }
    };

    /*! @brief Returns the UI element representing a specific individual flag.
     *  @tparam Key Individual value that must be known by this FlagsMaskSelectorBase.
     *  @return Pointer to the UI element associated with `Key`.
     *  @details The element is retrieved by the unique compile-time wrapper type generated for `Key`.
     * @warning If `Key` is not part of the generated selector entries, this method fails at compile time. */
    template<auto Key, class Self>
    [[nodiscard]] draupnir::utils::copy_const_from_t<Self, _UiElement>* getFlagElement(this Self&& self) {
        static_assert(isFlagKnown(Key), "Specified key is not known to this FlagsMaskSelectorBase.");
        return self.template _getUiElementImpl<Self, Key, 0>();
    }

    /*! @brief Returns the UI element representing a specific preset mask.
     *  @tparam Key Preset mask value that must be present in `_Flags::displayedMasks`.
     *  @return Pointer to the UI element associated with `Key`.
     *  @details The element is retrieved by the unique compile-time wrapper type generated for `Key`. If `Key` is not part
     *           of the generated preset mask entries, this method fails at compile time.
     * @note This method is available only when the selector contains at least one displayed preset mask.
     * @warning If `Key` is not part of the generated selector preset mask entries, this method fails at compile time. */
    template<auto Key, class Self> requires(_hasAnyDisplayedMasks)
    [[nodiscard]] draupnir::utils::copy_const_from_t<Self, _UiElement>* getMaskElement(this Self&& self) {
        static_assert(isMaskKnown(Key), "Specified key is not known to this FlagsMaskSelectorBase.");
        return self.template _getUiElementImpl<Self, Key, 0>();
    }

    /*! @brief Checks whether a specific individual flag is currently selected.
     *  @param flag Individual enum flag to check.
     *  @return `true` if `flag` is currently set in the displayed flags mask, `false` otherwise.
     * @warning If the `flag` provided does not correspond to any of the flag type known - `Q_ASSERT` will happen in the debug
     *          build. */
    [[nodiscard]] bool isFlagSelected(FlagType flag) const {
        Q_ASSERT_X(isFlagKnown(flag), Q_FUNC_INFO,
                   "Specified flag is not known to this FlagsMaskSelectorBase.");
        return m_displayedFlags.test_flag(flag);
    }

    /*! @brief Returns the currently displayed flags mask. The returned mask reflects the current internal selector state,
     *         including both programmatic changes and user-triggered changes.
     *  @return Current selected flags mask represented by this selector. */
    [[nodiscard]] Flags selectedMask() const { return m_displayedFlags; }

    /*! @brief Programmatically replaces the currently displayed flags mask.
     *  @param mask New mask to display.
     *  @details Updates the internal selected flags mask and synchronizes all generated UI elements with the new state. This
     *           method represents a programmatic state change and does not emit user-interaction signals from the derived
     *           implementation.
     * @todo User-Friendliness: As in theory `Flags` mask can contain random data so some reasonable Q_ASSERT_X / Q_ASSERT here
     *       would be nice to have. Things to cover: argument is not fitting to any possible combination of known keys. */
    void displayMaskSelected(Flags mask) {
        m_displayedFlags = mask;
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
     * @warning If the `flag` provided does not correspond to any of the flag type known - `Q_ASSERT` will happen in the debug
     *          build. Or `nullptr` will be dereferenced internally in the release build. */
    void displayFlagSelected(FlagType flag, bool isSelected) {
        Q_ASSERT_X(isFlagKnown(flag), Q_FUNC_INFO,
                   "Specified flag is not known to this FlagsMaskSelectorBase.");
        m_displayedFlags.set_flag(flag, isSelected);
        getUiElement(flag)->setChecked(isSelected);

        if constexpr (_hasAnyDisplayedMasks)
            _updateMaskUiElements();
    };

protected:
    /*! @brief Constructs the selector and creates all generated UI elements.
     *  @param initialState Initial flags mask to be displayed by the selector.
     *  @details The constructor creates one checkable UI element for each compile-time selector entry stored in `m_uiElementsTuple`.
     *
     *           Generated entries include:
     *           - individual flag entries from `MaybeFlagTraits`;
     *           - optional preset mask entries from `Flags::displayedMasks`.
     *
     *           Each created UI element is initialized with the display string provided by `_Flags::toDisplayString()` and
     *           connected to the corresponding user-interaction handler.
     *
     *           If `initialState` is not empty, all generated UI elements are synchronized with it after creation.
     *
     * @note Programmatic initialization does not emit derived-class user interaction signals. Only user-triggered UI element
     *       activation is expected to do that. */
    FlagsMaskSelectorBase(_Flags initialState = 0) :
        m_displayedFlags{initialState}
    {
        // We can not validate _Implementation within a class scope, as at that moment it will be incomplete type, thus we can not
        // properly access its internals.
        static_assert(draupnir::utils::is_template_base_of_v<FlagsMaskSelectorBase, _Implementation>,
                      "_Implementation must be derived from FlagsMaskSelectorBase");

        std::apply([this](auto&... element){
            ((
                 element.element = Ui::CheckableUiElementHelper<_UiElement>::createConnectedUiElement(
                     _getDisplayString<std::remove_cvref_t<decltype(element)>>(),
                     [this](bool checked) {
                         _onUserInteraction<std::remove_cvref_t<decltype(element)>>(checked);
                     }
                     )), ... );
        }, m_uiElementsTuple);

        if (initialState.any())
            _updateUiElements();
    };

    /*! @brief Updates text labels of all generated UI elements.
     *  @details This method is intended for Qt translation/retranslation flows where already-created UI elements need to update
     *           their visible text without changing the current selected flags mask. */
    void retranslateUiElements() {
        std::apply([this](auto&... element){
            ((
                 element.element->setText(FlagsMaskSelectorBase::_getDisplayString<std::remove_cvref_t<decltype(element)>>())
                 ), ... );
        }, m_uiElementsTuple);
    };

private:
    friend class FlagsMaskSelectorBaseTest;
    _Flags m_displayedFlags;
    _ElementsTuple m_uiElementsTuple;

    /*! @brief Recursively checks whether a flag entry exists in the selector entry tuple.
     *  @tparam Index Current tuple index being inspected.
     *  @param flag Individual flag value to check.
     *  @return `true` when a matching `_FlagTraitWrapper<...>` entry is found. */
    template<std::size_t Index = 0>
    static inline constexpr bool _isFlagKnown(_FlagType flag) {
        using _TupleElement = std::tuple_element_t<Index, _ElementsTuple>;

        // If we have _FlagTraitWrapper - than it makes sense to compare.
        if constexpr (draupnir::utils::is_instantiation_of_v<_TupleElement, _FlagTraitWrapper>) {
            if (_TupleElement::Type::value() == flag) {
                return true;
            };
        }

        if constexpr (Index + 1 < std::tuple_size_v<_ElementsTuple>) {
            return _isFlagKnown<Index+1>(flag);
        } else {
            return false;
        }
    }

    /*! @brief Recursively checks whether a preset mask entry exists in the selector entry tuple.
     *  @tparam Index Current tuple index being inspected.
     *  @param mask Preset mask value to check.
     *  @return `true` when a matching `_MaskTraitWrapper<...>` entry is found.
     * @note This method is available only when the selector contains at least one displayed preset mask. */
    template<std::size_t Index = 0>
    static inline constexpr bool _isMaskKnown(_Flags flag) requires(_hasAnyDisplayedMasks) {
        using _TupleElement = std::tuple_element_t<Index, _ElementsTuple>;

        // If we have _MaskTraitWrapper - than it makes sense to compare.
        if constexpr (draupnir::utils::is_instantiation_of_v<_TupleElement, _MaskTraitWrapper>) {
            if (_TupleElement::Type::value() == flag)
                return true;
        }

        if constexpr (Index + 1 < std::tuple_size_v<_ElementsTuple>) {
            return _isMaskKnown<Index+1>(flag);
        } else {
            return false;
        }
    }

    /*! @brief Recursively searches generated selector entries for a matching value.
     *  @tparam Index Current tuple index used by the recursive lookup.
     *  @param key Normalized flag or preset mask value to search for.
     *  @return Pointer to the matching UI element, or `nullptr` if no matching entry exists.
     *  @details This method walks through `m_uiElementsTuple` at compile time using recursive template instantiation. Each entry
     *           value is normalized to `_Flags` before comparison, which allows the same implementation to handle both individual
     *           flag entries and preset mask entries. */
    template<class Self, std::size_t Index = 0>
    [[nodiscard]] inline draupnir::utils::copy_const_from_t<Self, _UiElement>* _getUiElementImpl(this Self&& self, _Flags key) {
        using TupleElement = std::tuple_element_t<Index, _ElementsTuple>;

        if (_Flags{TupleElement::Type::value()} == key)
            return std::get<Index>(self.m_uiElementsTuple).element;

        if constexpr (Index + 1 < std::tuple_size_v<_ElementsTuple>) {
            return self.template _getUiElementImpl<Self, Index+1>(key);
        } else {
            return nullptr;
        }
    }

    /*! @brief Returns the UI element associated with a compile-time selector entry key.
     *  @tparam Self Actual cv/ref-qualified object type deduced from the explicit object parameter.
     *  @tparam Key Compile-time flag or preset mask value used as the lookup key.
     *  @tparam Index Current tuple index inspected by the recursive lookup.
     *  @return Pointer to the UI element associated with `Key`.
     *  @details Recursively scans the canonical selector entry tuple and returns the UI element stored in the entry whose
     *           trait value matches `Key`.
     *
     *           The returned pointer preserves constness of the selector object:
     *           - non-const selector returns `_UiElement*`;
     *           - const selector returns `const _UiElement*`.
     *
     *           If no entry with the requested key exists, compilation fails with a diagnostic. */
    template<class Self, auto Key, std::size_t Index = 0>
    [[nodiscard]] inline draupnir::utils::copy_const_from_t<Self, _UiElement>* _getUiElementImpl(this Self&& self) {
        using TupleElement = std::tuple_element_t<Index, _ElementsTuple>;

        if constexpr (TupleElement::Type::value() == Key) {
            return std::get<Index>(self.m_uiElementsTuple).element;
        } else if constexpr (Index + 1 < std::tuple_size_v<_ElementsTuple>) {
            return self.template _getUiElementImpl<Self, Key, Index + 1>();
        } else {
            static_assert(Index + 1 < std::tuple_size_v<_ElementsTuple>, "Requested UI element key is not known by this selector");
            return nullptr;
        }
    }

    /*! @brief Resolves display text for a selector entry.
     *  @tparam Wrapper Internal selector entry wrapper type.
     *  @return Display string used for the corresponding UI element.
     *  @details The display string is resolved in the following order:
     *           - from the entry trait itself, when it provides static `displayName()`;
     *           - from `_Flags::toDisplayName(value)`, when `_Flags` supports converting the entry value to a display
     *             string.
     *
     *             This allows explicit traits to override display text while keeping raw value-based entries lightweight. */
    template<class Wrapper>
    static QString _getDisplayString() {
        using Trait = typename Wrapper::Type;

        if constexpr (HasDisplayName<Trait>) {
            return Trait::displayName();
        } else if constexpr (HasValueToDisplayName<_Flags, _Flags>) {
            return _Flags::toDisplayName(_Flags{Trait::value()});
        } else {
            static_assert(HasDisplayName<Trait> || HasValueToDisplayName<_Flags, _Flags>,
                "Can not resolve display string for selector entry." );
            return {};
        }
    }

    /*! @brief Handles user interaction with an individual flag UI element.
     *  @param value Individual flag represented by the activated UI element.
     *  @param checked New checked state of the UI element.
     *  @details Updates the internal displayed flags mask by setting or clearing `value` according to `checked`.
     *
     *           After the internal mask is updated, preset mask UI elements are synchronized with the new state.
     *           Individual flag UI elements are not updated here because the interacted element already has the
     *           correct checked state.
     *
     *           Finally, the derived implementation method `_Implementation::flagSelectionChanged(value, checked)`
     *           is invoked. This method should be a Qt signal.
     * @note This handler is intended for user-triggered changes only. */
    template<class Wrapper> requires(draupnir::utils::is_instantiation_of_v<Wrapper, _FlagTraitWrapper>)
    void _onUserInteraction(bool checked) {
        const _FlagType value = Wrapper::Type::value();
        m_displayedFlags.set_flag(value, checked);

        if constexpr (_hasAnyDisplayedMasks)
            _updateMaskUiElements();

        if constexpr (_hasFlagSelectionChanged_v<_Implementation>)
            static_cast<_Implementation*>(this)->flagSelectionChanged(value, checked);
        if constexpr (_hasMaskChanged_v<_Implementation>)
            static_cast<_Implementation*>(this)->maskChanged(m_displayedFlags);
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
     * @note Preset mask UI elements behave as group toggles: checking one adds its bits, unchecking one removes its bits.
     * @note This method is available only when the selector contains at least one displayed preset mask. */
    template<class Wrapper> requires(draupnir::utils::is_instantiation_of_v<Wrapper, _MaskTraitWrapper>)
    void _onUserInteraction(bool checked) {
        const _Flags value = Wrapper::Type::value();
        m_displayedFlags = checked ?
            m_displayedFlags | value :
            m_displayedFlags & ~value ;

        std::apply([this](auto&... element){
            ((
                 _updateUiElementsOnMaskChange<std::remove_cvref_t<decltype(element)>>()
                 ), ... );
        }, m_uiElementsTuple);

        if constexpr (_hasMaskChanged_v<_Implementation>)
            static_cast<_Implementation*>(this)->maskChanged(m_displayedFlags);
    }

///@name Flag UI synchronization helpers
///@{
    /*! @brief Ignores preset mask UI elements during individual flag synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskValueWrapper<Value>` entries.
     *  @details `_updateFlagsUiElements()` iterates over all generated selector entries. Preset mask entries are intentionally
     *           ignored by this overload, because this synchronization pass updates only individual flag UI elements.
     * @note This overload is available only when the selector contains at least one displayed preset mask. */
    template<class TupleElement> requires (
        _hasAnyDisplayedMasks && draupnir::utils::is_instantiation_of_v<TupleElement, _MaskTraitWrapper>
    )
    void _updateFlagUiElement() {}

    /*! @brief Updates an individual flag UI element according to the current selected flags mask.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagValueWrapper<Value>` entries.
     *  @details The UI element is checked when the current displayed flags mask contains the individual enum flag represented
     *           by the entry. */
    template<class TupleElement> requires (
        draupnir::utils::is_instantiation_of_v<TupleElement, _FlagTraitWrapper>
    )
    void _updateFlagUiElement() {
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(m_displayedFlags.test_flag(TupleElement::Type::value()));
    }

    /*! @brief Updates checked state of all individual flag UI elements.
     *  @details Iterates over all generated selector entries stored in `m_uiElementsTuple` and updates only entries representing
     *           individual flags. Preset mask entries are ignored by the corresponding constrained overload of `_updateFlagUiElement()`. */
    void _updateFlagsUiElements() {
        std::apply([this](auto&... element){
            ((
                 this->template _updateFlagUiElement<std::remove_cvref_t<decltype(element)>>()
                 ), ... );
        }, m_uiElementsTuple);
    }
///@}

///@name Mask UI synchronization helpers
///@{
    /*! @brief Updates a preset mask UI element according to the current selected flags mask.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskTraitWrapper<MaskTrait>` entries.
     *  @details A preset mask UI element is checked when the current displayed flags mask is a superset of the mask represented
     *           by the entry.
     *
     *           For example:
     *           @code
     *           Element mask:  0b0011 | 0b1001
     *           Current mask:  0b0111 | 0b0111
     *           Checked:         true |  false
     *           @endcode
     * @note This overload is available only when the selector contains at least one displayed preset mask. */
    template<class TupleElement> requires (
        _hasAnyDisplayedMasks && draupnir::utils::is_instantiation_of_v<TupleElement, _MaskTraitWrapper>
    )
    void _updateMaskUiElement() {
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(m_displayedFlags.is_superset(TupleElement::Type::value()));
    }

    /*! @brief Ignores individual flag UI elements during preset mask synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagTraitWrapper<FlagTrait>` entries.
     *  @details `_updateMaskUiElements()` iterates over all generated selector entries. Individual flag entries are intentionally ignored
     *           by this overload, because this synchronization pass updates only preset mask UI elements.
     * @note This overload is available only when the selector contains at least one displayed preset mask. */
    template<class TupleElement> requires (
        _hasAnyDisplayedMasks && draupnir::utils::is_instantiation_of_v<TupleElement, _FlagTraitWrapper>
    )
    void _updateMaskUiElement() {}

    /*! @brief Updates checked state of all preset mask UI elements.
     *  @details Iterates over all generated selector entries stored in `m_uiElementsTuple` and updates only entries representing preset
     *           masks. Individual flag entries are ignored by the corresponding constrained overload of `_updateMaskUiElement()`.
     * @note This method is available only when the selector contains at least one displayed preset mask. */
    void _updateMaskUiElements() requires (_hasAnyDisplayedMasks) {
        std::apply([this](auto&... element){
            ((
                 this->template _updateMaskUiElement<std::remove_cvref_t<decltype(element)>>()
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
     *       represent individual flags.
     * @note This overload is available only when the selector contains at least one displayed preset mask. */
    template<class TupleElement> requires (
        _hasAnyDisplayedMasks && draupnir::utils::is_instantiation_of_v<TupleElement, _MaskTraitWrapper>
    )
    void _updateUiElementsOnMaskChange() {
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(m_displayedFlags.is_superset(TupleElement::Type::value()));
    }

    /*! @brief Synchronizes an individual flag UI element after a preset mask state change.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagValueWrapper<Value>` entries.
     *  @details Compares the current checked state of the individual flag UI element with the state implied by the resulting
     *           displayed flags mask.
     *
     *           If the checked state changed, the UI element is updated and the derived implementation is notified through:
     *           `_Implementation::flagSelectionChanged(flag, newState)`. This is used when a preset mask UI element is toggled,
     *           because one preset mask interaction may change multiple individual flags. */
    template<class TupleElement> requires (
        draupnir::utils::is_instantiation_of_v<TupleElement, _FlagTraitWrapper>
    )
    void _updateUiElementsOnMaskChange() {
        const bool newState = m_displayedFlags.test_flag(TupleElement::Type::value());
        const bool currentState = std::get<TupleElement>(m_uiElementsTuple).element->isChecked();
        std::get<TupleElement>(m_uiElementsTuple).element->setChecked(newState);

        if constexpr (_hasFlagSelectionChanged_v<_Implementation>) {
            if (newState != currentState)
                static_cast<_Implementation*>(this)->flagSelectionChanged(TupleElement::Type::value(), newState);
        }
    }
///@}

///@name Full UI synchronization helpers
///@{
    /*! @brief Updates a preset mask UI element during full UI synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_MaskValueWrapper<Value>` entries.
     *  @details Delegates to `_updateMaskUiElement()` so that preset mask entries are synchronized according to the
     *           current displayed flags mask.
     * @note This overload is available only when the selector contains at least one displayed preset mask. */
    template<class TupleElement> requires (
        _hasAnyDisplayedMasks && draupnir::utils::is_instantiation_of_v<TupleElement, _MaskTraitWrapper>
    )
    void _updateUiElement() { _updateMaskUiElement<TupleElement>(); }

    /*! @brief Updates an individual flag UI element during full UI synchronization.
     *  @tparam TupleElement Selector entry type. This overload is enabled only for `_FlagValueWrapper<Value>` entries.
     *  @details Delegates to `_updateFlagUiElement()` so that individual flag entries are synchronized according to the
     *           current displayed flags mask. */
    template<class TupleElement> requires (
        draupnir::utils::is_instantiation_of_v<TupleElement, _FlagTraitWrapper>
    )
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

};

}; // namespace Draupnir::Ui

#endif // FLAGSMASKSELECTORBASE_H
