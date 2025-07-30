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

#ifndef CENTRALTABBEDWIDEGT_H
#define CENTRALTABBEDWIDEGT_H

#include <QTabWidget>

#include <QEvent>

#include "AppSettings.h"

/*! @class CentralTabbedWidget draupnir-lib/include/ui/CentralTabbedWidget.h
 *  @brief Abstract base class for a central widget composed of tabs.
 *  @details Handles tab title retranslation and active tab setting loading. Designed to be extended by CentralTabbedWidgetTemplate. */

class CentralTabbedWidget : public QTabWidget
{
    Q_OBJECT
public:
    /*! @brief Constructs the CentralTabbedWidget and initialzies the base class (`QTabWidget`).
     *  @param parent Optional parent widget. */
    explicit CentralTabbedWidget(QWidget* parent = nullptr) :
        QTabWidget{parent}
    {}

    /*! @brief Destructor. Trivial. */
    ~CentralTabbedWidget() override = default;

    /*! @brief Load settings for tab state (e.g., active tab index).
     *  @param settings Pointer to AppSettings instance.
     * @note This method must be called only once. */
    virtual void loadSettings(AppSettings* settings) = 0;

    /*! @brief Returns widget pointer of tab with specified index.
     *  @param index index of widget within tabs
     *  @tparam Widget type of the widget (must match one of TabTrait::widget).
     *  @return Widget pointer. */
    template<class Widget>
    Widget* getWidgetByIndex(int index) {
        Q_ASSERT_X(index < count(), "CentralTabbedWidget::getWidgetByIndex",
                   "Provided index should be lower than amount of tabs.");
        Q_ASSERT_X(qobject_cast<Widget*>(QTabWidget::widget(index)), "CentralTabbedWidget::getWidgetByIndex",
                   "Widget at provided index does not match the specified type");
        return static_cast<Widget*>(QTabWidget::widget(index));
    }

protected:
    /*! @brief Handles Qt change event of type QEvent::Language change and calls CentralTabbedWidget::_retranslateTabs, which is
     *         implemented within CentralTabbedWidgetTemplate. */
    void changeEvent(QEvent* event) final {
        if (event->type() == QEvent::LanguageChange) {
            _retranslateTabs();
        }

        QTabWidget::changeEvent(event);
    }

private:
    virtual void _retranslateTabs() = 0;
};

/*! @class CentralTabbedWidegtTemplate draupnir-lib/include/ui/CentralTabbedWidget.h
 *  @brief Templated implementation of CentralTabbedWidget using tab traits.
 *  @details This class constructs and manages a QTabWidget where each tab is defined by a provided trait. TabTraits are structs
 *           that define:
 *           - a widget type: `typedef QWidget* widget;`
 *           - a static label method: `static QString tabDisplayName();`*/

template<class... TabTraits>
class CentralTabbedWidgetTemplate final : public CentralTabbedWidget
{
public:
    /*! @brief Constructs tab widgets from default constructors.
     *  @param parent Optional parent widget. */
    explicit CentralTabbedWidgetTemplate(QWidget* parent = nullptr) :
        CentralTabbedWidget{parent},
        p_settings{nullptr},
        m_widgets{std::make_tuple(new typename TabTraits::widget...)}
    {
        static_assert(sizeof...(TabTraits) > 0,"Empty list of widgets is not allowed.");

        _setupTabsImpl<0,TabTraits...>();
    }

    /*! @brief Constructs the widget with already constructed tab widgets.
     *  @param parent Optional parent widget.
     *  @param widgets External widget instances (one per TabTrait::widget). */
    explicit CentralTabbedWidgetTemplate(QWidget* parent, typename TabTraits::widget... widgets) :
        CentralTabbedWidget{parent},
        p_settings{nullptr},
        m_widgets{std::forward_as_tuple(widgets...)}
    {
        static_assert(sizeof...(TabTraits) > 0,"Empty list of widgets is not allowed.");

        _setupTabsImpl<0,TabTraits...>();
    }

    /*! @brief Constructs the widget with already constructed tab widgets.
     *  @param parent Optional parent widget.
     *  @param widgets External widget instances (one per TabTrait::widget). */
    explicit CentralTabbedWidgetTemplate(QWidget* parent, typename TabTraits::widget&&... widgets) :
        CentralTabbedWidget{parent},
        p_settings{nullptr},
        m_widgets{std::forward_as_tuple(widgets...)}
    {
        static_assert(sizeof...(TabTraits) > 0,"Empty list of widgets is not allowed.");

        _setupTabsImpl<0,TabTraits...>();
    }

    /*! @brief Destructor. Stores the current active tab index into settings (if set). */
    virtual ~CentralTabbedWidgetTemplate() override final {
        Q_ASSERT_X(p_settings, "CentralTabbedWidgetTemplate::~CentralTabbedWidgetTemplate",
                   "CentralTabbedWidgetTemplate::loadSettings must have been called before.");

        p_settings->setValue(
            AppSettings::GUI, activeTab_settingsKey,
            QTabWidget::currentIndex()
        );
    }

    /*! @brief Loads settings from AppSettings (e.g., active tab index).
     *  @param settings Pointer to AppSettings instance. Should not be null.
     * @note This method must be called only once. */
    void loadSettings(AppSettings* settings) final {
        Q_ASSERT_X(settings,"CentralTabbedWidegtTemplate::loadSettings",
                   "Provided AppSettings pointer is nullptr.");
        Q_ASSERT_X(p_settings == nullptr,"CentralTabbedWidegtTemplate::loadSettings",
                   "This method must be called only once.");

        p_settings = settings;

        bool ok = false;

        const int currentTab = settings->value(
            AppSettings::GUI, activeTab_settingsKey
        ).toInt(&ok);

        if ( (currentTab < count()) && (ok)) {
            QTabWidget::setCurrentIndex(currentTab);
        }
    }

    /*! @brief Returns widget pointer matching type Widget.
     *  @tparam Widget type of the widget (must match one of TabTrait::widget).
     *  @return Widget pointer. */
    template<class Widget>
    constexpr Widget* getWidget() {
        return std::get<Widget>(m_widgets);
    }

private:
    static inline QLatin1String activeTab_settingsKey{"activeTab"};

    AppSettings* p_settings;
    std::tuple<typename TabTraits::widget...> m_widgets;

    /*! @brief Internal recursive helper that adds each tab widget with its label.
     *  @tparam Index Current tab index.
     *  @tparam First First trait in the list.
     *  @tparam Rest Remaining traits. */
    template<std::size_t Index, class First, class... Rest>
    inline void _setupTabsImpl() {
        QTabWidget::addTab(
            std::get<Index>(m_widgets),
            First::tabDisplayName()
        );

        if constexpr (sizeof...(Rest) > 0)
            _setupTabsImpl<Index+1,Rest...>();
    }

    void _retranslateTabs() override final {
        _retranslateTabsImpl<0,TabTraits...>();
    }

    /*! @brief Internal recursive helper to update tab labels on language change.
     *  @tparam Index Current tab index.
     *  @tparam First First trait in the list.
     *  @tparam Rest Remaining traits. */
    template<std::size_t Index, class First, class... Rest>
    inline void _retranslateTabsImpl() {
        QTabWidget::setTabText(Index,First::tabDisplayName());

        if constexpr (sizeof...(Rest) > 0)
            _retranslateTabsImpl<Index+1,Rest...>();
    }
};

#endif // CENTRALTABBEDWIDEGT_H
