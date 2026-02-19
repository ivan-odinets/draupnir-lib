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

#include <initializer_list>

#include "draupnir/utils/template_detectors.h"
#include "draupnir/utils/type_detectors.h"

namespace draupnir::utils
{

/*! @class flags draupnir/utils/flags.h
 *  @ingroup Utils
 *  @brief Lightweight bitmask wrapper over an integer type.
 *  @tparam Int Integer storage type for the bitmask (e.g. `uint32_t`, `quint64`). Must satisfy @ref draupnir::utils::integer_concept.
 *
 *  @details `flags<Int>` is a small value-type that stores a bitmask in an integer and provides a `QFlags`-like interface:
 *            - construction from a raw mask or from a list of bit values;
 *            - bitwise operators (`&`, `|`, `^`, `~`) and their assignment forms;
 *            - helpers for checking/setting bits via @ref testFlag and @ref setFlag.
 *
 *            The class does not prescribe any particular meaning for bits. It is intended to be used either directly (for
 *            "raw" flags), or as a base for higher-level wrappers such as @ref draupnir::utils::enum_flags.
 *
 * @note `testFlag(0)` follows the QFlags-like rule: it returns `true` only when the stored mask is also zero. */

template<integer_concept Int>
class flags
{
public:
    /*! @brief Alias to access integer type being used within this @ref flags object instantiation. */
    using integer = Int;

    /*! @brief Constructs an empty flags object. Initializes the underlying mask to `0`. */
    constexpr inline flags() noexcept :
        m_mask{0} {}

    /*! @brief Constructs flags from a raw mask.
     *  @param mask Initial bitmask value. */
    constexpr inline flags(Int mask) noexcept :
        m_mask{mask} {}

    /*! @brief Constructs flags by OR-ing all values from the initializer list. Equivalent to: `Int result = 0; for (auto v
     *         : mask) result |= v;`
     *  @param mask List of bit values to combine. */
    constexpr inline flags(std::initializer_list<Int> mask) noexcept
        : m_mask{initializer_list_helper(mask.begin(), mask.end())} {}

    /*! @brief Implicit conversion to the underlying integer mask.
     *  @return Stored mask value. */
    constexpr inline operator Int() const noexcept { return m_mask; }

    /*! @brief Returns the underlying mask value explicitly.
     *  @return Stored mask value. */
    constexpr inline Int value() const noexcept { return m_mask; }

    /*! @brief Checks whether any bit is set.
     *  @return `true` if mask is non-zero, `false` otherwise. */
    constexpr inline bool any() const noexcept { return m_mask != 0; }

    /*! @brief Checks whether no bits are set.
     *  @return `true` if mask is zero, `false` otherwise. */
    constexpr inline bool none() const noexcept { return m_mask == 0; }

///@name Bitwise assignment operators.
///@{
    /*! @brief ANDs the stored mask with a raw mask.
     *  @param mask Mask to AND with. */
    constexpr inline flags& operator&=(Int mask) noexcept { m_mask &= mask; return *this; }

    /*! @brief ANDs the stored mask with another flags object.
     *  @param other Other flags. */
    constexpr inline flags& operator&=(const flags& other) noexcept { m_mask &= other.m_mask; return *this; }

    /*! @brief ORs the stored mask with a raw mask.
     *  @param mask Mask to OR with. */
    constexpr inline flags& operator|=(Int mask) noexcept { m_mask |= mask; return *this; }

    /*! @brief ORs the stored mask with another flags object.
     *  @param other Other flags. */
    constexpr inline flags& operator|=(const flags& other) noexcept { m_mask |= other.m_mask; return *this; }

    /*! @brief XORs the stored mask with a raw mask.
     *  @param mask Mask to XOR with. */
    constexpr inline flags& operator^=(Int mask) noexcept { m_mask ^= mask; return *this; }

    /*! @brief XORs the stored mask with another flags object.
     *  @param other Other flags. */
    constexpr inline flags& operator^=(const flags& other) noexcept { m_mask ^= other.m_mask; return *this; }
///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns a new flags object equal to `(*this | other)`. */
    constexpr inline flags operator|(const flags& other) const noexcept { return flags{m_mask | other.m_mask}; }

    /*! @brief Returns a new flags object equal to `(*this ^ other)`. */
    constexpr inline flags operator^(const flags& other) const noexcept { return flags{m_mask ^ other.m_mask}; }

    /*! @brief Returns a new flags object equal to `(*this & other)`. */
    constexpr inline flags operator&(const flags& other) const noexcept { return flags{m_mask & other.m_mask}; }

    /*! @brief Returns a new flags object with all bits inverted (`~mask`). */
    constexpr inline flags operator~() const noexcept { return flags{~m_mask}; }

    /*! @brief Checks whether the mask is zero. Returns `true` if mask is zero, `false` otherwise. */
    constexpr inline bool operator!() const noexcept { return !m_mask; }
///@}

    /*! @brief Tests whether all bits from @p flag are set in the current mask. Returns `true` if `(m_mask & flag) ==
     *         flag`.
     *  @param flag Bit pattern to test.
     * @note Special case: if `flag == 0`, this returns `true` only if `m_mask == 0`. */
    constexpr inline bool testFlag(Int flag) const noexcept {
        return (m_mask & flag) == flag && (flag != 0 || m_mask == flag );
    }

    /*! @brief Sets or clears bits from @p flag.
     *  @param flag Bit pattern to modify.
     *  @param on  If `true`, sets bits (`OR`). If `false`, clears bits (`AND` with `~flag`).
     *  @return `*this`. */
    constexpr inline flags& setFlag(Int flag, bool on = true) noexcept {
        return on ? (*this |= flag) : (*this &= ~flag);
    }

protected:
    Int m_mask;

    constexpr static inline Int initializer_list_helper(typename std::initializer_list<Int>::const_iterator it,
                                                        typename std::initializer_list<Int>::const_iterator end) noexcept {
        return (it == end ? Int(0) : (Int(*it) | initializer_list_helper(it + 1, end)));
    }
};

/*! @class enum_flags draupnir/utils/flags.h
 *  @ingroup Utils
 *  @brief Enum-friendly flags type.
 *  @tparam Enum Enum type whose underlying integral type is used as storage. Must satisfy @ref draupnir::utils::enum_concept.
 *
 *  @details `enum_flags<Enum>` is a convenience wrapper around @ref draupnir::utils::flags that uses `std::underlying_type_t<Enum>`
 *           as the mask storage type. It is intended to provide a strongly-typed "flags-for-enum" handle while reusing all bitmask
 *           operations from the base @ref draupnir::utils::flags. */

template<enum_concept Enum>
class enum_flags : public flags<std::underlying_type_t<Enum>>
{
public:
    /*! @brief Associated enum type. */
    using enum_type = Enum;

    /*! @brief Integer type used by the @ref draupnir::utils::flags base of this enum_flags object. */
    using integer = flags<std::underlying_type_t<Enum>>::integer;

    /*! @brief Assigns a single enum value to this flags object.
     *  @param value Enum value whose underlying bits become the new mask. */
    constexpr inline enum_flags& operator=(Enum value) noexcept {
        this->m_mask = value;
        return *this;
    }

///@name Bitwise assignment operators.
///@{
    /*! @brief ANDs the stored mask with a Enum flag.
     *  @param flag Fask to AND with. */
    constexpr inline enum_flags& operator&=(Enum flag) noexcept { this->m_mask &= flag; return *this; }

    /*! @brief ANDs the stored mask with a raw mask.
     *  @param mask Mask to AND with. */
    constexpr inline enum_flags& operator&=(integer mask) noexcept { this->m_mask &= mask; return *this; }

    /*! @brief ANDs the stored mask with another @ref enum_flags object.
     *  @param other Other enum_flags. */
    constexpr inline enum_flags& operator&=(const enum_flags& other) noexcept { this->m_mask &= other.m_mask; return *this; }

    /*! @brief ORs the stored mask with a Enum flag.
     *  @param flag Fask to OR with. */
    constexpr inline enum_flags& operator|=(Enum flag) noexcept { this->m_mask |= flag; return *this; }

    /*! @brief ORs the stored mask with a raw mask.
     *  @param mask Mask to OR with. */
    constexpr inline enum_flags& operator|=(integer mask) noexcept { this->m_mask |= mask; return *this; }

    /*! @brief ORs the stored mask with another flags object.
     *  @param other Other enum_flags. */
    constexpr inline enum_flags& operator|=(const enum_flags& other) noexcept { this->m_mask |= other.m_mask; return *this; }

    /*! @brief XORs the stored mask with a Enum flag.
     *  @param flag Fask to XOR with. */
    constexpr inline enum_flags& operator^=(Enum flag) noexcept { this->m_mask ^= flag; return *this; }

    /*! @brief XORs the stored mask with a raw mask.
     *  @param mask Mask to XOR with. */
    constexpr inline enum_flags& operator^=(integer mask) noexcept { this->m_mask ^= mask; return *this; }

    /*! @brief XORs the stored mask with another enum_flags object.
     *  @param other Other flags. */
    constexpr inline enum_flags& operator^=(const enum_flags& other) noexcept { this->m_mask ^= other.m_mask; return *this; }
///@}

///@name Bitwise non-assignment operators.
///@{
    /*! @brief Returns a new enum_flags object equal to `(*this | other)`. */
    constexpr inline enum_flags operator|(const enum_flags& other) const noexcept { return enum_flags{this->m_mask | other.m_mask}; }

    /*! @brief Returns a new enum_flags object equal to `(*this ^ other)`. */
    constexpr inline enum_flags operator^(const enum_flags& other) const noexcept { return enum_flags{this->m_mask ^ other.m_mask}; }

    /*! @brief Returns a new flags object equal to `(*this & other)`. */
    constexpr inline enum_flags operator&(const enum_flags& other) const noexcept { return enum_flags{this->m_mask & other.m_mask}; }

    /*! @brief Returns a new flags object with all bits inverted (`~mask`). */
    constexpr inline enum_flags operator~() const noexcept { return enum_flags{~this->m_mask}; }

    /*! @brief Checks whether the mask is zero.
     *  @return `true` if mask is zero, `false` otherwise. */
    constexpr inline bool operator!() const noexcept { return !this->m_mask; }
///@}
};

template<class Candidate>
concept enum_flags_concept = draupnir::utils::is_instantiation_of_v<Candidate,enum_flags>;

}; // namespace draupnir::utils

#endif // FLAGS_H
