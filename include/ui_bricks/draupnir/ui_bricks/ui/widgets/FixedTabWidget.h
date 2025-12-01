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

#ifndef FIXEDTABWIDGET_H
#define FIXEDTABWIDGET_H

#include <QTabWidget>

#include <QEvent>

namespace Draupnir::Ui
{

/*! @class FixedTabWidget draupnir/ui_bricks/ui/widgets/FixedTabWidget.h
 *  @ingroup UiBricks
 *  @brief Abstract base class for a widget composed of fixed amount of tabs. Handles tab title retranslation. */

class FixedTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    /*! @brief Constructs the CentralTabbedWidget and initialzies the base class (`QTabWidget`).
     *  @param parent Optional parent widget. */
    explicit FixedTabWidget(QWidget* parent = nullptr) :
        QTabWidget{parent}
    {}

    /*! @brief Destructor. Trivial. */
    ~FixedTabWidget() override = default;

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
            _retranslateUi();
        }

        QTabWidget::changeEvent(event);
    }

private:
    virtual void _retranslateUi() = 0;
};

}

#endif // FIXEDTABWIDGET_H
