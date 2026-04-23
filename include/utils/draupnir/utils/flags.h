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

#ifndef FLAGS_H
#define FLAGS_H

#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/integer_wrapper.h"
#include "draupnir/utils/template_detectors.h"

namespace draupnir::utils
{

/*! @class flags_base draupnir/utils/flags.h
 *  @ingroup Utils
 *  @brief CRTP base class providing common bitmask operations for flags-like types.
 *  @tparam Int Integer-like storage type used to store the mask.
 *  @tparam Derived Final derived flags type implementing the public policy.
 *
 *  @details `flags_base<Int, Derived>` implements storage and common operations shared by @ref draupnir::utils::flags and
 *           @ref draupnir::utils::enum_flags:
 *           - explicit access to stored value via @ref value;
 *           - boolean/query helpers such as @ref any, @ref none, and `operator bool`;
 *           - equality and three-way comparison with compatible built-in integer values;
 *           - bitwise operators (`&`, `|`, `^`, `~`) and their assignment forms.
 *
 *           This base class intentionally knows only about the integer-like storage model. Policy decisions such as:
 *           - whether enums are accepted;
 *           - whether wrapper types are accepted;
 *           - which exact enum or wrapper types are allowed
 *           are implemented by derived classes.
 *
 * @note This class is not intended to be used directly. Use @ref draupnir::utils::flags or @ref draupnir::utils::enum_flags
 *        instead. */

template<integer_like_concept Int, class Derived>
class flags_base
{
protected:
    using _normalizer = normalizer<Int>;

    static inline constexpr Int _zero = Int{};

public:
    /*! @brief Alias to access integer type used by this flags object instantiation. */
    using integer = Int;

///@name Constructors.
///@{
    /*! @brief Constructs an empty flags object.
     *  @details Initializes the underlying mask to `0`. */
    constexpr flags_base() noexcept = default;

    /*! @brief Constructs flags from an integer-like object that can be normalized into `Int`.
     *  @param mask Initial bitmask value. */
    template<integer_like_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr explicit flags_base(const Other& mask) noexcept: m_mask{_normalizer::normalize(mask)} {}
///@}

///@name Helpers.
///@{
    /*! @brief Explicit conversion to `bool`.
     *  @return `true` if `m_mask != 0`, and `false` if `m_mask == 0`. */
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return bool(m_mask); }

    /*! @brief Returns the underlying mask value explicitly.
     *  @return Stored mask value */
    [[nodiscard]] constexpr Int value() const noexcept { return m_mask; }

    /*! @brief Checks whether any bit is set.
     *  @return `true` if mask is non-zero, `false` otherwise. */
    [[nodiscard]] constexpr bool any() const noexcept { return m_mask != _zero; }

    /*! @brief Checks whether no bits are set.
     *  @return `true` if mask is zero, `false` otherwise. */
    [[nodiscard]] constexpr bool none() const noexcept { return m_mask == _zero; }
///@}

///@name Comparison operators.
///@{
    /*! @brief Compares two flags objects of the same derived type for equality. */
    [[nodiscard]] friend constexpr bool operator==(const Derived& lhs, const Derived& rhs) noexcept { return lhs.m_mask == rhs.m_mask; }

    /*! @brief Compares a built-in integer mask with flags object for equality. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr bool operator==(const Other& lhs, const Derived& rhs) noexcept { return _normalizer::normalize(lhs) == rhs.m_mask; }

    /*! @brief Compares flags object with a built-in integer mask for equality. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr bool operator==(const Derived& lhs, const Other& rhs) noexcept { return lhs.m_mask == _normalizer::normalize(rhs); }

    /*! @brief Performs three-way comparison between two flags objects of the same derived type. */
    [[nodiscard]] friend constexpr auto operator<=>(const Derived& lhs, const Derived& rhs) noexcept { return lhs.m_mask <=> rhs.m_mask; }

    /*! @brief Performs three-way comparison between a built-in integer mask and flags object. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr auto operator<=>(const Other& lhs, const Derived& rhs) noexcept { return _normalizer::normalize(lhs) <=> rhs.m_mask; }

    /*! @brief Performs three-way comparison between flags object and a built-in integer mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr auto operator<=>(const Derived& lhs, const Other& rhs) noexcept { return lhs.m_mask <=> _normalizer::normalize(rhs); }
///@}

///@name Bitwise assignment operators.
///@{
    /*! @brief ANDs the stored mask with a raw mask.
     *  @param mask Mask to AND with.
     *  @return `*this`. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator&=(Other mask) noexcept { m_mask &= _normalizer::normalize(mask); return _derived(); }

    /*! @brief ANDs the stored mask with another flags object.
     *  @param other Other flags.
     *  @return `*this`. */
    constexpr Derived& operator&=(const Derived& other) noexcept { m_mask &= other.m_mask; return _derived(); }

    /*! @brief ORs the stored mask with a raw mask.
     *  @param mask Mask to OR with.
     *  @return `*this`. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator|=(Other mask) noexcept { m_mask |= _normalizer::normalize(mask); return _derived(); }

    /*! @brief ORs the stored mask with another flags object.
     *  @param other Other flags.
     *  @return `*this`. */
    constexpr Derived& operator|=(const Derived& other) noexcept { m_mask |= other.m_mask; return _derived(); }

    /*! @brief XORs the stored mask with a raw mask.
     *  @param mask Mask to XOR with.
     *  @return `*this`. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator^=(Other mask) noexcept { m_mask ^= _normalizer::normalize(mask); return _derived(); }

    /*! @brief XORs the stored mask with another flags object.
     *  @param other Other flags.
     *  @return `*this`. */
    constexpr Derived& operator^=(const Derived& other) noexcept { m_mask ^= other.m_mask; return _derived(); }
///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns result of bitwise AND with a raw integer-like mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator&(Other lhs, Derived rhs) noexcept { return Derived{_normalizer::normalize(lhs) & rhs.m_mask}; }

    /*! @brief Returns result of bitwise AND with a raw integer-like mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator&(Derived lhs, Other rhs) noexcept { return Derived{lhs.m_mask & _normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise AND with another flags object of the same derived type. */
    [[nodiscard]] friend constexpr Derived operator&(Derived lhs, Derived rhs) noexcept { return Derived{lhs.m_mask & rhs.m_mask}; }

    /*! @brief Returns result of bitwise OR with a raw integer-like mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator|(Other lhs, Derived rhs) noexcept { return Derived{_normalizer::normalize(lhs) | rhs.m_mask}; }

    /*! @brief Returns result of bitwise OR with a raw integer-like mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator|(Derived lhs, Other rhs) noexcept { return Derived{lhs.m_mask | _normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise OR with another flags object of the same derived type. */
    [[nodiscard]] friend constexpr Derived operator|(Derived lhs, Derived rhs) noexcept { return Derived{lhs.m_mask | rhs.m_mask}; }

    /*! @brief Returns result of bitwise XOR with a raw integer-like mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator^(Other lhs, Derived rhs) noexcept { return Derived{_normalizer::normalize(lhs) ^ rhs.m_mask}; }

    /*! @brief Returns result of bitwise XOR with a raw integer-like mask. */
    template<integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator^(Derived lhs, Other rhs) noexcept { return Derived{lhs.m_mask ^ _normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise XOR with another flags object of the same derived type. */
    [[nodiscard]] friend constexpr Derived operator^(Derived lhs, Derived rhs) noexcept { return Derived{lhs.m_mask ^ rhs.m_mask}; }
///@}

    /*! @brief Returns bitwise negation of the stored value. */
    [[nodiscard]] constexpr Derived operator~() const noexcept { return Derived{~m_mask}; }

    /*! @brief Checks whether the stored mask is zero.
     *  @return `true` if the stored mask is zero, `false` otherwise.
     */
    constexpr bool operator!() const noexcept { return !m_mask; }

protected:
    constexpr Derived& _derived() noexcept { return static_cast<Derived&>(*this); }
    constexpr const Derived& _derived() const noexcept { return static_cast<const Derived&>(*this); }

    Int m_mask{};
};

/*! @class flags draupnir/utils/flags.h
 *  @ingroup Utils
 *  @brief Lightweight bitmask wrapper over an integer-like storage type.
 *  @tparam Int Integer-like storage type for the bitmask. Must satisfy @ref draupnir::utils::integer_like_concept.
 *
 *  @details `flags<Int>` is a small value-type that stores a bitmask and provides a `QFlags`-like interface. It can be
 *           constructed from:
 *           - built-in integer values that can be normalized into `Int`;
 *           - enum values that can be normalized into `Int`;
 *           - matching integer-wrapper type when `Int` itself is an integer wrapper.
 *
 *           It supports:
 *           - equality and three-way comparison;
 *           - bitwise operators (`&`, `|`, `^`, `~`) and their assignment forms;
 *           - helper methods @ref test_flag and @ref set_flag.
 *
 *           The class does not prescribe any particular meaning for bits. It is intended to be used directly as a generic
 *           flags container.
 *
 * @note `test_flag(0)` follows the QFlags-like rule: it returns `true` only when the stored mask is also zero. */

template<integer_like_concept Int>
class flags : public flags_base<Int,flags<Int>>
{
    using _base = flags_base<Int,flags<Int>>;

public:
    /*! @brief Integer type used by this flags object. */
    using integer = flags_base<Int,flags<Int>>::integer;

///@name Constructors.
///@{
    /*! @brief Constructs zero-initialized flags object. */
    constexpr flags() noexcept : _base{_base::_zero} {}

    /*! @brief Constructs flags from a built-in integer or enum value that can be normalized into `Int`.
     *  @param value Initial mask value. */
    template<enum_or_integer_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    constexpr flags(Other value) noexcept: _base{_base::_normalizer::normalize(value)} {}

    /*! @brief Constructs flags from matching integer-wrapper type.
     *  @param value Initial mask value. */
    template<integer_wrapper_concept OtherWrapper> requires(std::same_as<Int, OtherWrapper>)
    constexpr flags(OtherWrapper value) noexcept: _base{value.value()} {}

    /*! @brief Rejects construction from non-matching integer-wrapper type. */
    template<integer_wrapper_concept OtherWrapper> requires(!std::same_as<Int, OtherWrapper>)
    constexpr flags(OtherWrapper value) = delete;
///@}

///@name Helpers.
///@{
    /*! @brief Tests whether all bits from `flag` are set in the current mask.
     *  @param flag Bit pattern to test.
     *  @return `true` if all bits from `flag` are set.
     * @note Special case: if `flag == 0`, this returns `true` only if the stored mask is also zero. */
    template<enum_or_integer_concept Value>
    constexpr bool test_flag(Value flag) const noexcept requires(_base::_normalizer::template can_be_normalized_v<Value>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return (_base::m_mask & normalizedFlag) == normalizedFlag && (normalizedFlag != _base::_zero || _base::m_mask == normalizedFlag );
    }

    /*! @brief Tests whether all bits from `flag` are set in the current mask.
     *  @param flag Bit pattern to test.
     *  @return `true` if all bits from `flag` are set.
     * @note This overload is enabled only for matching integer-wrapper type when `Int` is itself a wrapper. */
    template<integer_wrapper_concept Value>
    constexpr bool test_flag(Value flag) const noexcept requires(std::same_as<Value,Int>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return (_base::m_mask & normalizedFlag) == normalizedFlag && (normalizedFlag != _base::_zero || _base::m_mask == normalizedFlag );
    }

    /*! @brief Sets or clears bits from `flag`.
     *  @param flag Bit pattern to modify.
     *  @param on If `true`, sets bits (`OR`). If `false`, clears bits (`AND` with `~flag`).
     *  @return `*this`. */
    template<enum_or_integer_concept Value>
    constexpr flags& set_flag(Value flag, bool on = true) noexcept requires(_base::_normalizer::template can_be_normalized_v<Value>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return on ? (*this |= normalizedFlag) : (*this &= ~normalizedFlag);
    }

    /*! @brief Sets or clears bits from `flag`.
     *  @param flag Bit pattern to modify.
     *  @param on If `true`, sets bits (`OR`). If `false`, clears bits (`AND` with `~flag`).
     *  @return `*this`.
     * @note This overload is enabled only for matching integer-wrapper type when `Int` is itself a wrapper. */
    template<integer_wrapper_concept Value>
    constexpr flags& set_flag(Value flag, bool on = true) noexcept requires(std::same_as<Value,Int>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return on ? (*this |= normalizedFlag) : (*this &= ~normalizedFlag);
    }
///@}

///@name Comparison operators.
///@{
    /*! @brief Compares matching integer-wrapper type with flags object for equality. */
    template<integer_wrapper_concept Other> requires(std::same_as<Int, Other>)
    [[nodiscard]] friend constexpr bool operator==(const Other& lhs, const flags& rhs) noexcept { return _base::_normalizer::normalize(lhs) == rhs.m_mask; }

    /*! @brief Compares flags object with matching integer-wrapper type for equality. */
    template<integer_wrapper_concept Other> requires(std::same_as<Int, Other>)
    [[nodiscard]] friend constexpr bool operator==(const flags& lhs, const Other& rhs) noexcept { return lhs.m_mask == _base::_normalizer::normalize(rhs); }

    /*! @brief Performs three-way comparison between matching integer-wrapper type and flags object. */
    template<integer_wrapper_concept Other> requires(std::same_as<Int, Other>)
    [[nodiscard]] friend constexpr auto operator<=>(const Other& lhs, const flags& rhs) noexcept { return _base::_normalizer::normalize(lhs) <=> rhs.m_mask; }

    /*! @brief Performs three-way comparison between flags object and matching integer-wrapper type. */
    template<integer_wrapper_concept Other> requires(std::same_as<Int, Other>)
    [[nodiscard]] friend constexpr auto operator<=>(const flags& lhs, const Other& rhs) noexcept { return lhs.m_mask <=> _base::_normalizer::normalize(rhs); }
///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns result of bitwise AND with a raw enum mask. */
    template<enum_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr flags operator&(Other lhs, flags rhs) noexcept { return flags{_base::_normalizer::normalize(lhs) & rhs.m_mask}; }

    /*! @brief Returns result of bitwise AND with a raw enum mask. */
    template<enum_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr flags operator&(flags lhs, Other rhs) noexcept { return flags{lhs.m_mask & _base::_normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise OR with a raw enum mask. */
    template<enum_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr flags operator|(Other lhs, flags rhs) noexcept { return flags{_base::_normalizer::normalize(lhs) | rhs.m_mask}; }

    /*! @brief Returns result of bitwise OR with a raw enum mask. */
    template<enum_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr flags operator|(flags lhs, Other rhs) noexcept { return flags{lhs.m_mask | _base::_normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise XOR with a raw enum mask. */
    template<enum_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr flags operator^(Other lhs, flags rhs) noexcept { return flags{_base::_normalizer::normalize(lhs) ^ rhs.m_mask}; }

    /*! @brief Returns result of bitwise XOR with a raw enum mask. */
    template<enum_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr flags operator^(flags lhs, Other rhs) noexcept { return flags{lhs.m_mask ^ _base::_normalizer::normalize(rhs)}; }
///@}
};

/*! @brief Concept satisfied by instantiations of @ref draupnir::utils::flags.
 *  @tparam Candidate Type to test. */

template<class Candidate>
concept flags_concept = draupnir::utils::is_instantiation_of_v<Candidate,flags>;

/*! @class enum_flags draupnir/utils/flags.h
 *  @ingroup Utils
 *  @brief Strongly-typed flags wrapper bound to a specific enum type.
 *  @tparam Enum Enum type whose underlying integral type is used as storage. Must satisfy @ref draupnir::utils::enum_concept.
 *
 *  @details `enum_flags<Enum>` stores a bitmask in `std::underlying_type_t<Enum>` and provides a flags interface with stricter
 *           type rules than @ref draupnir::utils::flags. It can be constructed from:
 *           - built-in integer values that can be normalized into the underlying integer type;
 *           - values of the matching enum type `Enum`.
 *
 *           It rejects:
 *           - non-matching enum types;
 *           - integer-wrapper types.
 *
 *           It supports:
 *           - equality and three-way comparison;
 *           - bitwise operators (`&`, `|`, `^`, `~`) and their assignment forms;
 *           - helper methods @ref test_flag and @ref set_flag.
 *
 * @note `test_flag(0)` follows the QFlags-like rule: it returns `true` only when the stored mask is also zero. */

template<enum_concept Enum>
class enum_flags : public flags_base<std::underlying_type_t<Enum>,enum_flags<Enum>>
{
    using _base = flags_base<std::underlying_type_t<Enum>,enum_flags<Enum>>;

public:
    /*! @brief Associated enum type. */
    using enum_type = Enum;

    /*! @brief Integer type used by this enum_flags object. */
    using integer = flags_base<std::underlying_type_t<Enum>,enum_flags<Enum>>::integer;

///@name Constructors.
///@{
    /*! @brief Constructs zero-initialized enum_flags object. */
    constexpr enum_flags() noexcept = default;

    /*! @brief Constructs enum_flags from a built-in integer value that can be normalized into the underlying storage type.
     *  @param value Initial mask value. */
    template<integer_concept Other> requires(_base::_normalizer::template can_be_normalized_v<Other>)
    constexpr enum_flags(Other value) noexcept: _base{_base::_normalizer::normalize(value)} {}

    /*! @brief Constructs enum_flags from matching enum value.
     *  @param value Initial mask value. */
    template<enum_concept OtherEnum> requires(std::same_as<Enum,OtherEnum>)
    constexpr enum_flags(OtherEnum value) noexcept: _base{_base::_normalizer::normalize(value)} {}

    /*! @brief Rejects construction from non-matching enum type. */
    template<enum_concept OtherEnum> requires(!std::same_as<Enum,OtherEnum>)
    constexpr enum_flags(OtherEnum value) noexcept = delete;
///@}

///@name Helpers.
///@{
    /*! @brief Tests whether all bits from `flag` are set in the current mask.
     *  @param flag Bit pattern to test.
     *  @return `true` if all bits from @p flag are set.
     * @note Special case: if `flag == 0`, this returns `true` only if the stored mask is also zero. */
    template<integer_concept Value>
    constexpr bool test_flag(Value flag) const noexcept requires(_base::_normalizer::template can_be_normalized_v<Value>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return (_base::m_mask & normalizedFlag) == normalizedFlag && (normalizedFlag != _base::_zero || _base::m_mask == normalizedFlag );
    }

    /*! @brief Tests whether all bits from `flag` are set in the current mask.
     *  @param flag Bit pattern to test.
     *  @return `true` if all bits from @p flag are set.
     * @note This overload is enabled only for the matching enum type. */
    template<enum_concept Value>
    constexpr bool test_flag(Value flag) const noexcept requires(std::same_as<Enum,Value>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return (_base::m_mask & normalizedFlag) == normalizedFlag && (normalizedFlag != _base::_zero || _base::m_mask == normalizedFlag );
    }

    /*! @brief Sets or clears bits from `flag`.
     *  @param flag Bit pattern to modify.
     *  @param on If `true`, sets bits (`OR`). If `false`, clears bits (`AND` with `~flag`).
     *  @return `*this`. */
    template<integer_concept Value>
    constexpr enum_flags& set_flag(Value flag, bool on = true) noexcept requires(_base::_normalizer::template can_be_normalized_v<Value>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return on ? (*this |= normalizedFlag) : (*this &= ~normalizedFlag);
    }

    /*! @brief Sets or clears bits from `flag`.
     *  @param flag Bit pattern to modify.
     *  @param on If `true`, sets bits (`OR`). If `false`, clears bits (`AND` with `~flag`).
     *  @return `*this`.
     * @note This overload is enabled only for the matching enum type. */
    template<enum_concept Value>
    constexpr enum_flags& set_flag(Value flag, bool on = true) noexcept requires(std::same_as<Value,Enum>) {
        const auto normalizedFlag = _base::_normalizer::normalize(flag);
        return on ? (*this |= normalizedFlag) : (*this &= ~normalizedFlag);
    }
///@}

///@name Comparison operators.
///@{
    /*! @brief Compares matching enum value with enum_flags object for equality. */
    template<enum_concept OtherEnum> requires(std::same_as<Enum,OtherEnum>)
    [[nodiscard]] friend constexpr bool operator==(const OtherEnum& lhs, const enum_flags& rhs) noexcept { return _base::_normalizer::normalize(lhs) == rhs.m_mask; }

    /*! @brief Compares enum_flags object with matching enum value for equality. */
    template<enum_concept OtherEnum> requires(std::same_as<Enum,OtherEnum>)
    [[nodiscard]] friend constexpr bool operator==(const enum_flags& lhs, const OtherEnum& rhs) noexcept { return lhs.m_mask == _base::_normalizer::normalize(rhs); }

    /*! @brief Performs three-way comparison between matching enum value and enum_flags object. */
    template<enum_concept OtherEnum> requires(std::same_as<Enum,OtherEnum>)
    [[nodiscard]] friend constexpr auto operator<=>(const OtherEnum& lhs, const enum_flags& rhs) noexcept { return _base::_normalizer::normalize(lhs) <=> rhs.m_mask; }

    /*! @brief Performs three-way comparison between enum_flags object and matching enum value. */
    template<enum_concept OtherEnum> requires(std::same_as<Enum,OtherEnum>)
    [[nodiscard]] friend constexpr auto operator<=>(const enum_flags& lhs, const OtherEnum& rhs) noexcept { return lhs.m_mask <=> _base::_normalizer::normalize(rhs); }
///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns result of bitwise AND with a matching enum mask. */
    template<enum_concept Other> requires(std::same_as<Enum,Other>)
    [[nodiscard]] friend constexpr enum_flags operator&(Other lhs, enum_flags rhs) noexcept { return enum_flags{_base::_normalizer::normalize(lhs) & rhs.m_mask}; }

    /*! @brief Returns result of bitwise AND with a matching enum mask. */
    template<enum_concept Other> requires(std::same_as<Enum,Other>)
    [[nodiscard]] friend constexpr enum_flags operator&(enum_flags lhs, Other rhs) noexcept { return enum_flags{lhs.m_mask & _base::_normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise OR with a matching enum mask. */
    template<enum_concept Other> requires(std::same_as<Enum,Other>)
    [[nodiscard]] friend constexpr enum_flags operator|(Other lhs, enum_flags rhs) noexcept { return enum_flags{_base::_normalizer::normalize(lhs) | rhs.m_mask}; }

    /*! @brief Returns result of bitwise OR with a matching enum mask. */
    template<enum_concept Other> requires(std::same_as<Enum,Other>)
    [[nodiscard]] friend constexpr enum_flags operator|(enum_flags lhs, Other rhs) noexcept { return enum_flags{lhs.m_mask | _base::_normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise XOR with a matching enum mask. */
    template<enum_concept Other> requires(std::same_as<Enum,Other>)
    [[nodiscard]] friend constexpr enum_flags operator^(Other lhs, enum_flags rhs) noexcept { return enum_flags{_base::_normalizer::normalize(lhs) ^ rhs.m_mask}; }

    /*! @brief Returns result of bitwise XOR with a matching enum mask. */
    template<enum_concept Other> requires(std::same_as<Enum,Other>)
    [[nodiscard]] friend constexpr enum_flags operator^(enum_flags lhs, Other rhs) noexcept { return enum_flags{lhs | _base::_normalizer::normalize(rhs)}; }
///@}
};

/*! @brief Concept satisfied by instantiations of @ref draupnir::utils::enum_flags.
 *  @tparam Candidate Type to test. */
template<class Candidate>
concept enum_flags_concept = draupnir::utils::is_instantiation_of_v<Candidate,enum_flags>;

/*! @brief Checks whether `Flags::test_flag(Value)` is a valid expression returning `bool`.
 *  @tparam Flags Flags-like type to test.
 *  @tparam Value Value type to pass into `test_flag`.
 *
 *  @details This concept is intended primarily for compile-time tests of the public API. It evaluates to `true` when the
 *           expression `std::declval<const Flags&>().test_flag(std::declval<Value>())` is well-formed and returns `bool`. */

template<class Flags, class Value>
concept test_flag_callable = requires(const Flags& flags, Value value) {
    { flags.test_flag(value) } -> std::same_as<bool>;
};

/*! @brief Checks whether `Flags::set_flag(Value, bool)` is a valid expression returning `Flags&`.
 *  @tparam Flags Flags-like type to test.
 *  @tparam Value Value type to pass into `set_flag`.
 *
 *  @details This concept is intended primarily for compile-time tests of the public API. It evaluates to `true` when the
 *           expression `std::declval<Flags&>().set_flag(std::declval<Value>(), true)` is well-formed and returns `Flags&`. */
template<class Flags, class Value>
concept set_flag_callable = requires(Flags& flags, Value value, bool on) {
    { flags.set_flag(value, on) } -> std::same_as<Flags&>;
};

}; // namespace draupnir::utils

#endif // FLAGS_H
