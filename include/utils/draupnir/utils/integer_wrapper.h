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

#ifndef INTEGER_WRAPPER_H
#define INTEGER_WRAPPER_H

#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/integer_normalization.h"

namespace draupnir::utils
{

/*! @class integer_wrapper draupnir/utils/integer_wrapper.h
 *  @ingroup Utils
 *  @brief CRTP-based type-safe wrapper over an integer-like type.
 *  @tparam Integer Underlying integer storage type. Must satisfy @ref draupnir::utils::integer_concept.
 *  @tparam Derived Final wrapper type inheriting from this template.
 *
 *  @details `integer_wrapper<Integer, Derived>` provides an integer-like API while preserving type safety between different
 *           wrapper types. Objects of the same `Derived` type can be compared and combined with each other, and can also
 *           interact with raw integer values convertible to `Integer`. Different wrapper types are intentionally not
 *           interoperable.
 *
 *           Usage:
 *           @code
 *           DEFINE_WRAPPED_INTEGER(UserId, int)
 *           DEFINE_WRAPPED_INTEGER(DeviceId, int)
 *
 *           UserId a{1};
 *           UserId b{2};
 *           DeviceId d{3};
 *
 *           a |= b;     // OK
 *           a |= 4;     // OK
 *           // a |= d;  // Error
 *           @endcode
 *
 * @note This wrapper intentionally does not provide implicit conversion to the underlying integer type.
 * @todo Extend integer-like API with arithmetic operators (`+`, `-`, `*`, `/`, `%`, unary `+`/`-`, and corresponding
 *       assignment operators). */

template<integer_concept Integer, class Derived>
class integer_wrapper
{
    /*! @brief Helper used to normalize raw integer and enum values into the underlying `Integer` type.
     *  @note Explicit normalization is required so that scoped enums (`enum class`) work correctly. */
    using _normalizer = normalizer<Integer>;

public:
    /*! @brief Alias for the underlying integer storage type. */
    using underlying_type = Integer;

    /*! @brief Constructs wrapper with zero value. */
    constexpr integer_wrapper() noexcept = default;

    /*! @brief Constructs wrapper from an value of type Other (enum, integer, scoped enum).
     *  @param value Initial value to store. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr integer_wrapper(Other value) noexcept:
        m_value{_normalizer::normalize(value)} {}

    /*! @brief Assigns a new integer or enum value to the wrapper.
     *  @tparam Other Source value type.
     *  @param value New value to store. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator=(Other value) noexcept { m_value = _normalizer::normalize(value); return static_cast<Derived&>(*this); };

    /*! @brief Explicit conversion to `bool`. Returns `true` if `m_value != 0`, and `false` if `m_value == 0`. */
    constexpr explicit operator bool() const noexcept { return m_value != 0; }

    /*! @brief Returns the stored underlying value.
     *  @return Current wrapped integer value. */
    [[nodiscard]] constexpr Integer value() const noexcept { return m_value; }

    /*! @brief Compares wrapper with an integer or enum value for equality.
     *  @tparam Other Right-hand side type.
     *  @param lhs Left-hand side wrapper.
     *  @param rhs Right-hand side integer or enum value.
     *  @return `true` if the stored value equals `rhs` converted to the underlying integer type, `false` otherwise. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    friend constexpr bool operator==(Derived lhs, Other rhs) noexcept { return lhs.m_value == _normalizer::normalize(rhs); }

    /*! @brief Compares an integer or enum value with wrapper for equality.
     *  @tparam Other Left-hand side type.
     *  @param lhs Left-hand side integer or enum value.
     *  @param rhs Right-hand side wrapper.
     *  @return `true` if `lhs` converted to the underlying integer type equals the stored value of `rhs`, `false` otherwise. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    friend constexpr bool operator==(Other lhs, Derived rhs) noexcept { return _normalizer::normalize(lhs) == rhs.m_value; }

    /*! @brief Compares two wrappers of the same derived type for equality.
     *  @param lhs Left-hand side wrapper.
     *  @param rhs Right-hand side wrapper.
     *  @return `true` if both wrappers store the same underlying value, `false` otherwise. */
    friend constexpr bool operator==(Derived lhs, Derived rhs) noexcept { return lhs.m_value == rhs.m_value; }

    /*! @brief Performs three-way comparison between wrapper and an integer or enum value.
     *  @tparam Other Right-hand side type.
     *  @param lhs Left-hand side wrapper.
     *  @param rhs Right-hand side integer or enum value.
     *  @return Result of comparing the stored value of `lhs` with `rhs` converted to the underlying integer type. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    friend constexpr auto operator<=>(Derived lhs, Other rhs) noexcept { return lhs.m_value <=> _normalizer::normalize(rhs); }

    /*! @brief Performs three-way comparison between an integer or enum value and wrapper.
     *  @tparam Other Left-hand side type.
     *  @param lhs Left-hand side integer or enum value.
     *  @param rhs Right-hand side wrapper.
     *  @return Result of comparing `lhs` converted to the underlying integer type with the stored value of `rhs`. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    friend constexpr auto operator<=>(Other lhs, Derived rhs) noexcept { return _normalizer::normalize(lhs) <=> rhs.m_value; }

    /*! @brief Performs three-way comparison between two wrappers of the same derived type.
     *  @param lhs Left-hand side wrapper.
     *  @param rhs Right-hand side wrapper.
     *  @return Result of comparing the stored underlying values of both wrappers. */
    friend constexpr auto operator<=>(Derived lhs, Derived rhs) noexcept { return lhs.m_value <=> rhs.m_value; }

///@name Bitwise assignment operators.
///@{
    /*! @brief Applies bitwise AND with an integer or enum mask.
     *  @tparam Other Mask type.
     *  @param mask Right-hand side mask.
     *  @return Reference to the derived wrapper object. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator&=(const Other& mask) noexcept { m_value &= _normalizer::normalize(mask); return static_cast<Derived&>(*this); };

    /*! @brief Applies bitwise AND with another wrapper of the same derived type.
     *  @param mask Right-hand side wrapper.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator&=(const Derived& mask) noexcept { m_value &= mask.m_value; return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise OR with an integer or enum mask.
     *  @tparam Other Mask type.
     *  @param mask Right-hand side mask.
     *  @return Reference to the derived wrapper object. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator|=(const Other& mask) noexcept { m_value |= _normalizer::normalize(mask); return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise OR with another wrapper of the same derived type.
     *  @param mask Right-hand side wrapper.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator|=(const Derived& mask) noexcept { m_value |= mask.m_value; return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise XOR with an integer or enum mask.
     *  @tparam Other Mask type.
     *  @param mask Right-hand side mask.
     *  @return Reference to the derived wrapper object. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    constexpr Derived& operator^=(const Other& mask) noexcept { m_value ^= _normalizer::normalize(mask); return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise XOR with another wrapper of the same derived type.
     *  @param mask Right-hand side wrapper.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator^=(const Derived& mask) noexcept { m_value ^= mask.m_value; return static_cast<Derived&>(*this); }
///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns result of bitwise AND with a raw integer-like mask. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator&(Other lhs, Derived rhs) noexcept { return Derived{_normalizer::normalize(lhs) & rhs.m_value}; }

    /*! @brief Returns result of bitwise AND with a raw integer-like mask. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator&(Derived lhs, Other rhs) noexcept { return Derived{lhs.m_value & _normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise AND with a raw integer-like mask. */
    [[nodiscard]] friend constexpr Derived operator&(Derived lhs, Derived rhs) noexcept { return Derived{lhs.m_value & rhs.m_value}; }

    /*! @brief Returns result of bitwise OR with a raw integer-like mask. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator|(Other lhs, Derived rhs) noexcept { return Derived{_normalizer::normalize(lhs) | rhs.m_value}; }

    /*! @brief Returns result of bitwise OR with a raw integer-like mask. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator|(Derived lhs, Other rhs) noexcept { return Derived{lhs.m_value | _normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise OR with a raw integer-like mask. */
    [[nodiscard]] friend constexpr Derived operator|(Derived lhs, Derived rhs) noexcept { return Derived{lhs.m_value | rhs.m_value}; }

    /*! @brief Returns result of bitwise XOR with a raw integer-like mask. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator^(Other lhs, Derived rhs) noexcept { return Derived{_normalizer::normalize(lhs) ^ rhs.m_value}; }

    /*! @brief Returns result of bitwise XOR with a raw integer-like mask. */
    template<enum_or_integer_concept Other> requires(_normalizer::template can_be_normalized_v<Other>)
    [[nodiscard]] friend constexpr Derived operator^(Derived lhs, Other rhs) noexcept { return Derived{lhs.m_value ^ _normalizer::normalize(rhs)}; }

    /*! @brief Returns result of bitwise XOR with a raw integer-like mask. */
    [[nodiscard]] friend constexpr Derived operator^(Derived lhs, Derived rhs) noexcept { return Derived{lhs.m_value ^ rhs.m_value}; }

    /*! @brief Returns bitwise negation of the stored value. */
    [[nodiscard]] constexpr Derived operator~() const noexcept { return Derived{~m_value}; }

    /*! @brief Checks whether the stored value is zero.
     *  @return `true` if stored value is zero, `false` otherwise. */
    constexpr bool operator!() const noexcept { return !m_value; }
///@}

///@name Assignment shift operators.
///@{
    /*! @brief Left-shifts the stored value by `shift` bits.
     *  @param shift Number of bits to shift to the left.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator<<=(int shift) noexcept { m_value <<= shift; return static_cast<Derived&>(*this); };

    /*! @brief Right-shifts the stored value by `shift` bits.
     *  @param shift Number of bits to shift to the right.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator>>=(int shift) noexcept { m_value >>= shift; return static_cast<Derived&>(*this); };
///@}

///@name Non-assignment shift operators.
///@{
    /*! @brief Returns a new wrapper with the stored value left-shifted by `shift` bits.
     *  @param shift Number of bits to shift to the left.
     *  @return New wrapper containing the shifted value. */
    [[nodiscard]] constexpr Derived operator<<(int shift) const noexcept { return Derived{m_value << shift}; }

    /*! @brief Returns a new wrapper with the stored value right-shifted by @p shift bits.
     *  @param shift Number of bits to shift to the right.
     *  @return New wrapper containing the shifted value. */
    [[nodiscard]] constexpr Derived operator>>(int shift) const noexcept { return Derived{m_value >> shift}; }
///@}

protected:
    Integer m_value{};
};

/*! @brief Concept satisfied by instantiations of @ref draupnir::utils::integer_wrapper.
 *  @tparam Candidate Type to test. */

template<class Candidate>
concept integer_wrapper_concept =
    draupnir::utils::is_template_base_of_v<integer_wrapper,Candidate>;

}; // namespace draupnir::utils

/*! @brief Declares a named integer wrapper type based on @ref draupnir::utils::integer_wrapper.
 *  @param name Name of the wrapper class to declare.
 *  @param primitive Underlying integer storage type. */

#define DEFINE_WRAPPED_INTEGER(name, primitive) \
class name : public draupnir::utils::integer_wrapper<primitive, name>          \
{                                                                              \
public:                                                                        \
    using draupnir::utils::integer_wrapper<primitive, name>::integer_wrapper;  \
    using draupnir::utils::integer_wrapper<primitive, name>::operator=;        \
};                                                                             \

#endif // INTEGER_WRAPPER_H
