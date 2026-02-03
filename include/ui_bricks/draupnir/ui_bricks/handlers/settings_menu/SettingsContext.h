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

#ifndef SETTINGSCONTEXT_H
#define SETTINGSCONTEXT_H

#include <QDebug>

/*! @class SettingsContext draupnir/ui_bricks/handlers/settings_menu/SettingsContext.h
 *  @ingroup UiBricks
 *  @brief Lightweight runtime context that provides access to a 'SettingsRegistry' instance.
 *  @tparam SettingsRegistry Registry type providing the settings storage and access API. Expected to provide:
 *          - template<class Trait> static constexpr bool contains();
 *          - template<class Trait> const typename Trait::Value& get();
 *          - template<class Trait> void set(const typename Trait::Value&);
 *
 *  @details SettingsContext is a small helper used by settings-related menu handlers. It stores a pointer to a 'SettingsRegistry'
 *           object and exposes typed accessors:
 *           - `get<SettingTrait>()` returns a const reference to the current value
 *           - `set<SettingTrait>(value)` updates the value
 *
 *           The context is intentionally "empty" after default construction and must be initialized exactly once via `loadSettings()`.
 *           In debug builds, this contract is enforced with `Q_ASSERT`:
 *           - `loadSettings()` asserts that the context was not initialized before and that the passed pointer is not null
 *           - `get()/set()` assert that the registry pointer is set
 *
 *           Additionally, `get()`/`set()` validate at compile time that the requested `SettingTrait` is supported by the `SettingsRegistry`
 *            type (via `SettingsRegistry::contains<SettingTrait>()`).
 *
 *  @note This class does not own the registry object. The caller must guarantee that the `SettingsRegistry instance outlives `SettingsContext`
 *        and all handlers that use it. */

namespace Draupnir::Handlers
{

template<class SettingsRegistry>
class SettingsContext
{
public:
    /*! @brief Constructs an uninitialized context (no registry attached). */
    SettingsContext() :
        p_registry{nullptr}
    {}

    /*! @brief Attaches a registry to this context. This method is intended to be called only once. In debug builds, it asserts that:
     *         `p_registry` is null (not initialized yet) and registry is not null
     *  @param registry Pointer to a SettingsRegistry instance.
     * @warning SettingsContext does not take ownership of `registry`. The registry must remain valid for the lifetime of this context. */
    void loadSettings(SettingsRegistry* registry) {
        Q_ASSERT(p_registry == nullptr);
        Q_ASSERT(registry != nullptr);
        p_registry = registry;
    }


    /*! @brief Returns a const reference to the value of the specified setting trait. A registry must be attached via `loadSettings()`. In
     *         debug builds this is enforced by `Q_ASSERT`. A compile-time check ensures the trait is supported by the `SettingsRegistry`
     *         type.
     *  @tparam SettingTrait Setting trait type.
     *  @return Const reference to the stored value. */
    template<class SettingTrait>
    const typename SettingTrait::Value& get() const {
        static_assert(SettingsRegistry::template contains<SettingTrait>());
        Q_ASSERT(p_registry);
        return p_registry->template get<SettingTrait>();
    }

    template<class SettingTrait>
    void set(const typename SettingTrait::Value& value) {
        static_assert(SettingsRegistry::template contains<SettingTrait>());
        Q_ASSERT(p_registry);
        p_registry->template set<SettingTrait>(value);
    }

private:
    SettingsRegistry* p_registry;
};

}; // namespace Draupnir::Handlers

#endif // SETTINGSCONTEXT_H
