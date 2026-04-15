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

#ifndef INTEGER_WRAPPER_H
#define INTEGER_WRAPPER_H

#include <concepts>

#include "draupnir/utils/concepts/type_concepts.h"
#include "draupnir/utils/template_detectors.h"

namespace draupnir::utils
{

/*! @class integer_wrapper draupnir/utils/integer_wrapper.h
 *  @ingroup Utils
 *  @brief CRTP-based type-safe wrapper over an integer-like type.
 *  @tparam Integer Underlying integer storage type. Must satisfy @ref draupnir::utils::integer_concept.
 *  @tparam Derived Final wrapper type inheriting from this template.
 *
 *  @details `integer_wrapper<Integer, Derived>` provides an integer-like API while preserving type safety between different
 *           wrapper types. Objects of the same @p Derived type can be compared and combined with each other, and can also
 *           interact with raw integer-like values convertible to @p Integer. Different wrapper types are intentionally not
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
public:
    /*! @brief Alias for the underlying integer storage type. */
    using underlying_type = Integer;

    /*! @brief Constructs wrapper with zero value. */
    constexpr integer_wrapper() noexcept = default;

    /*! @brief Constructs wrapper from an underlying integer value.
     *  @param value Initial value to store. */
    constexpr explicit integer_wrapper(Integer value) noexcept:
        m_value{value} {}

    /*! @brief Assigns a new underlying integer value.
     *  @param value New value to store.
     *  @return `*this`. */
    constexpr Derived& operator=(Integer value) noexcept { m_value = value; return static_cast<Derived&>(*this); };

    /*! @brief Returns the stored underlying value.
     *  @return Current wrapped integer value. */
    [[nodiscard]] constexpr Integer value() const noexcept { return m_value; }

    /*! @brief Compares wrapper with a raw integer-like value for equality.
     *  @tparam Other Right-hand side type convertible to `Integer`, excluding other @ref integer_wrapper types.
     *  @param other Right-hand side value.
     *  @return `true` if stored value equals `other`, `false` otherwise. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr bool operator==(const Other& other) const noexcept { return m_value == static_cast<Integer>(other); }

    /*! @brief Compares two wrappers of the same derived type for equality.
     *  @tparam Other Type of the right-hand side operand.
     *  @param lhs Left-hand side wrapper.
     *  @param rhs Right-hand side wrapper.
     *  @return `true` if stored values are equal, `false` otherwise. */
    friend constexpr bool operator==(const Derived& lhs, const Derived& rhs) noexcept { return lhs.m_value == rhs.m_value; }

    /*! @brief Three-way comparison with a raw integer-like value.
     *  @tparam Other Type of the right-hand side operand.
     *  @param other Right-hand side raw value.
     *  @return Result of comparing stored value with `other`. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr auto operator<=>(const Other& other) const noexcept { return m_value <=> static_cast<Integer>(other); }

    /*! @brief Three-way comparison with another wrapper of the same derived type.
     *  @tparam Other Type of the right-hand side operand.
     *  @param other Right-hand side wrapper.
     *  @return Result of comparing stored values. */
    friend constexpr auto operator<=>(const Derived& lhs, const Derived& rhs) noexcept { return lhs.m_value <=> rhs.m_value; }

///@name Bitwise assignment operators.
///@{
    /*! @brief Applies bitwise AND with a raw integer-like mask.
     *  @tparam Other Mask type convertible to `Integer`, excluding other @ref integer_wrapper types.
     *  @param mask Right-hand side mask.
     *  @return Reference to the derived wrapper object. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr Derived& operator&=(const Other& mask) noexcept { m_value &= mask; return static_cast<Derived&>(*this); };

    /*! @brief Applies bitwise AND with another wrapper of the same derived type.
     *  @param mask Right-hand side wrapper.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator&=(const Derived& mask) noexcept { m_value &= mask.m_value; return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise OR with a raw integer-like mask.
     *  @tparam Other Mask type convertible to `Integer`, excluding other @ref integer_wrapper types.
     *  @param mask Right-hand side mask.
     *  @return Reference to the derived wrapper object. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr Derived& operator|=(const Other& mask) noexcept { m_value |= mask; return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise OR with another wrapper of the same derived type.
     *  @param mask Right-hand side wrapper.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator|=(const Derived& mask) noexcept { m_value |= mask.m_value; return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise XOR with a raw integer-like mask.
     *  @tparam Other Mask type convertible to `Integer`, excluding other @ref integer_wrapper types.
     *  @param mask Right-hand side mask.
     *  @return Reference to the derived wrapper object. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr Derived& operator^=(const Other& mask) noexcept { m_value ^= mask; return static_cast<Derived&>(*this); }

    /*! @brief Applies bitwise XOR with another wrapper of the same derived type.
     *  @param mask Right-hand side wrapper.
     *  @return Reference to the derived wrapper object. */
    constexpr Derived& operator^=(const Derived& mask) noexcept { m_value ^= mask.m_value; return static_cast<Derived&>(*this); }

///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns result of bitwise AND with a raw integer-like mask. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr Derived operator&(const Other& mask) const noexcept { return Derived{m_value & mask}; }

    /*! @brief Returns result of bitwise AND with another wrapper of the same derived type. */
    constexpr Derived operator&(const Derived& mask) const noexcept { return Derived{m_value & mask.m_value}; }

    /*! @brief Returns result of bitwise OR with a raw integer-like mask. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr Derived operator|(const Other& mask) const noexcept { return Derived{m_value | mask}; }

    /*! @brief Returns result of bitwise OR with another wrapper of the same derived type. */
    constexpr Derived operator|(const Derived& mask) const noexcept { return Derived{m_value | mask.m_value}; }

    /*! @brief Returns result of bitwise XOR with a raw integer-like mask. */
    template<class Other> requires
        std::convertible_to<Other, Integer> && (!is_template_base_of_v<integer_wrapper, std::remove_cvref_t<Other>>)
    constexpr Derived operator^(const Other& mask) const noexcept { return Derived{m_value ^ mask}; }

    /*! @brief Returns result of bitwise XOR with another wrapper of the same derived type. */
    constexpr Derived operator^(const Derived& mask) const noexcept { return Derived{m_value ^ mask.m_value}; }

    /*! @brief Returns bitwise negation of the stored value. */
    constexpr Derived operator~() const noexcept { return Derived{~m_value}; }

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
    constexpr Derived operator<<(int shift) const noexcept { return Derived{m_value << shift}; }

    /*! @brief Returns a new wrapper with the stored value right-shifted by @p shift bits.
     *  @param shift Number of bits to shift to the right.
     *  @return New wrapper containing the shifted value. */
    constexpr Derived operator>>(int shift) const noexcept { return Derived{m_value >> shift}; }
///@}

protected:
    Integer m_value{};
};

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
