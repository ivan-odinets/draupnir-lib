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

#ifndef PROXYEDITWIDGET_H
#define PROXYEDITWIDGET_H

#include <QWidget>

#include <QNetworkProxy>

class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;

namespace Draupnir::Proxy
{

/*! @class ProxyEditWidget draupnir/ui/widgets/ProxyEditWidget.h
 *  @ingroup ProxyHelper
 *  @brief QWidget-based editor for QNetworkProxy objects.
 *
 *  @details Provides a user interface for editing and viewing QNetworkProxy settings, such as proxy type, host, and port. Does
 *           not support authentication proxies.
 *           Typical usage:
 *           - Call ProxyEditWidget::setProxy() to display an existing QNetworkProxy.
 *           - User edits fields in the widget.
 *           - Call ProxyEditWidget::proxy() to retrieve the updated proxy object.
 *
 * @note This widget does not support proxies with authentication.
 *
 * @todo Extend configurable things to allow full configuration of the QNetworkProxy objects.
 * @todo Maybe add some unit test for this widget? */

class ProxyEditWidget final : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Constructs the widget and initializes all controls.
     *  @param parent - parent widget. */
    explicit ProxyEditWidget(QWidget* parent = nullptr);

    /*! @brief Destructor. */
    ~ProxyEditWidget() final {}

    /*! @brief Returns the QNetworkProxy currently represented by the widget state.
     *  @return QNetworkProxy with values from the editor. */
    QNetworkProxy proxy() const;

    /*! @brief Updates the widget UI to show the specified QNetworkProxy.
     *  @param proxy QNetworkProxy to display in the editor. */
    void setProxy(const QNetworkProxy& proxy);

protected:
    /*! @brief Handles language change events to update translations.
     *  @param event - change event (should be QEvent::LanguageChange). */
    void changeEvent(QEvent* event) final;

private:
    /*! @brief Sets translated text for all labels and repopulates the proxy type selector. Called on construction and
     *         on language change. */
    void _retranslateUi();

    QLabel*        w_proxyTypeLabel;
    QComboBox*     w_proxyTypeSelector;
    QLabel*        w_proxyHostLabel;
    QLineEdit*     w_proxyHostSelector;
    QLabel*        w_proxyPortLabel;
    QSpinBox*      w_proxyPortSelector;
};

} // namespace Draupnir::Proxy

#endif // PROXYEDITWIDGET_H
