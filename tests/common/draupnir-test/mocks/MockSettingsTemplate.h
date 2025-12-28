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

#ifndef MOCKSETTINGSTEMPLATE_H
#define MOCKSETTINGSTEMPLATE_H

#include <type_traits>

#include "draupnir/settings_registry/core/SettingsBackendInterface.h"
#include "draupnir/settings_registry/core/SettingTemplate.h"

/*! @class MockSettingsTemplate tests/common/mocks/MockSettingsTemplate.h
 *  @brief This is a mock class implementing Draupnir::Settings::SettingsBackendInterface interface. This class initializes
 *         internal tuple with SettingTemplate objects for each of the SettingTraits specified. Settings values are initialized
 *         as default values. */

template<class... SettingTraits>
class MockSettingsTemplate final : public Draupnir::Settings::SettingsBackendInterface
{
public:
    ~MockSettingsTemplate() final = default;

    bool contains(const QString& key) const final {
        return _containsImpl<SettingTraits...>(key);
    }

    QVariant value(const QString& key, const QVariant& defaultValue = QVariant{}) {
        return _valueImpl<SettingTraits...>(key,defaultValue);
    }

    void setValue(const QString& key, const QVariant& value) final {
        return _setValueImpl<SettingTraits...>(key,value);
    }

    template<class Trait>
    typename Trait::Value get() {
        return std::get<Draupnir::Settings::SettingTemplate<Trait>>(m_dummyTuple).value;
    }

private:
    std::tuple<
        Draupnir::Settings::SettingTemplate<SettingTraits>...
    > m_dummyTuple;

    template<class First,class... Rest>
    bool _containsImpl(const QString& key) const {
        if (First::key() == key) {
            return true;
        }

        if constexpr (sizeof...(Rest) > 0) {
            return _containsImpl<Rest...>(key);
        } else {
            return false;
        }
    }

    template<class First,class... Rest>
    QVariant _valueImpl(const QString& key, const QVariant& defaultValue = QVariant()) {
        if (First::key() == key) {
            return QVariant::fromValue<
                typename Draupnir::Settings::SettingTemplate<First>::Value
            >(std::get<Draupnir::Settings::SettingTemplate<First>>(m_dummyTuple).value);
        }

        if constexpr (sizeof...(Rest) > 0) {
            return _valueImpl<Rest...>(key,defaultValue);
        } else {
            return QVariant{};
        }
    }

    template<class First,class... Rest>
    void _setValueImpl(const QString& key, const QVariant& value) {
        if (First::key() == key) {
            std::get<Draupnir::Settings::SettingTemplate<First>>(m_dummyTuple).value = value.value<typename First::Value>();
            return;
        }

        if constexpr (sizeof...(Rest) > 0) {
            _setValueImpl<Rest...>(key,value);
        }
    }
};

#endif // MOCKSETTINGSTEMPLATE_H
