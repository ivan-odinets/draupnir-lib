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

#include "ProxyEditWidget.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMetaType>
#include <QSpinBox>
#include <QVBoxLayout>

Q_DECLARE_METATYPE(QNetworkProxy::ProxyType);

ProxyEditWidget::ProxyEditWidget(QWidget* parent) :
    QWidget{parent},
    w_proxyTypeLabel{new QLabel}, w_proxyTypeSelector{new QComboBox},
    w_proxyHostLabel{new QLabel}, w_proxyHostSelector{new QLineEdit},
    w_proxyPortLabel{new QLabel}, w_proxyPortSelector{new QSpinBox}
{
    w_proxyPortSelector->setMinimum(0);
    w_proxyPortSelector->setMaximum(65535);

    QFormLayout* editLayout = new QFormLayout;
    editLayout->addRow(w_proxyTypeLabel,w_proxyTypeSelector);
    editLayout->addRow(w_proxyHostLabel,w_proxyHostSelector);
    editLayout->addRow(w_proxyPortLabel,w_proxyPortSelector);
    setLayout(editLayout);

    _retranslateUi();

    w_proxyHostSelector->setPlaceholderText(tr("e.g., 127.0.0.1"));
}

void ProxyEditWidget::setProxy(const QNetworkProxy& proxy)
{
    for (int i = 0; i < w_proxyTypeSelector->count(); i++) {
        if (proxy.type() == w_proxyTypeSelector->itemData(i).value<QNetworkProxy::ProxyType>()) {
            w_proxyTypeSelector->setCurrentIndex(i);
            break;
        }
    }

    w_proxyHostSelector->setText(proxy.hostName());
    w_proxyPortSelector->setValue(proxy.port());
}

QNetworkProxy ProxyEditWidget::proxy() const
{
    QNetworkProxy result;

    result.setType(w_proxyTypeSelector->currentData().value<QNetworkProxy::ProxyType>());
    result.setHostName(w_proxyHostSelector->text());
    result.setPort(w_proxyPortSelector->value());

    return result;
}

void ProxyEditWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        _retranslateUi();
    }

    QWidget::changeEvent(event);
}

void ProxyEditWidget::_retranslateUi()
{
    w_proxyTypeSelector->clear();
    w_proxyTypeSelector->addItem(tr("Default"),       QVariant::fromValue(QNetworkProxy::DefaultProxy)       );
    w_proxyTypeSelector->addItem(tr("SOCKS5"),        QVariant::fromValue(QNetworkProxy::Socks5Proxy)        );
    w_proxyTypeSelector->addItem(tr("No Proxy"),      QVariant::fromValue(QNetworkProxy::NoProxy)            );
    w_proxyTypeSelector->addItem(tr("HTTP"),          QVariant::fromValue(QNetworkProxy::HttpProxy)          );
    w_proxyTypeSelector->addItem(tr("HTTP caching"),  QVariant::fromValue(QNetworkProxy::HttpCachingProxy)   );
    w_proxyTypeSelector->addItem(tr("FTP caching"),   QVariant::fromValue(QNetworkProxy::FtpCachingProxy)    );

    w_proxyTypeLabel->setText(tr("Type:"));
    w_proxyHostLabel->setText(tr("Host:"));
    w_proxyPortLabel->setText(tr("Port:"));
}
