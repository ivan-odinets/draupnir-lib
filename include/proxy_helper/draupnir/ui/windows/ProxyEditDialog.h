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

#ifndef PROXYEDITDIALOG_H
#define PROXYEDITDIALOG_H

#include <QDialog>

#include <QNetworkProxy>

namespace Draupnir::Proxy {

class ProxyEditWidget;

/*! @class ProxyEditDialog draupnir/ui/windows/ProxyEditDialog.h
 *  @ingroup ProxyHelper
 *  @brief Modal dialog for editing and viewing QNetworkProxy objects.
 *
 *  @details Provides a user-friendly dialog window for configuring proxy settings using a ProxyEditWidget. Typically used to
 *           let the user create or edit proxy settings and confirm/cancel their changes.
 *
 *           Usage:
 *           - Use ProxyEditDialog::setProxy() to display an existing proxy.
 *           - After ProxyEditDialog::exec(), call ProxyEditDialog::proxy() to retrieve user edits if accepted.
 *
 * @note This dialog does not support proxies with authentication. */

class ProxyEditDialog final : public QDialog
{
    Q_OBJECT
public:
    /*! @brief Constructs the dialog and initializes the internal ProxyEditWidget.
     *  @param parent - parent widget. */
    explicit ProxyEditDialog(QWidget* parent = nullptr);

    /*! @brief Destructor. */
    ~ProxyEditDialog() final = default;

    /*! @brief Returns the QNetworkProxy currently represented in the dialog.
     *  @return Edited QNetworkProxy value. */
    QNetworkProxy proxy() const;

    /*! @brief Updates the dialog UI to display the specified proxy.
     *  @param proxy QNetworkProxy to show in the editor. */
    void setProxy(const QNetworkProxy& proxy);

private:
    ProxyEditWidget* w_proxyEditWidget;
};

}; // namespace Draupnir::Proxy

#endif // PROXYEDITDIALOG_H
