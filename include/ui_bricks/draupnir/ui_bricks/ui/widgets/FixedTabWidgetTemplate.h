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

#include "draupnir/ui_bricks/ui/widgets/FixedTabWidget.h"

#include <QAction>

#include "draupnir/settings_registry/concepts/SettingTraitConcept.h"
#include "draupnir/settings_registry/utils/SettingsTraitsConcatenator.h"
#include "draupnir/ui_bricks/concepts/TabTraitConcept.h"
#include "draupnir/utils/type_list.h"

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
 * @todo Add possibility to obtain several widgets via one call (e.g. getAll<TextEditTrait>()). Return as std::tuple. */

template<class WidgetIndexSetting, TabTraitConcept... TabTraits>
class FixedTabWidgetTemplate final : public FixedTabWidget
{
public:
    /*! @brief type_list of TabTraits... objects. */
    using TabList = draupnir::utils::type_list<TabTraits...>;

    /*! @brief Aggregated SettingsBundleTemplate for settings being used within this widget and widgets for its tabs. */
    using SettingsBundle = typename Draupnir::Settings::SettingsTraitsConcatenator<
        WidgetIndexSetting,
        typename TabTraits::Widget...>
    ::toSettingsBundle;

    /*! @brief Constructs tab widgets from default constructors.
     *  @param parent Optional parent widget. */
    explicit FixedTabWidgetTemplate(QWidget* parent = nullptr) :
        FixedTabWidget{parent},
        m_widgets{std::make_tuple(_createWidgetOrNullptr<TabTraits>()...)}
    {
        static_assert(sizeof...(TabTraits) > 0,"Empty list of widgets is not allowed.");

        _setupTabsImpl<0,TabTraits...>();
    }

    /*! @brief Destructor. Saves the active tab index to settings (if loaded). The index is written via `WidgetIndexSetting`
     *         into the SettingsBundle.*/
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
     *           once after construction, before user interaction.
     * @todo As other `loadSettings`-methods this needs to be standartized.
     * @todo Add settings loading for subwidgets. */
    template<class SettingsRegistry>
    void loadSettings(SettingsRegistry* registry) requires(!SettingsBundle::isEmpty()) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsRegistry>(),
                      "Provided SettingsRegistry can not populate the SettingsBundle.");
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
    auto& getWidget() {
        return std::get<Widget*>(m_widgets);
    }

    /*! @brief Returns the widget pointer at given tab index.
     *  @tparam Index Index of the tab.
     *  @return Stored widget pointer at the specified index.*/
    template<std::size_t Index>
    auto& getWidget() {
        return std::get<Index>(m_widgets);
    }

    /*! @brief Returns the widget pointer at given TabTrait
     *  @tparam Index Index of the tab.
     *  @return Stored widget pointer at the specified index.*/
    template<TabTraitConcept Tab>
    auto& getWidgetByTrait() {
        static_assert(draupnir::utils::is_one_of_v<Tab,TabTraits...>);
        return std::get<TabList::template index_of_v<Tab>>();
    }

    /*! @brief Replaces the widget at specified tab index.
     *  @tparam TabTrait trait of the new widget tab.
     *  @param widget New widget to insert.
     *  @details The old widget will be deleted via delete. The new widget will be inserted into the tab with the same
     *           label (and tooltip, if supported). Can be used for dynamic replacement of tab content. */
    template<TabTraitConcept Tab>
    void setWidgetForTrait(typename Tab::Widget* widget) {
        static_assert(draupnir::utils::is_one_of_v<Tab,TabTraits...>,
                      "TabTrait specified is not present within the TabTraits... pack");
        constexpr int Index = TabList::template index_of_v<Tab>;

        if (std::get<Index>(m_widgets) != nullptr) {
            QTabWidget::removeTab(Index);
            delete std::get<Index>(m_widgets);
        }
        QTabWidget::insertTab(Index,widget,Tab::displayName());

        if constexpr (TabTrait::HasTooltip<Tab>) {
            QTabWidget::setTabToolTip(Index,Tab::tooltip());
        }

        std::get<Index>(m_widgets) = widget;
    }

private:
    SettingsBundle m_settings;
    std::tuple<typename TabTraits::Widget*...> m_widgets;

    /*! @brief If TabTrait::Widget is default constructible - creates it. If it requires constructor arguments - returns
     *         nullptr. */
    template<class TabTrait>
    inline typename TabTrait::Widget* _createWidgetOrNullptr() {
        if constexpr (std::is_default_constructible_v<typename TabTrait::Widget>) {
            return new typename TabTrait::Widget;
        } else {
            return nullptr;
        }
    }

    /*! @brief Internal recursive helper that adds each tab widget with its label.
     *  @tparam Index Current tab index.
     *  @tparam First First trait in the list.
     *  @tparam Rest Remaining traits. */
    template<std::size_t Index, class First, class... Rest>
    inline void _setupTabsImpl() {
        // This is called only during setup. So if within std::tuple is nullptr - than this widget
        // is nullptr and not default constructible. Thus we need to iterate only for those tuple
        // elements which widgets are default constructible.
        if (std::is_default_constructible_v<typename First::Widget>) {
            const int tabIndex = QTabWidget::addTab(std::get<Index>(m_widgets),First::displayName());

            if constexpr (TabTrait::HasTooltip<First>)
                QTabWidget::setTabToolTip(tabIndex,First::tooltip());
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

        if constexpr (TabTrait::HasTooltip<First>) {
            QTabWidget::setTabToolTip(Index,First::tooltip());
        }

        if constexpr (sizeof...(Rest) > 0)
            _retranslateTabsImpl<Index+1,Rest...>();
    }
};

}; // namespace Draupnir::Ui

#endif // FIXEDTABWIDGETTEMPLATE_H
