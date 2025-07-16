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

#ifndef FIXED_TUPLE_MAP_H
#define FIXED_TUPLE_MAP_H

#include <array>
#include <tuple>
#include <type_traits>
#include <functional>

#include "fixed_map.h"

/*! @class fixed_tuple_map draupnir-lib/containers/fixed_tuple_map.h
 *  @brief Compile-time associative container mapping keys to tuples of values.
 *  @details This class extends fixed_map, associating each key with a tuple containing one value of each specified type. It
 *           provides convenient accessors for getting and setting individual tuple elements by type, as well as functional
 *           iteration over all tuples.
 *
 *  @tparam keys_array - static constexpr C-style array of enum values to be used as keys.
 *  @tparam value_types... - variadic list of types; each key will be associated with a tuple of these types.
 * @note The container size and key set are defined at compile-time. */

template<const auto& keys_array,class... value_types>
class fixed_tuple_map : public fixed_map<keys_array,std::tuple<value_types...> >
{
public:
    /*! @brief Default constructor. Initializes the underlying fixed_map and sets all tuple values to their default-constructed
     *         states. */
    constexpr fixed_tuple_map() :
        fixed_map<keys_array,std::tuple<value_types...> >()
    {}

    /*! @brief @brief Returns a reference to the value of the specified type for a given key.
     *  @tparam value_type - type of the value to access. Must be present in the tuple type list.
     *  @param key - key for which the value is requested.
     *  @return Reference to the value of the specified type for the given key.
     * @throws static_assert if value_type is not included in the tuple type list. */
    template<class value_type>
    [[nodiscard]] inline value_type& get(typename fixed_map<keys_array,std::tuple<value_types...> >::key_type key) {
        static_assert(is_one_of_v<value_type,value_types...>,"Provided value_type is not known.");
        return std::get<value_type>(fixed_map<keys_array,std::tuple<value_types...> >::get(key));
    }

    /*! @brief Sets the value of the specified type for a given key.
     *  @tparam value_type - type of the value to set. Must be present in the tuple type list.
     *  @param key - key for which the value is to be set.
     *  @param value - value to assign to the tuple element.
     * @throws static_assert if value_type is not included in the tuple type list. */
    template<class value_type>
    inline void set(typename fixed_map<keys_array,std::tuple<value_types...> >::key_type key, const value_type& value) {
        static_assert(is_one_of_v<value_type,value_types...>,"Provided Value is not known.");
        std::get<value_type>(fixed_map<keys_array,std::tuple<value_types...> >::get(key)) = value;
    }

    /*! @brief Applies the given callable to the tuple of values for each key in the map.
     *  @tparam F - callable type, accepting the tuple elements as parameters.
     *  @param function - functional object or lambda to apply.
     * @note The callable will be invoked with unpacked tuple elements for each stored tuple. */
    template<class F>
    void for_each_tuple(F&& function) const {
        for (const auto& element : fixed_map<keys_array,std::tuple<value_types...> >::m_data) {
            std::apply(function,element.second);
        }
    }
};

#endif // FIXED_TUPLE_MAP_H
