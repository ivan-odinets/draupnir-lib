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

#ifndef FIXEDTABWIDGETTEMPLATE_H
#define FIXEDTABWIDGETTEMPLATE_H

#include <QAction>

#include "draupnir/ui_bricks/ui/widgets/FixedTabWidget.h"

#include "draupnir/SettingsBundleTemplate.h"
#include "draupnir/utils/index_of.h"

namespace Draupnir::Ui
{

/*! @class FixedTabWidgetTemplate draupnir/ui/widgets/FixedTabWidgetTemplate.h
 *  @ingroup UiBricks
 *  @brief Templated tab widget implementation using tab traits.
 *  @tparam WidgetIndexSetting A setting trait used to store and restore the selected tab index.
 *  @tparam TabTraits Variadic list of tab traits describing each tab (type, label, optional tooltip).
 *
 *  @details This class provides a customizable `QTabWidget` based on a variadic list of tab traits. Each tab is defined
 *           by a trait struct that specifies:
 *           - a widget type via `using Widget = ...;`
 *           - a static `displayName()` method returning the tab label (`QString`)
 *           - an optional static `toolTip()` method returning the tab tooltip (`QString`)
 *
 *           The widget supports:
 *           - full auto-construction (widgets created from default constructors)
 *           - partial/manual injection via nullptr placeholders
 *           - runtime replacement of widgets via `setWidget<Index>()`
 *           - persistent storage of active tab index using the provided `WidgetIndexSetting`
 *
 *           ### Example trait:
 *           @code
 *           struct SettingsTab {
 *               using Widget = SettingsWidget*;
 *               static QString displayName() { return "Settings"; }
 *               static QString tooltip() { return "Modify application settings"; }
 *           };
 *           @endcode
 *
 * @todo Add possibility to aggregate settings from TabTraits (or in particular - nested widget). */

template<class WidgetIndexSetting, class... TabTraits>
class FixedTabWidgetTemplate final : public FixedTabWidget
{
public:
    using SettingsBundle = std::conditional_t<
        std::is_same_v<WidgetIndexSetting,void>,
        Settings::SettingsBundleTemplate<>,
        Draupnir::Settings::SettingsBundleTemplate<WidgetIndexSetting>
    >;

    /*! @brief Constructs tab widgets from default constructors.
     *  @param parent Optional parent widget. */
    explicit FixedTabWidgetTemplate(QWidget* parent = nullptr) :
        FixedTabWidget{parent},
        m_widgets{std::make_tuple(new typename TabTraits::Widget...)}
    {
        static_assert(sizeof...(TabTraits) > 0,"Empty list of widgets is not allowed.");

        _setupTabsImpl<0,TabTraits...>();
    }

    /*! @brief Constructs the widget with externally provided tab widgets.
     *  @param parent Optional parent widget.
     *  @param widgets Widget instances to be used for the tabs (must match TabTraits::Widget types).
     * @note You can pass `nullptr` for any widget to skip adding the corresponding tab. */
    explicit FixedTabWidgetTemplate(QWidget* parent, typename TabTraits::Widget*... widgets) :
        FixedTabWidget{parent},
        m_widgets{std::forward_as_tuple(widgets...)}
    {
        static_assert(sizeof...(TabTraits) > 0,"Empty list of widgets is not allowed.");

        _setupTabsImpl<0,TabTraits...>();
    }

    /*! @brief Destructor. Saves the active tab index to settings (if loaded).
     *  @details The index is written via WidgetIndexSetting into the SettingsBundle.*/
    virtual ~FixedTabWidgetTemplate() override final {
        if constexpr (!SettingsBundle::isEmpty()) {
            Q_ASSERT_X(m_settings.isValid(),"FixedTabWidgetTemplate::~FixedTabWidgetTemplate",
                       "FixedTabWidgetTemplate::loadSettings method must have been called before.");
            m_settings.template set<WidgetIndexSetting>(this->currentIndex());
        }
    }

    /*! @brief Loads UI state from SettingsRegistry.
     *  @param registry Pointer to the settings registry instance.
     *  @details Loads the saved active tab index using WidgetIndexSetting and applies it to the tab widget. Must be called
     *           once after construction, before user interaction. */
    template<class SettingsRegistry, bool isEnabled = !SettingsBundle::isEmpty()>
    std::enable_if_t<isEnabled,void> loadSettings(SettingsRegistry* registry) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsRegistry>(),
        "Provided SettingsRegistry can not populate the SettingsBundle (missing WidgetIndexSetting).");
        Q_ASSERT_X(registry,"FixedTabWidget::loadSettings",
                   "Specified SettingsRegistry* pointer is nullptr");
        Q_ASSERT_X(!m_settings.isValid(),"FixedTabWidget::loadSettings",
                   "This method must be called only once.");
        m_settings = registry->template getSettingsBundle<SettingsBundle>();

        setCurrentIndex(m_settings.template get<WidgetIndexSetting>());
    }

    /*! @brief Returns the widget pointer by matching type.
     *  @tparam Widget Type of the widget (must match one of TabTraits::Widget).
     *  @return Stored widget pointer of given type.*/
    template<class Widget>
    Widget* getWidget() {
        return std::get<Widget*>(m_widgets);
    }

    /*! @brief Returns the widget pointer at given tab index.
     *  @tparam Index Index of the tab.
     *  @return Stored widget pointer at the specified index.*/
    template<std::size_t Index>
    auto* getWidget() {
        return std::get<Index>(m_widgets);
    }

    /*! @brief Replaces the widget at specified tab index.
     *  @tparam TabTrait trait of the new widget tab.
     *  @param widget New widget to insert.
     *  @details The old widget will be deleted via delete. The new widget will be inserted into the tab with the same
     *           label (and tooltip, if supported). Can be used for dynamic replacement of tab content. */
    template<class TabTrait>
    void setWidget(typename TabTrait::Widget* widget) {
        static_assert(draupnir::utils::is_one_of_v<TabTrait,TabTraits...>,
                "TabTrait specified is not present within the TabTraits... pack");
        constexpr int Index = draupnir::utils::index_of<TabTrait,TabTraits...>::value;

        if (std::get<Index>(m_widgets) != nullptr) {
            QTabWidget::removeTab(Index);
            delete std::get<Index>(m_widgets);
        }
        QTabWidget::insertTab(Index,widget,TabTrait::displayName());

        if constexpr (has_tooltip<TabTrait>::value) {
            QTabWidget::setTabToolTip(Index,TabTrait::tooltip());
        }

        std::get<Index>(m_widgets) = widget;
    }

private:
    template<class T, class = void>
    struct has_tooltip : std::false_type {};

    template<class T>
    struct has_tooltip<
        T,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(T::tooltip())>
        )>
    > : std::true_type {};

    SettingsBundle m_settings;
    std::tuple<typename TabTraits::Widget*...> m_widgets;

    /*! @brief Internal recursive helper that adds each tab widget with its label.
     *  @tparam Index Current tab index.
     *  @tparam First First trait in the list.
     *  @tparam Rest Remaining traits. */
    template<std::size_t Index, class First, class... Rest>
    inline void _setupTabsImpl() {
        if (std::get<Index>(m_widgets)) {
            const int tabIndex = QTabWidget::addTab(
                std::get<Index>(m_widgets),
                First::displayName()
            );

            if constexpr (has_tooltip<First>::value) {
                QTabWidget::setTabToolTip(tabIndex,First::tooltip());
            }
        }

        if constexpr (sizeof...(Rest) > 0)
            _setupTabsImpl<Index+1,Rest...>();
    }

    void _retranslateUi() override final {
        _retranslateTabsImpl<0,TabTraits...>();
    }

    /*! @brief Internal recursive helper to update tab labels on language change.
     *  @tparam Index Current tab index.
     *  @tparam First First trait in the list.
     *  @tparam Rest Remaining traits. */
    template<std::size_t Index, class First, class... Rest>
    inline void _retranslateTabsImpl() {
        QTabWidget::setTabText(Index,First::displayName());

        if constexpr (has_tooltip<First>::value) {
            QTabWidget::setTabToolTip(Index,First::tooltip());
        }

        if constexpr (sizeof...(Rest) > 0)
            _retranslateTabsImpl<Index+1,Rest...>();
    }
};

};

#endif // FIXEDTABWIDGETTEMPLATE_H
