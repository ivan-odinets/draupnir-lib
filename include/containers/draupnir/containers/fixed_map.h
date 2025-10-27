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

#ifndef FIXED_MAP_H
#define FIXED_MAP_H

#include <array>
#include <cassert>
#include <functional>

#include "draupnir/utils/template_constructors.h"

namespace draupnir::containers {

/*! @class fixed_map draupnir/containers/fixed_map.h
 *  @ingroup Containers
 *  @brief Compile-time associative container for key-to-type value mapping.
 *  @tparam keys_array - static constexpr С-style array of the key values.
 *  @tparam value_type - type to store.
 *
 *  @details Object of this type provides a std::map/QMap-like interface for accesing values of specified type. Compared
 *           to std::map / QMap amount of keys within fixed_map is specified during compile-time as an argument. Some details:
 *           - The container holds an std::array of std::pair objects, each pair storing a key from the provided array
 *             and a value object of the specified type.
 *           - These pairs can be accessed via standard iterators exported through fixed_map::begin, ::cbegin,
 *             ::end and ::cend.
 *           - Values can be retrieved using fixed_map::get or the operator[].
 *           - By default value_type objects are initialized as follows: numbers to 0, pointers to nullptr and other types
 *             via their default constructor.
 *
 * @note The container size and key set are defined at compile-time.
 *
 * @todo Improve container performance for cases when: keys array specified consists of integer sequence (or enum with
 *       increment of the values); array specified is list of boolean flags.
 * @todo Extend documentation for this class usage. And maybe add simple example program to the specific examples directory.
 * @todo Add constexpr variable versions of static constexpr methods. */

template<const auto& keys_array, class value_type>
class fixed_map
{
public:
    /*! @brief Size of the provided array. */
    static constexpr std::size_t keys_size = std::size(keys_array);

    /*! @brief Element type of the provided array. */
    using key_type = decltype(keys_array[0]);

    /*! @brief Store provided keys_array for building this fixed_map as static constexpr member. */
    static constexpr auto keys = keys_array;

    /*! @brief Default constructor. Initializes internal array of pairs. First element of the pair is initialized with key from
     *         provided keys_array. Second - as "zero value" with the following rules: numbers as 0, pointers as nullptr and other
     *         types as their default constructor. */
    constexpr fixed_map() :
        m_data{_create_pairs_array(std::make_index_sequence<keys_size>{})}
    {}

    /*! @brief Static constexpr method which checks if the other instantiation of a fixed_map template is using the same keys_array
     *         as this instantiation. */
    template<class other_map>
    static constexpr bool same_keys() {
        return (keys_size != other_map::keys_size) ?
                    false :
                    _same_keys_impl<other_map>();
    };

    /*! @brief Method which checks if the object of other instantiation of a fixed_map template is using the same keys_array as this
     *         object. */
    template<class other_map>
    bool same_keys(const other_map&) const { return same_keys<other_map>(); }

    /*! @brief This method returns true if the provided key cotained within the keys_array. */
    static constexpr bool contains(key_type key) { return _contains_impl<0>(key); }

    /*! @brief Returns first value of the provided keys_array. */
    static constexpr key_type first_key() { return keys_array[0]; }

    /*! @brief Returns last value of the provided keys_array. */
    static constexpr key_type last_key() { return keys_array[keys_size-1]; }

    /*! @brief Typedef of the iterator which can be used to iterate over the content of fixed_map. */
    using iterator = typename std::array<std::pair<const key_type,value_type>, keys_size>::iterator;

    /*! @brief Returns begin iterator of the internal std::array of std::pair objects. */
    iterator begin() { return m_data.begin(); }

    /*! @brief Returns cbegin iterator of the internal std::array of std::pair objects. */
    auto cbegin() const { return m_data.cbegin(); }

    /*! @brief Returns end iterator of the internal std::array of std::pair objects. */
    iterator end() { return m_data.end(); }

    /*! @brief Returns cend iterator of the internal std::array of std::pair objects. */
    auto cend() const { return m_data.cend(); }

    /*! @brief This method will iterate over internal std::array of pairs and execute provided functional object for every
     *         value_type stored inside. */
    template<class F>
    void for_each_value(F&& function) {
        for (std::pair<const key_type,value_type>& element : m_data) {
            function(element.second);
        }
    }

    /*! @brief Const version of previous method. This method will iterate over internal std::array of pairs and execute
     *         provided functional object for every value_type stored inside. */
    template<class F>
    void for_each_value(F&& function) const {
        for (const std::pair<const key_type,value_type>& element : m_data) {
            function(element.second);
        }
    }

    /*! @brief This method will iterate over internal std::array of pairs and execute provided functional object for every
     *         std::pair<const key_type,value_type> stored inside the std::array. */
    template<class F>
    void for_each_pair(F&& function) {
        for (std::pair<const key_type,value_type>& element : m_data) {
            function(element);
        }
    }

    /*! @brief Const version of previous method. This method will iterate over internal std::array of pairs and execute
     *         provided functional object for every std::pair<const key_type,value_type> stored inside the std::array. */
    template<class F>
    void for_each_pair(F&& function) const {
        for (const std::pair<const key_type,value_type>& element : m_data) {
            function(element);
        }
    }

    /*! @brief Returns reference to the value_type object associated with the given key.
     * @warning If the key is not available in the keys_array template argument - assert statement will be executed. */
    [[nodiscard]] constexpr value_type& get(key_type key) { return _get_impl<0>(key); }

    /*! @brief Returns reference to the value_type object associated with the given key.
     * @warning If the key is not available in the keys_array template argument - assert statement will be executed. */
    [[nodiscard]] constexpr value_type& operator[](key_type key) { return _get_impl<0>(key); }

    /*! @brief Returns reference to the value_type object for a specific index.
     * @warning If the specified index is larger than size of fixed_map - out of bounds access to the array will happen. */
    [[nodiscard]] constexpr value_type& value_by_index(int index) { return m_data[index].second; }

    /*! @brief Clears the fixed_map container. Values associated with all keys are reset to default ones. This means: numbers
     *         as 0, pointers as nullptr and other types as their default constructor. */
    void clear() { _clear_impl<0>(); }

protected:
    /*! @brief Values are stored in an std::array of std::pair objects. Each pair holds one of the provided keys (as const
     *         key_type) and its associated value_type object. Objects of value_type can be accessed / modified by methods
     *         like fixed_map::get and fixed_map::for_each_value. */
    std::array<std::pair<const key_type,value_type>, keys_size> m_data;

private:
    template<std::size_t... I>
    static constexpr std::array<std::pair<const key_type,value_type>, keys_size> _create_pairs_array(std::index_sequence<I...>) {
        return std::array<std::pair<const key_type,value_type>, keys_size>{
            { std::pair<const key_type,value_type>{keys_array[I], draupnir::utils::make_zero_value<value_type>()} ... }
        };
    }

    template<class other_map, std::size_t Index = 0>
    static constexpr bool _same_keys_impl() {
        if constexpr (Index < keys_size) {
            return (keys_array[Index] != other_map::keys[Index]) ?
                        false :
                        _same_keys_impl<other_map,Index+1>();
        } else {
            return true;
        }
    }

    template<std::size_t Index = 0>
    static constexpr bool _contains_impl(key_type key) {
        if constexpr (Index < keys_size) {
            return (keys[Index] == key) ?
                        true :
                        _contains_impl<Index+1>(key);
        } else {
            return false;
        }
    }

    template<std::size_t I>
    inline value_type& _get_impl(key_type key) {
        if constexpr (I < keys_size) {
            return (m_data[I].first == key) ?
                m_data[I].second :
                _get_impl<I+1>(key);
        } else {
            assert(false && "Provided key is not known by this fixed_map.");
            // This should not be executed. But can be executed if the logic is bad.
            // To suppress compiler warnings about "No Return"
            return *reinterpret_cast<value_type*>(0xDEADBEEF);
        }
    };

    template<std::size_t Index>
    inline void _clear_impl() {
        if constexpr (Index < keys_size) {
            m_data[Index].second = draupnir::utils::make_zero_value<value_type>();
            _clear_impl<Index+1>();
        }
    }
};

};

#endif // FIXED_MAP_H
