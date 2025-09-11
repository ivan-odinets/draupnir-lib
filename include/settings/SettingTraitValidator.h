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

#ifndef SETTINGTRAITVALIDATOR_H
#define SETTINGTRAITVALIDATOR_H

#include <QString>

#include <type_traits>

namespace Draupnir::Settings
{

/*! @class SettingTraitValidator draupnir-lib/incldue/settings/SettingTraitValidator.h
 *  @brief Compile-time validator for SettingTrait requirements.
 *  @details This helper performs SFINAE-based checks to ensure a given SettingTrait conforms to the expected interface:
 *           - Вefines nested type Value;
 *           - Provides static QString key();
 *           - Provides static Value defaultValue().
 *
 *           Use isValidTrait<SettingTrait>() in static contexts (e.g. static_assert) or to gate template instantiations
 *           via SFINAE/if constexpr.
 *
 * @note The detection utilities (has_valueType, has_key, has_defaultValue) are implemented via std::void_t. They evaluate
 *       to std::true_type if and only if the corresponding expression is well-formed. */

class SettingTraitValidator
{
public:
    /*! @brief Check whether SettingTrait satisfies the minimal trait contract.
     *  @details The contract is:
     *           - typename SettingTrait::Value exists;
     *           - static QString SettingTrait::key();
     *           - static SettingTrait::Value SettingTrait::defaultValue();
     *
     *           On failure, this function triggers compile-time diagnostics via static_assert with a descriptive message.
     *  @tparam SettingTrait Candidate trait type to validate.
     *  @return true if all required members are present; otherwise compilation fails. */
//    template<class SettingTrait>
//    static bool constexpr isValidTrait() {
//        static_assert(has_valueType<SettingTrait>::value,
//                "Provided SettingTrait does not have the Value type defined");
//        static_assert(has_key<SettingTrait>::value,
//                "Provided SettingTrait does not have the static QString key() method defined");
//        static_assert(has_defaultValue<SettingTrait>::value,
//                "Provided SettingTrait does not have the static typename SettingTrait::Value defaultValue() method defined");

//        return has_valueType<SettingTrait>::value &&
//               has_key<SettingTrait>::value &&
//               has_defaultValue<SettingTrait>::value;
//    }

    /*! @brief Checks that typename SettingTrait::Value exists. */
    template<class SettingTrait, class = void>
    struct has_valueType : std::false_type {};

    template<class SettingTrait>
    struct has_valueType<
        SettingTrait,
        std::void_t<typename SettingTrait::Value>
    > : std::true_type {};

    /*! @brief Checks that static QString SettingTrait::key() is well-formed and returns QString. */
    template<class, class = std::void_t<>>
    struct has_key : std::false_type {};

    template<class SettingTrait>
    struct has_key<
        SettingTrait,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(SettingTrait::key())>
        )>
    > : std::true_type {};

    /*! @brief Checks that static Value SettingTrait::defaultValue() is well-formed and returns Value. */
    template<class, class = std::void_t<>>
    struct has_defaultValue : std::false_type {};

    template<class SettingTrait>
    struct has_defaultValue<
        SettingTrait,
        std::void_t<decltype(
            std::is_same_v<typename SettingTrait::Value,decltype(SettingTrait::defaultValue())>
        )>
    > : std::true_type {};
};

}; // namespace Draupnir::Settings

#endif // SETTINGTRAITVALIDATOR_H
