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

#ifndef SETTINGSVALUEUSERINPUT_H
#define SETTINGSVALUEUSERINPUT_H

#include <optional>

#include <QApplication>
#include <QIcon>
#include <QInputDialog>

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/utils/type_presense.h"

namespace Draupnir::Handlers
{

/*! @ingroup UiBricks
 *  @brief Restricts integer inputs supported by `SettingsValueUserInput` via `QInputDialog`.
 *  @tparam Number Candidate numeric type (cv/ref qualifiers are ignored).
 *
 *  @details QInputDialog::IntInput operates on an underlying QSpinBox which natively stores an `int`. This concept deliberately
 *           limits integer value types to a small safe set that can be losslessly handled through an `int` dialog (with optional
 *           range tightening).
 *
 * @note This is an intentional limitation to prevent accidental usage of types like long/long long/size_t in generic settings input. */

template<typename Number>
concept IsSupportedIntegerType =
    draupnir::utils::is_one_of_v<std::remove_cvref_t<Number>,
        short, unsigned short, int
    >;

/*! @ingroup UiBricks
 *  @brief Restricts floating-point inputs supported by `SettingsValueUserInput` via `QInputDialog`.
 *  @tparam Number Candidate numeric type (cv/ref qualifiers are ignored).
 *
 *  @details QInputDialog::DoubleInput operates on an underlying QDoubleSpinBox which stores a `double`. This concept limits floating-point
 *           value types to those that can be represented safely using a double-based input dialog. */

template<typename Number>
concept IsSupportedFloatingPointType =
    draupnir::utils::is_one_of_v<std::remove_cvref_t<Number>,
        double, float
    >;

/*! @class SettingsValueUserInput draupnir/ui_bricks/utils/SettingsValueUserInput.h
 *  @ingroup UiBricks
 *  @brief Type-directed `QInputDialog`-based user input helper for settings values.
 *  @tparam SettingTrait A settings trait describing the setting. Must satisfy @ref Draupnir::Settings::SettingTraitConcept .
 *  @tparam Value The value type to be input (specialization key).
 *
 *  @details Primary template should be specialized per supported Value type. Default template produce `static_assert` Each specialization
 *           must provide a single static entry point:
 *           @code
 *           std::optional<Value> SettingsValueUserInput<Trait, Value>::getValue(oldValue);
 *           @endcode
 *
 * @todo For specializations: Improve dialog size. */

template<Settings::SettingTraitConcept SettingTrait,class Value>
class SettingsValueUserInput {
    static_assert(!std::is_same_v<Value,Value>);
};

/*! @class SettingsValueUserInput<SettingTrait, Number> draupnir/ui_bricks/utils/SettingsValueUserInput.h
 *  @ingroup UiBricks
 *  @tparam SettingTrait Settings trait.
 *  @tparam Number Integer value type. Must satisfy IsSupportedIntegerType.
 *  @brief `QInputDialog`-based input for supported integer types.
 *
 *  @details Uses `QInputDialog::IntInput` (`QSpinBox`). The dialog is initialized with `oldValue`. Input range is restricted in the
 *           following priority order:
 *           1) If `SettingTrait` provides minimal/maximal bounds (@ref Draupnir::Settings::HasMinimalValue / @ref Draupnir::Settings::HasMaximalValue),
 *              those bounds are applied.
 *           2) Otherwise, if `Number`'s numeric limits are narrower than int's, the bounds are tightened to `Number`'s representable range.
 *
 *           If `SettingTrait` it provides `settingDescription()`, it will be used as dialog label.
 *
 * @note `QInputDialog` stores the integer internally as `int`. Returned value is cast to `Number`. Ensure trait-provided min/max fit into
 *        `int` if you expose them.
 * @todo Add some static_assert to validate that minimalValue() / maximalValue() exposed by `SettingTrait` are makins sense. */

template<Settings::SettingTraitConcept SettingTrait, typename Number>
    requires(IsSupportedIntegerType<Number>)
class SettingsValueUserInput<SettingTrait,Number>
{
public:
    /*! @brief Shows an integer input dialog and returns the new value if accepted.
     *  @param oldValue Current/previous value to prefill in the dialog.
     *  @return Optional new value, or std::nullopt if the user cancelled. */
    static std::optional<Number> getValue(const Number& oldValue) {
        QInputDialog dialog{qApp->activeWindow()};
        dialog.setInputMode(QInputDialog::IntInput);
        dialog.setIntValue(oldValue);

        // Configure dialog appearence
        dialog.setWindowIcon(qApp->windowIcon());
        dialog.setWindowTitle(qApp->applicationName());

        // Optinally set the text for the dialog.
        if constexpr (Settings::SettingTrait::HasSettingDescription<SettingTrait>)
            dialog.setLabelText(SettingTrait::settingDescription());

        // Optionaly restrict user input for minimal value
        if constexpr (Settings::SettingTrait::HasMinimalValue<SettingTrait>)
            dialog.setIntMinimum(SettingTrait::minimalValue());
        else if constexpr (std::numeric_limits<Number>::min() > std::numeric_limits<int>::min())
            dialog.setIntMinimum(std::numeric_limits<Number>::min());
        else
            dialog.setIntMinimum(std::numeric_limits<int>::min());

        // Optionally restrict user input for maximal value
        if constexpr (Settings::SettingTrait::HasMaximalValue<SettingTrait>)
            dialog.setIntMaximum(SettingTrait::maximalValue());
        else if constexpr (std::numeric_limits<Number>::max() < std::numeric_limits<int>::max())
            dialog.setIntMaximum(std::numeric_limits<Number>::max());
        else
            dialog.setIntMaximum(std::numeric_limits<int>::max());

        // Show the dialog and exit
        dialog.exec();
        return (dialog.result() == QDialog::Accepted) ?
            std::optional<Number>{dialog.intValue()} :
            std::nullopt;
    }
};

/*! @class SettingsValueUserInput<SettingTrait, Number> draupnir/ui_bricks/utils/SettingsValueUserInput.h
 *  @ingroup UiBricks
 *  @brief `QInputDialog`-based input for supported floating-point types.
 *  @tparam SettingTrait Settings trait. If it provides `settingDescription()`, it will be used as dialog label.
 *  @tparam Number Floating-point value type. Must satisfy IsSupportedFloatingPointType.
 *
 *  @details Uses `QInputDialog::DoubleInput` (`QDoubleSpinBox`). The dialog is initialized with `oldValue`.
 *           Input range is restricted in the following priority order:
 *           1) If `SettingTrait` provides minimal/maximal bounds (`HasMinimalValue` / `HasMaximalValue`),
 *              those bounds are applied.
 *           2) Otherwise, if Number's representable range is narrower than double's, the bounds are tightened
 *              to Number's representable range.
 *
 *           If `SettingTrait` it provides `settingDescription()`, it will be used as dialog label.
 *
 * @note QInputDialog stores the value internally as `double`. Returned value is cast to Number.
 * @todo Add some static_assert to validate that minimalValue() / maximalValue() exposed by `SettingTrait` are makins sense. */

template<Settings::SettingTraitConcept SettingTrait, typename Number>
    requires(IsSupportedFloatingPointType<Number>)
class SettingsValueUserInput<SettingTrait,Number>
{
public:
    /*! @brief Shows a floating-point input dialog and returns the new value if accepted.
     *  @param oldValue Current/previous value to prefill in the dialog.
     *  @return Optional new value, or std::nullopt if the user cancelled. */
    static std::optional<Number> getValue(const Number& oldValue) {
        QInputDialog dialog{qApp->activeWindow()};
        dialog.setInputMode(QInputDialog::DoubleInput);
        dialog.setDoubleValue(oldValue);

        // Configure dialog appearence
        dialog.setWindowIcon(qApp->windowIcon());
        dialog.setWindowTitle(qApp->applicationName());

        // Optinally set the text for the dialog.
        if constexpr (Settings::SettingTrait::HasSettingDescription<SettingTrait>)
            dialog.setLabelText(SettingTrait::settingDescription());

        // Optionaly restrict user input for minimal value
        if constexpr (Settings::SettingTrait::HasMinimalValue<SettingTrait>)
            dialog.setDoubleMinimum(SettingTrait::minimalValue());
        else if constexpr (std::numeric_limits<Number>::lowest() > std::numeric_limits<double>::lowest())
            dialog.setDoubleMinimum(std::numeric_limits<Number>::min());
        else
            dialog.setDoubleMinimum(std::numeric_limits<double>::lowest());

        // Optionally restrict user input for maximal value
        if constexpr (Settings::SettingTrait::HasMaximalValue<SettingTrait>)
            dialog.setDoubleMaximum(SettingTrait::maximalValue());
        else if constexpr (std::numeric_limits<Number>::max() < std::numeric_limits<double>::max())
            dialog.setDoubleMaximum(std::numeric_limits<Number>::max());
        else
            dialog.setDoubleMaximum(std::numeric_limits<double>::max());

        // Show the dialog and exit
        dialog.exec();
        return (dialog.result() == QDialog::Accepted) ?
            std::optional<Number>{dialog.doubleValue()} :
            std::nullopt;
    }
};

/*! @class SettingsValueUserInput<SettingTrait, QString> draupnir/ui_bricks/utils/SettingsValueUserInput.h
 *  @ingroup UiBricks
 *  @tparam SettingTrait Settings trait. If it provides `settingDescription()`, it will be used as dialog label.
 *  @brief `QInputDialog`-based input for `QString` values.
 *
 *  @details Uses `QInputDialog::TextInput`. The dialog is initialized with `oldValue`. If SettingTrait provides a description
 *           (settingDescription()), it is shown as dialog label.
 * @todo Add support for non-QString strings (e.g. std::string). */

template<Settings::SettingTraitConcept SettingTrait>
class SettingsValueUserInput<SettingTrait,QString>
{
public:
    /*! @brief Shows a text input dialog and returns the new value if accepted.
     *  @param oldValue Current/previous text to prefill in the dialog.
     *  @return Optional new text, or std::nullopt if the user cancelled. */
    static std::optional<QString> getValue(const QString& oldValue) {
        QInputDialog dialog{qApp->activeWindow()};
        dialog.setInputMode(QInputDialog::TextInput);
        dialog.setTextValue(oldValue);

        // Configure dialog appearence
        dialog.setWindowIcon(qApp->windowIcon());
        dialog.setWindowTitle(qApp->applicationName());

        // Optinally set the text for the dialog.
        if constexpr (Settings::SettingTrait::HasSettingDescription<SettingTrait>)
            dialog.setLabelText(SettingTrait::settingDescription());

        // Show the dialog and exit
        dialog.exec();
        return (dialog.result() == QDialog::Accepted) ?
            std::optional<QString>{dialog.textValue()} :
            std::nullopt;
    }
};

}; // namespace Draupnir::Handlers

#endif // SETTINGSVALUEUSERINPUT_H
